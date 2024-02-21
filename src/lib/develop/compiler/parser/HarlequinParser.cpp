
// Generated from D:/dev/oss/Harlequin/Harlequin.g4 by ANTLR 4.13.1


#include "HarlequinVisitor.h"

#include "HarlequinParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct HarlequinParserStaticData final {
  HarlequinParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  HarlequinParserStaticData(const HarlequinParserStaticData&) = delete;
  HarlequinParserStaticData(HarlequinParserStaticData&&) = delete;
  HarlequinParserStaticData& operator=(const HarlequinParserStaticData&) = delete;
  HarlequinParserStaticData& operator=(HarlequinParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag harlequinParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
HarlequinParserStaticData *harlequinParserStaticData = nullptr;

void harlequinParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (harlequinParserStaticData != nullptr) {
    return;
  }
#else
  assert(harlequinParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<HarlequinParserStaticData>(
    std::vector<std::string>{
      "root", "itemDef", "compilerDirective", "qualifiedId", "usingStmt", 
      "usingAliasStmt", "namespaceDecl", "namespaceDef", "classDecl", "classType", 
      "accessBaseSpecifier", "accessLimitSpecifier", "accessSpecifier", 
      "storageSpecifier", "functionSpecifier", "constQualifier", "methodDeclSpecSeq", 
      "classVarDeclSpecSeq", "localVarDeclSpecSeq", "argVarDeclSpecSeq", 
      "classInheritance", "classExtends", "classImpls", "classDef", "arrayTypeDecl", 
      "arrayTypeDef", "typeNameDecl", "bracedInitializer", "ctorCall", "classVarDeclStmt", 
      "localVarDeclStmt", "varDeclStmt", "varDecl", "nextVarDecl", "ctrlFlowExpr", 
      "arrayParamSeq", "arrayParamElem", "lambdaDecl", "ctorDecl", "methodDecl", 
      "methodQualifier", "methodStorageModifier", "methodArgSeq", "methodArg", 
      "codeBlock", "codeBlockStmt", "codeStmt", "exprStmt", "expr", "returnStmt", 
      "loopStmt", "loopWhileDef", "loopDoWhileDef", "loopForDef", "conditionStmt", 
      "condIfDef", "condElifDef", "condElseDef", "condSwitchDef", "condCaseDef", 
      "condCaseResult", "exceptionHandler", "tryBlockDef", "catchBlockDef", 
      "finallyBlockDef", "asmBlock", "asmStmt", "asmInstr", "asmOperand", 
      "asmLit", "asmReg", "asmVar", "asmLabel"
    },
    std::vector<std::string>{
      "", "'('", "')'", "'['", "']'", "'{'", "'}'", "'namespace'", "'class'", 
      "'interface'", "'using'", "'as'", "'static'", "'inline'", "'const'", 
      "'virtual'", "'native'", "'extends'", "'implements'", "'return'", 
      "'if'", "'elif'", "'else'", "'for'", "'do'", "'while'", "'break'", 
      "'continue'", "'switch'", "'case'", "'default'", "'try'", "'catch'", 
      "'finally'", "'lambda'", "'capture'", "'constructor'", "'public'", 
      "'protected'", "'private'", "'internal'", "'limited'", "'new'", "'__asm'", 
      "'__copy'", "'+'", "'-'", "'*'", "'/'", "'%'", "'<<'", "'>>'", "'<<<'", 
      "'>>>'", "'&'", "'|'", "'^'", "'='", "'+='", "'-='", "'*='", "'/='", 
      "'%='", "'<<='", "'>>='", "'<<<='", "'>>>='", "'&='", "'|='", "'^='", 
      "'&&'", "'||'", "'=='", "'!='", "'<'", "'<='", "'>'", "'>='", "'**'", 
      "'**='", "'++'", "'--'", "'!'", "'~'", "'.'", "','", "':'", "'#'", 
      "'line'", "'null'", "", "", "", "", "", "", "';'"
    },
    std::vector<std::string>{
      "", "LeftParen", "RightParen", "LeftBracket", "RightBracket", "LeftBrace", 
      "RightBrace", "NamespaceKw", "ClassKw", "InterfaceKw", "UsingKw", 
      "AsKw", "StaticKw", "InlineKw", "ConstKw", "VirtualKw", "NativeKw", 
      "ExtendsKw", "ImplementsKw", "ReturnKw", "IfKw", "ElifKw", "ElseKw", 
      "ForKw", "DoKw", "WhileKw", "BreakKw", "ContinueKw", "SwitchKw", "CaseKw", 
      "DefaultKw", "TryKw", "CatchKw", "FinallyKw", "LambdaKw", "CaptureKw", 
      "ConstructorKw", "PublicKw", "ProtectedKw", "PrivateKw", "InternalKw", 
      "LimitedKw", "NewKw", "AsmIntrinKw", "CopyIntrinKw", "Add", "Sub", 
      "Mult", "Div", "Mod", "LeftShift", "RightShift", "LeftRotate", "RightRotate", 
      "BitAnd", "BitOr", "BitXor", "Assign", "AddAssign", "SubAssign", "MultAssign", 
      "DivAssign", "ModAssign", "LeftShiftAssign", "RightShiftAssign", "LeftRotateAssign", 
      "RightRotateAssign", "BitAndAssign", "BitOrAssign", "BitXorAssign", 
      "LogicAnd", "LogicOr", "EqualTo", "NotEqualTo", "LessThan", "LessThanOrEqualTo", 
      "GreaterThan", "GreaterThanOrEqualTo", "Expo", "ExpoAssign", "Incr", 
      "Decr", "Excl", "Tilde", "Dot", "Comma", "Colon", "Pound", "LineKw", 
      "NullLit", "BoolLit", "AsmRegId", "IntLit", "RealLit", "StrLit", "Id", 
      "Term", "LineComment", "BlockComment", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,99,769,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,2,72,7,72,1,0,5,0,148,8,0,10,0,12,0,151,9,0,1,0,1,0,1,1,
  	1,1,1,1,1,1,3,1,159,8,1,1,2,1,2,1,2,1,2,1,3,1,3,1,3,5,3,168,8,3,10,3,
  	12,3,171,9,3,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,5,5,183,8,5,10,5,
  	12,5,186,9,5,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,7,5,7,197,8,7,10,7,12,
  	7,200,9,7,1,7,1,7,1,8,3,8,205,8,8,1,8,3,8,208,8,8,1,8,1,8,1,8,3,8,213,
  	8,8,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,11,1,11,1,11,5,11,226,8,11,
  	10,11,12,11,229,9,11,1,11,1,11,1,12,1,12,3,12,235,8,12,1,12,1,12,3,12,
  	239,8,12,3,12,241,8,12,1,13,1,13,1,14,1,14,1,15,1,15,1,16,3,16,250,8,
  	16,1,16,3,16,253,8,16,1,16,3,16,256,8,16,1,17,3,17,259,8,17,1,17,3,17,
  	262,8,17,1,17,3,17,265,8,17,1,18,3,18,268,8,18,1,18,3,18,271,8,18,1,19,
  	3,19,274,8,19,1,20,1,20,3,20,278,8,20,1,20,1,20,3,20,282,8,20,3,20,284,
  	8,20,1,21,1,21,1,21,1,22,1,22,1,22,1,22,5,22,293,8,22,10,22,12,22,296,
  	9,22,1,23,1,23,1,23,1,23,5,23,302,8,23,10,23,12,23,305,9,23,1,23,1,23,
  	1,24,1,24,3,24,311,8,24,1,24,3,24,314,8,24,1,24,1,24,1,25,1,25,1,25,1,
  	25,3,25,322,8,25,1,25,1,25,3,25,326,8,25,1,25,1,25,1,26,1,26,3,26,332,
  	8,26,1,27,1,27,3,27,336,8,27,1,27,1,27,1,28,1,28,3,28,342,8,28,1,28,1,
  	28,1,29,1,29,1,29,1,30,1,30,1,30,1,31,1,31,1,31,1,32,1,32,1,32,1,32,3,
  	32,359,8,32,1,32,1,32,5,32,363,8,32,10,32,12,32,366,9,32,1,33,1,33,1,
  	33,3,33,371,8,33,1,34,1,34,3,34,375,8,34,1,35,1,35,1,35,5,35,380,8,35,
  	10,35,12,35,383,9,35,1,35,3,35,386,8,35,1,36,1,36,5,36,390,8,36,10,36,
  	12,36,393,9,36,1,36,1,36,3,36,397,8,36,1,37,1,37,1,37,1,37,1,37,3,37,
  	404,8,37,1,37,1,37,1,37,1,37,1,37,1,37,5,37,412,8,37,10,37,12,37,415,
  	9,37,1,37,3,37,418,8,37,1,37,3,37,421,8,37,1,37,1,37,1,38,1,38,1,38,1,
  	38,3,38,429,8,38,1,38,1,38,1,38,1,39,1,39,1,39,1,39,1,39,3,39,439,8,39,
  	1,39,1,39,3,39,443,8,39,1,39,1,39,3,39,447,8,39,1,40,1,40,1,41,1,41,1,
  	42,1,42,1,42,5,42,456,8,42,10,42,12,42,459,9,42,1,42,3,42,462,8,42,1,
  	43,1,43,1,43,1,43,1,43,3,43,469,8,43,3,43,471,8,43,1,44,1,44,5,44,475,
  	8,44,10,44,12,44,478,9,44,1,44,1,44,1,45,1,45,1,45,3,45,485,8,45,1,46,
  	1,46,3,46,489,8,46,1,47,1,47,3,47,493,8,47,1,47,1,47,1,47,1,47,1,47,1,
  	47,3,47,501,8,47,1,48,1,48,1,48,1,48,1,48,1,48,1,48,5,48,510,8,48,10,
  	48,12,48,513,9,48,1,48,1,48,1,48,1,48,1,48,3,48,520,8,48,1,48,3,48,523,
  	8,48,1,48,1,48,3,48,527,8,48,1,48,3,48,530,8,48,1,48,1,48,1,48,1,48,4,
  	48,536,8,48,11,48,12,48,537,1,48,3,48,541,8,48,1,48,1,48,1,48,1,48,1,
  	48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,
  	48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,
  	48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,1,48,5,
  	48,589,8,48,10,48,12,48,592,9,48,3,48,594,8,48,1,48,1,48,1,48,1,48,1,
  	48,1,48,5,48,602,8,48,10,48,12,48,605,9,48,1,49,1,49,1,49,1,50,1,50,1,
  	50,3,50,613,8,50,1,51,1,51,1,51,1,51,1,51,1,51,1,52,1,52,1,52,1,52,1,
  	52,1,52,1,52,1,52,1,53,1,53,1,53,3,53,632,8,53,1,53,1,53,3,53,636,8,53,
  	1,53,1,53,3,53,640,8,53,1,53,1,53,1,53,1,54,1,54,5,54,647,8,54,10,54,
  	12,54,650,9,54,1,54,3,54,653,8,54,1,54,1,54,1,54,5,54,658,8,54,10,54,
  	12,54,661,9,54,1,54,1,54,3,54,665,8,54,1,55,1,55,1,55,1,55,1,55,1,55,
  	1,56,1,56,1,56,1,56,1,56,1,56,1,57,1,57,1,57,1,58,1,58,1,58,1,58,1,58,
  	1,59,1,59,1,59,3,59,690,8,59,1,59,1,59,3,59,694,8,59,1,60,1,60,3,60,698,
  	8,60,1,61,1,61,5,61,702,8,61,10,61,12,61,705,9,61,1,61,3,61,708,8,61,
  	1,62,1,62,1,62,1,63,1,63,1,63,3,63,716,8,63,1,63,1,63,1,63,1,64,1,64,
  	1,64,1,65,1,65,1,65,5,65,727,8,65,10,65,12,65,730,9,65,1,65,1,65,1,66,
  	1,66,3,66,736,8,66,1,67,1,67,1,67,1,67,5,67,742,8,67,10,67,12,67,745,
  	9,67,3,67,747,8,67,1,67,3,67,750,8,67,1,68,1,68,1,68,1,68,3,68,756,8,
  	68,1,69,1,69,1,70,1,70,1,71,1,71,1,71,1,71,1,72,1,72,1,72,1,72,0,1,96,
  	73,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,
  	48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,
  	94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,
  	130,132,134,136,138,140,142,144,0,14,1,0,8,9,1,0,37,39,2,0,13,13,15,16,
  	2,0,46,46,80,83,1,0,26,27,2,0,89,90,92,93,1,0,47,49,1,0,45,46,1,0,50,
  	53,1,0,74,77,1,0,72,73,2,0,57,69,79,79,1,0,80,81,2,0,89,90,92,94,820,
  	0,149,1,0,0,0,2,158,1,0,0,0,4,160,1,0,0,0,6,164,1,0,0,0,8,172,1,0,0,0,
  	10,176,1,0,0,0,12,189,1,0,0,0,14,193,1,0,0,0,16,204,1,0,0,0,18,216,1,
  	0,0,0,20,218,1,0,0,0,22,220,1,0,0,0,24,240,1,0,0,0,26,242,1,0,0,0,28,
  	244,1,0,0,0,30,246,1,0,0,0,32,249,1,0,0,0,34,258,1,0,0,0,36,267,1,0,0,
  	0,38,273,1,0,0,0,40,283,1,0,0,0,42,285,1,0,0,0,44,288,1,0,0,0,46,297,
  	1,0,0,0,48,308,1,0,0,0,50,317,1,0,0,0,52,329,1,0,0,0,54,333,1,0,0,0,56,
  	339,1,0,0,0,58,345,1,0,0,0,60,348,1,0,0,0,62,351,1,0,0,0,64,354,1,0,0,
  	0,66,367,1,0,0,0,68,374,1,0,0,0,70,376,1,0,0,0,72,396,1,0,0,0,74,398,
  	1,0,0,0,76,424,1,0,0,0,78,433,1,0,0,0,80,448,1,0,0,0,82,450,1,0,0,0,84,
  	452,1,0,0,0,86,463,1,0,0,0,88,472,1,0,0,0,90,484,1,0,0,0,92,488,1,0,0,
  	0,94,500,1,0,0,0,96,540,1,0,0,0,98,606,1,0,0,0,100,612,1,0,0,0,102,614,
  	1,0,0,0,104,620,1,0,0,0,106,628,1,0,0,0,108,664,1,0,0,0,110,666,1,0,0,
  	0,112,672,1,0,0,0,114,678,1,0,0,0,116,681,1,0,0,0,118,689,1,0,0,0,120,
  	697,1,0,0,0,122,699,1,0,0,0,124,709,1,0,0,0,126,712,1,0,0,0,128,720,1,
  	0,0,0,130,723,1,0,0,0,132,735,1,0,0,0,134,737,1,0,0,0,136,755,1,0,0,0,
  	138,757,1,0,0,0,140,759,1,0,0,0,142,761,1,0,0,0,144,765,1,0,0,0,146,148,
  	3,2,1,0,147,146,1,0,0,0,148,151,1,0,0,0,149,147,1,0,0,0,149,150,1,0,0,
  	0,150,152,1,0,0,0,151,149,1,0,0,0,152,153,5,0,0,1,153,1,1,0,0,0,154,159,
  	3,8,4,0,155,159,3,10,5,0,156,159,3,12,6,0,157,159,3,16,8,0,158,154,1,
  	0,0,0,158,155,1,0,0,0,158,156,1,0,0,0,158,157,1,0,0,0,159,3,1,0,0,0,160,
  	161,5,87,0,0,161,162,5,88,0,0,162,163,5,92,0,0,163,5,1,0,0,0,164,169,
  	5,95,0,0,165,166,5,84,0,0,166,168,5,95,0,0,167,165,1,0,0,0,168,171,1,
  	0,0,0,169,167,1,0,0,0,169,170,1,0,0,0,170,7,1,0,0,0,171,169,1,0,0,0,172,
  	173,5,10,0,0,173,174,3,6,3,0,174,175,5,96,0,0,175,9,1,0,0,0,176,177,5,
  	10,0,0,177,178,3,6,3,0,178,179,5,11,0,0,179,184,5,95,0,0,180,181,5,85,
  	0,0,181,183,5,95,0,0,182,180,1,0,0,0,183,186,1,0,0,0,184,182,1,0,0,0,
  	184,185,1,0,0,0,185,187,1,0,0,0,186,184,1,0,0,0,187,188,5,96,0,0,188,
  	11,1,0,0,0,189,190,5,7,0,0,190,191,3,6,3,0,191,192,3,14,7,0,192,13,1,
  	0,0,0,193,198,5,5,0,0,194,197,3,12,6,0,195,197,3,16,8,0,196,194,1,0,0,
  	0,196,195,1,0,0,0,197,200,1,0,0,0,198,196,1,0,0,0,198,199,1,0,0,0,199,
  	201,1,0,0,0,200,198,1,0,0,0,201,202,5,6,0,0,202,15,1,0,0,0,203,205,3,
  	20,10,0,204,203,1,0,0,0,204,205,1,0,0,0,205,207,1,0,0,0,206,208,3,26,
  	13,0,207,206,1,0,0,0,207,208,1,0,0,0,208,209,1,0,0,0,209,210,3,18,9,0,
  	210,212,5,95,0,0,211,213,3,40,20,0,212,211,1,0,0,0,212,213,1,0,0,0,213,
  	214,1,0,0,0,214,215,3,46,23,0,215,17,1,0,0,0,216,217,7,0,0,0,217,19,1,
  	0,0,0,218,219,7,1,0,0,219,21,1,0,0,0,220,221,5,41,0,0,221,222,5,3,0,0,
  	222,227,3,6,3,0,223,224,5,85,0,0,224,226,3,6,3,0,225,223,1,0,0,0,226,
  	229,1,0,0,0,227,225,1,0,0,0,227,228,1,0,0,0,228,230,1,0,0,0,229,227,1,
  	0,0,0,230,231,5,4,0,0,231,23,1,0,0,0,232,234,3,20,10,0,233,235,3,22,11,
  	0,234,233,1,0,0,0,234,235,1,0,0,0,235,241,1,0,0,0,236,238,3,22,11,0,237,
  	239,3,20,10,0,238,237,1,0,0,0,238,239,1,0,0,0,239,241,1,0,0,0,240,232,
  	1,0,0,0,240,236,1,0,0,0,241,25,1,0,0,0,242,243,5,12,0,0,243,27,1,0,0,
  	0,244,245,7,2,0,0,245,29,1,0,0,0,246,247,5,14,0,0,247,31,1,0,0,0,248,
  	250,3,24,12,0,249,248,1,0,0,0,249,250,1,0,0,0,250,252,1,0,0,0,251,253,
  	3,26,13,0,252,251,1,0,0,0,252,253,1,0,0,0,253,255,1,0,0,0,254,256,3,28,
  	14,0,255,254,1,0,0,0,255,256,1,0,0,0,256,33,1,0,0,0,257,259,3,24,12,0,
  	258,257,1,0,0,0,258,259,1,0,0,0,259,261,1,0,0,0,260,262,3,26,13,0,261,
  	260,1,0,0,0,261,262,1,0,0,0,262,264,1,0,0,0,263,265,3,30,15,0,264,263,
  	1,0,0,0,264,265,1,0,0,0,265,35,1,0,0,0,266,268,3,26,13,0,267,266,1,0,
  	0,0,267,268,1,0,0,0,268,270,1,0,0,0,269,271,3,30,15,0,270,269,1,0,0,0,
  	270,271,1,0,0,0,271,37,1,0,0,0,272,274,3,30,15,0,273,272,1,0,0,0,273,
  	274,1,0,0,0,274,39,1,0,0,0,275,277,3,42,21,0,276,278,3,44,22,0,277,276,
  	1,0,0,0,277,278,1,0,0,0,278,284,1,0,0,0,279,281,3,44,22,0,280,282,3,42,
  	21,0,281,280,1,0,0,0,281,282,1,0,0,0,282,284,1,0,0,0,283,275,1,0,0,0,
  	283,279,1,0,0,0,284,41,1,0,0,0,285,286,5,17,0,0,286,287,3,6,3,0,287,43,
  	1,0,0,0,288,289,5,18,0,0,289,294,3,6,3,0,290,291,5,85,0,0,291,293,3,6,
  	3,0,292,290,1,0,0,0,293,296,1,0,0,0,294,292,1,0,0,0,294,295,1,0,0,0,295,
  	45,1,0,0,0,296,294,1,0,0,0,297,303,5,5,0,0,298,302,3,58,29,0,299,302,
  	3,78,39,0,300,302,3,76,38,0,301,298,1,0,0,0,301,299,1,0,0,0,301,300,1,
  	0,0,0,302,305,1,0,0,0,303,301,1,0,0,0,303,304,1,0,0,0,304,306,1,0,0,0,
  	305,303,1,0,0,0,306,307,5,6,0,0,307,47,1,0,0,0,308,310,5,3,0,0,309,311,
  	5,85,0,0,310,309,1,0,0,0,310,311,1,0,0,0,311,313,1,0,0,0,312,314,5,85,
  	0,0,313,312,1,0,0,0,313,314,1,0,0,0,314,315,1,0,0,0,315,316,5,4,0,0,316,
  	49,1,0,0,0,317,318,5,3,0,0,318,321,3,96,48,0,319,320,5,85,0,0,320,322,
  	3,96,48,0,321,319,1,0,0,0,321,322,1,0,0,0,322,325,1,0,0,0,323,324,5,85,
  	0,0,324,326,3,96,48,0,325,323,1,0,0,0,325,326,1,0,0,0,326,327,1,0,0,0,
  	327,328,5,4,0,0,328,51,1,0,0,0,329,331,5,95,0,0,330,332,3,48,24,0,331,
  	330,1,0,0,0,331,332,1,0,0,0,332,53,1,0,0,0,333,335,5,5,0,0,334,336,3,
  	96,48,0,335,334,1,0,0,0,335,336,1,0,0,0,336,337,1,0,0,0,337,338,5,6,0,
  	0,338,55,1,0,0,0,339,341,5,1,0,0,340,342,3,96,48,0,341,340,1,0,0,0,341,
  	342,1,0,0,0,342,343,1,0,0,0,343,344,5,2,0,0,344,57,1,0,0,0,345,346,3,
  	34,17,0,346,347,3,62,31,0,347,59,1,0,0,0,348,349,3,36,18,0,349,350,3,
  	62,31,0,350,61,1,0,0,0,351,352,3,64,32,0,352,353,5,96,0,0,353,63,1,0,
  	0,0,354,355,3,52,26,0,355,358,5,95,0,0,356,357,5,57,0,0,357,359,3,96,
  	48,0,358,356,1,0,0,0,358,359,1,0,0,0,359,364,1,0,0,0,360,361,5,85,0,0,
  	361,363,3,66,33,0,362,360,1,0,0,0,363,366,1,0,0,0,364,362,1,0,0,0,364,
  	365,1,0,0,0,365,65,1,0,0,0,366,364,1,0,0,0,367,370,5,95,0,0,368,369,5,
  	57,0,0,369,371,3,96,48,0,370,368,1,0,0,0,370,371,1,0,0,0,371,67,1,0,0,
  	0,372,375,3,64,32,0,373,375,3,96,48,0,374,372,1,0,0,0,374,373,1,0,0,0,
  	375,69,1,0,0,0,376,381,3,72,36,0,377,378,5,85,0,0,378,380,3,72,36,0,379,
  	377,1,0,0,0,380,383,1,0,0,0,381,379,1,0,0,0,381,382,1,0,0,0,382,385,1,
  	0,0,0,383,381,1,0,0,0,384,386,5,85,0,0,385,384,1,0,0,0,385,386,1,0,0,
  	0,386,71,1,0,0,0,387,391,5,5,0,0,388,390,3,70,35,0,389,388,1,0,0,0,390,
  	393,1,0,0,0,391,389,1,0,0,0,391,392,1,0,0,0,392,394,1,0,0,0,393,391,1,
  	0,0,0,394,397,5,6,0,0,395,397,3,96,48,0,396,387,1,0,0,0,396,395,1,0,0,
  	0,397,73,1,0,0,0,398,399,5,34,0,0,399,400,3,52,26,0,400,401,5,95,0,0,
  	401,403,5,1,0,0,402,404,3,84,42,0,403,402,1,0,0,0,403,404,1,0,0,0,404,
  	405,1,0,0,0,405,420,5,2,0,0,406,407,5,35,0,0,407,408,5,3,0,0,408,413,
  	5,95,0,0,409,410,5,85,0,0,410,412,5,95,0,0,411,409,1,0,0,0,412,415,1,
  	0,0,0,413,411,1,0,0,0,413,414,1,0,0,0,414,417,1,0,0,0,415,413,1,0,0,0,
  	416,418,5,85,0,0,417,416,1,0,0,0,417,418,1,0,0,0,418,419,1,0,0,0,419,
  	421,5,4,0,0,420,406,1,0,0,0,420,421,1,0,0,0,421,422,1,0,0,0,422,423,3,
  	88,44,0,423,75,1,0,0,0,424,425,3,32,16,0,425,426,5,36,0,0,426,428,5,1,
  	0,0,427,429,3,84,42,0,428,427,1,0,0,0,428,429,1,0,0,0,429,430,1,0,0,0,
  	430,431,5,2,0,0,431,432,3,88,44,0,432,77,1,0,0,0,433,434,3,32,16,0,434,
  	435,3,52,26,0,435,436,5,95,0,0,436,438,5,1,0,0,437,439,3,84,42,0,438,
  	437,1,0,0,0,438,439,1,0,0,0,439,440,1,0,0,0,440,442,5,2,0,0,441,443,3,
  	30,15,0,442,441,1,0,0,0,442,443,1,0,0,0,443,446,1,0,0,0,444,447,3,88,
  	44,0,445,447,5,96,0,0,446,444,1,0,0,0,446,445,1,0,0,0,447,79,1,0,0,0,
  	448,449,5,14,0,0,449,81,1,0,0,0,450,451,7,2,0,0,451,83,1,0,0,0,452,457,
  	3,86,43,0,453,454,5,85,0,0,454,456,3,86,43,0,455,453,1,0,0,0,456,459,
  	1,0,0,0,457,455,1,0,0,0,457,458,1,0,0,0,458,461,1,0,0,0,459,457,1,0,0,
  	0,460,462,5,85,0,0,461,460,1,0,0,0,461,462,1,0,0,0,462,85,1,0,0,0,463,
  	464,3,38,19,0,464,470,3,52,26,0,465,468,5,95,0,0,466,467,5,57,0,0,467,
  	469,3,96,48,0,468,466,1,0,0,0,468,469,1,0,0,0,469,471,1,0,0,0,470,465,
  	1,0,0,0,470,471,1,0,0,0,471,87,1,0,0,0,472,476,5,5,0,0,473,475,3,90,45,
  	0,474,473,1,0,0,0,475,478,1,0,0,0,476,474,1,0,0,0,476,477,1,0,0,0,477,
  	479,1,0,0,0,478,476,1,0,0,0,479,480,5,6,0,0,480,89,1,0,0,0,481,485,3,
  	88,44,0,482,485,3,60,30,0,483,485,3,94,47,0,484,481,1,0,0,0,484,482,1,
  	0,0,0,484,483,1,0,0,0,485,91,1,0,0,0,486,489,3,88,44,0,487,489,3,94,47,
  	0,488,486,1,0,0,0,488,487,1,0,0,0,489,93,1,0,0,0,490,493,3,96,48,0,491,
  	493,3,98,49,0,492,490,1,0,0,0,492,491,1,0,0,0,492,493,1,0,0,0,493,494,
  	1,0,0,0,494,501,5,96,0,0,495,501,3,108,54,0,496,501,3,100,50,0,497,501,
  	3,122,61,0,498,501,3,74,37,0,499,501,3,130,65,0,500,492,1,0,0,0,500,495,
  	1,0,0,0,500,496,1,0,0,0,500,497,1,0,0,0,500,498,1,0,0,0,500,499,1,0,0,
  	0,501,95,1,0,0,0,502,503,6,48,-1,0,503,504,5,1,0,0,504,505,3,96,48,0,
  	505,506,5,2,0,0,506,541,1,0,0,0,507,511,5,5,0,0,508,510,3,70,35,0,509,
  	508,1,0,0,0,510,513,1,0,0,0,511,509,1,0,0,0,511,512,1,0,0,0,512,514,1,
  	0,0,0,513,511,1,0,0,0,514,541,5,6,0,0,515,516,7,3,0,0,516,541,3,96,48,
  	18,517,519,5,42,0,0,518,520,5,95,0,0,519,518,1,0,0,0,519,520,1,0,0,0,
  	520,522,1,0,0,0,521,523,3,56,28,0,522,521,1,0,0,0,522,523,1,0,0,0,523,
  	526,1,0,0,0,524,527,3,50,25,0,525,527,3,48,24,0,526,524,1,0,0,0,526,525,
  	1,0,0,0,526,527,1,0,0,0,527,529,1,0,0,0,528,530,3,54,27,0,529,528,1,0,
  	0,0,529,530,1,0,0,0,530,541,1,0,0,0,531,541,7,4,0,0,532,541,5,44,0,0,
  	533,541,7,5,0,0,534,536,5,94,0,0,535,534,1,0,0,0,536,537,1,0,0,0,537,
  	535,1,0,0,0,537,538,1,0,0,0,538,541,1,0,0,0,539,541,5,95,0,0,540,502,
  	1,0,0,0,540,507,1,0,0,0,540,515,1,0,0,0,540,517,1,0,0,0,540,531,1,0,0,
  	0,540,532,1,0,0,0,540,533,1,0,0,0,540,535,1,0,0,0,540,539,1,0,0,0,541,
  	603,1,0,0,0,542,543,10,24,0,0,543,544,5,78,0,0,544,602,3,96,48,24,545,
  	546,10,20,0,0,546,547,5,84,0,0,547,602,3,96,48,21,548,549,10,17,0,0,549,
  	550,7,6,0,0,550,602,3,96,48,18,551,552,10,16,0,0,552,553,7,7,0,0,553,
  	602,3,96,48,17,554,555,10,15,0,0,555,556,7,8,0,0,556,602,3,96,48,16,557,
  	558,10,14,0,0,558,559,7,9,0,0,559,602,3,96,48,15,560,561,10,13,0,0,561,
  	562,7,10,0,0,562,602,3,96,48,14,563,564,10,12,0,0,564,565,5,54,0,0,565,
  	602,3,96,48,13,566,567,10,11,0,0,567,568,5,56,0,0,568,602,3,96,48,12,
  	569,570,10,10,0,0,570,571,5,55,0,0,571,602,3,96,48,11,572,573,10,9,0,
  	0,573,574,5,70,0,0,574,602,3,96,48,10,575,576,10,8,0,0,576,577,5,71,0,
  	0,577,602,3,96,48,9,578,579,10,7,0,0,579,580,7,11,0,0,580,602,3,96,48,
  	8,581,582,10,25,0,0,582,602,7,12,0,0,583,584,10,23,0,0,584,593,5,1,0,
  	0,585,590,3,96,48,0,586,587,5,85,0,0,587,589,3,96,48,0,588,586,1,0,0,
  	0,589,592,1,0,0,0,590,588,1,0,0,0,590,591,1,0,0,0,591,594,1,0,0,0,592,
  	590,1,0,0,0,593,585,1,0,0,0,593,594,1,0,0,0,594,595,1,0,0,0,595,602,5,
  	2,0,0,596,597,10,21,0,0,597,598,5,3,0,0,598,599,3,96,48,0,599,600,5,4,
  	0,0,600,602,1,0,0,0,601,542,1,0,0,0,601,545,1,0,0,0,601,548,1,0,0,0,601,
  	551,1,0,0,0,601,554,1,0,0,0,601,557,1,0,0,0,601,560,1,0,0,0,601,563,1,
  	0,0,0,601,566,1,0,0,0,601,569,1,0,0,0,601,572,1,0,0,0,601,575,1,0,0,0,
  	601,578,1,0,0,0,601,581,1,0,0,0,601,583,1,0,0,0,601,596,1,0,0,0,602,605,
  	1,0,0,0,603,601,1,0,0,0,603,604,1,0,0,0,604,97,1,0,0,0,605,603,1,0,0,
  	0,606,607,5,19,0,0,607,608,3,96,48,0,608,99,1,0,0,0,609,613,3,102,51,
  	0,610,613,3,104,52,0,611,613,3,106,53,0,612,609,1,0,0,0,612,610,1,0,0,
  	0,612,611,1,0,0,0,613,101,1,0,0,0,614,615,5,25,0,0,615,616,5,1,0,0,616,
  	617,3,68,34,0,617,618,5,2,0,0,618,619,3,92,46,0,619,103,1,0,0,0,620,621,
  	5,24,0,0,621,622,3,92,46,0,622,623,5,25,0,0,623,624,5,1,0,0,624,625,3,
  	68,34,0,625,626,5,2,0,0,626,627,5,96,0,0,627,105,1,0,0,0,628,629,5,23,
  	0,0,629,631,5,1,0,0,630,632,3,68,34,0,631,630,1,0,0,0,631,632,1,0,0,0,
  	632,633,1,0,0,0,633,635,5,96,0,0,634,636,3,68,34,0,635,634,1,0,0,0,635,
  	636,1,0,0,0,636,637,1,0,0,0,637,639,5,96,0,0,638,640,3,68,34,0,639,638,
  	1,0,0,0,639,640,1,0,0,0,640,641,1,0,0,0,641,642,5,2,0,0,642,643,3,92,
  	46,0,643,107,1,0,0,0,644,648,3,110,55,0,645,647,3,112,56,0,646,645,1,
  	0,0,0,647,650,1,0,0,0,648,646,1,0,0,0,648,649,1,0,0,0,649,652,1,0,0,0,
  	650,648,1,0,0,0,651,653,3,114,57,0,652,651,1,0,0,0,652,653,1,0,0,0,653,
  	665,1,0,0,0,654,655,3,116,58,0,655,659,5,5,0,0,656,658,3,118,59,0,657,
  	656,1,0,0,0,658,661,1,0,0,0,659,657,1,0,0,0,659,660,1,0,0,0,660,662,1,
  	0,0,0,661,659,1,0,0,0,662,663,5,6,0,0,663,665,1,0,0,0,664,644,1,0,0,0,
  	664,654,1,0,0,0,665,109,1,0,0,0,666,667,5,20,0,0,667,668,5,1,0,0,668,
  	669,3,68,34,0,669,670,5,2,0,0,670,671,3,92,46,0,671,111,1,0,0,0,672,673,
  	5,21,0,0,673,674,5,1,0,0,674,675,3,68,34,0,675,676,5,2,0,0,676,677,3,
  	92,46,0,677,113,1,0,0,0,678,679,5,22,0,0,679,680,3,92,46,0,680,115,1,
  	0,0,0,681,682,5,28,0,0,682,683,5,1,0,0,683,684,3,68,34,0,684,685,5,2,
  	0,0,685,117,1,0,0,0,686,687,5,29,0,0,687,690,3,120,60,0,688,690,5,30,
  	0,0,689,686,1,0,0,0,689,688,1,0,0,0,690,691,1,0,0,0,691,693,5,86,0,0,
  	692,694,3,92,46,0,693,692,1,0,0,0,693,694,1,0,0,0,694,119,1,0,0,0,695,
  	698,7,13,0,0,696,698,5,95,0,0,697,695,1,0,0,0,697,696,1,0,0,0,698,121,
  	1,0,0,0,699,703,3,124,62,0,700,702,3,126,63,0,701,700,1,0,0,0,702,705,
  	1,0,0,0,703,701,1,0,0,0,703,704,1,0,0,0,704,707,1,0,0,0,705,703,1,0,0,
  	0,706,708,3,128,64,0,707,706,1,0,0,0,707,708,1,0,0,0,708,123,1,0,0,0,
  	709,710,5,31,0,0,710,711,3,92,46,0,711,125,1,0,0,0,712,713,5,32,0,0,713,
  	715,5,1,0,0,714,716,3,68,34,0,715,714,1,0,0,0,715,716,1,0,0,0,716,717,
  	1,0,0,0,717,718,5,2,0,0,718,719,3,92,46,0,719,127,1,0,0,0,720,721,5,33,
  	0,0,721,722,3,92,46,0,722,129,1,0,0,0,723,724,5,43,0,0,724,728,5,5,0,
  	0,725,727,3,132,66,0,726,725,1,0,0,0,727,730,1,0,0,0,728,726,1,0,0,0,
  	728,729,1,0,0,0,729,731,1,0,0,0,730,728,1,0,0,0,731,732,5,6,0,0,732,131,
  	1,0,0,0,733,736,3,134,67,0,734,736,3,144,72,0,735,733,1,0,0,0,735,734,
  	1,0,0,0,736,133,1,0,0,0,737,746,5,95,0,0,738,743,3,136,68,0,739,740,5,
  	85,0,0,740,742,3,136,68,0,741,739,1,0,0,0,742,745,1,0,0,0,743,741,1,0,
  	0,0,743,744,1,0,0,0,744,747,1,0,0,0,745,743,1,0,0,0,746,738,1,0,0,0,746,
  	747,1,0,0,0,747,749,1,0,0,0,748,750,5,96,0,0,749,748,1,0,0,0,749,750,
  	1,0,0,0,750,135,1,0,0,0,751,756,3,138,69,0,752,756,3,140,70,0,753,756,
  	3,142,71,0,754,756,3,144,72,0,755,751,1,0,0,0,755,752,1,0,0,0,755,753,
  	1,0,0,0,755,754,1,0,0,0,756,137,1,0,0,0,757,758,7,13,0,0,758,139,1,0,
  	0,0,759,760,5,91,0,0,760,141,1,0,0,0,761,762,5,3,0,0,762,763,5,95,0,0,
  	763,764,5,4,0,0,764,143,1,0,0,0,765,766,5,86,0,0,766,767,5,95,0,0,767,
  	145,1,0,0,0,91,149,158,169,184,196,198,204,207,212,227,234,238,240,249,
  	252,255,258,261,264,267,270,273,277,281,283,294,301,303,310,313,321,325,
  	331,335,341,358,364,370,374,381,385,391,396,403,413,417,420,428,438,442,
  	446,457,461,468,470,476,484,488,492,500,511,519,522,526,529,537,540,590,
  	593,601,603,612,631,635,639,648,652,659,664,689,693,697,703,707,715,728,
  	735,743,746,749,755
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  harlequinParserStaticData = staticData.release();
}

}

HarlequinParser::HarlequinParser(TokenStream *input) : HarlequinParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

HarlequinParser::HarlequinParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  HarlequinParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *harlequinParserStaticData->atn, harlequinParserStaticData->decisionToDFA, harlequinParserStaticData->sharedContextCache, options);
}

