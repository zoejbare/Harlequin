
// Generated from D:/dev/oss/Harlequin/Harlequin.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  HarlequinParser : public antlr4::Parser {
public:
  enum {
    LeftParen = 1, RightParen = 2, LeftBracket = 3, RightBracket = 4, LeftBrace = 5, 
    RightBrace = 6, NamespaceKw = 7, ClassKw = 8, InterfaceKw = 9, UsingKw = 10, 
    AsKw = 11, StaticKw = 12, InlineKw = 13, ConstKw = 14, VirtualKw = 15, 
    NativeKw = 16, ExtendsKw = 17, ImplementsKw = 18, ReturnKw = 19, IfKw = 20, 
    ElifKw = 21, ElseKw = 22, ForKw = 23, DoKw = 24, WhileKw = 25, BreakKw = 26, 
    ContinueKw = 27, SwitchKw = 28, CaseKw = 29, DefaultKw = 30, TryKw = 31, 
    CatchKw = 32, FinallyKw = 33, LambdaKw = 34, CaptureKw = 35, ConstructorKw = 36, 
    PublicKw = 37, ProtectedKw = 38, PrivateKw = 39, InternalKw = 40, LimitedKw = 41, 
    NewKw = 42, AsmIntrinKw = 43, CopyIntrinKw = 44, Add = 45, Sub = 46, 
    Mult = 47, Div = 48, Mod = 49, LeftShift = 50, RightShift = 51, LeftRotate = 52, 
    RightRotate = 53, BitAnd = 54, BitOr = 55, BitXor = 56, Assign = 57, 
    AddAssign = 58, SubAssign = 59, MultAssign = 60, DivAssign = 61, ModAssign = 62, 
    LeftShiftAssign = 63, RightShiftAssign = 64, LeftRotateAssign = 65, 
    RightRotateAssign = 66, BitAndAssign = 67, BitOrAssign = 68, BitXorAssign = 69, 
    LogicAnd = 70, LogicOr = 71, EqualTo = 72, NotEqualTo = 73, LessThan = 74, 
    LessThanOrEqualTo = 75, GreaterThan = 76, GreaterThanOrEqualTo = 77, 
    Expo = 78, ExpoAssign = 79, Incr = 80, Decr = 81, Excl = 82, Tilde = 83, 
    Dot = 84, Comma = 85, Colon = 86, Pound = 87, LineKw = 88, NullLit = 89, 
    BoolLit = 90, AsmRegId = 91, IntLit = 92, RealLit = 93, StrLit = 94, 
    Id = 95, Term = 96, LineComment = 97, BlockComment = 98, WS = 99
  };

  enum {
    RuleRoot = 0, RuleCompilerDirective = 1, RuleQualifiedId = 2, RuleUsingStmt = 3, 
    RuleUsingAliasStmt = 4, RuleNamespaceDecl = 5, RuleNamespaceDef = 6, 
    RuleClassDecl = 7, RuleClassType = 8, RuleAccessBaseSpecifier = 9, RuleAccessLimitSpecifier = 10, 
    RuleAccessSpecifier = 11, RuleStorageSpecifier = 12, RuleFunctionSpecifier = 13, 
    RuleConstQualifier = 14, RuleMethodDeclSpecSeq = 15, RuleClassVarDeclSpecSeq = 16, 
    RuleLocalVarDeclSpecSeq = 17, RuleArgVarDeclSpecSeq = 18, RuleClassInheritance = 19, 
    RuleClassExtends = 20, RuleClassImpls = 21, RuleClassDef = 22, RuleArrayTypeDecl = 23, 
    RuleArrayTypeDef = 24, RuleTypeNameDecl = 25, RuleBracedInitializer = 26, 
    RuleCtorCall = 27, RuleClassVarDeclStmt = 28, RuleLocalVarDeclStmt = 29, 
    RuleVarDeclStmt = 30, RuleVarDecl = 31, RuleNextVarDecl = 32, RuleCtrlFlowExpr = 33, 
    RuleArrayParamSeq = 34, RuleArrayParamElem = 35, RuleLambdaDecl = 36, 
    RuleCtorDecl = 37, RuleMethodDecl = 38, RuleMethodQualifier = 39, RuleMethodStorageModifier = 40, 
    RuleMethodArgSeq = 41, RuleMethodArg = 42, RuleCodeBlock = 43, RuleCodeBlockStmt = 44, 
    RuleCodeStmt = 45, RuleExprStmt = 46, RuleExpr = 47, RuleReturnStmt = 48, 
    RuleLoopStmt = 49, RuleLoopWhileDef = 50, RuleLoopDoWhileDef = 51, RuleLoopForDef = 52, 
    RuleConditionStmt = 53, RuleCondIfDef = 54, RuleCondElifDef = 55, RuleCondElseDef = 56, 
    RuleCondSwitchDef = 57, RuleCondCaseDef = 58, RuleCondCaseResult = 59, 
    RuleExceptionHandler = 60, RuleTryBlockDef = 61, RuleCatchBlockDef = 62, 
    RuleFinallyBlockDef = 63, RuleAsmBlock = 64, RuleAsmStmt = 65, RuleAsmInstr = 66, 
    RuleAsmOperand = 67, RuleAsmLit = 68, RuleAsmReg = 69, RuleAsmVar = 70, 
    RuleAsmLabel = 71
  };

  explicit HarlequinParser(antlr4::TokenStream *input);

  HarlequinParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~HarlequinParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class RootContext;
  class CompilerDirectiveContext;
  class QualifiedIdContext;
  class UsingStmtContext;
  class UsingAliasStmtContext;
  class NamespaceDeclContext;
  class NamespaceDefContext;
  class ClassDeclContext;
  class ClassTypeContext;
  class AccessBaseSpecifierContext;
  class AccessLimitSpecifierContext;
  class AccessSpecifierContext;
  class StorageSpecifierContext;
  class FunctionSpecifierContext;
  class ConstQualifierContext;
  class MethodDeclSpecSeqContext;
  class ClassVarDeclSpecSeqContext;
  class LocalVarDeclSpecSeqContext;
  class ArgVarDeclSpecSeqContext;
  class ClassInheritanceContext;
  class ClassExtendsContext;
  class ClassImplsContext;
  class ClassDefContext;
  class ArrayTypeDeclContext;
  class ArrayTypeDefContext;
  class TypeNameDeclContext;
  class BracedInitializerContext;
  class CtorCallContext;
  class ClassVarDeclStmtContext;
  class LocalVarDeclStmtContext;
  class VarDeclStmtContext;
  class VarDeclContext;
  class NextVarDeclContext;
  class CtrlFlowExprContext;
  class ArrayParamSeqContext;
  class ArrayParamElemContext;
  class LambdaDeclContext;
  class CtorDeclContext;
  class MethodDeclContext;
  class MethodQualifierContext;
  class MethodStorageModifierContext;
  class MethodArgSeqContext;
  class MethodArgContext;
  class CodeBlockContext;
  class CodeBlockStmtContext;
  class CodeStmtContext;
  class ExprStmtContext;
  class ExprContext;
  class ReturnStmtContext;
  class LoopStmtContext;
  class LoopWhileDefContext;
  class LoopDoWhileDefContext;
  class LoopForDefContext;
  class ConditionStmtContext;
  class CondIfDefContext;
  class CondElifDefContext;
  class CondElseDefContext;
  class CondSwitchDefContext;
  class CondCaseDefContext;
  class CondCaseResultContext;
  class ExceptionHandlerContext;
  class TryBlockDefContext;
  class CatchBlockDefContext;
  class FinallyBlockDefContext;
  class AsmBlockContext;
  class AsmStmtContext;
  class AsmInstrContext;
  class AsmOperandContext;
  class AsmLitContext;
  class AsmRegContext;
  class AsmVarContext;
  class AsmLabelContext; 

  class  RootContext : public antlr4::ParserRuleContext {
  public:
    RootContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<UsingStmtContext *> usingStmt();
    UsingStmtContext* usingStmt(size_t i);
    std::vector<UsingAliasStmtContext *> usingAliasStmt();
    UsingAliasStmtContext* usingAliasStmt(size_t i);
    std::vector<NamespaceDeclContext *> namespaceDecl();
    NamespaceDeclContext* namespaceDecl(size_t i);
    std::vector<ClassDeclContext *> classDecl();
    ClassDeclContext* classDecl(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootContext* root();

  class  CompilerDirectiveContext : public antlr4::ParserRuleContext {
  public:
    CompilerDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Pound();
    antlr4::tree::TerminalNode *LineKw();
    antlr4::tree::TerminalNode *IntLit();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompilerDirectiveContext* compilerDirective();

  class  QualifiedIdContext : public antlr4::ParserRuleContext {
  public:
    QualifiedIdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Id();
    antlr4::tree::TerminalNode* Id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Dot();
    antlr4::tree::TerminalNode* Dot(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QualifiedIdContext* qualifiedId();

  class  UsingStmtContext : public antlr4::ParserRuleContext {
  public:
    UsingStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UsingKw();
    QualifiedIdContext *qualifiedId();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UsingStmtContext* usingStmt();

  class  UsingAliasStmtContext : public antlr4::ParserRuleContext {
  public:
    UsingAliasStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *UsingKw();
    QualifiedIdContext *qualifiedId();
    antlr4::tree::TerminalNode *AsKw();
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UsingAliasStmtContext* usingAliasStmt();

  class  NamespaceDeclContext : public antlr4::ParserRuleContext {
  public:
    NamespaceDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NamespaceKw();
    QualifiedIdContext *qualifiedId();
    NamespaceDefContext *namespaceDef();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamespaceDeclContext* namespaceDecl();

  class  NamespaceDefContext : public antlr4::ParserRuleContext {
  public:
    NamespaceDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<NamespaceDeclContext *> namespaceDecl();
    NamespaceDeclContext* namespaceDecl(size_t i);
    std::vector<ClassDeclContext *> classDecl();
    ClassDeclContext* classDecl(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamespaceDefContext* namespaceDef();

  class  ClassDeclContext : public antlr4::ParserRuleContext {
  public:
    ClassDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTypeContext *classType();
    antlr4::tree::TerminalNode *Id();
    ClassDefContext *classDef();
    AccessBaseSpecifierContext *accessBaseSpecifier();
    StorageSpecifierContext *storageSpecifier();
    ClassInheritanceContext *classInheritance();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassDeclContext* classDecl();

  class  ClassTypeContext : public antlr4::ParserRuleContext {
  public:
    ClassTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ClassKw();
    antlr4::tree::TerminalNode *InterfaceKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassTypeContext* classType();

  class  AccessBaseSpecifierContext : public antlr4::ParserRuleContext {
  public:
    AccessBaseSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PublicKw();
    antlr4::tree::TerminalNode *ProtectedKw();
    antlr4::tree::TerminalNode *PrivateKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AccessBaseSpecifierContext* accessBaseSpecifier();

  class  AccessLimitSpecifierContext : public antlr4::ParserRuleContext {
  public:
    AccessLimitSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LimitedKw();
    antlr4::tree::TerminalNode *LeftBracket();
    std::vector<QualifiedIdContext *> qualifiedId();
    QualifiedIdContext* qualifiedId(size_t i);
    antlr4::tree::TerminalNode *RightBracket();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AccessLimitSpecifierContext* accessLimitSpecifier();

  class  AccessSpecifierContext : public antlr4::ParserRuleContext {
  public:
    AccessSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AccessBaseSpecifierContext *accessBaseSpecifier();
    AccessLimitSpecifierContext *accessLimitSpecifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AccessSpecifierContext* accessSpecifier();

  class  StorageSpecifierContext : public antlr4::ParserRuleContext {
  public:
    StorageSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *StaticKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StorageSpecifierContext* storageSpecifier();

  class  FunctionSpecifierContext : public antlr4::ParserRuleContext {
  public:
    FunctionSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *InlineKw();
    antlr4::tree::TerminalNode *VirtualKw();
    antlr4::tree::TerminalNode *NativeKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionSpecifierContext* functionSpecifier();

  class  ConstQualifierContext : public antlr4::ParserRuleContext {
  public:
    ConstQualifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ConstKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstQualifierContext* constQualifier();

  class  MethodDeclSpecSeqContext : public antlr4::ParserRuleContext {
  public:
    MethodDeclSpecSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AccessSpecifierContext *accessSpecifier();
    StorageSpecifierContext *storageSpecifier();
    FunctionSpecifierContext *functionSpecifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodDeclSpecSeqContext* methodDeclSpecSeq();

  class  ClassVarDeclSpecSeqContext : public antlr4::ParserRuleContext {
  public:
    ClassVarDeclSpecSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AccessSpecifierContext *accessSpecifier();
    StorageSpecifierContext *storageSpecifier();
    ConstQualifierContext *constQualifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassVarDeclSpecSeqContext* classVarDeclSpecSeq();

  class  LocalVarDeclSpecSeqContext : public antlr4::ParserRuleContext {
  public:
    LocalVarDeclSpecSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StorageSpecifierContext *storageSpecifier();
    ConstQualifierContext *constQualifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocalVarDeclSpecSeqContext* localVarDeclSpecSeq();

  class  ArgVarDeclSpecSeqContext : public antlr4::ParserRuleContext {
  public:
    ArgVarDeclSpecSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstQualifierContext *constQualifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgVarDeclSpecSeqContext* argVarDeclSpecSeq();

  class  ClassInheritanceContext : public antlr4::ParserRuleContext {
  public:
    ClassInheritanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassExtendsContext *classExtends();
    ClassImplsContext *classImpls();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassInheritanceContext* classInheritance();

  class  ClassExtendsContext : public antlr4::ParserRuleContext {
  public:
    ClassExtendsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ExtendsKw();
    QualifiedIdContext *qualifiedId();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassExtendsContext* classExtends();

  class  ClassImplsContext : public antlr4::ParserRuleContext {
  public:
    ClassImplsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ImplementsKw();
    std::vector<QualifiedIdContext *> qualifiedId();
    QualifiedIdContext* qualifiedId(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassImplsContext* classImpls();

  class  ClassDefContext : public antlr4::ParserRuleContext {
  public:
    ClassDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<ClassDeclContext *> classDecl();
    ClassDeclContext* classDecl(size_t i);
    std::vector<ClassVarDeclStmtContext *> classVarDeclStmt();
    ClassVarDeclStmtContext* classVarDeclStmt(size_t i);
    std::vector<MethodDeclContext *> methodDecl();
    MethodDeclContext* methodDecl(size_t i);
    std::vector<CtorDeclContext *> ctorDecl();
    CtorDeclContext* ctorDecl(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassDefContext* classDef();

  class  ArrayTypeDeclContext : public antlr4::ParserRuleContext {
  public:
    ArrayTypeDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBracket();
    antlr4::tree::TerminalNode *RightBracket();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayTypeDeclContext* arrayTypeDecl();

  class  ArrayTypeDefContext : public antlr4::ParserRuleContext {
  public:
    ArrayTypeDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBracket();
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *RightBracket();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayTypeDefContext* arrayTypeDef();

  class  TypeNameDeclContext : public antlr4::ParserRuleContext {
  public:
    TypeNameDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    ArrayTypeDeclContext *arrayTypeDecl();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeNameDeclContext* typeNameDecl();

  class  BracedInitializerContext : public antlr4::ParserRuleContext {
  public:
    BracedInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BracedInitializerContext* bracedInitializer();

  class  CtorCallContext : public antlr4::ParserRuleContext {
  public:
    CtorCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CtorCallContext* ctorCall();

  class  ClassVarDeclStmtContext : public antlr4::ParserRuleContext {
  public:
    ClassVarDeclStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassVarDeclSpecSeqContext *classVarDeclSpecSeq();
    VarDeclStmtContext *varDeclStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassVarDeclStmtContext* classVarDeclStmt();

  class  LocalVarDeclStmtContext : public antlr4::ParserRuleContext {
  public:
    LocalVarDeclStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LocalVarDeclSpecSeqContext *localVarDeclSpecSeq();
    VarDeclStmtContext *varDeclStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocalVarDeclStmtContext* localVarDeclStmt();

  class  VarDeclStmtContext : public antlr4::ParserRuleContext {
  public:
    VarDeclStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarDeclContext *varDecl();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarDeclStmtContext* varDeclStmt();

  class  VarDeclContext : public antlr4::ParserRuleContext {
  public:
    VarDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeNameDeclContext *typeNameDecl();
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Assign();
    ExprContext *expr();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);
    std::vector<NextVarDeclContext *> nextVarDecl();
    NextVarDeclContext* nextVarDecl(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarDeclContext* varDecl();

  class  NextVarDeclContext : public antlr4::ParserRuleContext {
  public:
    NextVarDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Assign();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NextVarDeclContext* nextVarDecl();

  class  CtrlFlowExprContext : public antlr4::ParserRuleContext {
  public:
    CtrlFlowExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarDeclContext *varDecl();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CtrlFlowExprContext* ctrlFlowExpr();

  class  ArrayParamSeqContext : public antlr4::ParserRuleContext {
  public:
    ArrayParamSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ArrayParamElemContext *> arrayParamElem();
    ArrayParamElemContext* arrayParamElem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayParamSeqContext* arrayParamSeq();

  class  ArrayParamElemContext : public antlr4::ParserRuleContext {
  public:
    ArrayParamElemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<ArrayParamSeqContext *> arrayParamSeq();
    ArrayParamSeqContext* arrayParamSeq(size_t i);
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayParamElemContext* arrayParamElem();

  class  LambdaDeclContext : public antlr4::ParserRuleContext {
  public:
    LambdaDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LambdaKw();
    TypeNameDeclContext *typeNameDecl();
    std::vector<antlr4::tree::TerminalNode *> Id();
    antlr4::tree::TerminalNode* Id(size_t i);
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    CodeBlockContext *codeBlock();
    MethodArgSeqContext *methodArgSeq();
    antlr4::tree::TerminalNode *CaptureKw();
    antlr4::tree::TerminalNode *LeftBracket();
    antlr4::tree::TerminalNode *RightBracket();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LambdaDeclContext* lambdaDecl();

  class  CtorDeclContext : public antlr4::ParserRuleContext {
  public:
    CtorDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MethodDeclSpecSeqContext *methodDeclSpecSeq();
    antlr4::tree::TerminalNode *ConstructorKw();
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    CodeBlockContext *codeBlock();
    MethodArgSeqContext *methodArgSeq();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CtorDeclContext* ctorDecl();

  class  MethodDeclContext : public antlr4::ParserRuleContext {
  public:
    MethodDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MethodDeclSpecSeqContext *methodDeclSpecSeq();
    TypeNameDeclContext *typeNameDecl();
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    CodeBlockContext *codeBlock();
    antlr4::tree::TerminalNode *Term();
    MethodArgSeqContext *methodArgSeq();
    ConstQualifierContext *constQualifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodDeclContext* methodDecl();

  class  MethodQualifierContext : public antlr4::ParserRuleContext {
  public:
    MethodQualifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ConstKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodQualifierContext* methodQualifier();

  class  MethodStorageModifierContext : public antlr4::ParserRuleContext {
  public:
    MethodStorageModifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *InlineKw();
    antlr4::tree::TerminalNode *NativeKw();
    antlr4::tree::TerminalNode *VirtualKw();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodStorageModifierContext* methodStorageModifier();

  class  MethodArgSeqContext : public antlr4::ParserRuleContext {
  public:
    MethodArgSeqContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MethodArgContext *> methodArg();
    MethodArgContext* methodArg(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodArgSeqContext* methodArgSeq();

  class  MethodArgContext : public antlr4::ParserRuleContext {
  public:
    MethodArgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ArgVarDeclSpecSeqContext *argVarDeclSpecSeq();
    TypeNameDeclContext *typeNameDecl();
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Assign();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodArgContext* methodArg();

  class  CodeBlockContext : public antlr4::ParserRuleContext {
  public:
    CodeBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<CodeBlockStmtContext *> codeBlockStmt();
    CodeBlockStmtContext* codeBlockStmt(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CodeBlockContext* codeBlock();

  class  CodeBlockStmtContext : public antlr4::ParserRuleContext {
  public:
    CodeBlockStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CodeBlockContext *codeBlock();
    LocalVarDeclStmtContext *localVarDeclStmt();
    ExprStmtContext *exprStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CodeBlockStmtContext* codeBlockStmt();

  class  CodeStmtContext : public antlr4::ParserRuleContext {
  public:
    CodeStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CodeBlockContext *codeBlock();
    ExprStmtContext *exprStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CodeStmtContext* codeStmt();

  class  ExprStmtContext : public antlr4::ParserRuleContext {
  public:
    ExprStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Term();
    ExprContext *expr();
    ReturnStmtContext *returnStmt();
    ConditionStmtContext *conditionStmt();
    LoopStmtContext *loopStmt();
    ExceptionHandlerContext *exceptionHandler();
    LambdaDeclContext *lambdaDecl();
    AsmBlockContext *asmBlock();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExprStmtContext* exprStmt();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() = default;
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ExprIntrinsicContext : public ExprContext {
  public:
    ExprIntrinsicContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *CopyIntrinKw();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprEqualityContext : public ExprContext {
  public:
    ExprEqualityContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LessThan();
    antlr4::tree::TerminalNode *LessThanOrEqualTo();
    antlr4::tree::TerminalNode *GreaterThan();
    antlr4::tree::TerminalNode *GreaterThanOrEqualTo();
    antlr4::tree::TerminalNode *EqualTo();
    antlr4::tree::TerminalNode *NotEqualTo();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprControlFlowContext : public ExprContext {
  public:
    ExprControlFlowContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *BreakKw();
    antlr4::tree::TerminalNode *ContinueKw();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprLiteralContext : public ExprContext {
  public:
    ExprLiteralContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *NullLit();
    antlr4::tree::TerminalNode *BoolLit();
    antlr4::tree::TerminalNode *RealLit();
    antlr4::tree::TerminalNode *IntLit();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprDotContext : public ExprContext {
  public:
    ExprDotContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *Dot();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprArithmeticContext : public ExprContext {
  public:
    ExprArithmeticContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *Mult();
    antlr4::tree::TerminalNode *Div();
    antlr4::tree::TerminalNode *Mod();
    antlr4::tree::TerminalNode *Add();
    antlr4::tree::TerminalNode *Sub();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprParenContext : public ExprContext {
  public:
    ExprParenContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *LeftParen();
    ExprContext *expr();
    antlr4::tree::TerminalNode *RightParen();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprUnaryPostfixContext : public ExprContext {
  public:
    ExprUnaryPostfixContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *Incr();
    antlr4::tree::TerminalNode *Decr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprInstantiateContext : public ExprContext {
  public:
    ExprInstantiateContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *NewKw();
    antlr4::tree::TerminalNode *Id();
    CtorCallContext *ctorCall();
    ArrayTypeDefContext *arrayTypeDef();
    ArrayTypeDeclContext *arrayTypeDecl();
    BracedInitializerContext *bracedInitializer();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprExponentContext : public ExprContext {
  public:
    ExprExponentContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *Expo();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprCallContext : public ExprContext {
  public:
    ExprCallContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprIdContext : public ExprContext {
  public:
    ExprIdContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *Id();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprAssignmentContext : public ExprContext {
  public:
    ExprAssignmentContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *Assign();
    antlr4::tree::TerminalNode *ExpoAssign();
    antlr4::tree::TerminalNode *AddAssign();
    antlr4::tree::TerminalNode *SubAssign();
    antlr4::tree::TerminalNode *MultAssign();
    antlr4::tree::TerminalNode *DivAssign();
    antlr4::tree::TerminalNode *ModAssign();
    antlr4::tree::TerminalNode *LeftRotateAssign();
    antlr4::tree::TerminalNode *RightRotateAssign();
    antlr4::tree::TerminalNode *LeftShiftAssign();
    antlr4::tree::TerminalNode *RightShiftAssign();
    antlr4::tree::TerminalNode *BitAndAssign();
    antlr4::tree::TerminalNode *BitXorAssign();
    antlr4::tree::TerminalNode *BitOrAssign();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprIndexContext : public ExprContext {
  public:
    ExprIndexContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LeftBracket();
    antlr4::tree::TerminalNode *RightBracket();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprLogicContext : public ExprContext {
  public:
    ExprLogicContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LogicAnd();
    antlr4::tree::TerminalNode *LogicOr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprArrayContext : public ExprContext {
  public:
    ExprArrayContext(ExprContext *ctx);

    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<ArrayParamSeqContext *> arrayParamSeq();
    ArrayParamSeqContext* arrayParamSeq(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprBitwiseContext : public ExprContext {
  public:
    ExprBitwiseContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *BitAnd();
    antlr4::tree::TerminalNode *BitXor();
    antlr4::tree::TerminalNode *BitOr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprUnaryPrefixContext : public ExprContext {
  public:
    ExprUnaryPrefixContext(ExprContext *ctx);

    ExprContext *expr();
    antlr4::tree::TerminalNode *Incr();
    antlr4::tree::TerminalNode *Decr();
    antlr4::tree::TerminalNode *Sub();
    antlr4::tree::TerminalNode *Excl();
    antlr4::tree::TerminalNode *Tilde();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprStrLiteralContext : public ExprContext {
  public:
    ExprStrLiteralContext(ExprContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> StrLit();
    antlr4::tree::TerminalNode* StrLit(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExprBitShiftContext : public ExprContext {
  public:
    ExprBitShiftContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    antlr4::tree::TerminalNode *LeftRotate();
    antlr4::tree::TerminalNode *RightRotate();
    antlr4::tree::TerminalNode *LeftShift();
    antlr4::tree::TerminalNode *RightShift();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  ReturnStmtContext : public antlr4::ParserRuleContext {
  public:
    ReturnStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ReturnKw();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnStmtContext* returnStmt();

  class  LoopStmtContext : public antlr4::ParserRuleContext {
  public:
    LoopStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LoopWhileDefContext *loopWhileDef();
    LoopDoWhileDefContext *loopDoWhileDef();
    LoopForDefContext *loopForDef();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoopStmtContext* loopStmt();

  class  LoopWhileDefContext : public antlr4::ParserRuleContext {
  public:
    LoopWhileDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WhileKw();
    antlr4::tree::TerminalNode *LeftParen();
    CtrlFlowExprContext *ctrlFlowExpr();
    antlr4::tree::TerminalNode *RightParen();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoopWhileDefContext* loopWhileDef();

  class  LoopDoWhileDefContext : public antlr4::ParserRuleContext {
  public:
    LoopDoWhileDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DoKw();
    CodeStmtContext *codeStmt();
    antlr4::tree::TerminalNode *WhileKw();
    antlr4::tree::TerminalNode *LeftParen();
    CtrlFlowExprContext *ctrlFlowExpr();
    antlr4::tree::TerminalNode *RightParen();
    antlr4::tree::TerminalNode *Term();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoopDoWhileDefContext* loopDoWhileDef();

  class  LoopForDefContext : public antlr4::ParserRuleContext {
  public:
    LoopForDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ForKw();
    antlr4::tree::TerminalNode *LeftParen();
    std::vector<antlr4::tree::TerminalNode *> Term();
    antlr4::tree::TerminalNode* Term(size_t i);
    antlr4::tree::TerminalNode *RightParen();
    CodeStmtContext *codeStmt();
    std::vector<CtrlFlowExprContext *> ctrlFlowExpr();
    CtrlFlowExprContext* ctrlFlowExpr(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LoopForDefContext* loopForDef();

  class  ConditionStmtContext : public antlr4::ParserRuleContext {
  public:
    ConditionStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CondIfDefContext *condIfDef();
    std::vector<CondElifDefContext *> condElifDef();
    CondElifDefContext* condElifDef(size_t i);
    CondElseDefContext *condElseDef();
    CondSwitchDefContext *condSwitchDef();
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<CondCaseDefContext *> condCaseDef();
    CondCaseDefContext* condCaseDef(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionStmtContext* conditionStmt();

  class  CondIfDefContext : public antlr4::ParserRuleContext {
  public:
    CondIfDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IfKw();
    antlr4::tree::TerminalNode *LeftParen();
    CtrlFlowExprContext *ctrlFlowExpr();
    antlr4::tree::TerminalNode *RightParen();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CondIfDefContext* condIfDef();

  class  CondElifDefContext : public antlr4::ParserRuleContext {
  public:
    CondElifDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ElifKw();
    antlr4::tree::TerminalNode *LeftParen();
    CtrlFlowExprContext *ctrlFlowExpr();
    antlr4::tree::TerminalNode *RightParen();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CondElifDefContext* condElifDef();

  class  CondElseDefContext : public antlr4::ParserRuleContext {
  public:
    CondElseDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ElseKw();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CondElseDefContext* condElseDef();

  class  CondSwitchDefContext : public antlr4::ParserRuleContext {
  public:
    CondSwitchDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SwitchKw();
    antlr4::tree::TerminalNode *LeftParen();
    CtrlFlowExprContext *ctrlFlowExpr();
    antlr4::tree::TerminalNode *RightParen();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CondSwitchDefContext* condSwitchDef();

  class  CondCaseDefContext : public antlr4::ParserRuleContext {
  public:
    CondCaseDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *CaseKw();
    CondCaseResultContext *condCaseResult();
    antlr4::tree::TerminalNode *DefaultKw();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CondCaseDefContext* condCaseDef();

  class  CondCaseResultContext : public antlr4::ParserRuleContext {
  public:
    CondCaseResultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    CondCaseResultContext() = default;
    void copyFrom(CondCaseResultContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CaseIdContext : public CondCaseResultContext {
  public:
    CaseIdContext(CondCaseResultContext *ctx);

    antlr4::tree::TerminalNode *Id();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CaseLiteralContext : public CondCaseResultContext {
  public:
    CaseLiteralContext(CondCaseResultContext *ctx);

    antlr4::tree::TerminalNode *NullLit();
    antlr4::tree::TerminalNode *BoolLit();
    antlr4::tree::TerminalNode *RealLit();
    antlr4::tree::TerminalNode *IntLit();
    antlr4::tree::TerminalNode *StrLit();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  CondCaseResultContext* condCaseResult();

  class  ExceptionHandlerContext : public antlr4::ParserRuleContext {
  public:
    ExceptionHandlerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TryBlockDefContext *tryBlockDef();
    std::vector<CatchBlockDefContext *> catchBlockDef();
    CatchBlockDefContext* catchBlockDef(size_t i);
    FinallyBlockDefContext *finallyBlockDef();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExceptionHandlerContext* exceptionHandler();

  class  TryBlockDefContext : public antlr4::ParserRuleContext {
  public:
    TryBlockDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TryKw();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TryBlockDefContext* tryBlockDef();

  class  CatchBlockDefContext : public antlr4::ParserRuleContext {
  public:
    CatchBlockDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CatchKw();
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    CodeStmtContext *codeStmt();
    CtrlFlowExprContext *ctrlFlowExpr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CatchBlockDefContext* catchBlockDef();

  class  FinallyBlockDefContext : public antlr4::ParserRuleContext {
  public:
    FinallyBlockDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FinallyKw();
    CodeStmtContext *codeStmt();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FinallyBlockDefContext* finallyBlockDef();

  class  AsmBlockContext : public antlr4::ParserRuleContext {
  public:
    AsmBlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AsmIntrinKw();
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    std::vector<AsmStmtContext *> asmStmt();
    AsmStmtContext* asmStmt(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmBlockContext* asmBlock();

  class  AsmStmtContext : public antlr4::ParserRuleContext {
  public:
    AsmStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsmInstrContext *asmInstr();
    AsmLabelContext *asmLabel();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmStmtContext* asmStmt();

  class  AsmInstrContext : public antlr4::ParserRuleContext {
  public:
    AsmInstrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    std::vector<AsmOperandContext *> asmOperand();
    AsmOperandContext* asmOperand(size_t i);
    antlr4::tree::TerminalNode *Term();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmInstrContext* asmInstr();

  class  AsmOperandContext : public antlr4::ParserRuleContext {
  public:
    AsmOperandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsmLitContext *asmLit();
    AsmRegContext *asmReg();
    AsmVarContext *asmVar();
    AsmLabelContext *asmLabel();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmOperandContext* asmOperand();

  class  AsmLitContext : public antlr4::ParserRuleContext {
  public:
    AsmLitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NullLit();
    antlr4::tree::TerminalNode *BoolLit();
    antlr4::tree::TerminalNode *RealLit();
    antlr4::tree::TerminalNode *IntLit();
    antlr4::tree::TerminalNode *StrLit();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmLitContext* asmLit();

  class  AsmRegContext : public antlr4::ParserRuleContext {
  public:
    AsmRegContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AsmRegId();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmRegContext* asmReg();

  class  AsmVarContext : public antlr4::ParserRuleContext {
  public:
    AsmVarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBracket();
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *RightBracket();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmVarContext* asmVar();

  class  AsmLabelContext : public antlr4::ParserRuleContext {
  public:
    AsmLabelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *Id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmLabelContext* asmLabel();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

