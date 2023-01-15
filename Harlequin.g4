//
// Copyright (c) 2023, Zoe J. Bare
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
grammar Harlequin;

//---------------------------------------------------------------------------------------------------------------------

// Root parser rule
module
	: moduleDef*
	;

// Module item defintion rule
moduleDef
	: usingStmt Term
	| namespaceDecl
	| classDecl
	;

// Fully qualified ID name rule
qualifiedId
	: Id (Dot Id)*
	;

// 'using' statement rule
usingStmt
	: UsingKw qualifiedId
	;

// Namespace declaration rule
namespaceDecl
	: NamespaceKw Id namespaceDef
	;

// Namespace definition rule
namespaceDef
	: LeftBrace classDecl* RightBrace
	;

// Class declaration rule
classDecl
	: classModifier* classType? Id classExtends? classImpls? classDef
	;

// Class type rule
classType
	: ClassKw
	| InterfaceKw
	;

// Class access modifiers rule
classModifier
	: AccessSpecKw
	| StaticKw
	;

// Class 'extends' rule
classExtends
	: ExtendsKw qualifiedId
	;

// Class 'implements' rule
classImpls
	: ImplementsKw qualifiedId (Comma qualifiedId)* Comma?
	;

// Class definition rule
classDef
	: LeftBrace (classVarDecl Term | methodDecl)* RightBrace
	;

// Class variable declaration rule
classVarDecl
	: AccessSpecKw? StaticKw? varDecl
	;

// Typename rule
typeName
	: Id (LeftBracket RightBracket)*
	;

// Variable declaration rule
varDecl
	: ConstKw? typeName varDef (Comma varDef)* ?
	;

// Variable definition rule
varDef
	: Id Assign expr
	;

// Variable declaration or expression statement rule (primarily for use within paren'd statements)
parenExpr
	: varDecl
	| expr
	;

// Array initializer list sequence rule
arrayParamSeq
	: arrayParamElem (Comma arrayParamElem)* Comma?
	;

// Array initializer list element rule
arrayParamElem
	: (LeftBrace arrayParamSeq* RightBrace) | expr
	;

// Lambda (embedded function) declaration rule
lambdaDecl
	: LambdaKw typeName Id LeftParen methodParamSeq? RightParen (CaptureKw LeftBracket Id (Comma Id)* RightBracket)? codeBlock
	;

// Method declaration rule
methodDecl
	: AccessSpecKw? methodModifier* typeName Id LeftParen methodParamSeq? RightParen (codeBlock | Term)
	;

// Method access modifiers rule
methodModifier
	: RetainKw
	| NativeKw
	| StaticKw
	| VirtualKw
	;

// Method parameter sequence rule
methodParamSeq
	: methodParam (Comma methodParam)* Comma?
	;

// Method parameter rule
methodParam
	: typeName Id?
	;

// Code statement rule
codeStmt
	: codeBlock
	| varDecl Term
	| exprStmt
	;

// Braced code block rule
codeBlock
	: LeftBrace codeStmt* RightBrace
	;

// Expression statement rule
exprStmt
	: (expr | returnStmt)? Term
	| conditionStmt
	| loopStmt
	| exceptionHandler
	| lambdaDecl
	| asmBlock
	;

// Expression rule
expr
	: expr (Incr | Decr)                                                            # unaryPostfix
    | <assoc=right> expr Expo expr                                                  # exponent
    | expr LeftParen (expr (Comma expr)*)? RightParen                               # call
	| LeftParen expr RightParen                                                     # paren
    | expr LeftBracket expr RightBracket                                            # index
	| expr Dot expr                                                                 # dot
	| LeftBrace arrayParamSeq* RightBrace                                           # array
	| (Incr | Decr | Sub | Excl | Tilde) expr                                       # unaryPrefix
	| expr (Mult | Div | Mod) expr                                                  # arithmetic
	| expr (Add | Sub) expr                                                         # arithmetic
	| expr (LeftRotate | RightRotate | LeftShift | RightShift) expr                 # bitShift
	| expr (LessThan | LessThanOrEqualTo | GreaterThan | GreaterThanOrEqualTo) expr # equality
	| expr (EqualTo | NotEqualTo) expr                                              # equality
	| expr BitAnd expr                                                              # bitwise
	| expr BitXor expr                                                              # bitwise
	| expr BitOr expr                                                               # bitwise
	| expr LogicAnd expr                                                            # logic
	| expr LogicOr expr                                                             # logic
	| expr (Assign | ExpoAssign | AddAssign | SubAssign | MultAssign | DivAssign | ModAssign | LeftRotateAssign | RightRotateAssign | LeftShiftAssign | RightShiftAssign | BitAndAssign | BitXorAssign | BitOrAssign) expr # assignment
	| expr Comma expr                                                               # comma
	| (BreakKw | ContinueKw)                                                        # control
	| CopyIntrinKw                                                                  # intrinsic
	| (NullLit | BoolLit | RealLit | IntLit | StrLit)                               # literal
	| Id                                                                            # id
	;