HarlequinParser::~HarlequinParser() {
  delete _interpreter;
}

const atn::ATN& HarlequinParser::getATN() const {
  return *harlequinParserStaticData->atn;
}

std::string HarlequinParser::getGrammarFileName() const {
  return "Harlequin.g4";
}

const std::vector<std::string>& HarlequinParser::getRuleNames() const {
  return harlequinParserStaticData->ruleNames;
}

const dfa::Vocabulary& HarlequinParser::getVocabulary() const {
  return harlequinParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView HarlequinParser::getSerializedATN() const {
  return harlequinParserStaticData->serializedATN;
}


//----------------- RootContext ------------------------------------------------------------------

HarlequinParser::RootContext::RootContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::RootContext::EOF() {
  return getToken(HarlequinParser::EOF, 0);
}

std::vector<HarlequinParser::ItemDefContext *> HarlequinParser::RootContext::itemDef() {
  return getRuleContexts<HarlequinParser::ItemDefContext>();
}

HarlequinParser::ItemDefContext* HarlequinParser::RootContext::itemDef(size_t i) {
  return getRuleContext<HarlequinParser::ItemDefContext>(i);
}


size_t HarlequinParser::RootContext::getRuleIndex() const {
  return HarlequinParser::RuleRoot;
}


std::any HarlequinParser::RootContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitRoot(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::RootContext* HarlequinParser::root() {
  RootContext *_localctx = _tracker.createInstance<RootContext>(_ctx, getState());
  enterRule(_localctx, 0, HarlequinParser::RuleRoot);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(149);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072680320) != 0)) {
      setState(146);
      itemDef();
      setState(151);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(152);
    match(HarlequinParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ItemDefContext ------------------------------------------------------------------

HarlequinParser::ItemDefContext::ItemDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::UsingStmtContext* HarlequinParser::ItemDefContext::usingStmt() {
  return getRuleContext<HarlequinParser::UsingStmtContext>(0);
}

HarlequinParser::UsingAliasStmtContext* HarlequinParser::ItemDefContext::usingAliasStmt() {
  return getRuleContext<HarlequinParser::UsingAliasStmtContext>(0);
}

HarlequinParser::NamespaceDeclContext* HarlequinParser::ItemDefContext::namespaceDecl() {
  return getRuleContext<HarlequinParser::NamespaceDeclContext>(0);
}

HarlequinParser::ClassDeclContext* HarlequinParser::ItemDefContext::classDecl() {
  return getRuleContext<HarlequinParser::ClassDeclContext>(0);
}


size_t HarlequinParser::ItemDefContext::getRuleIndex() const {
  return HarlequinParser::RuleItemDef;
}


std::any HarlequinParser::ItemDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitItemDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ItemDefContext* HarlequinParser::itemDef() {
  ItemDefContext *_localctx = _tracker.createInstance<ItemDefContext>(_ctx, getState());
  enterRule(_localctx, 2, HarlequinParser::RuleItemDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(158);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(154);
      usingStmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(155);
      usingAliasStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(156);
      namespaceDecl();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(157);
      classDecl();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CompilerDirectiveContext ------------------------------------------------------------------

HarlequinParser::CompilerDirectiveContext::CompilerDirectiveContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CompilerDirectiveContext::Pound() {
  return getToken(HarlequinParser::Pound, 0);
}

tree::TerminalNode* HarlequinParser::CompilerDirectiveContext::LineKw() {
  return getToken(HarlequinParser::LineKw, 0);
}

tree::TerminalNode* HarlequinParser::CompilerDirectiveContext::IntLit() {
  return getToken(HarlequinParser::IntLit, 0);
}


size_t HarlequinParser::CompilerDirectiveContext::getRuleIndex() const {
  return HarlequinParser::RuleCompilerDirective;
}


std::any HarlequinParser::CompilerDirectiveContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCompilerDirective(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CompilerDirectiveContext* HarlequinParser::compilerDirective() {
  CompilerDirectiveContext *_localctx = _tracker.createInstance<CompilerDirectiveContext>(_ctx, getState());
  enterRule(_localctx, 4, HarlequinParser::RuleCompilerDirective);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(160);
    match(HarlequinParser::Pound);
    setState(161);
    match(HarlequinParser::LineKw);
    setState(162);
    match(HarlequinParser::IntLit);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QualifiedIdContext ------------------------------------------------------------------

HarlequinParser::QualifiedIdContext::QualifiedIdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> HarlequinParser::QualifiedIdContext::Id() {
  return getTokens(HarlequinParser::Id);
}

tree::TerminalNode* HarlequinParser::QualifiedIdContext::Id(size_t i) {
  return getToken(HarlequinParser::Id, i);
}

std::vector<tree::TerminalNode *> HarlequinParser::QualifiedIdContext::Dot() {
  return getTokens(HarlequinParser::Dot);
}

tree::TerminalNode* HarlequinParser::QualifiedIdContext::Dot(size_t i) {
  return getToken(HarlequinParser::Dot, i);
}


size_t HarlequinParser::QualifiedIdContext::getRuleIndex() const {
  return HarlequinParser::RuleQualifiedId;
}


std::any HarlequinParser::QualifiedIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitQualifiedId(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::QualifiedIdContext* HarlequinParser::qualifiedId() {
  QualifiedIdContext *_localctx = _tracker.createInstance<QualifiedIdContext>(_ctx, getState());
  enterRule(_localctx, 6, HarlequinParser::RuleQualifiedId);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(164);
    match(HarlequinParser::Id);
    setState(169);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Dot) {
      setState(165);
      match(HarlequinParser::Dot);
      setState(166);
      match(HarlequinParser::Id);
      setState(171);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UsingStmtContext ------------------------------------------------------------------

HarlequinParser::UsingStmtContext::UsingStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::UsingStmtContext::UsingKw() {
  return getToken(HarlequinParser::UsingKw, 0);
}

HarlequinParser::QualifiedIdContext* HarlequinParser::UsingStmtContext::qualifiedId() {
  return getRuleContext<HarlequinParser::QualifiedIdContext>(0);
}

tree::TerminalNode* HarlequinParser::UsingStmtContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}


size_t HarlequinParser::UsingStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleUsingStmt;
}


std::any HarlequinParser::UsingStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitUsingStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::UsingStmtContext* HarlequinParser::usingStmt() {
  UsingStmtContext *_localctx = _tracker.createInstance<UsingStmtContext>(_ctx, getState());
  enterRule(_localctx, 8, HarlequinParser::RuleUsingStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(HarlequinParser::UsingKw);
    setState(173);
    qualifiedId();
    setState(174);
    match(HarlequinParser::Term);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UsingAliasStmtContext ------------------------------------------------------------------

HarlequinParser::UsingAliasStmtContext::UsingAliasStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::UsingKw() {
  return getToken(HarlequinParser::UsingKw, 0);
}

HarlequinParser::QualifiedIdContext* HarlequinParser::UsingAliasStmtContext::qualifiedId() {
  return getRuleContext<HarlequinParser::QualifiedIdContext>(0);
}

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::AsKw() {
  return getToken(HarlequinParser::AsKw, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::UsingAliasStmtContext::Id() {
  return getTokens(HarlequinParser::Id);
}

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::Id(size_t i) {
  return getToken(HarlequinParser::Id, i);
}

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::UsingAliasStmtContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::UsingAliasStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleUsingAliasStmt;
}


std::any HarlequinParser::UsingAliasStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitUsingAliasStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::UsingAliasStmtContext* HarlequinParser::usingAliasStmt() {
  UsingAliasStmtContext *_localctx = _tracker.createInstance<UsingAliasStmtContext>(_ctx, getState());
  enterRule(_localctx, 10, HarlequinParser::RuleUsingAliasStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    match(HarlequinParser::UsingKw);
    setState(177);
    qualifiedId();
    setState(178);
    match(HarlequinParser::AsKw);
    setState(179);
    match(HarlequinParser::Id);
    setState(184);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(180);
      match(HarlequinParser::Comma);
      setState(181);
      match(HarlequinParser::Id);
      setState(186);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(187);
    match(HarlequinParser::Term);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamespaceDeclContext ------------------------------------------------------------------

HarlequinParser::NamespaceDeclContext::NamespaceDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::NamespaceDeclContext::NamespaceKw() {
  return getToken(HarlequinParser::NamespaceKw, 0);
}

HarlequinParser::QualifiedIdContext* HarlequinParser::NamespaceDeclContext::qualifiedId() {
  return getRuleContext<HarlequinParser::QualifiedIdContext>(0);
}

HarlequinParser::NamespaceDefContext* HarlequinParser::NamespaceDeclContext::namespaceDef() {
  return getRuleContext<HarlequinParser::NamespaceDefContext>(0);
}


size_t HarlequinParser::NamespaceDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleNamespaceDecl;
}


std::any HarlequinParser::NamespaceDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitNamespaceDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::NamespaceDeclContext* HarlequinParser::namespaceDecl() {
  NamespaceDeclContext *_localctx = _tracker.createInstance<NamespaceDeclContext>(_ctx, getState());
  enterRule(_localctx, 12, HarlequinParser::RuleNamespaceDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(189);
    match(HarlequinParser::NamespaceKw);
    setState(190);
    qualifiedId();
    setState(191);
    namespaceDef();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamespaceDefContext ------------------------------------------------------------------

HarlequinParser::NamespaceDefContext::NamespaceDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::NamespaceDefContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::NamespaceDefContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::NamespaceDeclContext *> HarlequinParser::NamespaceDefContext::namespaceDecl() {
  return getRuleContexts<HarlequinParser::NamespaceDeclContext>();
}

HarlequinParser::NamespaceDeclContext* HarlequinParser::NamespaceDefContext::namespaceDecl(size_t i) {
  return getRuleContext<HarlequinParser::NamespaceDeclContext>(i);
}

std::vector<HarlequinParser::ClassDeclContext *> HarlequinParser::NamespaceDefContext::classDecl() {
  return getRuleContexts<HarlequinParser::ClassDeclContext>();
}

HarlequinParser::ClassDeclContext* HarlequinParser::NamespaceDefContext::classDecl(size_t i) {
  return getRuleContext<HarlequinParser::ClassDeclContext>(i);
}


size_t HarlequinParser::NamespaceDefContext::getRuleIndex() const {
  return HarlequinParser::RuleNamespaceDef;
}


std::any HarlequinParser::NamespaceDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitNamespaceDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::NamespaceDefContext* HarlequinParser::namespaceDef() {
  NamespaceDefContext *_localctx = _tracker.createInstance<NamespaceDefContext>(_ctx, getState());
  enterRule(_localctx, 14, HarlequinParser::RuleNamespaceDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(193);
    match(HarlequinParser::LeftBrace);
    setState(198);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072679296) != 0)) {
      setState(196);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case HarlequinParser::NamespaceKw: {
          setState(194);
          namespaceDecl();
          break;
        }

        case HarlequinParser::ClassKw:
        case HarlequinParser::InterfaceKw:
        case HarlequinParser::StaticKw:
        case HarlequinParser::PublicKw:
        case HarlequinParser::ProtectedKw:
        case HarlequinParser::PrivateKw: {
          setState(195);
          classDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(200);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(201);
    match(HarlequinParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassDeclContext ------------------------------------------------------------------

HarlequinParser::ClassDeclContext::ClassDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::ClassTypeContext* HarlequinParser::ClassDeclContext::classType() {
  return getRuleContext<HarlequinParser::ClassTypeContext>(0);
}

tree::TerminalNode* HarlequinParser::ClassDeclContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

HarlequinParser::ClassDefContext* HarlequinParser::ClassDeclContext::classDef() {
  return getRuleContext<HarlequinParser::ClassDefContext>(0);
}

HarlequinParser::AccessBaseSpecifierContext* HarlequinParser::ClassDeclContext::accessBaseSpecifier() {
  return getRuleContext<HarlequinParser::AccessBaseSpecifierContext>(0);
}

HarlequinParser::StorageSpecifierContext* HarlequinParser::ClassDeclContext::storageSpecifier() {
  return getRuleContext<HarlequinParser::StorageSpecifierContext>(0);
}

HarlequinParser::ClassInheritanceContext* HarlequinParser::ClassDeclContext::classInheritance() {
  return getRuleContext<HarlequinParser::ClassInheritanceContext>(0);
}


size_t HarlequinParser::ClassDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleClassDecl;
}


std::any HarlequinParser::ClassDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassDeclContext* HarlequinParser::classDecl() {
  ClassDeclContext *_localctx = _tracker.createInstance<ClassDeclContext>(_ctx, getState());
  enterRule(_localctx, 16, HarlequinParser::RuleClassDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(204);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072674304) != 0)) {
      setState(203);
      accessBaseSpecifier();
    }
    setState(207);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(206);
      storageSpecifier();
    }
    setState(209);
    classType();
    setState(210);
    match(HarlequinParser::Id);
    setState(212);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ExtendsKw

    || _la == HarlequinParser::ImplementsKw) {
      setState(211);
      classInheritance();
    }
    setState(214);
    classDef();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassTypeContext ------------------------------------------------------------------

HarlequinParser::ClassTypeContext::ClassTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ClassTypeContext::ClassKw() {
  return getToken(HarlequinParser::ClassKw, 0);
}

tree::TerminalNode* HarlequinParser::ClassTypeContext::InterfaceKw() {
  return getToken(HarlequinParser::InterfaceKw, 0);
}


size_t HarlequinParser::ClassTypeContext::getRuleIndex() const {
  return HarlequinParser::RuleClassType;
}


std::any HarlequinParser::ClassTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassType(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassTypeContext* HarlequinParser::classType() {
  ClassTypeContext *_localctx = _tracker.createInstance<ClassTypeContext>(_ctx, getState());
  enterRule(_localctx, 18, HarlequinParser::RuleClassType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(216);
    _la = _input->LA(1);
    if (!(_la == HarlequinParser::ClassKw

    || _la == HarlequinParser::InterfaceKw)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AccessBaseSpecifierContext ------------------------------------------------------------------

HarlequinParser::AccessBaseSpecifierContext::AccessBaseSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AccessBaseSpecifierContext::PublicKw() {
  return getToken(HarlequinParser::PublicKw, 0);
}

tree::TerminalNode* HarlequinParser::AccessBaseSpecifierContext::ProtectedKw() {
  return getToken(HarlequinParser::ProtectedKw, 0);
}

tree::TerminalNode* HarlequinParser::AccessBaseSpecifierContext::PrivateKw() {
  return getToken(HarlequinParser::PrivateKw, 0);
}


size_t HarlequinParser::AccessBaseSpecifierContext::getRuleIndex() const {
  return HarlequinParser::RuleAccessBaseSpecifier;
}


std::any HarlequinParser::AccessBaseSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAccessBaseSpecifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AccessBaseSpecifierContext* HarlequinParser::accessBaseSpecifier() {
  AccessBaseSpecifierContext *_localctx = _tracker.createInstance<AccessBaseSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 20, HarlequinParser::RuleAccessBaseSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(218);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072674304) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AccessLimitSpecifierContext ------------------------------------------------------------------

HarlequinParser::AccessLimitSpecifierContext::AccessLimitSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AccessLimitSpecifierContext::LimitedKw() {
  return getToken(HarlequinParser::LimitedKw, 0);
}

tree::TerminalNode* HarlequinParser::AccessLimitSpecifierContext::LeftBracket() {
  return getToken(HarlequinParser::LeftBracket, 0);
}

std::vector<HarlequinParser::QualifiedIdContext *> HarlequinParser::AccessLimitSpecifierContext::qualifiedId() {
  return getRuleContexts<HarlequinParser::QualifiedIdContext>();
}

HarlequinParser::QualifiedIdContext* HarlequinParser::AccessLimitSpecifierContext::qualifiedId(size_t i) {
  return getRuleContext<HarlequinParser::QualifiedIdContext>(i);
}

tree::TerminalNode* HarlequinParser::AccessLimitSpecifierContext::RightBracket() {
  return getToken(HarlequinParser::RightBracket, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::AccessLimitSpecifierContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::AccessLimitSpecifierContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::AccessLimitSpecifierContext::getRuleIndex() const {
  return HarlequinParser::RuleAccessLimitSpecifier;
}


std::any HarlequinParser::AccessLimitSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAccessLimitSpecifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AccessLimitSpecifierContext* HarlequinParser::accessLimitSpecifier() {
  AccessLimitSpecifierContext *_localctx = _tracker.createInstance<AccessLimitSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 22, HarlequinParser::RuleAccessLimitSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(220);
    match(HarlequinParser::LimitedKw);
    setState(221);
    match(HarlequinParser::LeftBracket);
    setState(222);
    qualifiedId();
    setState(227);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(223);
      match(HarlequinParser::Comma);
      setState(224);
      qualifiedId();
      setState(229);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(230);
    match(HarlequinParser::RightBracket);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AccessSpecifierContext ------------------------------------------------------------------

HarlequinParser::AccessSpecifierContext::AccessSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::AccessBaseSpecifierContext* HarlequinParser::AccessSpecifierContext::accessBaseSpecifier() {
  return getRuleContext<HarlequinParser::AccessBaseSpecifierContext>(0);
}

HarlequinParser::AccessLimitSpecifierContext* HarlequinParser::AccessSpecifierContext::accessLimitSpecifier() {
  return getRuleContext<HarlequinParser::AccessLimitSpecifierContext>(0);
}


size_t HarlequinParser::AccessSpecifierContext::getRuleIndex() const {
  return HarlequinParser::RuleAccessSpecifier;
}


std::any HarlequinParser::AccessSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAccessSpecifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AccessSpecifierContext* HarlequinParser::accessSpecifier() {
  AccessSpecifierContext *_localctx = _tracker.createInstance<AccessSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 24, HarlequinParser::RuleAccessSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(240);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::PublicKw:
      case HarlequinParser::ProtectedKw:
      case HarlequinParser::PrivateKw: {
        enterOuterAlt(_localctx, 1);
        setState(232);
        accessBaseSpecifier();
        setState(234);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::LimitedKw) {
          setState(233);
          accessLimitSpecifier();
        }
        break;
      }

      case HarlequinParser::LimitedKw: {
        enterOuterAlt(_localctx, 2);
        setState(236);
        accessLimitSpecifier();
        setState(238);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 962072674304) != 0)) {
          setState(237);
          accessBaseSpecifier();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StorageSpecifierContext ------------------------------------------------------------------

HarlequinParser::StorageSpecifierContext::StorageSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::StorageSpecifierContext::StaticKw() {
  return getToken(HarlequinParser::StaticKw, 0);
}


size_t HarlequinParser::StorageSpecifierContext::getRuleIndex() const {
  return HarlequinParser::RuleStorageSpecifier;
}


std::any HarlequinParser::StorageSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitStorageSpecifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::StorageSpecifierContext* HarlequinParser::storageSpecifier() {
  StorageSpecifierContext *_localctx = _tracker.createInstance<StorageSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 26, HarlequinParser::RuleStorageSpecifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(242);
    match(HarlequinParser::StaticKw);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionSpecifierContext ------------------------------------------------------------------

HarlequinParser::FunctionSpecifierContext::FunctionSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::FunctionSpecifierContext::InlineKw() {
  return getToken(HarlequinParser::InlineKw, 0);
}

tree::TerminalNode* HarlequinParser::FunctionSpecifierContext::VirtualKw() {
  return getToken(HarlequinParser::VirtualKw, 0);
}

tree::TerminalNode* HarlequinParser::FunctionSpecifierContext::NativeKw() {
  return getToken(HarlequinParser::NativeKw, 0);
}


size_t HarlequinParser::FunctionSpecifierContext::getRuleIndex() const {
  return HarlequinParser::RuleFunctionSpecifier;
}


std::any HarlequinParser::FunctionSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitFunctionSpecifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::FunctionSpecifierContext* HarlequinParser::functionSpecifier() {
  FunctionSpecifierContext *_localctx = _tracker.createInstance<FunctionSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 28, HarlequinParser::RuleFunctionSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(244);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 106496) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstQualifierContext ------------------------------------------------------------------

HarlequinParser::ConstQualifierContext::ConstQualifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ConstQualifierContext::ConstKw() {
  return getToken(HarlequinParser::ConstKw, 0);
}


size_t HarlequinParser::ConstQualifierContext::getRuleIndex() const {
  return HarlequinParser::RuleConstQualifier;
}


std::any HarlequinParser::ConstQualifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitConstQualifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ConstQualifierContext* HarlequinParser::constQualifier() {
  ConstQualifierContext *_localctx = _tracker.createInstance<ConstQualifierContext>(_ctx, getState());
  enterRule(_localctx, 30, HarlequinParser::RuleConstQualifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(246);
    match(HarlequinParser::ConstKw);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodDeclSpecSeqContext ------------------------------------------------------------------

HarlequinParser::MethodDeclSpecSeqContext::MethodDeclSpecSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::AccessSpecifierContext* HarlequinParser::MethodDeclSpecSeqContext::accessSpecifier() {
  return getRuleContext<HarlequinParser::AccessSpecifierContext>(0);
}

HarlequinParser::StorageSpecifierContext* HarlequinParser::MethodDeclSpecSeqContext::storageSpecifier() {
  return getRuleContext<HarlequinParser::StorageSpecifierContext>(0);
}

HarlequinParser::FunctionSpecifierContext* HarlequinParser::MethodDeclSpecSeqContext::functionSpecifier() {
  return getRuleContext<HarlequinParser::FunctionSpecifierContext>(0);
}


size_t HarlequinParser::MethodDeclSpecSeqContext::getRuleIndex() const {
  return HarlequinParser::RuleMethodDeclSpecSeq;
}


std::any HarlequinParser::MethodDeclSpecSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitMethodDeclSpecSeq(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::MethodDeclSpecSeqContext* HarlequinParser::methodDeclSpecSeq() {
  MethodDeclSpecSeqContext *_localctx = _tracker.createInstance<MethodDeclSpecSeqContext>(_ctx, getState());
  enterRule(_localctx, 32, HarlequinParser::RuleMethodDeclSpecSeq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(249);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3161095929856) != 0)) {
      setState(248);
      accessSpecifier();
    }
    setState(252);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(251);
      storageSpecifier();
    }
    setState(255);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 106496) != 0)) {
      setState(254);
      functionSpecifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassVarDeclSpecSeqContext ------------------------------------------------------------------

HarlequinParser::ClassVarDeclSpecSeqContext::ClassVarDeclSpecSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::AccessSpecifierContext* HarlequinParser::ClassVarDeclSpecSeqContext::accessSpecifier() {
  return getRuleContext<HarlequinParser::AccessSpecifierContext>(0);
}

HarlequinParser::StorageSpecifierContext* HarlequinParser::ClassVarDeclSpecSeqContext::storageSpecifier() {
  return getRuleContext<HarlequinParser::StorageSpecifierContext>(0);
}

HarlequinParser::ConstQualifierContext* HarlequinParser::ClassVarDeclSpecSeqContext::constQualifier() {
  return getRuleContext<HarlequinParser::ConstQualifierContext>(0);
}


size_t HarlequinParser::ClassVarDeclSpecSeqContext::getRuleIndex() const {
  return HarlequinParser::RuleClassVarDeclSpecSeq;
}


std::any HarlequinParser::ClassVarDeclSpecSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassVarDeclSpecSeq(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassVarDeclSpecSeqContext* HarlequinParser::classVarDeclSpecSeq() {
  ClassVarDeclSpecSeqContext *_localctx = _tracker.createInstance<ClassVarDeclSpecSeqContext>(_ctx, getState());
  enterRule(_localctx, 34, HarlequinParser::RuleClassVarDeclSpecSeq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(258);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3161095929856) != 0)) {
      setState(257);
      accessSpecifier();
    }
    setState(261);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(260);
      storageSpecifier();
    }
    setState(264);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(263);
      constQualifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LocalVarDeclSpecSeqContext ------------------------------------------------------------------

HarlequinParser::LocalVarDeclSpecSeqContext::LocalVarDeclSpecSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::StorageSpecifierContext* HarlequinParser::LocalVarDeclSpecSeqContext::storageSpecifier() {
  return getRuleContext<HarlequinParser::StorageSpecifierContext>(0);
}

HarlequinParser::ConstQualifierContext* HarlequinParser::LocalVarDeclSpecSeqContext::constQualifier() {
  return getRuleContext<HarlequinParser::ConstQualifierContext>(0);
}


size_t HarlequinParser::LocalVarDeclSpecSeqContext::getRuleIndex() const {
  return HarlequinParser::RuleLocalVarDeclSpecSeq;
}


std::any HarlequinParser::LocalVarDeclSpecSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLocalVarDeclSpecSeq(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LocalVarDeclSpecSeqContext* HarlequinParser::localVarDeclSpecSeq() {
  LocalVarDeclSpecSeqContext *_localctx = _tracker.createInstance<LocalVarDeclSpecSeqContext>(_ctx, getState());
  enterRule(_localctx, 36, HarlequinParser::RuleLocalVarDeclSpecSeq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(267);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(266);
      storageSpecifier();
    }
    setState(270);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(269);
      constQualifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgVarDeclSpecSeqContext ------------------------------------------------------------------

HarlequinParser::ArgVarDeclSpecSeqContext::ArgVarDeclSpecSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::ConstQualifierContext* HarlequinParser::ArgVarDeclSpecSeqContext::constQualifier() {
  return getRuleContext<HarlequinParser::ConstQualifierContext>(0);
}


size_t HarlequinParser::ArgVarDeclSpecSeqContext::getRuleIndex() const {
  return HarlequinParser::RuleArgVarDeclSpecSeq;
}


std::any HarlequinParser::ArgVarDeclSpecSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitArgVarDeclSpecSeq(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ArgVarDeclSpecSeqContext* HarlequinParser::argVarDeclSpecSeq() {
  ArgVarDeclSpecSeqContext *_localctx = _tracker.createInstance<ArgVarDeclSpecSeqContext>(_ctx, getState());
  enterRule(_localctx, 38, HarlequinParser::RuleArgVarDeclSpecSeq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(273);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(272);
      constQualifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassInheritanceContext ------------------------------------------------------------------

HarlequinParser::ClassInheritanceContext::ClassInheritanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::ClassExtendsContext* HarlequinParser::ClassInheritanceContext::classExtends() {
  return getRuleContext<HarlequinParser::ClassExtendsContext>(0);
}

HarlequinParser::ClassImplsContext* HarlequinParser::ClassInheritanceContext::classImpls() {
  return getRuleContext<HarlequinParser::ClassImplsContext>(0);
}


size_t HarlequinParser::ClassInheritanceContext::getRuleIndex() const {
  return HarlequinParser::RuleClassInheritance;
}


std::any HarlequinParser::ClassInheritanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassInheritance(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassInheritanceContext* HarlequinParser::classInheritance() {
  ClassInheritanceContext *_localctx = _tracker.createInstance<ClassInheritanceContext>(_ctx, getState());
  enterRule(_localctx, 40, HarlequinParser::RuleClassInheritance);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(283);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::ExtendsKw: {
        enterOuterAlt(_localctx, 1);
        setState(275);
        classExtends();
        setState(277);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::ImplementsKw) {
          setState(276);
          classImpls();
        }
        break;
      }

      case HarlequinParser::ImplementsKw: {
        enterOuterAlt(_localctx, 2);
        setState(279);
        classImpls();
        setState(281);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::ExtendsKw) {
          setState(280);
          classExtends();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassExtendsContext ------------------------------------------------------------------

HarlequinParser::ClassExtendsContext::ClassExtendsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ClassExtendsContext::ExtendsKw() {
  return getToken(HarlequinParser::ExtendsKw, 0);
}

HarlequinParser::QualifiedIdContext* HarlequinParser::ClassExtendsContext::qualifiedId() {
  return getRuleContext<HarlequinParser::QualifiedIdContext>(0);
}


size_t HarlequinParser::ClassExtendsContext::getRuleIndex() const {
  return HarlequinParser::RuleClassExtends;
}


std::any HarlequinParser::ClassExtendsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassExtends(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassExtendsContext* HarlequinParser::classExtends() {
  ClassExtendsContext *_localctx = _tracker.createInstance<ClassExtendsContext>(_ctx, getState());
  enterRule(_localctx, 42, HarlequinParser::RuleClassExtends);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(285);
    match(HarlequinParser::ExtendsKw);
    setState(286);
    qualifiedId();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassImplsContext ------------------------------------------------------------------

HarlequinParser::ClassImplsContext::ClassImplsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ClassImplsContext::ImplementsKw() {
  return getToken(HarlequinParser::ImplementsKw, 0);
}

std::vector<HarlequinParser::QualifiedIdContext *> HarlequinParser::ClassImplsContext::qualifiedId() {
  return getRuleContexts<HarlequinParser::QualifiedIdContext>();
}

HarlequinParser::QualifiedIdContext* HarlequinParser::ClassImplsContext::qualifiedId(size_t i) {
  return getRuleContext<HarlequinParser::QualifiedIdContext>(i);
}

std::vector<tree::TerminalNode *> HarlequinParser::ClassImplsContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::ClassImplsContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::ClassImplsContext::getRuleIndex() const {
  return HarlequinParser::RuleClassImpls;
}


std::any HarlequinParser::ClassImplsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassImpls(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassImplsContext* HarlequinParser::classImpls() {
  ClassImplsContext *_localctx = _tracker.createInstance<ClassImplsContext>(_ctx, getState());
  enterRule(_localctx, 44, HarlequinParser::RuleClassImpls);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(288);
    match(HarlequinParser::ImplementsKw);
    setState(289);
    qualifiedId();
    setState(294);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(290);
      match(HarlequinParser::Comma);
      setState(291);
      qualifiedId();
      setState(296);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassDefContext ------------------------------------------------------------------

HarlequinParser::ClassDefContext::ClassDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ClassDefContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::ClassDefContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::ClassVarDeclStmtContext *> HarlequinParser::ClassDefContext::classVarDeclStmt() {
  return getRuleContexts<HarlequinParser::ClassVarDeclStmtContext>();
}

HarlequinParser::ClassVarDeclStmtContext* HarlequinParser::ClassDefContext::classVarDeclStmt(size_t i) {
  return getRuleContext<HarlequinParser::ClassVarDeclStmtContext>(i);
}

std::vector<HarlequinParser::MethodDeclContext *> HarlequinParser::ClassDefContext::methodDecl() {
  return getRuleContexts<HarlequinParser::MethodDeclContext>();
}

HarlequinParser::MethodDeclContext* HarlequinParser::ClassDefContext::methodDecl(size_t i) {
  return getRuleContext<HarlequinParser::MethodDeclContext>(i);
}

std::vector<HarlequinParser::CtorDeclContext *> HarlequinParser::ClassDefContext::ctorDecl() {
  return getRuleContexts<HarlequinParser::CtorDeclContext>();
}

HarlequinParser::CtorDeclContext* HarlequinParser::ClassDefContext::ctorDecl(size_t i) {
  return getRuleContext<HarlequinParser::CtorDeclContext>(i);
}


size_t HarlequinParser::ClassDefContext::getRuleIndex() const {
  return HarlequinParser::RuleClassDef;
}


std::any HarlequinParser::ClassDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassDefContext* HarlequinParser::classDef() {
  ClassDefContext *_localctx = _tracker.createInstance<ClassDefContext>(_ctx, getState());
  enterRule(_localctx, 46, HarlequinParser::RuleClassDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(297);
    match(HarlequinParser::LeftBrace);
    setState(303);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3229815533568) != 0) || _la == HarlequinParser::Id) {
      setState(301);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
      case 1: {
        setState(298);
        classVarDeclStmt();
        break;
      }

      case 2: {
        setState(299);
        methodDecl();
        break;
      }

      case 3: {
        setState(300);
        ctorDecl();
        break;
      }

      default:
        break;
      }
      setState(305);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(306);
    match(HarlequinParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayTypeDeclContext ------------------------------------------------------------------

HarlequinParser::ArrayTypeDeclContext::ArrayTypeDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ArrayTypeDeclContext::LeftBracket() {
  return getToken(HarlequinParser::LeftBracket, 0);
}

tree::TerminalNode* HarlequinParser::ArrayTypeDeclContext::RightBracket() {
  return getToken(HarlequinParser::RightBracket, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::ArrayTypeDeclContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::ArrayTypeDeclContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::ArrayTypeDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleArrayTypeDecl;
}


std::any HarlequinParser::ArrayTypeDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitArrayTypeDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ArrayTypeDeclContext* HarlequinParser::arrayTypeDecl() {
  ArrayTypeDeclContext *_localctx = _tracker.createInstance<ArrayTypeDeclContext>(_ctx, getState());
  enterRule(_localctx, 48, HarlequinParser::RuleArrayTypeDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(308);
    match(HarlequinParser::LeftBracket);
    setState(310);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      setState(309);
      match(HarlequinParser::Comma);
      break;
    }

    default:
      break;
    }
    setState(313);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(312);
      match(HarlequinParser::Comma);
    }
    setState(315);
    match(HarlequinParser::RightBracket);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayTypeDefContext ------------------------------------------------------------------

HarlequinParser::ArrayTypeDefContext::ArrayTypeDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ArrayTypeDefContext::LeftBracket() {
  return getToken(HarlequinParser::LeftBracket, 0);
}

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ArrayTypeDefContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ArrayTypeDefContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ArrayTypeDefContext::RightBracket() {
  return getToken(HarlequinParser::RightBracket, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::ArrayTypeDefContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::ArrayTypeDefContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::ArrayTypeDefContext::getRuleIndex() const {
  return HarlequinParser::RuleArrayTypeDef;
}


std::any HarlequinParser::ArrayTypeDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitArrayTypeDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ArrayTypeDefContext* HarlequinParser::arrayTypeDef() {
  ArrayTypeDefContext *_localctx = _tracker.createInstance<ArrayTypeDefContext>(_ctx, getState());
  enterRule(_localctx, 50, HarlequinParser::RuleArrayTypeDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(317);
    match(HarlequinParser::LeftBracket);
    setState(318);
    expr(0);
    setState(321);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      setState(319);
      match(HarlequinParser::Comma);
      setState(320);
      expr(0);
      break;
    }

    default:
      break;
    }
    setState(325);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(323);
      match(HarlequinParser::Comma);
      setState(324);
      expr(0);
    }
    setState(327);
    match(HarlequinParser::RightBracket);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeNameDeclContext ------------------------------------------------------------------

HarlequinParser::TypeNameDeclContext::TypeNameDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::TypeNameDeclContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

HarlequinParser::ArrayTypeDeclContext* HarlequinParser::TypeNameDeclContext::arrayTypeDecl() {
  return getRuleContext<HarlequinParser::ArrayTypeDeclContext>(0);
}


size_t HarlequinParser::TypeNameDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleTypeNameDecl;
}


std::any HarlequinParser::TypeNameDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitTypeNameDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::TypeNameDeclContext* HarlequinParser::typeNameDecl() {
  TypeNameDeclContext *_localctx = _tracker.createInstance<TypeNameDeclContext>(_ctx, getState());
  enterRule(_localctx, 52, HarlequinParser::RuleTypeNameDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(329);
    match(HarlequinParser::Id);
    setState(331);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::LeftBracket) {
      setState(330);
      arrayTypeDecl();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BracedInitializerContext ------------------------------------------------------------------

HarlequinParser::BracedInitializerContext::BracedInitializerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::BracedInitializerContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::BracedInitializerContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

HarlequinParser::ExprContext* HarlequinParser::BracedInitializerContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::BracedInitializerContext::getRuleIndex() const {
  return HarlequinParser::RuleBracedInitializer;
}


std::any HarlequinParser::BracedInitializerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitBracedInitializer(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::BracedInitializerContext* HarlequinParser::bracedInitializer() {
  BracedInitializerContext *_localctx = _tracker.createInstance<BracedInitializerContext>(_ctx, getState());
  enterRule(_localctx, 54, HarlequinParser::RuleBracedInitializer);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(333);
    match(HarlequinParser::LeftBrace);
    setState(335);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(334);
      expr(0);
    }
    setState(337);
    match(HarlequinParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CtorCallContext ------------------------------------------------------------------

HarlequinParser::CtorCallContext::CtorCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CtorCallContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

tree::TerminalNode* HarlequinParser::CtorCallContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::ExprContext* HarlequinParser::CtorCallContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::CtorCallContext::getRuleIndex() const {
  return HarlequinParser::RuleCtorCall;
}


std::any HarlequinParser::CtorCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCtorCall(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CtorCallContext* HarlequinParser::ctorCall() {
  CtorCallContext *_localctx = _tracker.createInstance<CtorCallContext>(_ctx, getState());
  enterRule(_localctx, 56, HarlequinParser::RuleCtorCall);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(339);
    match(HarlequinParser::LeftParen);
    setState(341);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(340);
      expr(0);
    }
    setState(343);
    match(HarlequinParser::RightParen);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassVarDeclStmtContext ------------------------------------------------------------------

HarlequinParser::ClassVarDeclStmtContext::ClassVarDeclStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::ClassVarDeclSpecSeqContext* HarlequinParser::ClassVarDeclStmtContext::classVarDeclSpecSeq() {
  return getRuleContext<HarlequinParser::ClassVarDeclSpecSeqContext>(0);
}

HarlequinParser::VarDeclStmtContext* HarlequinParser::ClassVarDeclStmtContext::varDeclStmt() {
  return getRuleContext<HarlequinParser::VarDeclStmtContext>(0);
}


size_t HarlequinParser::ClassVarDeclStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleClassVarDeclStmt;
}


std::any HarlequinParser::ClassVarDeclStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitClassVarDeclStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ClassVarDeclStmtContext* HarlequinParser::classVarDeclStmt() {
  ClassVarDeclStmtContext *_localctx = _tracker.createInstance<ClassVarDeclStmtContext>(_ctx, getState());
  enterRule(_localctx, 58, HarlequinParser::RuleClassVarDeclStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(345);
    classVarDeclSpecSeq();
    setState(346);
    varDeclStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LocalVarDeclStmtContext ------------------------------------------------------------------

HarlequinParser::LocalVarDeclStmtContext::LocalVarDeclStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::LocalVarDeclSpecSeqContext* HarlequinParser::LocalVarDeclStmtContext::localVarDeclSpecSeq() {
  return getRuleContext<HarlequinParser::LocalVarDeclSpecSeqContext>(0);
}

HarlequinParser::VarDeclStmtContext* HarlequinParser::LocalVarDeclStmtContext::varDeclStmt() {
  return getRuleContext<HarlequinParser::VarDeclStmtContext>(0);
}


size_t HarlequinParser::LocalVarDeclStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleLocalVarDeclStmt;
}


std::any HarlequinParser::LocalVarDeclStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLocalVarDeclStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LocalVarDeclStmtContext* HarlequinParser::localVarDeclStmt() {
  LocalVarDeclStmtContext *_localctx = _tracker.createInstance<LocalVarDeclStmtContext>(_ctx, getState());
  enterRule(_localctx, 60, HarlequinParser::RuleLocalVarDeclStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(348);
    localVarDeclSpecSeq();
    setState(349);
    varDeclStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclStmtContext ------------------------------------------------------------------

HarlequinParser::VarDeclStmtContext::VarDeclStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::VarDeclContext* HarlequinParser::VarDeclStmtContext::varDecl() {
  return getRuleContext<HarlequinParser::VarDeclContext>(0);
}

tree::TerminalNode* HarlequinParser::VarDeclStmtContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}


size_t HarlequinParser::VarDeclStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleVarDeclStmt;
}


std::any HarlequinParser::VarDeclStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitVarDeclStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::VarDeclStmtContext* HarlequinParser::varDeclStmt() {
  VarDeclStmtContext *_localctx = _tracker.createInstance<VarDeclStmtContext>(_ctx, getState());
  enterRule(_localctx, 62, HarlequinParser::RuleVarDeclStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(351);
    varDecl();
    setState(352);
    match(HarlequinParser::Term);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

HarlequinParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::TypeNameDeclContext* HarlequinParser::VarDeclContext::typeNameDecl() {
  return getRuleContext<HarlequinParser::TypeNameDeclContext>(0);
}

tree::TerminalNode* HarlequinParser::VarDeclContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

tree::TerminalNode* HarlequinParser::VarDeclContext::Assign() {
  return getToken(HarlequinParser::Assign, 0);
}

HarlequinParser::ExprContext* HarlequinParser::VarDeclContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}

std::vector<tree::TerminalNode *> HarlequinParser::VarDeclContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::VarDeclContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}

std::vector<HarlequinParser::NextVarDeclContext *> HarlequinParser::VarDeclContext::nextVarDecl() {
  return getRuleContexts<HarlequinParser::NextVarDeclContext>();
}

HarlequinParser::NextVarDeclContext* HarlequinParser::VarDeclContext::nextVarDecl(size_t i) {
  return getRuleContext<HarlequinParser::NextVarDeclContext>(i);
}


size_t HarlequinParser::VarDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleVarDecl;
}


std::any HarlequinParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::VarDeclContext* HarlequinParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 64, HarlequinParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(354);
    typeNameDecl();
    setState(355);
    match(HarlequinParser::Id);
    setState(358);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Assign) {
      setState(356);
      match(HarlequinParser::Assign);
      setState(357);
      expr(0);
    }
    setState(364);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(360);
      match(HarlequinParser::Comma);
      setState(361);
      nextVarDecl();
      setState(366);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NextVarDeclContext ------------------------------------------------------------------

HarlequinParser::NextVarDeclContext::NextVarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::NextVarDeclContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

tree::TerminalNode* HarlequinParser::NextVarDeclContext::Assign() {
  return getToken(HarlequinParser::Assign, 0);
}

HarlequinParser::ExprContext* HarlequinParser::NextVarDeclContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::NextVarDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleNextVarDecl;
}


std::any HarlequinParser::NextVarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitNextVarDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::NextVarDeclContext* HarlequinParser::nextVarDecl() {
  NextVarDeclContext *_localctx = _tracker.createInstance<NextVarDeclContext>(_ctx, getState());
  enterRule(_localctx, 66, HarlequinParser::RuleNextVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(367);
    match(HarlequinParser::Id);
    setState(370);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Assign) {
      setState(368);
      match(HarlequinParser::Assign);
      setState(369);
      expr(0);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CtrlFlowExprContext ------------------------------------------------------------------

HarlequinParser::CtrlFlowExprContext::CtrlFlowExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::VarDeclContext* HarlequinParser::CtrlFlowExprContext::varDecl() {
  return getRuleContext<HarlequinParser::VarDeclContext>(0);
}

HarlequinParser::ExprContext* HarlequinParser::CtrlFlowExprContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::CtrlFlowExprContext::getRuleIndex() const {
  return HarlequinParser::RuleCtrlFlowExpr;
}


std::any HarlequinParser::CtrlFlowExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCtrlFlowExpr(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::ctrlFlowExpr() {
  CtrlFlowExprContext *_localctx = _tracker.createInstance<CtrlFlowExprContext>(_ctx, getState());
  enterRule(_localctx, 68, HarlequinParser::RuleCtrlFlowExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(374);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(372);
      varDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(373);
      expr(0);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayParamSeqContext ------------------------------------------------------------------

HarlequinParser::ArrayParamSeqContext::ArrayParamSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<HarlequinParser::ArrayParamElemContext *> HarlequinParser::ArrayParamSeqContext::arrayParamElem() {
  return getRuleContexts<HarlequinParser::ArrayParamElemContext>();
}

HarlequinParser::ArrayParamElemContext* HarlequinParser::ArrayParamSeqContext::arrayParamElem(size_t i) {
  return getRuleContext<HarlequinParser::ArrayParamElemContext>(i);
}

std::vector<tree::TerminalNode *> HarlequinParser::ArrayParamSeqContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::ArrayParamSeqContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::ArrayParamSeqContext::getRuleIndex() const {
  return HarlequinParser::RuleArrayParamSeq;
}


std::any HarlequinParser::ArrayParamSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitArrayParamSeq(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ArrayParamSeqContext* HarlequinParser::arrayParamSeq() {
  ArrayParamSeqContext *_localctx = _tracker.createInstance<ArrayParamSeqContext>(_ctx, getState());
  enterRule(_localctx, 70, HarlequinParser::RuleArrayParamSeq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(376);
    arrayParamElem();
    setState(381);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(377);
        match(HarlequinParser::Comma);
        setState(378);
        arrayParamElem(); 
      }
      setState(383);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx);
    }
    setState(385);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(384);
      match(HarlequinParser::Comma);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayParamElemContext ------------------------------------------------------------------

HarlequinParser::ArrayParamElemContext::ArrayParamElemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ArrayParamElemContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::ArrayParamElemContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::ArrayParamSeqContext *> HarlequinParser::ArrayParamElemContext::arrayParamSeq() {
  return getRuleContexts<HarlequinParser::ArrayParamSeqContext>();
}

HarlequinParser::ArrayParamSeqContext* HarlequinParser::ArrayParamElemContext::arrayParamSeq(size_t i) {
  return getRuleContext<HarlequinParser::ArrayParamSeqContext>(i);
}

HarlequinParser::ExprContext* HarlequinParser::ArrayParamElemContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::ArrayParamElemContext::getRuleIndex() const {
  return HarlequinParser::RuleArrayParamElem;
}


std::any HarlequinParser::ArrayParamElemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitArrayParamElem(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ArrayParamElemContext* HarlequinParser::arrayParamElem() {
  ArrayParamElemContext *_localctx = _tracker.createInstance<ArrayParamElemContext>(_ctx, getState());
  enterRule(_localctx, 72, HarlequinParser::RuleArrayParamElem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(396);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(387);
      match(HarlequinParser::LeftBrace);
      setState(391);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 80)) & 62991) != 0)) {
        setState(388);
        arrayParamSeq();
        setState(393);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(394);
      match(HarlequinParser::RightBrace);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(395);
      expr(0);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LambdaDeclContext ------------------------------------------------------------------

HarlequinParser::LambdaDeclContext::LambdaDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::LambdaKw() {
  return getToken(HarlequinParser::LambdaKw, 0);
}

HarlequinParser::TypeNameDeclContext* HarlequinParser::LambdaDeclContext::typeNameDecl() {
  return getRuleContext<HarlequinParser::TypeNameDeclContext>(0);
}

std::vector<tree::TerminalNode *> HarlequinParser::LambdaDeclContext::Id() {
  return getTokens(HarlequinParser::Id);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::Id(size_t i) {
  return getToken(HarlequinParser::Id, i);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeBlockContext* HarlequinParser::LambdaDeclContext::codeBlock() {
  return getRuleContext<HarlequinParser::CodeBlockContext>(0);
}

HarlequinParser::MethodArgSeqContext* HarlequinParser::LambdaDeclContext::methodArgSeq() {
  return getRuleContext<HarlequinParser::MethodArgSeqContext>(0);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::CaptureKw() {
  return getToken(HarlequinParser::CaptureKw, 0);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::LeftBracket() {
  return getToken(HarlequinParser::LeftBracket, 0);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::RightBracket() {
  return getToken(HarlequinParser::RightBracket, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::LambdaDeclContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::LambdaDeclContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::LambdaDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleLambdaDecl;
}


std::any HarlequinParser::LambdaDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLambdaDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LambdaDeclContext* HarlequinParser::lambdaDecl() {
  LambdaDeclContext *_localctx = _tracker.createInstance<LambdaDeclContext>(_ctx, getState());
  enterRule(_localctx, 74, HarlequinParser::RuleLambdaDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(398);
    match(HarlequinParser::LambdaKw);
    setState(399);
    typeNameDecl();
    setState(400);
    match(HarlequinParser::Id);
    setState(401);
    match(HarlequinParser::LeftParen);
    setState(403);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(402);
      methodArgSeq();
    }
    setState(405);
    match(HarlequinParser::RightParen);
    setState(420);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::CaptureKw) {
      setState(406);
      match(HarlequinParser::CaptureKw);
      setState(407);
      match(HarlequinParser::LeftBracket);
      setState(408);
      match(HarlequinParser::Id);
      setState(413);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(409);
          match(HarlequinParser::Comma);
          setState(410);
          match(HarlequinParser::Id); 
        }
        setState(415);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx);
      }
      setState(417);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == HarlequinParser::Comma) {
        setState(416);
        match(HarlequinParser::Comma);
      }
      setState(419);
      match(HarlequinParser::RightBracket);
    }
    setState(422);
    codeBlock();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CtorDeclContext ------------------------------------------------------------------

HarlequinParser::CtorDeclContext::CtorDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::MethodDeclSpecSeqContext* HarlequinParser::CtorDeclContext::methodDeclSpecSeq() {
  return getRuleContext<HarlequinParser::MethodDeclSpecSeqContext>(0);
}

tree::TerminalNode* HarlequinParser::CtorDeclContext::ConstructorKw() {
  return getToken(HarlequinParser::ConstructorKw, 0);
}

tree::TerminalNode* HarlequinParser::CtorDeclContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

tree::TerminalNode* HarlequinParser::CtorDeclContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeBlockContext* HarlequinParser::CtorDeclContext::codeBlock() {
  return getRuleContext<HarlequinParser::CodeBlockContext>(0);
}

HarlequinParser::MethodArgSeqContext* HarlequinParser::CtorDeclContext::methodArgSeq() {
  return getRuleContext<HarlequinParser::MethodArgSeqContext>(0);
}


size_t HarlequinParser::CtorDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleCtorDecl;
}


std::any HarlequinParser::CtorDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCtorDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CtorDeclContext* HarlequinParser::ctorDecl() {
  CtorDeclContext *_localctx = _tracker.createInstance<CtorDeclContext>(_ctx, getState());
  enterRule(_localctx, 76, HarlequinParser::RuleCtorDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(424);
    methodDeclSpecSeq();
    setState(425);
    match(HarlequinParser::ConstructorKw);
    setState(426);
    match(HarlequinParser::LeftParen);
    setState(428);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(427);
      methodArgSeq();
    }
    setState(430);
    match(HarlequinParser::RightParen);
    setState(431);
    codeBlock();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodDeclContext ------------------------------------------------------------------

HarlequinParser::MethodDeclContext::MethodDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::MethodDeclSpecSeqContext* HarlequinParser::MethodDeclContext::methodDeclSpecSeq() {
  return getRuleContext<HarlequinParser::MethodDeclSpecSeqContext>(0);
}

HarlequinParser::TypeNameDeclContext* HarlequinParser::MethodDeclContext::typeNameDecl() {
  return getRuleContext<HarlequinParser::TypeNameDeclContext>(0);
}

tree::TerminalNode* HarlequinParser::MethodDeclContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

tree::TerminalNode* HarlequinParser::MethodDeclContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

tree::TerminalNode* HarlequinParser::MethodDeclContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeBlockContext* HarlequinParser::MethodDeclContext::codeBlock() {
  return getRuleContext<HarlequinParser::CodeBlockContext>(0);
}

tree::TerminalNode* HarlequinParser::MethodDeclContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}

HarlequinParser::MethodArgSeqContext* HarlequinParser::MethodDeclContext::methodArgSeq() {
  return getRuleContext<HarlequinParser::MethodArgSeqContext>(0);
}

HarlequinParser::ConstQualifierContext* HarlequinParser::MethodDeclContext::constQualifier() {
  return getRuleContext<HarlequinParser::ConstQualifierContext>(0);
}


size_t HarlequinParser::MethodDeclContext::getRuleIndex() const {
  return HarlequinParser::RuleMethodDecl;
}


std::any HarlequinParser::MethodDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitMethodDecl(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::MethodDeclContext* HarlequinParser::methodDecl() {
  MethodDeclContext *_localctx = _tracker.createInstance<MethodDeclContext>(_ctx, getState());
  enterRule(_localctx, 78, HarlequinParser::RuleMethodDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(433);
    methodDeclSpecSeq();
    setState(434);
    typeNameDecl();
    setState(435);
    match(HarlequinParser::Id);
    setState(436);
    match(HarlequinParser::LeftParen);
    setState(438);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(437);
      methodArgSeq();
    }
    setState(440);
    match(HarlequinParser::RightParen);
    setState(442);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(441);
      constQualifier();
    }
    setState(446);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftBrace: {
        setState(444);
        codeBlock();
        break;
      }

      case HarlequinParser::Term: {
        setState(445);
        match(HarlequinParser::Term);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodQualifierContext ------------------------------------------------------------------

HarlequinParser::MethodQualifierContext::MethodQualifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::MethodQualifierContext::ConstKw() {
  return getToken(HarlequinParser::ConstKw, 0);
}


size_t HarlequinParser::MethodQualifierContext::getRuleIndex() const {
  return HarlequinParser::RuleMethodQualifier;
}


std::any HarlequinParser::MethodQualifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitMethodQualifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::MethodQualifierContext* HarlequinParser::methodQualifier() {
  MethodQualifierContext *_localctx = _tracker.createInstance<MethodQualifierContext>(_ctx, getState());
  enterRule(_localctx, 80, HarlequinParser::RuleMethodQualifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(448);
    match(HarlequinParser::ConstKw);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodStorageModifierContext ------------------------------------------------------------------

HarlequinParser::MethodStorageModifierContext::MethodStorageModifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::MethodStorageModifierContext::InlineKw() {
  return getToken(HarlequinParser::InlineKw, 0);
}

tree::TerminalNode* HarlequinParser::MethodStorageModifierContext::NativeKw() {
  return getToken(HarlequinParser::NativeKw, 0);
}

tree::TerminalNode* HarlequinParser::MethodStorageModifierContext::VirtualKw() {
  return getToken(HarlequinParser::VirtualKw, 0);
}


size_t HarlequinParser::MethodStorageModifierContext::getRuleIndex() const {
  return HarlequinParser::RuleMethodStorageModifier;
}


std::any HarlequinParser::MethodStorageModifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitMethodStorageModifier(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::MethodStorageModifierContext* HarlequinParser::methodStorageModifier() {
  MethodStorageModifierContext *_localctx = _tracker.createInstance<MethodStorageModifierContext>(_ctx, getState());
  enterRule(_localctx, 82, HarlequinParser::RuleMethodStorageModifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(450);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 106496) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodArgSeqContext ------------------------------------------------------------------

HarlequinParser::MethodArgSeqContext::MethodArgSeqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<HarlequinParser::MethodArgContext *> HarlequinParser::MethodArgSeqContext::methodArg() {
  return getRuleContexts<HarlequinParser::MethodArgContext>();
}

HarlequinParser::MethodArgContext* HarlequinParser::MethodArgSeqContext::methodArg(size_t i) {
  return getRuleContext<HarlequinParser::MethodArgContext>(i);
}

std::vector<tree::TerminalNode *> HarlequinParser::MethodArgSeqContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::MethodArgSeqContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::MethodArgSeqContext::getRuleIndex() const {
  return HarlequinParser::RuleMethodArgSeq;
}


std::any HarlequinParser::MethodArgSeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitMethodArgSeq(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::MethodArgSeqContext* HarlequinParser::methodArgSeq() {
  MethodArgSeqContext *_localctx = _tracker.createInstance<MethodArgSeqContext>(_ctx, getState());
  enterRule(_localctx, 84, HarlequinParser::RuleMethodArgSeq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(452);
    methodArg();
    setState(457);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(453);
        match(HarlequinParser::Comma);
        setState(454);
        methodArg(); 
      }
      setState(459);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx);
    }
    setState(461);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(460);
      match(HarlequinParser::Comma);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MethodArgContext ------------------------------------------------------------------

HarlequinParser::MethodArgContext::MethodArgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::ArgVarDeclSpecSeqContext* HarlequinParser::MethodArgContext::argVarDeclSpecSeq() {
  return getRuleContext<HarlequinParser::ArgVarDeclSpecSeqContext>(0);
}

HarlequinParser::TypeNameDeclContext* HarlequinParser::MethodArgContext::typeNameDecl() {
  return getRuleContext<HarlequinParser::TypeNameDeclContext>(0);
}

tree::TerminalNode* HarlequinParser::MethodArgContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

tree::TerminalNode* HarlequinParser::MethodArgContext::Assign() {
  return getToken(HarlequinParser::Assign, 0);
}

HarlequinParser::ExprContext* HarlequinParser::MethodArgContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::MethodArgContext::getRuleIndex() const {
  return HarlequinParser::RuleMethodArg;
}


std::any HarlequinParser::MethodArgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitMethodArg(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::MethodArgContext* HarlequinParser::methodArg() {
  MethodArgContext *_localctx = _tracker.createInstance<MethodArgContext>(_ctx, getState());
  enterRule(_localctx, 86, HarlequinParser::RuleMethodArg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(463);
    argVarDeclSpecSeq();
    setState(464);
    typeNameDecl();
    setState(470);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Id) {
      setState(465);
      match(HarlequinParser::Id);
      setState(468);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == HarlequinParser::Assign) {
        setState(466);
        match(HarlequinParser::Assign);
        setState(467);
        expr(0);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CodeBlockContext ------------------------------------------------------------------

HarlequinParser::CodeBlockContext::CodeBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CodeBlockContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::CodeBlockContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::CodeBlockStmtContext *> HarlequinParser::CodeBlockContext::codeBlockStmt() {
  return getRuleContexts<HarlequinParser::CodeBlockStmtContext>();
}

HarlequinParser::CodeBlockStmtContext* HarlequinParser::CodeBlockContext::codeBlockStmt(size_t i) {
  return getRuleContext<HarlequinParser::CodeBlockStmtContext>(i);
}


size_t HarlequinParser::CodeBlockContext::getRuleIndex() const {
  return HarlequinParser::RuleCodeBlock;
}


std::any HarlequinParser::CodeBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCodeBlock(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CodeBlockContext* HarlequinParser::codeBlock() {
  CodeBlockContext *_localctx = _tracker.createInstance<CodeBlockContext>(_ctx, getState());
  enterRule(_localctx, 88, HarlequinParser::RuleCodeBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(472);
    match(HarlequinParser::LeftBrace);
    setState(476);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 101174927183906) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 128527) != 0)) {
      setState(473);
      codeBlockStmt();
      setState(478);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(479);
    match(HarlequinParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CodeBlockStmtContext ------------------------------------------------------------------

HarlequinParser::CodeBlockStmtContext::CodeBlockStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::CodeBlockContext* HarlequinParser::CodeBlockStmtContext::codeBlock() {
  return getRuleContext<HarlequinParser::CodeBlockContext>(0);
}

HarlequinParser::LocalVarDeclStmtContext* HarlequinParser::CodeBlockStmtContext::localVarDeclStmt() {
  return getRuleContext<HarlequinParser::LocalVarDeclStmtContext>(0);
}

HarlequinParser::ExprStmtContext* HarlequinParser::CodeBlockStmtContext::exprStmt() {
  return getRuleContext<HarlequinParser::ExprStmtContext>(0);
}


size_t HarlequinParser::CodeBlockStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleCodeBlockStmt;
}


std::any HarlequinParser::CodeBlockStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCodeBlockStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CodeBlockStmtContext* HarlequinParser::codeBlockStmt() {
  CodeBlockStmtContext *_localctx = _tracker.createInstance<CodeBlockStmtContext>(_ctx, getState());
  enterRule(_localctx, 90, HarlequinParser::RuleCodeBlockStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(484);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(481);
      codeBlock();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(482);
      localVarDeclStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(483);
      exprStmt();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CodeStmtContext ------------------------------------------------------------------

HarlequinParser::CodeStmtContext::CodeStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::CodeBlockContext* HarlequinParser::CodeStmtContext::codeBlock() {
  return getRuleContext<HarlequinParser::CodeBlockContext>(0);
}

HarlequinParser::ExprStmtContext* HarlequinParser::CodeStmtContext::exprStmt() {
  return getRuleContext<HarlequinParser::ExprStmtContext>(0);
}


size_t HarlequinParser::CodeStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleCodeStmt;
}


std::any HarlequinParser::CodeStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCodeStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CodeStmtContext* HarlequinParser::codeStmt() {
  CodeStmtContext *_localctx = _tracker.createInstance<CodeStmtContext>(_ctx, getState());
  enterRule(_localctx, 92, HarlequinParser::RuleCodeStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(488);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 57, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(486);
      codeBlock();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(487);
      exprStmt();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprStmtContext ------------------------------------------------------------------

HarlequinParser::ExprStmtContext::ExprStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ExprStmtContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}

HarlequinParser::ExprContext* HarlequinParser::ExprStmtContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}

HarlequinParser::ReturnStmtContext* HarlequinParser::ExprStmtContext::returnStmt() {
  return getRuleContext<HarlequinParser::ReturnStmtContext>(0);
}

HarlequinParser::ConditionStmtContext* HarlequinParser::ExprStmtContext::conditionStmt() {
  return getRuleContext<HarlequinParser::ConditionStmtContext>(0);
}

HarlequinParser::LoopStmtContext* HarlequinParser::ExprStmtContext::loopStmt() {
  return getRuleContext<HarlequinParser::LoopStmtContext>(0);
}

HarlequinParser::ExceptionHandlerContext* HarlequinParser::ExprStmtContext::exceptionHandler() {
  return getRuleContext<HarlequinParser::ExceptionHandlerContext>(0);
}

HarlequinParser::LambdaDeclContext* HarlequinParser::ExprStmtContext::lambdaDecl() {
  return getRuleContext<HarlequinParser::LambdaDeclContext>(0);
}

HarlequinParser::AsmBlockContext* HarlequinParser::ExprStmtContext::asmBlock() {
  return getRuleContext<HarlequinParser::AsmBlockContext>(0);
}


size_t HarlequinParser::ExprStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleExprStmt;
}


std::any HarlequinParser::ExprStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ExprStmtContext* HarlequinParser::exprStmt() {
  ExprStmtContext *_localctx = _tracker.createInstance<ExprStmtContext>(_ctx, getState());
  enterRule(_localctx, 94, HarlequinParser::RuleExprStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(500);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftParen:
      case HarlequinParser::LeftBrace:
      case HarlequinParser::ReturnKw:
      case HarlequinParser::BreakKw:
      case HarlequinParser::ContinueKw:
      case HarlequinParser::NewKw:
      case HarlequinParser::CopyIntrinKw:
      case HarlequinParser::Sub:
      case HarlequinParser::Incr:
      case HarlequinParser::Decr:
      case HarlequinParser::Excl:
      case HarlequinParser::Tilde:
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit:
      case HarlequinParser::Id:
      case HarlequinParser::Term: {
        enterOuterAlt(_localctx, 1);
        setState(492);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case HarlequinParser::LeftParen:
          case HarlequinParser::LeftBrace:
          case HarlequinParser::BreakKw:
          case HarlequinParser::ContinueKw:
          case HarlequinParser::NewKw:
          case HarlequinParser::CopyIntrinKw:
          case HarlequinParser::Sub:
          case HarlequinParser::Incr:
          case HarlequinParser::Decr:
          case HarlequinParser::Excl:
          case HarlequinParser::Tilde:
          case HarlequinParser::NullLit:
          case HarlequinParser::BoolLit:
          case HarlequinParser::IntLit:
          case HarlequinParser::RealLit:
          case HarlequinParser::StrLit:
          case HarlequinParser::Id: {
            setState(490);
            expr(0);
            break;
          }

          case HarlequinParser::ReturnKw: {
            setState(491);
            returnStmt();
            break;
          }

          case HarlequinParser::Term: {
            break;
          }

        default:
          break;
        }
        setState(494);
        match(HarlequinParser::Term);
        break;
      }

      case HarlequinParser::IfKw:
      case HarlequinParser::SwitchKw: {
        enterOuterAlt(_localctx, 2);
        setState(495);
        conditionStmt();
        break;
      }

      case HarlequinParser::ForKw:
      case HarlequinParser::DoKw:
      case HarlequinParser::WhileKw: {
        enterOuterAlt(_localctx, 3);
        setState(496);
        loopStmt();
        break;
      }

      case HarlequinParser::TryKw: {
        enterOuterAlt(_localctx, 4);
        setState(497);
        exceptionHandler();
        break;
      }

      case HarlequinParser::LambdaKw: {
        enterOuterAlt(_localctx, 5);
        setState(498);
        lambdaDecl();
        break;
      }

      case HarlequinParser::AsmIntrinKw: {
        enterOuterAlt(_localctx, 6);
        setState(499);
        asmBlock();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

HarlequinParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t HarlequinParser::ExprContext::getRuleIndex() const {
  return HarlequinParser::RuleExpr;
}

void HarlequinParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExprIntrinsicContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprIntrinsicContext::CopyIntrinKw() {
  return getToken(HarlequinParser::CopyIntrinKw, 0);
}

HarlequinParser::ExprIntrinsicContext::ExprIntrinsicContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprIntrinsicContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprIntrinsic(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprEqualityContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprEqualityContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprEqualityContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprEqualityContext::LessThan() {
  return getToken(HarlequinParser::LessThan, 0);
}

tree::TerminalNode* HarlequinParser::ExprEqualityContext::LessThanOrEqualTo() {
  return getToken(HarlequinParser::LessThanOrEqualTo, 0);
}

tree::TerminalNode* HarlequinParser::ExprEqualityContext::GreaterThan() {
  return getToken(HarlequinParser::GreaterThan, 0);
}

tree::TerminalNode* HarlequinParser::ExprEqualityContext::GreaterThanOrEqualTo() {
  return getToken(HarlequinParser::GreaterThanOrEqualTo, 0);
}

tree::TerminalNode* HarlequinParser::ExprEqualityContext::EqualTo() {
  return getToken(HarlequinParser::EqualTo, 0);
}

tree::TerminalNode* HarlequinParser::ExprEqualityContext::NotEqualTo() {
  return getToken(HarlequinParser::NotEqualTo, 0);
}

HarlequinParser::ExprEqualityContext::ExprEqualityContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprEqualityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprEquality(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprControlFlowContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprControlFlowContext::BreakKw() {
  return getToken(HarlequinParser::BreakKw, 0);
}

tree::TerminalNode* HarlequinParser::ExprControlFlowContext::ContinueKw() {
  return getToken(HarlequinParser::ContinueKw, 0);
}

HarlequinParser::ExprControlFlowContext::ExprControlFlowContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprControlFlowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprControlFlow(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprLiteralContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprLiteralContext::NullLit() {
  return getToken(HarlequinParser::NullLit, 0);
}

tree::TerminalNode* HarlequinParser::ExprLiteralContext::BoolLit() {
  return getToken(HarlequinParser::BoolLit, 0);
}

tree::TerminalNode* HarlequinParser::ExprLiteralContext::RealLit() {
  return getToken(HarlequinParser::RealLit, 0);
}

tree::TerminalNode* HarlequinParser::ExprLiteralContext::IntLit() {
  return getToken(HarlequinParser::IntLit, 0);
}

HarlequinParser::ExprLiteralContext::ExprLiteralContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprLiteral(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprDotContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprDotContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprDotContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprDotContext::Dot() {
  return getToken(HarlequinParser::Dot, 0);
}

HarlequinParser::ExprDotContext::ExprDotContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprDotContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprDot(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprArithmeticContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprArithmeticContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprArithmeticContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprArithmeticContext::Mult() {
  return getToken(HarlequinParser::Mult, 0);
}

tree::TerminalNode* HarlequinParser::ExprArithmeticContext::Div() {
  return getToken(HarlequinParser::Div, 0);
}

tree::TerminalNode* HarlequinParser::ExprArithmeticContext::Mod() {
  return getToken(HarlequinParser::Mod, 0);
}

tree::TerminalNode* HarlequinParser::ExprArithmeticContext::Add() {
  return getToken(HarlequinParser::Add, 0);
}

tree::TerminalNode* HarlequinParser::ExprArithmeticContext::Sub() {
  return getToken(HarlequinParser::Sub, 0);
}

HarlequinParser::ExprArithmeticContext::ExprArithmeticContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprArithmeticContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprArithmetic(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprParenContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprParenContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

HarlequinParser::ExprContext* HarlequinParser::ExprParenContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}

tree::TerminalNode* HarlequinParser::ExprParenContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::ExprParenContext::ExprParenContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprUnaryPostfixContext ------------------------------------------------------------------

HarlequinParser::ExprContext* HarlequinParser::ExprUnaryPostfixContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPostfixContext::Incr() {
  return getToken(HarlequinParser::Incr, 0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPostfixContext::Decr() {
  return getToken(HarlequinParser::Decr, 0);
}

HarlequinParser::ExprUnaryPostfixContext::ExprUnaryPostfixContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprUnaryPostfixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprUnaryPostfix(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprInstantiateContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprInstantiateContext::NewKw() {
  return getToken(HarlequinParser::NewKw, 0);
}

tree::TerminalNode* HarlequinParser::ExprInstantiateContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

HarlequinParser::CtorCallContext* HarlequinParser::ExprInstantiateContext::ctorCall() {
  return getRuleContext<HarlequinParser::CtorCallContext>(0);
}

HarlequinParser::ArrayTypeDefContext* HarlequinParser::ExprInstantiateContext::arrayTypeDef() {
  return getRuleContext<HarlequinParser::ArrayTypeDefContext>(0);
}

HarlequinParser::ArrayTypeDeclContext* HarlequinParser::ExprInstantiateContext::arrayTypeDecl() {
  return getRuleContext<HarlequinParser::ArrayTypeDeclContext>(0);
}

HarlequinParser::BracedInitializerContext* HarlequinParser::ExprInstantiateContext::bracedInitializer() {
  return getRuleContext<HarlequinParser::BracedInitializerContext>(0);
}

HarlequinParser::ExprInstantiateContext::ExprInstantiateContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprInstantiateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprInstantiate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprExponentContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprExponentContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprExponentContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprExponentContext::Expo() {
  return getToken(HarlequinParser::Expo, 0);
}

HarlequinParser::ExprExponentContext::ExprExponentContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprExponentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprExponent(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprCallContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprCallContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprCallContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprCallContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

tree::TerminalNode* HarlequinParser::ExprCallContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::ExprCallContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::ExprCallContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}

HarlequinParser::ExprCallContext::ExprCallContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprCall(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprIdContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprIdContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

HarlequinParser::ExprIdContext::ExprIdContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprId(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprAssignmentContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprAssignmentContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprAssignmentContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::Assign() {
  return getToken(HarlequinParser::Assign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::ExpoAssign() {
  return getToken(HarlequinParser::ExpoAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::AddAssign() {
  return getToken(HarlequinParser::AddAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::SubAssign() {
  return getToken(HarlequinParser::SubAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::MultAssign() {
  return getToken(HarlequinParser::MultAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::DivAssign() {
  return getToken(HarlequinParser::DivAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::ModAssign() {
  return getToken(HarlequinParser::ModAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::LeftRotateAssign() {
  return getToken(HarlequinParser::LeftRotateAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::RightRotateAssign() {
  return getToken(HarlequinParser::RightRotateAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::LeftShiftAssign() {
  return getToken(HarlequinParser::LeftShiftAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::RightShiftAssign() {
  return getToken(HarlequinParser::RightShiftAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::BitAndAssign() {
  return getToken(HarlequinParser::BitAndAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::BitXorAssign() {
  return getToken(HarlequinParser::BitXorAssign, 0);
}

tree::TerminalNode* HarlequinParser::ExprAssignmentContext::BitOrAssign() {
  return getToken(HarlequinParser::BitOrAssign, 0);
}

HarlequinParser::ExprAssignmentContext::ExprAssignmentContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprAssignment(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprIndexContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprIndexContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprIndexContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprIndexContext::LeftBracket() {
  return getToken(HarlequinParser::LeftBracket, 0);
}

tree::TerminalNode* HarlequinParser::ExprIndexContext::RightBracket() {
  return getToken(HarlequinParser::RightBracket, 0);
}

HarlequinParser::ExprIndexContext::ExprIndexContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprIndexContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprIndex(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprLogicContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprLogicContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprLogicContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprLogicContext::LogicAnd() {
  return getToken(HarlequinParser::LogicAnd, 0);
}

tree::TerminalNode* HarlequinParser::ExprLogicContext::LogicOr() {
  return getToken(HarlequinParser::LogicOr, 0);
}

HarlequinParser::ExprLogicContext::ExprLogicContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprLogicContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprLogic(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprArrayContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::ExprArrayContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::ExprArrayContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::ArrayParamSeqContext *> HarlequinParser::ExprArrayContext::arrayParamSeq() {
  return getRuleContexts<HarlequinParser::ArrayParamSeqContext>();
}

HarlequinParser::ArrayParamSeqContext* HarlequinParser::ExprArrayContext::arrayParamSeq(size_t i) {
  return getRuleContext<HarlequinParser::ArrayParamSeqContext>(i);
}

HarlequinParser::ExprArrayContext::ExprArrayContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprArrayContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprArray(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprBitwiseContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprBitwiseContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprBitwiseContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprBitwiseContext::BitAnd() {
  return getToken(HarlequinParser::BitAnd, 0);
}

tree::TerminalNode* HarlequinParser::ExprBitwiseContext::BitXor() {
  return getToken(HarlequinParser::BitXor, 0);
}

tree::TerminalNode* HarlequinParser::ExprBitwiseContext::BitOr() {
  return getToken(HarlequinParser::BitOr, 0);
}

HarlequinParser::ExprBitwiseContext::ExprBitwiseContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprBitwiseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprBitwise(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprUnaryPrefixContext ------------------------------------------------------------------

HarlequinParser::ExprContext* HarlequinParser::ExprUnaryPrefixContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPrefixContext::Incr() {
  return getToken(HarlequinParser::Incr, 0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPrefixContext::Decr() {
  return getToken(HarlequinParser::Decr, 0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPrefixContext::Sub() {
  return getToken(HarlequinParser::Sub, 0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPrefixContext::Excl() {
  return getToken(HarlequinParser::Excl, 0);
}

tree::TerminalNode* HarlequinParser::ExprUnaryPrefixContext::Tilde() {
  return getToken(HarlequinParser::Tilde, 0);
}

HarlequinParser::ExprUnaryPrefixContext::ExprUnaryPrefixContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprUnaryPrefixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprUnaryPrefix(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprStrLiteralContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> HarlequinParser::ExprStrLiteralContext::StrLit() {
  return getTokens(HarlequinParser::StrLit);
}

tree::TerminalNode* HarlequinParser::ExprStrLiteralContext::StrLit(size_t i) {
  return getToken(HarlequinParser::StrLit, i);
}

HarlequinParser::ExprStrLiteralContext::ExprStrLiteralContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprStrLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprStrLiteral(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExprBitShiftContext ------------------------------------------------------------------

std::vector<HarlequinParser::ExprContext *> HarlequinParser::ExprBitShiftContext::expr() {
  return getRuleContexts<HarlequinParser::ExprContext>();
}

HarlequinParser::ExprContext* HarlequinParser::ExprBitShiftContext::expr(size_t i) {
  return getRuleContext<HarlequinParser::ExprContext>(i);
}

tree::TerminalNode* HarlequinParser::ExprBitShiftContext::LeftRotate() {
  return getToken(HarlequinParser::LeftRotate, 0);
}

tree::TerminalNode* HarlequinParser::ExprBitShiftContext::RightRotate() {
  return getToken(HarlequinParser::RightRotate, 0);
}

tree::TerminalNode* HarlequinParser::ExprBitShiftContext::LeftShift() {
  return getToken(HarlequinParser::LeftShift, 0);
}

tree::TerminalNode* HarlequinParser::ExprBitShiftContext::RightShift() {
  return getToken(HarlequinParser::RightShift, 0);
}

HarlequinParser::ExprBitShiftContext::ExprBitShiftContext(ExprContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::ExprBitShiftContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExprBitShift(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ExprContext* HarlequinParser::expr() {
   return expr(0);
}

HarlequinParser::ExprContext* HarlequinParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  HarlequinParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  HarlequinParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 96;
  enterRecursionRule(_localctx, 96, HarlequinParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(540);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftParen: {
        _localctx = _tracker.createInstance<ExprParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(503);
        match(HarlequinParser::LeftParen);
        setState(504);
        expr(0);
        setState(505);
        match(HarlequinParser::RightParen);
        break;
      }

      case HarlequinParser::LeftBrace: {
        _localctx = _tracker.createInstance<ExprArrayContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(507);
        match(HarlequinParser::LeftBrace);
        setState(511);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 80)) & 62991) != 0)) {
          setState(508);
          arrayParamSeq();
          setState(513);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(514);
        match(HarlequinParser::RightBrace);
        break;
      }

      case HarlequinParser::Sub:
      case HarlequinParser::Incr:
      case HarlequinParser::Decr:
      case HarlequinParser::Excl:
      case HarlequinParser::Tilde: {
        _localctx = _tracker.createInstance<ExprUnaryPrefixContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(515);
        _la = _input->LA(1);
        if (!(((((_la - 46) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 46)) & 257698037761) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(516);
        expr(18);
        break;
      }

      case HarlequinParser::NewKw: {
        _localctx = _tracker.createInstance<ExprInstantiateContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(517);
        match(HarlequinParser::NewKw);
        setState(519);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 61, _ctx)) {
        case 1: {
          setState(518);
          match(HarlequinParser::Id);
          break;
        }

        default:
          break;
        }
        setState(522);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
        case 1: {
          setState(521);
          ctorCall();
          break;
        }

        default:
          break;
        }
        setState(526);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx)) {
        case 1: {
          setState(524);
          arrayTypeDef();
          break;
        }

        case 2: {
          setState(525);
          arrayTypeDecl();
          break;
        }

        default:
          break;
        }
        setState(529);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx)) {
        case 1: {
          setState(528);
          bracedInitializer();
          break;
        }

        default:
          break;
        }
        break;
      }

      case HarlequinParser::BreakKw:
      case HarlequinParser::ContinueKw: {
        _localctx = _tracker.createInstance<ExprControlFlowContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(531);
        _la = _input->LA(1);
        if (!(_la == HarlequinParser::BreakKw

        || _la == HarlequinParser::ContinueKw)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case HarlequinParser::CopyIntrinKw: {
        _localctx = _tracker.createInstance<ExprIntrinsicContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(532);
        match(HarlequinParser::CopyIntrinKw);
        break;
      }

      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit: {
        _localctx = _tracker.createInstance<ExprLiteralContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(533);
        _la = _input->LA(1);
        if (!(((((_la - 89) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 89)) & 27) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case HarlequinParser::StrLit: {
        _localctx = _tracker.createInstance<ExprStrLiteralContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(535); 
        _errHandler->sync(this);
        alt = 1;
        do {
          switch (alt) {
            case 1: {
                  setState(534);
                  match(HarlequinParser::StrLit);
                  break;
                }

          default:
            throw NoViableAltException(this);
          }
          setState(537); 
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx);
        } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
        break;
      }

      case HarlequinParser::Id: {
        _localctx = _tracker.createInstance<ExprIdContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(539);
        match(HarlequinParser::Id);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(603);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 70, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(601);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ExprExponentContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(542);

          if (!(precpred(_ctx, 24))) throw FailedPredicateException(this, "precpred(_ctx, 24)");
          setState(543);
          match(HarlequinParser::Expo);
          setState(544);
          expr(24);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ExprDotContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(545);

          if (!(precpred(_ctx, 20))) throw FailedPredicateException(this, "precpred(_ctx, 20)");
          setState(546);
          match(HarlequinParser::Dot);
          setState(547);
          expr(21);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ExprArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(548);

          if (!(precpred(_ctx, 17))) throw FailedPredicateException(this, "precpred(_ctx, 17)");
          setState(549);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 985162418487296) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(550);
          expr(18);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<ExprArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(551);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(552);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::Add

          || _la == HarlequinParser::Sub)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(553);
          expr(17);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<ExprBitShiftContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(554);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");
          setState(555);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 16888498602639360) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(556);
          expr(16);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<ExprEqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(557);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(558);
          _la = _input->LA(1);
          if (!(((((_la - 74) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 74)) & 15) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(559);
          expr(15);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<ExprEqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(560);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(561);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::EqualTo

          || _la == HarlequinParser::NotEqualTo)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(562);
          expr(14);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(563);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(564);
          match(HarlequinParser::BitAnd);
          setState(565);
          expr(13);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(566);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(567);
          match(HarlequinParser::BitXor);
          setState(568);
          expr(12);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(569);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(570);
          match(HarlequinParser::BitOr);
          setState(571);
          expr(11);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<ExprLogicContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(572);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(573);
          match(HarlequinParser::LogicAnd);
          setState(574);
          expr(10);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<ExprLogicContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(575);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(576);
          match(HarlequinParser::LogicOr);
          setState(577);
          expr(9);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<ExprAssignmentContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(578);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(579);
          _la = _input->LA(1);
          if (!(((((_la - 57) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 57)) & 4202495) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(580);
          expr(8);
          break;
        }

        case 14: {
          auto newContext = _tracker.createInstance<ExprUnaryPostfixContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(581);

          if (!(precpred(_ctx, 25))) throw FailedPredicateException(this, "precpred(_ctx, 25)");
          setState(582);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::Incr

          || _la == HarlequinParser::Decr)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          break;
        }

        case 15: {
          auto newContext = _tracker.createInstance<ExprCallContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(583);

          if (!(precpred(_ctx, 23))) throw FailedPredicateException(this, "precpred(_ctx, 23)");
          setState(584);
          match(HarlequinParser::LeftParen);
          setState(593);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 80)) & 62991) != 0)) {
            setState(585);
            expr(0);
            setState(590);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == HarlequinParser::Comma) {
              setState(586);
              match(HarlequinParser::Comma);
              setState(587);
              expr(0);
              setState(592);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(595);
          match(HarlequinParser::RightParen);
          break;
        }

        case 16: {
          auto newContext = _tracker.createInstance<ExprIndexContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(596);

          if (!(precpred(_ctx, 21))) throw FailedPredicateException(this, "precpred(_ctx, 21)");
          setState(597);
          match(HarlequinParser::LeftBracket);
          setState(598);
          expr(0);
          setState(599);
          match(HarlequinParser::RightBracket);
          break;
        }

        default:
          break;
        } 
      }
      setState(605);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 70, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

HarlequinParser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::ReturnStmtContext::ReturnKw() {
  return getToken(HarlequinParser::ReturnKw, 0);
}

HarlequinParser::ExprContext* HarlequinParser::ReturnStmtContext::expr() {
  return getRuleContext<HarlequinParser::ExprContext>(0);
}


size_t HarlequinParser::ReturnStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleReturnStmt;
}


std::any HarlequinParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ReturnStmtContext* HarlequinParser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 98, HarlequinParser::RuleReturnStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(606);
    match(HarlequinParser::ReturnKw);
    setState(607);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoopStmtContext ------------------------------------------------------------------

HarlequinParser::LoopStmtContext::LoopStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::LoopWhileDefContext* HarlequinParser::LoopStmtContext::loopWhileDef() {
  return getRuleContext<HarlequinParser::LoopWhileDefContext>(0);
}

HarlequinParser::LoopDoWhileDefContext* HarlequinParser::LoopStmtContext::loopDoWhileDef() {
  return getRuleContext<HarlequinParser::LoopDoWhileDefContext>(0);
}

HarlequinParser::LoopForDefContext* HarlequinParser::LoopStmtContext::loopForDef() {
  return getRuleContext<HarlequinParser::LoopForDefContext>(0);
}


size_t HarlequinParser::LoopStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleLoopStmt;
}


std::any HarlequinParser::LoopStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLoopStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LoopStmtContext* HarlequinParser::loopStmt() {
  LoopStmtContext *_localctx = _tracker.createInstance<LoopStmtContext>(_ctx, getState());
  enterRule(_localctx, 100, HarlequinParser::RuleLoopStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(612);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::WhileKw: {
        enterOuterAlt(_localctx, 1);
        setState(609);
        loopWhileDef();
        break;
      }

      case HarlequinParser::DoKw: {
        enterOuterAlt(_localctx, 2);
        setState(610);
        loopDoWhileDef();
        break;
      }

      case HarlequinParser::ForKw: {
        enterOuterAlt(_localctx, 3);
        setState(611);
        loopForDef();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoopWhileDefContext ------------------------------------------------------------------

HarlequinParser::LoopWhileDefContext::LoopWhileDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::LoopWhileDefContext::WhileKw() {
  return getToken(HarlequinParser::WhileKw, 0);
}

tree::TerminalNode* HarlequinParser::LoopWhileDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::LoopWhileDefContext::ctrlFlowExpr() {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(0);
}

tree::TerminalNode* HarlequinParser::LoopWhileDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::LoopWhileDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::LoopWhileDefContext::getRuleIndex() const {
  return HarlequinParser::RuleLoopWhileDef;
}


std::any HarlequinParser::LoopWhileDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLoopWhileDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LoopWhileDefContext* HarlequinParser::loopWhileDef() {
  LoopWhileDefContext *_localctx = _tracker.createInstance<LoopWhileDefContext>(_ctx, getState());
  enterRule(_localctx, 102, HarlequinParser::RuleLoopWhileDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(614);
    match(HarlequinParser::WhileKw);
    setState(615);
    match(HarlequinParser::LeftParen);
    setState(616);
    ctrlFlowExpr();
    setState(617);
    match(HarlequinParser::RightParen);
    setState(618);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoopDoWhileDefContext ------------------------------------------------------------------

HarlequinParser::LoopDoWhileDefContext::LoopDoWhileDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::LoopDoWhileDefContext::DoKw() {
  return getToken(HarlequinParser::DoKw, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::LoopDoWhileDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}

tree::TerminalNode* HarlequinParser::LoopDoWhileDefContext::WhileKw() {
  return getToken(HarlequinParser::WhileKw, 0);
}

tree::TerminalNode* HarlequinParser::LoopDoWhileDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::LoopDoWhileDefContext::ctrlFlowExpr() {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(0);
}

tree::TerminalNode* HarlequinParser::LoopDoWhileDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

tree::TerminalNode* HarlequinParser::LoopDoWhileDefContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}


size_t HarlequinParser::LoopDoWhileDefContext::getRuleIndex() const {
  return HarlequinParser::RuleLoopDoWhileDef;
}


std::any HarlequinParser::LoopDoWhileDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLoopDoWhileDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LoopDoWhileDefContext* HarlequinParser::loopDoWhileDef() {
  LoopDoWhileDefContext *_localctx = _tracker.createInstance<LoopDoWhileDefContext>(_ctx, getState());
  enterRule(_localctx, 104, HarlequinParser::RuleLoopDoWhileDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(620);
    match(HarlequinParser::DoKw);
    setState(621);
    codeStmt();
    setState(622);
    match(HarlequinParser::WhileKw);
    setState(623);
    match(HarlequinParser::LeftParen);
    setState(624);
    ctrlFlowExpr();
    setState(625);
    match(HarlequinParser::RightParen);
    setState(626);
    match(HarlequinParser::Term);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LoopForDefContext ------------------------------------------------------------------

HarlequinParser::LoopForDefContext::LoopForDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::LoopForDefContext::ForKw() {
  return getToken(HarlequinParser::ForKw, 0);
}

tree::TerminalNode* HarlequinParser::LoopForDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::LoopForDefContext::Term() {
  return getTokens(HarlequinParser::Term);
}

tree::TerminalNode* HarlequinParser::LoopForDefContext::Term(size_t i) {
  return getToken(HarlequinParser::Term, i);
}

tree::TerminalNode* HarlequinParser::LoopForDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::LoopForDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}

std::vector<HarlequinParser::CtrlFlowExprContext *> HarlequinParser::LoopForDefContext::ctrlFlowExpr() {
  return getRuleContexts<HarlequinParser::CtrlFlowExprContext>();
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::LoopForDefContext::ctrlFlowExpr(size_t i) {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(i);
}


size_t HarlequinParser::LoopForDefContext::getRuleIndex() const {
  return HarlequinParser::RuleLoopForDef;
}


std::any HarlequinParser::LoopForDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitLoopForDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::LoopForDefContext* HarlequinParser::loopForDef() {
  LoopForDefContext *_localctx = _tracker.createInstance<LoopForDefContext>(_ctx, getState());
  enterRule(_localctx, 106, HarlequinParser::RuleLoopForDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(628);
    match(HarlequinParser::ForKw);
    setState(629);
    match(HarlequinParser::LeftParen);
    setState(631);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(630);
      ctrlFlowExpr();
    }
    setState(633);
    match(HarlequinParser::Term);
    setState(635);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(634);
      ctrlFlowExpr();
    }
    setState(637);
    match(HarlequinParser::Term);
    setState(639);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(638);
      ctrlFlowExpr();
    }
    setState(641);
    match(HarlequinParser::RightParen);
    setState(642);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionStmtContext ------------------------------------------------------------------

HarlequinParser::ConditionStmtContext::ConditionStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::CondIfDefContext* HarlequinParser::ConditionStmtContext::condIfDef() {
  return getRuleContext<HarlequinParser::CondIfDefContext>(0);
}

std::vector<HarlequinParser::CondElifDefContext *> HarlequinParser::ConditionStmtContext::condElifDef() {
  return getRuleContexts<HarlequinParser::CondElifDefContext>();
}

HarlequinParser::CondElifDefContext* HarlequinParser::ConditionStmtContext::condElifDef(size_t i) {
  return getRuleContext<HarlequinParser::CondElifDefContext>(i);
}

HarlequinParser::CondElseDefContext* HarlequinParser::ConditionStmtContext::condElseDef() {
  return getRuleContext<HarlequinParser::CondElseDefContext>(0);
}

HarlequinParser::CondSwitchDefContext* HarlequinParser::ConditionStmtContext::condSwitchDef() {
  return getRuleContext<HarlequinParser::CondSwitchDefContext>(0);
}

tree::TerminalNode* HarlequinParser::ConditionStmtContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::ConditionStmtContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::CondCaseDefContext *> HarlequinParser::ConditionStmtContext::condCaseDef() {
  return getRuleContexts<HarlequinParser::CondCaseDefContext>();
}

HarlequinParser::CondCaseDefContext* HarlequinParser::ConditionStmtContext::condCaseDef(size_t i) {
  return getRuleContext<HarlequinParser::CondCaseDefContext>(i);
}


size_t HarlequinParser::ConditionStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleConditionStmt;
}


std::any HarlequinParser::ConditionStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitConditionStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ConditionStmtContext* HarlequinParser::conditionStmt() {
  ConditionStmtContext *_localctx = _tracker.createInstance<ConditionStmtContext>(_ctx, getState());
  enterRule(_localctx, 108, HarlequinParser::RuleConditionStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(664);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::IfKw: {
        enterOuterAlt(_localctx, 1);
        setState(644);
        condIfDef();
        setState(648);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(645);
            condElifDef(); 
          }
          setState(650);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx);
        }
        setState(652);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 76, _ctx)) {
        case 1: {
          setState(651);
          condElseDef();
          break;
        }

        default:
          break;
        }
        break;
      }

      case HarlequinParser::SwitchKw: {
        enterOuterAlt(_localctx, 2);
        setState(654);
        condSwitchDef();
        setState(655);
        match(HarlequinParser::LeftBrace);
        setState(659);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == HarlequinParser::CaseKw

        || _la == HarlequinParser::DefaultKw) {
          setState(656);
          condCaseDef();
          setState(661);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(662);
        match(HarlequinParser::RightBrace);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondIfDefContext ------------------------------------------------------------------

HarlequinParser::CondIfDefContext::CondIfDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CondIfDefContext::IfKw() {
  return getToken(HarlequinParser::IfKw, 0);
}

tree::TerminalNode* HarlequinParser::CondIfDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::CondIfDefContext::ctrlFlowExpr() {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(0);
}

tree::TerminalNode* HarlequinParser::CondIfDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::CondIfDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::CondIfDefContext::getRuleIndex() const {
  return HarlequinParser::RuleCondIfDef;
}


std::any HarlequinParser::CondIfDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCondIfDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CondIfDefContext* HarlequinParser::condIfDef() {
  CondIfDefContext *_localctx = _tracker.createInstance<CondIfDefContext>(_ctx, getState());
  enterRule(_localctx, 110, HarlequinParser::RuleCondIfDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(666);
    match(HarlequinParser::IfKw);
    setState(667);
    match(HarlequinParser::LeftParen);
    setState(668);
    ctrlFlowExpr();
    setState(669);
    match(HarlequinParser::RightParen);
    setState(670);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondElifDefContext ------------------------------------------------------------------

HarlequinParser::CondElifDefContext::CondElifDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CondElifDefContext::ElifKw() {
  return getToken(HarlequinParser::ElifKw, 0);
}

tree::TerminalNode* HarlequinParser::CondElifDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::CondElifDefContext::ctrlFlowExpr() {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(0);
}

tree::TerminalNode* HarlequinParser::CondElifDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::CondElifDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::CondElifDefContext::getRuleIndex() const {
  return HarlequinParser::RuleCondElifDef;
}


std::any HarlequinParser::CondElifDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCondElifDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CondElifDefContext* HarlequinParser::condElifDef() {
  CondElifDefContext *_localctx = _tracker.createInstance<CondElifDefContext>(_ctx, getState());
  enterRule(_localctx, 112, HarlequinParser::RuleCondElifDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(672);
    match(HarlequinParser::ElifKw);
    setState(673);
    match(HarlequinParser::LeftParen);
    setState(674);
    ctrlFlowExpr();
    setState(675);
    match(HarlequinParser::RightParen);
    setState(676);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondElseDefContext ------------------------------------------------------------------

HarlequinParser::CondElseDefContext::CondElseDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CondElseDefContext::ElseKw() {
  return getToken(HarlequinParser::ElseKw, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::CondElseDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::CondElseDefContext::getRuleIndex() const {
  return HarlequinParser::RuleCondElseDef;
}


std::any HarlequinParser::CondElseDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCondElseDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CondElseDefContext* HarlequinParser::condElseDef() {
  CondElseDefContext *_localctx = _tracker.createInstance<CondElseDefContext>(_ctx, getState());
  enterRule(_localctx, 114, HarlequinParser::RuleCondElseDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(678);
    match(HarlequinParser::ElseKw);
    setState(679);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondSwitchDefContext ------------------------------------------------------------------

HarlequinParser::CondSwitchDefContext::CondSwitchDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CondSwitchDefContext::SwitchKw() {
  return getToken(HarlequinParser::SwitchKw, 0);
}

tree::TerminalNode* HarlequinParser::CondSwitchDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::CondSwitchDefContext::ctrlFlowExpr() {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(0);
}

tree::TerminalNode* HarlequinParser::CondSwitchDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}


size_t HarlequinParser::CondSwitchDefContext::getRuleIndex() const {
  return HarlequinParser::RuleCondSwitchDef;
}


std::any HarlequinParser::CondSwitchDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCondSwitchDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CondSwitchDefContext* HarlequinParser::condSwitchDef() {
  CondSwitchDefContext *_localctx = _tracker.createInstance<CondSwitchDefContext>(_ctx, getState());
  enterRule(_localctx, 116, HarlequinParser::RuleCondSwitchDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(681);
    match(HarlequinParser::SwitchKw);
    setState(682);
    match(HarlequinParser::LeftParen);
    setState(683);
    ctrlFlowExpr();
    setState(684);
    match(HarlequinParser::RightParen);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondCaseDefContext ------------------------------------------------------------------

HarlequinParser::CondCaseDefContext::CondCaseDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CondCaseDefContext::Colon() {
  return getToken(HarlequinParser::Colon, 0);
}

tree::TerminalNode* HarlequinParser::CondCaseDefContext::CaseKw() {
  return getToken(HarlequinParser::CaseKw, 0);
}

HarlequinParser::CondCaseResultContext* HarlequinParser::CondCaseDefContext::condCaseResult() {
  return getRuleContext<HarlequinParser::CondCaseResultContext>(0);
}

tree::TerminalNode* HarlequinParser::CondCaseDefContext::DefaultKw() {
  return getToken(HarlequinParser::DefaultKw, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::CondCaseDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::CondCaseDefContext::getRuleIndex() const {
  return HarlequinParser::RuleCondCaseDef;
}


std::any HarlequinParser::CondCaseDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCondCaseDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CondCaseDefContext* HarlequinParser::condCaseDef() {
  CondCaseDefContext *_localctx = _tracker.createInstance<CondCaseDefContext>(_ctx, getState());
  enterRule(_localctx, 118, HarlequinParser::RuleCondCaseDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(689);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::CaseKw: {
        setState(686);
        match(HarlequinParser::CaseKw);
        setState(687);
        condCaseResult();
        break;
      }

      case HarlequinParser::DefaultKw: {
        setState(688);
        match(HarlequinParser::DefaultKw);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(691);
    match(HarlequinParser::Colon);
    setState(693);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 101174927163426) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 128527) != 0)) {
      setState(692);
      codeStmt();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CondCaseResultContext ------------------------------------------------------------------

HarlequinParser::CondCaseResultContext::CondCaseResultContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t HarlequinParser::CondCaseResultContext::getRuleIndex() const {
  return HarlequinParser::RuleCondCaseResult;
}

void HarlequinParser::CondCaseResultContext::copyFrom(CondCaseResultContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CaseIdContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::CaseIdContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

HarlequinParser::CaseIdContext::CaseIdContext(CondCaseResultContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::CaseIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCaseId(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CaseLiteralContext ------------------------------------------------------------------

tree::TerminalNode* HarlequinParser::CaseLiteralContext::NullLit() {
  return getToken(HarlequinParser::NullLit, 0);
}

tree::TerminalNode* HarlequinParser::CaseLiteralContext::BoolLit() {
  return getToken(HarlequinParser::BoolLit, 0);
}

tree::TerminalNode* HarlequinParser::CaseLiteralContext::RealLit() {
  return getToken(HarlequinParser::RealLit, 0);
}

tree::TerminalNode* HarlequinParser::CaseLiteralContext::IntLit() {
  return getToken(HarlequinParser::IntLit, 0);
}

tree::TerminalNode* HarlequinParser::CaseLiteralContext::StrLit() {
  return getToken(HarlequinParser::StrLit, 0);
}

HarlequinParser::CaseLiteralContext::CaseLiteralContext(CondCaseResultContext *ctx) { copyFrom(ctx); }


std::any HarlequinParser::CaseLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCaseLiteral(this);
  else
    return visitor->visitChildren(this);
}
HarlequinParser::CondCaseResultContext* HarlequinParser::condCaseResult() {
  CondCaseResultContext *_localctx = _tracker.createInstance<CondCaseResultContext>(_ctx, getState());
  enterRule(_localctx, 120, HarlequinParser::RuleCondCaseResult);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(697);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit: {
        _localctx = _tracker.createInstance<HarlequinParser::CaseLiteralContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(695);
        _la = _input->LA(1);
        if (!(((((_la - 89) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 89)) & 59) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case HarlequinParser::Id: {
        _localctx = _tracker.createInstance<HarlequinParser::CaseIdContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(696);
        match(HarlequinParser::Id);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExceptionHandlerContext ------------------------------------------------------------------

HarlequinParser::ExceptionHandlerContext::ExceptionHandlerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::TryBlockDefContext* HarlequinParser::ExceptionHandlerContext::tryBlockDef() {
  return getRuleContext<HarlequinParser::TryBlockDefContext>(0);
}

std::vector<HarlequinParser::CatchBlockDefContext *> HarlequinParser::ExceptionHandlerContext::catchBlockDef() {
  return getRuleContexts<HarlequinParser::CatchBlockDefContext>();
}

HarlequinParser::CatchBlockDefContext* HarlequinParser::ExceptionHandlerContext::catchBlockDef(size_t i) {
  return getRuleContext<HarlequinParser::CatchBlockDefContext>(i);
}

HarlequinParser::FinallyBlockDefContext* HarlequinParser::ExceptionHandlerContext::finallyBlockDef() {
  return getRuleContext<HarlequinParser::FinallyBlockDefContext>(0);
}


size_t HarlequinParser::ExceptionHandlerContext::getRuleIndex() const {
  return HarlequinParser::RuleExceptionHandler;
}


std::any HarlequinParser::ExceptionHandlerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitExceptionHandler(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::ExceptionHandlerContext* HarlequinParser::exceptionHandler() {
  ExceptionHandlerContext *_localctx = _tracker.createInstance<ExceptionHandlerContext>(_ctx, getState());
  enterRule(_localctx, 122, HarlequinParser::RuleExceptionHandler);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(699);
    tryBlockDef();
    setState(703);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(700);
        catchBlockDef(); 
      }
      setState(705);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx);
    }
    setState(707);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 83, _ctx)) {
    case 1: {
      setState(706);
      finallyBlockDef();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TryBlockDefContext ------------------------------------------------------------------

HarlequinParser::TryBlockDefContext::TryBlockDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::TryBlockDefContext::TryKw() {
  return getToken(HarlequinParser::TryKw, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::TryBlockDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::TryBlockDefContext::getRuleIndex() const {
  return HarlequinParser::RuleTryBlockDef;
}


std::any HarlequinParser::TryBlockDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitTryBlockDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::TryBlockDefContext* HarlequinParser::tryBlockDef() {
  TryBlockDefContext *_localctx = _tracker.createInstance<TryBlockDefContext>(_ctx, getState());
  enterRule(_localctx, 124, HarlequinParser::RuleTryBlockDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(709);
    match(HarlequinParser::TryKw);
    setState(710);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CatchBlockDefContext ------------------------------------------------------------------

HarlequinParser::CatchBlockDefContext::CatchBlockDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::CatchBlockDefContext::CatchKw() {
  return getToken(HarlequinParser::CatchKw, 0);
}

tree::TerminalNode* HarlequinParser::CatchBlockDefContext::LeftParen() {
  return getToken(HarlequinParser::LeftParen, 0);
}

tree::TerminalNode* HarlequinParser::CatchBlockDefContext::RightParen() {
  return getToken(HarlequinParser::RightParen, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::CatchBlockDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}

HarlequinParser::CtrlFlowExprContext* HarlequinParser::CatchBlockDefContext::ctrlFlowExpr() {
  return getRuleContext<HarlequinParser::CtrlFlowExprContext>(0);
}


size_t HarlequinParser::CatchBlockDefContext::getRuleIndex() const {
  return HarlequinParser::RuleCatchBlockDef;
}


std::any HarlequinParser::CatchBlockDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitCatchBlockDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::CatchBlockDefContext* HarlequinParser::catchBlockDef() {
  CatchBlockDefContext *_localctx = _tracker.createInstance<CatchBlockDefContext>(_ctx, getState());
  enterRule(_localctx, 126, HarlequinParser::RuleCatchBlockDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(712);
    match(HarlequinParser::CatchKw);
    setState(713);
    match(HarlequinParser::LeftParen);
    setState(715);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(714);
      ctrlFlowExpr();
    }
    setState(717);
    match(HarlequinParser::RightParen);
    setState(718);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FinallyBlockDefContext ------------------------------------------------------------------

HarlequinParser::FinallyBlockDefContext::FinallyBlockDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::FinallyBlockDefContext::FinallyKw() {
  return getToken(HarlequinParser::FinallyKw, 0);
}

HarlequinParser::CodeStmtContext* HarlequinParser::FinallyBlockDefContext::codeStmt() {
  return getRuleContext<HarlequinParser::CodeStmtContext>(0);
}


size_t HarlequinParser::FinallyBlockDefContext::getRuleIndex() const {
  return HarlequinParser::RuleFinallyBlockDef;
}


std::any HarlequinParser::FinallyBlockDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitFinallyBlockDef(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::FinallyBlockDefContext* HarlequinParser::finallyBlockDef() {
  FinallyBlockDefContext *_localctx = _tracker.createInstance<FinallyBlockDefContext>(_ctx, getState());
  enterRule(_localctx, 128, HarlequinParser::RuleFinallyBlockDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(720);
    match(HarlequinParser::FinallyKw);
    setState(721);
    codeStmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmBlockContext ------------------------------------------------------------------

HarlequinParser::AsmBlockContext::AsmBlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AsmBlockContext::AsmIntrinKw() {
  return getToken(HarlequinParser::AsmIntrinKw, 0);
}

tree::TerminalNode* HarlequinParser::AsmBlockContext::LeftBrace() {
  return getToken(HarlequinParser::LeftBrace, 0);
}

tree::TerminalNode* HarlequinParser::AsmBlockContext::RightBrace() {
  return getToken(HarlequinParser::RightBrace, 0);
}

std::vector<HarlequinParser::AsmStmtContext *> HarlequinParser::AsmBlockContext::asmStmt() {
  return getRuleContexts<HarlequinParser::AsmStmtContext>();
}

HarlequinParser::AsmStmtContext* HarlequinParser::AsmBlockContext::asmStmt(size_t i) {
  return getRuleContext<HarlequinParser::AsmStmtContext>(i);
}


size_t HarlequinParser::AsmBlockContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmBlock;
}


std::any HarlequinParser::AsmBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmBlock(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmBlockContext* HarlequinParser::asmBlock() {
  AsmBlockContext *_localctx = _tracker.createInstance<AsmBlockContext>(_ctx, getState());
  enterRule(_localctx, 130, HarlequinParser::RuleAsmBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(723);
    match(HarlequinParser::AsmIntrinKw);
    setState(724);
    match(HarlequinParser::LeftBrace);
    setState(728);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Colon

    || _la == HarlequinParser::Id) {
      setState(725);
      asmStmt();
      setState(730);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(731);
    match(HarlequinParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmStmtContext ------------------------------------------------------------------

HarlequinParser::AsmStmtContext::AsmStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::AsmInstrContext* HarlequinParser::AsmStmtContext::asmInstr() {
  return getRuleContext<HarlequinParser::AsmInstrContext>(0);
}

HarlequinParser::AsmLabelContext* HarlequinParser::AsmStmtContext::asmLabel() {
  return getRuleContext<HarlequinParser::AsmLabelContext>(0);
}


size_t HarlequinParser::AsmStmtContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmStmt;
}


std::any HarlequinParser::AsmStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmStmt(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmStmtContext* HarlequinParser::asmStmt() {
  AsmStmtContext *_localctx = _tracker.createInstance<AsmStmtContext>(_ctx, getState());
  enterRule(_localctx, 132, HarlequinParser::RuleAsmStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(735);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::Id: {
        enterOuterAlt(_localctx, 1);
        setState(733);
        asmInstr();
        break;
      }

      case HarlequinParser::Colon: {
        enterOuterAlt(_localctx, 2);
        setState(734);
        asmLabel();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmInstrContext ------------------------------------------------------------------

HarlequinParser::AsmInstrContext::AsmInstrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AsmInstrContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

std::vector<HarlequinParser::AsmOperandContext *> HarlequinParser::AsmInstrContext::asmOperand() {
  return getRuleContexts<HarlequinParser::AsmOperandContext>();
}

HarlequinParser::AsmOperandContext* HarlequinParser::AsmInstrContext::asmOperand(size_t i) {
  return getRuleContext<HarlequinParser::AsmOperandContext>(i);
}

tree::TerminalNode* HarlequinParser::AsmInstrContext::Term() {
  return getToken(HarlequinParser::Term, 0);
}

std::vector<tree::TerminalNode *> HarlequinParser::AsmInstrContext::Comma() {
  return getTokens(HarlequinParser::Comma);
}

tree::TerminalNode* HarlequinParser::AsmInstrContext::Comma(size_t i) {
  return getToken(HarlequinParser::Comma, i);
}


size_t HarlequinParser::AsmInstrContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmInstr;
}


std::any HarlequinParser::AsmInstrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmInstr(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmInstrContext* HarlequinParser::asmInstr() {
  AsmInstrContext *_localctx = _tracker.createInstance<AsmInstrContext>(_ctx, getState());
  enterRule(_localctx, 134, HarlequinParser::RuleAsmInstr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(737);
    match(HarlequinParser::Id);
    setState(746);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 88, _ctx)) {
    case 1: {
      setState(738);
      asmOperand();
      setState(743);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == HarlequinParser::Comma) {
        setState(739);
        match(HarlequinParser::Comma);
        setState(740);
        asmOperand();
        setState(745);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    default:
      break;
    }
    setState(749);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Term) {
      setState(748);
      match(HarlequinParser::Term);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmOperandContext ------------------------------------------------------------------

HarlequinParser::AsmOperandContext::AsmOperandContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

HarlequinParser::AsmLitContext* HarlequinParser::AsmOperandContext::asmLit() {
  return getRuleContext<HarlequinParser::AsmLitContext>(0);
}

HarlequinParser::AsmRegContext* HarlequinParser::AsmOperandContext::asmReg() {
  return getRuleContext<HarlequinParser::AsmRegContext>(0);
}

HarlequinParser::AsmVarContext* HarlequinParser::AsmOperandContext::asmVar() {
  return getRuleContext<HarlequinParser::AsmVarContext>(0);
}

HarlequinParser::AsmLabelContext* HarlequinParser::AsmOperandContext::asmLabel() {
  return getRuleContext<HarlequinParser::AsmLabelContext>(0);
}


size_t HarlequinParser::AsmOperandContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmOperand;
}


std::any HarlequinParser::AsmOperandContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmOperand(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmOperandContext* HarlequinParser::asmOperand() {
  AsmOperandContext *_localctx = _tracker.createInstance<AsmOperandContext>(_ctx, getState());
  enterRule(_localctx, 136, HarlequinParser::RuleAsmOperand);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(755);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit: {
        enterOuterAlt(_localctx, 1);
        setState(751);
        asmLit();
        break;
      }

      case HarlequinParser::AsmRegId: {
        enterOuterAlt(_localctx, 2);
        setState(752);
        asmReg();
        break;
      }

      case HarlequinParser::LeftBracket: {
        enterOuterAlt(_localctx, 3);
        setState(753);
        asmVar();
        break;
      }

      case HarlequinParser::Colon: {
        enterOuterAlt(_localctx, 4);
        setState(754);
        asmLabel();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmLitContext ------------------------------------------------------------------

HarlequinParser::AsmLitContext::AsmLitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AsmLitContext::NullLit() {
  return getToken(HarlequinParser::NullLit, 0);
}

tree::TerminalNode* HarlequinParser::AsmLitContext::BoolLit() {
  return getToken(HarlequinParser::BoolLit, 0);
}

tree::TerminalNode* HarlequinParser::AsmLitContext::RealLit() {
  return getToken(HarlequinParser::RealLit, 0);
}

tree::TerminalNode* HarlequinParser::AsmLitContext::IntLit() {
  return getToken(HarlequinParser::IntLit, 0);
}

tree::TerminalNode* HarlequinParser::AsmLitContext::StrLit() {
  return getToken(HarlequinParser::StrLit, 0);
}


size_t HarlequinParser::AsmLitContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmLit;
}


std::any HarlequinParser::AsmLitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmLit(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmLitContext* HarlequinParser::asmLit() {
  AsmLitContext *_localctx = _tracker.createInstance<AsmLitContext>(_ctx, getState());
  enterRule(_localctx, 138, HarlequinParser::RuleAsmLit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(757);
    _la = _input->LA(1);
    if (!(((((_la - 89) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 89)) & 59) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmRegContext ------------------------------------------------------------------

HarlequinParser::AsmRegContext::AsmRegContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AsmRegContext::AsmRegId() {
  return getToken(HarlequinParser::AsmRegId, 0);
}


size_t HarlequinParser::AsmRegContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmReg;
}


std::any HarlequinParser::AsmRegContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmReg(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmRegContext* HarlequinParser::asmReg() {
  AsmRegContext *_localctx = _tracker.createInstance<AsmRegContext>(_ctx, getState());
  enterRule(_localctx, 140, HarlequinParser::RuleAsmReg);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(759);
    match(HarlequinParser::AsmRegId);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmVarContext ------------------------------------------------------------------

HarlequinParser::AsmVarContext::AsmVarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AsmVarContext::LeftBracket() {
  return getToken(HarlequinParser::LeftBracket, 0);
}

tree::TerminalNode* HarlequinParser::AsmVarContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

tree::TerminalNode* HarlequinParser::AsmVarContext::RightBracket() {
  return getToken(HarlequinParser::RightBracket, 0);
}


size_t HarlequinParser::AsmVarContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmVar;
}


std::any HarlequinParser::AsmVarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmVar(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmVarContext* HarlequinParser::asmVar() {
  AsmVarContext *_localctx = _tracker.createInstance<AsmVarContext>(_ctx, getState());
  enterRule(_localctx, 142, HarlequinParser::RuleAsmVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(761);
    match(HarlequinParser::LeftBracket);
    setState(762);
    match(HarlequinParser::Id);
    setState(763);
    match(HarlequinParser::RightBracket);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsmLabelContext ------------------------------------------------------------------

HarlequinParser::AsmLabelContext::AsmLabelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* HarlequinParser::AsmLabelContext::Colon() {
  return getToken(HarlequinParser::Colon, 0);
}

tree::TerminalNode* HarlequinParser::AsmLabelContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}


size_t HarlequinParser::AsmLabelContext::getRuleIndex() const {
  return HarlequinParser::RuleAsmLabel;
}


std::any HarlequinParser::AsmLabelContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<HarlequinVisitor*>(visitor))
    return parserVisitor->visitAsmLabel(this);
  else
    return visitor->visitChildren(this);
}

HarlequinParser::AsmLabelContext* HarlequinParser::asmLabel() {
  AsmLabelContext *_localctx = _tracker.createInstance<AsmLabelContext>(_ctx, getState());
  enterRule(_localctx, 144, HarlequinParser::RuleAsmLabel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(765);
    match(HarlequinParser::Colon);
    setState(766);
    match(HarlequinParser::Id);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool HarlequinParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 48: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool HarlequinParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 24);
    case 1: return precpred(_ctx, 20);
    case 2: return precpred(_ctx, 17);
    case 3: return precpred(_ctx, 16);
    case 4: return precpred(_ctx, 15);
    case 5: return precpred(_ctx, 14);
    case 6: return precpred(_ctx, 13);
    case 7: return precpred(_ctx, 12);
    case 8: return precpred(_ctx, 11);
    case 9: return precpred(_ctx, 10);
    case 10: return precpred(_ctx, 9);
    case 11: return precpred(_ctx, 8);
    case 12: return precpred(_ctx, 7);
    case 13: return precpred(_ctx, 25);
    case 14: return precpred(_ctx, 23);
    case 15: return precpred(_ctx, 21);

  default:
    break;
  }
  return true;
}

void HarlequinParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  harlequinParserInitialize();
#else
  ::antlr4::internal::call_once(harlequinParserOnceFlag, harlequinParserInitialize);
#endif
}