// Method return statement rule
returnStmt
	: ReturnKw expr
	;

// Root loop statement rule
loopStmt
	: loopWhileDef
	| loopDoWhileDef
	| loopForDef
	;

// While-loop definition rule
loopWhileDef
	: WhileKw LeftParen parenExpr RightParen codeStmt
	;

// Do-while-loop definition rule
loopDoWhileDef
	: DoKw codeStmt WhileKw LeftParen parenExpr RightParen Term
	;

// For-loop definition rule
loopForDef
	: ForKw LeftParen parenExpr? Term parenExpr? Term parenExpr? RightParen codeStmt
	;

// Root conditional statement rule
conditionStmt
	: condIfDef condElifDef* condElseDef?
	| condSwitchDef LeftBrace condCaseDef* RightBrace
	;

// Condition defintion (if-fragment) rule
condIfDef
	: IfKw LeftParen parenExpr RightParen codeStmt
	;

// Condition defintion (elif-fragment) rule
condElifDef
	: ElifKw LeftParen parenExpr RightParen codeStmt
	;

// Condition defintion (else-fragment) rule
condElseDef
	: ElseKw codeStmt
	;

// Condition (switch) rule
condSwitchDef
	: SwitchKw LeftParen parenExpr RightParen
	;

// Condition (case definition) rule
condCaseDef
	: (CaseKw condCaseResult | DefaultKw) Colon codeStmt?
	;

// Condition case result literal/ID rule
condCaseResult
	: (NullLit | BoolLit | RealLit | IntLit | StrLit) # caseLiteral
	| Id                                              # caseId
	;

// Root exception handling rule
exceptionHandler
	: tryBlockDef catchBlockDef* finallyBlockDef?
	;

// Try-block definition rule
tryBlockDef
	: TryKw codeStmt
	;

// Catch-block definition rule
catchBlockDef
	: CatchKw LeftParen parenExpr? RightParen codeStmt
	;

// Finally-block definition rule
finallyBlockDef
	: FinallyKw codeStmt
	;

// Assembly code declaration rule
asmBlock
	: AsmIntrinKw LeftBrace (asmStmt | asmLabel)* RightBrace
	;

// Assembly instruction statement rule
asmStmt
	: Id (asmOperand (Comma asmOperand)*)?
	;

// Assembly instruction operand rule
asmOperand
	: asmLit
	| asmReg
	| asmVar
	| asmLabel
	;

// Assembly literal value rule
asmLit
	: NullLit
	| BoolLit
	| RealLit
	| IntLit
	| StrLit
	;

// Assembly register ID rule
asmReg
	: AsmRegId
	;

// Assembly variable reference rule
asmVar
	: LeftBracket Id RightBracket
	;

// Assembly code label rule
asmLabel
	: Colon Id
	;

//---------------------------------------------------------------------------------------------------------------------

// Code block tokens
LeftParen:    '(' ;
RightParen:   ')' ;

LeftBracket:  '[' ;
RightBracket: ']' ;

LeftBrace:    '{' ;
RightBrace:   '}' ;

// Language keyword tokens
NamespaceKw:  'namespace' ;
ClassKw:      'class' ;
InterfaceKw:  'interface' ;
UsingKw:      'using' ;
StaticKw:     'static' ;
RetainKw:     'retain' ;
ConstKw:      'const' ;
VirtualKw:    'virtual' ;
NativeKw:     'native' ;
ExtendsKw:    'extends' ;
ImplementsKw: 'implements' ;
ReturnKw:     'return' ;
IfKw:         'if' ;
ElifKw:       'elif' ;
ElseKw:       'else' ;
ForKw:        'for' ;
DoKw:         'do' ;
WhileKw:      'while' ;
BreakKw:      'break' ;
ContinueKw:   'continue' ;
SwitchKw:     'switch' ;
CaseKw:       'case' ;
DefaultKw:    'default' ;
TryKw:        'try' ;
CatchKw:      'catch' ;
FinallyKw:    'finally' ;
LambdaKw:     'lambda' ;
CaptureKw:    'capture' ;

// Intrinsic keyword tokens
AsmIntrinKw:  '__asm' ;
CopyIntrinKw: '__copy' ;

// Class access keyword tokens
AccessSpecKw
	: 'public'
	| 'protected'
	| 'private'
	;

// Arithmetic/bitwise operator tokens
Add:         '+' ;
Sub:         '-' ;
Mult:        '*' ;
Div:         '/' ;
Mod:         '%' ;
LeftShift:   '<<' ;
RightShift:  '>>' ;
LeftRotate:  '<<<' ;
RightRotate: '>>>' ;
BitAnd:      '&' ;
BitOr:       '|' ;
BitXor:      '^' ;

// Assignment operator tokens
Assign:            '=' ;
AddAssign:         '+=' ;
SubAssign:         '-=' ;
MultAssign:        '*=' ;
DivAssign:         '/=' ;
ModAssign:         '%=' ;
LeftShiftAssign:   '<<=' ;
RightShiftAssign:  '>>=' ;
LeftRotateAssign:  '<<<=' ;
RightRotateAssign: '>>>=' ;
BitAndAssign:      '&=' ;
BitOrAssign:       '|=' ;
BitXorAssign:      '^=' ;

// Logical operator tokens
LogicAnd: '&&' ;
LogicOr:  '||' ;

// Equality operator tokens
EqualTo: '==' ;
NotEqualTo: '!=' ;

// Relational (less-than) operator tokens
LessThan:          '<' ;
LessThanOrEqualTo: '<=' ;

// Relational (greater-than) operator tokens
GreaterThan:          '>' ;
GreaterThanOrEqualTo: '>=' ;

// Exponent operator tokens
Expo:       '**' ;
ExpoAssign: '**=' ;

// Unary operator tokens
Incr: '++' ;
Decr: '--' ;
Excl: '!' ;
Tilde: '~' ;

// Syntax operator tokens
Dot:   '.' ;
Comma: ',' ;
Colon: ':' ;

AsmRegId: [rR] FDigit+ ;

// Value keyword literal tokens
NullLit: 'null' ;
BoolLit: 'true' | 'false' ;

// Lexer base rules (must be defined after most other lexer rules to avoid parser ambiguities)
IntLit:  (FDigit+ | ('0x' FHex+) | ('0o' FOctal+) | ('0b' FBinary+)) FIntSuffix? ;
RealLit: (FDecimal ('f' | 'F')?) | ((FDigit+ | FDecimal+) FExpo FDigit+) ;
StrLit:  FStrSingleQuote | FStrDoubleQuote ;
Id:      FAlpha+ (FAlpha | FDigit+)* ;
Term:    ';' ;

// Comments are discarded
LineComment:  '//' .*? '\r'? '\n' -> skip ;
BlockComment: '/*' .*? '*/'       -> skip ;

// Whitespace is discarded
WS: [ \t\r\n]+ -> skip ;

// Lexer rule helpers
fragment FDigit:  '0' | ([1-9] [0-9]*) ;
fragment FAlpha:  [$_a-zA-Z] ;
fragment FHex:    FDigit | [a-fA-F] ;
fragment FOctal:  [0-7] ;
fragment FBinary: [0-1] ;
fragment FExpo:   [Ee] [+\-]? ;
fragment FEsc:    '\\\\' ;

fragment FIntSuffix
	: [Uu]
	| [Ll]
	| [Ll][Ll]
	| [Uu][Ll]
	| [Uu][Ll][Ll]
	;

fragment FStrDoubleQuote: '"' (FEsc | '\\"' | .)*? '"' ;
fragment FStrSingleQuote: '\'' (FEsc | '\\\'' | .)*? '\'' ;

fragment FDecimal
	: FDigit+ '.' FDigit*
	| '.' FDigit+
	;

//---------------------------------------------------------------------------------------------------------------------
