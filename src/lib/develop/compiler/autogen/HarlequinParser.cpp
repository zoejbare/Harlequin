
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
      "root", "compilerDirective", "qualifiedId", "usingStmt", "usingAliasStmt", 
      "namespaceDecl", "namespaceDef", "classDecl", "classType", "accessBaseSpecifier", 
      "accessLimitSpecifier", "accessSpecifier", "storageSpecifier", "functionSpecifier", 
      "constQualifier", "methodDeclSpecSeq", "classVarDeclSpecSeq", "localVarDeclSpecSeq", 
      "argVarDeclSpecSeq", "classInheritance", "classExtends", "classImpls", 
      "classDef", "arrayTypeDecl", "arrayTypeDef", "typeNameDecl", "bracedInitializer", 
      "ctorCall", "classVarDeclStmt", "localVarDeclStmt", "varDeclStmt", 
      "varDecl", "nextVarDecl", "ctrlFlowExpr", "arrayParamSeq", "arrayParamElem", 
      "lambdaDecl", "ctorDecl", "methodDecl", "methodQualifier", "methodStorageModifier", 
      "methodArgSeq", "methodArg", "codeBlock", "codeBlockStmt", "codeStmt", 
      "exprStmt", "expr", "returnStmt", "loopStmt", "loopWhileDef", "loopDoWhileDef", 
      "loopForDef", "conditionStmt", "condIfDef", "condElifDef", "condElseDef", 
      "condSwitchDef", "condCaseDef", "condCaseResult", "exceptionHandler", 
      "tryBlockDef", "catchBlockDef", "finallyBlockDef", "asmBlock", "asmStmt", 
      "asmInstr", "asmOperand", "asmLit", "asmReg", "asmVar", "asmLabel"
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
  	4,1,99,758,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,70,7,
  	70,2,71,7,71,1,0,1,0,1,0,1,0,5,0,149,8,0,10,0,12,0,152,9,0,1,0,1,0,1,
  	1,1,1,1,1,1,1,1,2,1,2,1,2,5,2,163,8,2,10,2,12,2,166,9,2,1,3,1,3,1,3,1,
  	3,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,6,1,6,1,6,5,6,185,8,6,10,
  	6,12,6,188,9,6,1,6,1,6,1,7,3,7,193,8,7,1,7,3,7,196,8,7,1,7,1,7,1,7,3,
  	7,201,8,7,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,10,1,10,1,10,5,10,214,8,
  	10,10,10,12,10,217,9,10,1,10,1,10,1,11,1,11,3,11,223,8,11,1,11,1,11,3,
  	11,227,8,11,3,11,229,8,11,1,12,1,12,1,13,1,13,1,14,1,14,1,15,3,15,238,
  	8,15,1,15,3,15,241,8,15,1,15,3,15,244,8,15,1,16,3,16,247,8,16,1,16,3,
  	16,250,8,16,1,16,3,16,253,8,16,1,17,3,17,256,8,17,1,17,3,17,259,8,17,
  	1,18,3,18,262,8,18,1,19,1,19,3,19,266,8,19,1,19,1,19,3,19,270,8,19,3,
  	19,272,8,19,1,20,1,20,1,20,1,21,1,21,1,21,1,21,5,21,281,8,21,10,21,12,
  	21,284,9,21,1,22,1,22,1,22,1,22,1,22,5,22,291,8,22,10,22,12,22,294,9,
  	22,1,22,1,22,1,23,1,23,3,23,300,8,23,1,23,3,23,303,8,23,1,23,1,23,1,24,
  	1,24,1,24,1,24,3,24,311,8,24,1,24,1,24,3,24,315,8,24,1,24,1,24,1,25,1,
  	25,3,25,321,8,25,1,26,1,26,3,26,325,8,26,1,26,1,26,1,27,1,27,3,27,331,
  	8,27,1,27,1,27,1,28,1,28,1,28,1,29,1,29,1,29,1,30,1,30,1,30,1,31,1,31,
  	1,31,1,31,3,31,348,8,31,1,31,1,31,5,31,352,8,31,10,31,12,31,355,9,31,
  	1,32,1,32,1,32,3,32,360,8,32,1,33,1,33,3,33,364,8,33,1,34,1,34,1,34,5,
  	34,369,8,34,10,34,12,34,372,9,34,1,34,3,34,375,8,34,1,35,1,35,5,35,379,
  	8,35,10,35,12,35,382,9,35,1,35,1,35,3,35,386,8,35,1,36,1,36,1,36,1,36,
  	1,36,3,36,393,8,36,1,36,1,36,1,36,1,36,1,36,1,36,5,36,401,8,36,10,36,
  	12,36,404,9,36,1,36,3,36,407,8,36,1,36,3,36,410,8,36,1,36,1,36,1,37,1,
  	37,1,37,1,37,3,37,418,8,37,1,37,1,37,1,37,1,38,1,38,1,38,1,38,1,38,3,
  	38,428,8,38,1,38,1,38,3,38,432,8,38,1,38,1,38,3,38,436,8,38,1,39,1,39,
  	1,40,1,40,1,41,1,41,1,41,5,41,445,8,41,10,41,12,41,448,9,41,1,41,3,41,
  	451,8,41,1,42,1,42,1,42,1,42,1,42,3,42,458,8,42,3,42,460,8,42,1,43,1,
  	43,5,43,464,8,43,10,43,12,43,467,9,43,1,43,1,43,1,44,1,44,1,44,3,44,474,
  	8,44,1,45,1,45,3,45,478,8,45,1,46,1,46,3,46,482,8,46,1,46,1,46,1,46,1,
  	46,1,46,1,46,3,46,490,8,46,1,47,1,47,1,47,1,47,1,47,1,47,1,47,5,47,499,
  	8,47,10,47,12,47,502,9,47,1,47,1,47,1,47,1,47,1,47,3,47,509,8,47,1,47,
  	3,47,512,8,47,1,47,1,47,3,47,516,8,47,1,47,3,47,519,8,47,1,47,1,47,1,
  	47,1,47,4,47,525,8,47,11,47,12,47,526,1,47,3,47,530,8,47,1,47,1,47,1,
  	47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,
  	47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,
  	47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,
  	47,1,47,5,47,578,8,47,10,47,12,47,581,9,47,3,47,583,8,47,1,47,1,47,1,
  	47,1,47,1,47,1,47,5,47,591,8,47,10,47,12,47,594,9,47,1,48,1,48,1,48,1,
  	49,1,49,1,49,3,49,602,8,49,1,50,1,50,1,50,1,50,1,50,1,50,1,51,1,51,1,
  	51,1,51,1,51,1,51,1,51,1,51,1,52,1,52,1,52,3,52,621,8,52,1,52,1,52,3,
  	52,625,8,52,1,52,1,52,3,52,629,8,52,1,52,1,52,1,52,1,53,1,53,5,53,636,
  	8,53,10,53,12,53,639,9,53,1,53,3,53,642,8,53,1,53,1,53,1,53,5,53,647,
  	8,53,10,53,12,53,650,9,53,1,53,1,53,3,53,654,8,53,1,54,1,54,1,54,1,54,
  	1,54,1,54,1,55,1,55,1,55,1,55,1,55,1,55,1,56,1,56,1,56,1,57,1,57,1,57,
  	1,57,1,57,1,58,1,58,1,58,3,58,679,8,58,1,58,1,58,3,58,683,8,58,1,59,1,
  	59,3,59,687,8,59,1,60,1,60,5,60,691,8,60,10,60,12,60,694,9,60,1,60,3,
  	60,697,8,60,1,61,1,61,1,61,1,62,1,62,1,62,3,62,705,8,62,1,62,1,62,1,62,
  	1,63,1,63,1,63,1,64,1,64,1,64,5,64,716,8,64,10,64,12,64,719,9,64,1,64,
  	1,64,1,65,1,65,3,65,725,8,65,1,66,1,66,1,66,1,66,5,66,731,8,66,10,66,
  	12,66,734,9,66,3,66,736,8,66,1,66,3,66,739,8,66,1,67,1,67,1,67,1,67,3,
  	67,745,8,67,1,68,1,68,1,69,1,69,1,70,1,70,1,70,1,70,1,71,1,71,1,71,1,
  	71,0,1,94,72,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,
  	42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,
  	88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,
  	126,128,130,132,134,136,138,140,142,0,14,1,0,8,9,1,0,37,39,2,0,13,13,
  	15,16,2,0,46,46,80,83,1,0,26,27,2,0,89,90,92,93,1,0,47,49,1,0,45,46,1,
  	0,50,53,1,0,74,77,1,0,72,73,2,0,57,69,79,79,1,0,80,81,2,0,89,90,92,94,
  	810,0,150,1,0,0,0,2,155,1,0,0,0,4,159,1,0,0,0,6,167,1,0,0,0,8,171,1,0,
  	0,0,10,177,1,0,0,0,12,181,1,0,0,0,14,192,1,0,0,0,16,204,1,0,0,0,18,206,
  	1,0,0,0,20,208,1,0,0,0,22,228,1,0,0,0,24,230,1,0,0,0,26,232,1,0,0,0,28,
  	234,1,0,0,0,30,237,1,0,0,0,32,246,1,0,0,0,34,255,1,0,0,0,36,261,1,0,0,
  	0,38,271,1,0,0,0,40,273,1,0,0,0,42,276,1,0,0,0,44,285,1,0,0,0,46,297,
  	1,0,0,0,48,306,1,0,0,0,50,318,1,0,0,0,52,322,1,0,0,0,54,328,1,0,0,0,56,
  	334,1,0,0,0,58,337,1,0,0,0,60,340,1,0,0,0,62,343,1,0,0,0,64,356,1,0,0,
  	0,66,363,1,0,0,0,68,365,1,0,0,0,70,385,1,0,0,0,72,387,1,0,0,0,74,413,
  	1,0,0,0,76,422,1,0,0,0,78,437,1,0,0,0,80,439,1,0,0,0,82,441,1,0,0,0,84,
  	452,1,0,0,0,86,461,1,0,0,0,88,473,1,0,0,0,90,477,1,0,0,0,92,489,1,0,0,
  	0,94,529,1,0,0,0,96,595,1,0,0,0,98,601,1,0,0,0,100,603,1,0,0,0,102,609,
  	1,0,0,0,104,617,1,0,0,0,106,653,1,0,0,0,108,655,1,0,0,0,110,661,1,0,0,
  	0,112,667,1,0,0,0,114,670,1,0,0,0,116,678,1,0,0,0,118,686,1,0,0,0,120,
  	688,1,0,0,0,122,698,1,0,0,0,124,701,1,0,0,0,126,709,1,0,0,0,128,712,1,
  	0,0,0,130,724,1,0,0,0,132,726,1,0,0,0,134,744,1,0,0,0,136,746,1,0,0,0,
  	138,748,1,0,0,0,140,750,1,0,0,0,142,754,1,0,0,0,144,149,3,6,3,0,145,149,
  	3,8,4,0,146,149,3,10,5,0,147,149,3,14,7,0,148,144,1,0,0,0,148,145,1,0,
  	0,0,148,146,1,0,0,0,148,147,1,0,0,0,149,152,1,0,0,0,150,148,1,0,0,0,150,
  	151,1,0,0,0,151,153,1,0,0,0,152,150,1,0,0,0,153,154,5,0,0,1,154,1,1,0,
  	0,0,155,156,5,87,0,0,156,157,5,88,0,0,157,158,5,92,0,0,158,3,1,0,0,0,
  	159,164,5,95,0,0,160,161,5,84,0,0,161,163,5,95,0,0,162,160,1,0,0,0,163,
  	166,1,0,0,0,164,162,1,0,0,0,164,165,1,0,0,0,165,5,1,0,0,0,166,164,1,0,
  	0,0,167,168,5,10,0,0,168,169,3,4,2,0,169,170,5,96,0,0,170,7,1,0,0,0,171,
  	172,5,10,0,0,172,173,3,4,2,0,173,174,5,11,0,0,174,175,5,95,0,0,175,176,
  	5,96,0,0,176,9,1,0,0,0,177,178,5,7,0,0,178,179,3,4,2,0,179,180,3,12,6,
  	0,180,11,1,0,0,0,181,186,5,5,0,0,182,185,3,10,5,0,183,185,3,14,7,0,184,
  	182,1,0,0,0,184,183,1,0,0,0,185,188,1,0,0,0,186,184,1,0,0,0,186,187,1,
  	0,0,0,187,189,1,0,0,0,188,186,1,0,0,0,189,190,5,6,0,0,190,13,1,0,0,0,
  	191,193,3,18,9,0,192,191,1,0,0,0,192,193,1,0,0,0,193,195,1,0,0,0,194,
  	196,3,24,12,0,195,194,1,0,0,0,195,196,1,0,0,0,196,197,1,0,0,0,197,198,
  	3,16,8,0,198,200,5,95,0,0,199,201,3,38,19,0,200,199,1,0,0,0,200,201,1,
  	0,0,0,201,202,1,0,0,0,202,203,3,44,22,0,203,15,1,0,0,0,204,205,7,0,0,
  	0,205,17,1,0,0,0,206,207,7,1,0,0,207,19,1,0,0,0,208,209,5,41,0,0,209,
  	210,5,3,0,0,210,215,3,4,2,0,211,212,5,85,0,0,212,214,3,4,2,0,213,211,
  	1,0,0,0,214,217,1,0,0,0,215,213,1,0,0,0,215,216,1,0,0,0,216,218,1,0,0,
  	0,217,215,1,0,0,0,218,219,5,4,0,0,219,21,1,0,0,0,220,222,3,18,9,0,221,
  	223,3,20,10,0,222,221,1,0,0,0,222,223,1,0,0,0,223,229,1,0,0,0,224,226,
  	3,20,10,0,225,227,3,18,9,0,226,225,1,0,0,0,226,227,1,0,0,0,227,229,1,
  	0,0,0,228,220,1,0,0,0,228,224,1,0,0,0,229,23,1,0,0,0,230,231,5,12,0,0,
  	231,25,1,0,0,0,232,233,7,2,0,0,233,27,1,0,0,0,234,235,5,14,0,0,235,29,
  	1,0,0,0,236,238,3,22,11,0,237,236,1,0,0,0,237,238,1,0,0,0,238,240,1,0,
  	0,0,239,241,3,24,12,0,240,239,1,0,0,0,240,241,1,0,0,0,241,243,1,0,0,0,
  	242,244,3,26,13,0,243,242,1,0,0,0,243,244,1,0,0,0,244,31,1,0,0,0,245,
  	247,3,22,11,0,246,245,1,0,0,0,246,247,1,0,0,0,247,249,1,0,0,0,248,250,
  	3,24,12,0,249,248,1,0,0,0,249,250,1,0,0,0,250,252,1,0,0,0,251,253,3,28,
  	14,0,252,251,1,0,0,0,252,253,1,0,0,0,253,33,1,0,0,0,254,256,3,24,12,0,
  	255,254,1,0,0,0,255,256,1,0,0,0,256,258,1,0,0,0,257,259,3,28,14,0,258,
  	257,1,0,0,0,258,259,1,0,0,0,259,35,1,0,0,0,260,262,3,28,14,0,261,260,
  	1,0,0,0,261,262,1,0,0,0,262,37,1,0,0,0,263,265,3,40,20,0,264,266,3,42,
  	21,0,265,264,1,0,0,0,265,266,1,0,0,0,266,272,1,0,0,0,267,269,3,42,21,
  	0,268,270,3,40,20,0,269,268,1,0,0,0,269,270,1,0,0,0,270,272,1,0,0,0,271,
  	263,1,0,0,0,271,267,1,0,0,0,272,39,1,0,0,0,273,274,5,17,0,0,274,275,3,
  	4,2,0,275,41,1,0,0,0,276,277,5,18,0,0,277,282,3,4,2,0,278,279,5,85,0,
  	0,279,281,3,4,2,0,280,278,1,0,0,0,281,284,1,0,0,0,282,280,1,0,0,0,282,
  	283,1,0,0,0,283,43,1,0,0,0,284,282,1,0,0,0,285,292,5,5,0,0,286,291,3,
  	14,7,0,287,291,3,56,28,0,288,291,3,76,38,0,289,291,3,74,37,0,290,286,
  	1,0,0,0,290,287,1,0,0,0,290,288,1,0,0,0,290,289,1,0,0,0,291,294,1,0,0,
  	0,292,290,1,0,0,0,292,293,1,0,0,0,293,295,1,0,0,0,294,292,1,0,0,0,295,
  	296,5,6,0,0,296,45,1,0,0,0,297,299,5,3,0,0,298,300,5,85,0,0,299,298,1,
  	0,0,0,299,300,1,0,0,0,300,302,1,0,0,0,301,303,5,85,0,0,302,301,1,0,0,
  	0,302,303,1,0,0,0,303,304,1,0,0,0,304,305,5,4,0,0,305,47,1,0,0,0,306,
  	307,5,3,0,0,307,310,3,94,47,0,308,309,5,85,0,0,309,311,3,94,47,0,310,
  	308,1,0,0,0,310,311,1,0,0,0,311,314,1,0,0,0,312,313,5,85,0,0,313,315,
  	3,94,47,0,314,312,1,0,0,0,314,315,1,0,0,0,315,316,1,0,0,0,316,317,5,4,
  	0,0,317,49,1,0,0,0,318,320,5,95,0,0,319,321,3,46,23,0,320,319,1,0,0,0,
  	320,321,1,0,0,0,321,51,1,0,0,0,322,324,5,5,0,0,323,325,3,94,47,0,324,
  	323,1,0,0,0,324,325,1,0,0,0,325,326,1,0,0,0,326,327,5,6,0,0,327,53,1,
  	0,0,0,328,330,5,1,0,0,329,331,3,94,47,0,330,329,1,0,0,0,330,331,1,0,0,
  	0,331,332,1,0,0,0,332,333,5,2,0,0,333,55,1,0,0,0,334,335,3,32,16,0,335,
  	336,3,60,30,0,336,57,1,0,0,0,337,338,3,34,17,0,338,339,3,60,30,0,339,
  	59,1,0,0,0,340,341,3,62,31,0,341,342,5,96,0,0,342,61,1,0,0,0,343,344,
  	3,50,25,0,344,347,5,95,0,0,345,346,5,57,0,0,346,348,3,94,47,0,347,345,
  	1,0,0,0,347,348,1,0,0,0,348,353,1,0,0,0,349,350,5,85,0,0,350,352,3,64,
  	32,0,351,349,1,0,0,0,352,355,1,0,0,0,353,351,1,0,0,0,353,354,1,0,0,0,
  	354,63,1,0,0,0,355,353,1,0,0,0,356,359,5,95,0,0,357,358,5,57,0,0,358,
  	360,3,94,47,0,359,357,1,0,0,0,359,360,1,0,0,0,360,65,1,0,0,0,361,364,
  	3,62,31,0,362,364,3,94,47,0,363,361,1,0,0,0,363,362,1,0,0,0,364,67,1,
  	0,0,0,365,370,3,70,35,0,366,367,5,85,0,0,367,369,3,70,35,0,368,366,1,
  	0,0,0,369,372,1,0,0,0,370,368,1,0,0,0,370,371,1,0,0,0,371,374,1,0,0,0,
  	372,370,1,0,0,0,373,375,5,85,0,0,374,373,1,0,0,0,374,375,1,0,0,0,375,
  	69,1,0,0,0,376,380,5,5,0,0,377,379,3,68,34,0,378,377,1,0,0,0,379,382,
  	1,0,0,0,380,378,1,0,0,0,380,381,1,0,0,0,381,383,1,0,0,0,382,380,1,0,0,
  	0,383,386,5,6,0,0,384,386,3,94,47,0,385,376,1,0,0,0,385,384,1,0,0,0,386,
  	71,1,0,0,0,387,388,5,34,0,0,388,389,3,50,25,0,389,390,5,95,0,0,390,392,
  	5,1,0,0,391,393,3,82,41,0,392,391,1,0,0,0,392,393,1,0,0,0,393,394,1,0,
  	0,0,394,409,5,2,0,0,395,396,5,35,0,0,396,397,5,3,0,0,397,402,5,95,0,0,
  	398,399,5,85,0,0,399,401,5,95,0,0,400,398,1,0,0,0,401,404,1,0,0,0,402,
  	400,1,0,0,0,402,403,1,0,0,0,403,406,1,0,0,0,404,402,1,0,0,0,405,407,5,
  	85,0,0,406,405,1,0,0,0,406,407,1,0,0,0,407,408,1,0,0,0,408,410,5,4,0,
  	0,409,395,1,0,0,0,409,410,1,0,0,0,410,411,1,0,0,0,411,412,3,86,43,0,412,
  	73,1,0,0,0,413,414,3,30,15,0,414,415,5,36,0,0,415,417,5,1,0,0,416,418,
  	3,82,41,0,417,416,1,0,0,0,417,418,1,0,0,0,418,419,1,0,0,0,419,420,5,2,
  	0,0,420,421,3,86,43,0,421,75,1,0,0,0,422,423,3,30,15,0,423,424,3,50,25,
  	0,424,425,5,95,0,0,425,427,5,1,0,0,426,428,3,82,41,0,427,426,1,0,0,0,
  	427,428,1,0,0,0,428,429,1,0,0,0,429,431,5,2,0,0,430,432,3,28,14,0,431,
  	430,1,0,0,0,431,432,1,0,0,0,432,435,1,0,0,0,433,436,3,86,43,0,434,436,
  	5,96,0,0,435,433,1,0,0,0,435,434,1,0,0,0,436,77,1,0,0,0,437,438,5,14,
  	0,0,438,79,1,0,0,0,439,440,7,2,0,0,440,81,1,0,0,0,441,446,3,84,42,0,442,
  	443,5,85,0,0,443,445,3,84,42,0,444,442,1,0,0,0,445,448,1,0,0,0,446,444,
  	1,0,0,0,446,447,1,0,0,0,447,450,1,0,0,0,448,446,1,0,0,0,449,451,5,85,
  	0,0,450,449,1,0,0,0,450,451,1,0,0,0,451,83,1,0,0,0,452,453,3,36,18,0,
  	453,459,3,50,25,0,454,457,5,95,0,0,455,456,5,57,0,0,456,458,3,94,47,0,
  	457,455,1,0,0,0,457,458,1,0,0,0,458,460,1,0,0,0,459,454,1,0,0,0,459,460,
  	1,0,0,0,460,85,1,0,0,0,461,465,5,5,0,0,462,464,3,88,44,0,463,462,1,0,
  	0,0,464,467,1,0,0,0,465,463,1,0,0,0,465,466,1,0,0,0,466,468,1,0,0,0,467,
  	465,1,0,0,0,468,469,5,6,0,0,469,87,1,0,0,0,470,474,3,86,43,0,471,474,
  	3,58,29,0,472,474,3,92,46,0,473,470,1,0,0,0,473,471,1,0,0,0,473,472,1,
  	0,0,0,474,89,1,0,0,0,475,478,3,86,43,0,476,478,3,92,46,0,477,475,1,0,
  	0,0,477,476,1,0,0,0,478,91,1,0,0,0,479,482,3,94,47,0,480,482,3,96,48,
  	0,481,479,1,0,0,0,481,480,1,0,0,0,481,482,1,0,0,0,482,483,1,0,0,0,483,
  	490,5,96,0,0,484,490,3,106,53,0,485,490,3,98,49,0,486,490,3,120,60,0,
  	487,490,3,72,36,0,488,490,3,128,64,0,489,481,1,0,0,0,489,484,1,0,0,0,
  	489,485,1,0,0,0,489,486,1,0,0,0,489,487,1,0,0,0,489,488,1,0,0,0,490,93,
  	1,0,0,0,491,492,6,47,-1,0,492,493,5,1,0,0,493,494,3,94,47,0,494,495,5,
  	2,0,0,495,530,1,0,0,0,496,500,5,5,0,0,497,499,3,68,34,0,498,497,1,0,0,
  	0,499,502,1,0,0,0,500,498,1,0,0,0,500,501,1,0,0,0,501,503,1,0,0,0,502,
  	500,1,0,0,0,503,530,5,6,0,0,504,505,7,3,0,0,505,530,3,94,47,18,506,508,
  	5,42,0,0,507,509,5,95,0,0,508,507,1,0,0,0,508,509,1,0,0,0,509,511,1,0,
  	0,0,510,512,3,54,27,0,511,510,1,0,0,0,511,512,1,0,0,0,512,515,1,0,0,0,
  	513,516,3,48,24,0,514,516,3,46,23,0,515,513,1,0,0,0,515,514,1,0,0,0,515,
  	516,1,0,0,0,516,518,1,0,0,0,517,519,3,52,26,0,518,517,1,0,0,0,518,519,
  	1,0,0,0,519,530,1,0,0,0,520,530,7,4,0,0,521,530,5,44,0,0,522,530,7,5,
  	0,0,523,525,5,94,0,0,524,523,1,0,0,0,525,526,1,0,0,0,526,524,1,0,0,0,
  	526,527,1,0,0,0,527,530,1,0,0,0,528,530,5,95,0,0,529,491,1,0,0,0,529,
  	496,1,0,0,0,529,504,1,0,0,0,529,506,1,0,0,0,529,520,1,0,0,0,529,521,1,
  	0,0,0,529,522,1,0,0,0,529,524,1,0,0,0,529,528,1,0,0,0,530,592,1,0,0,0,
  	531,532,10,24,0,0,532,533,5,78,0,0,533,591,3,94,47,24,534,535,10,20,0,
  	0,535,536,5,84,0,0,536,591,3,94,47,21,537,538,10,17,0,0,538,539,7,6,0,
  	0,539,591,3,94,47,18,540,541,10,16,0,0,541,542,7,7,0,0,542,591,3,94,47,
  	17,543,544,10,15,0,0,544,545,7,8,0,0,545,591,3,94,47,16,546,547,10,14,
  	0,0,547,548,7,9,0,0,548,591,3,94,47,15,549,550,10,13,0,0,550,551,7,10,
  	0,0,551,591,3,94,47,14,552,553,10,12,0,0,553,554,5,54,0,0,554,591,3,94,
  	47,13,555,556,10,11,0,0,556,557,5,56,0,0,557,591,3,94,47,12,558,559,10,
  	10,0,0,559,560,5,55,0,0,560,591,3,94,47,11,561,562,10,9,0,0,562,563,5,
  	70,0,0,563,591,3,94,47,10,564,565,10,8,0,0,565,566,5,71,0,0,566,591,3,
  	94,47,9,567,568,10,7,0,0,568,569,7,11,0,0,569,591,3,94,47,8,570,571,10,
  	25,0,0,571,591,7,12,0,0,572,573,10,23,0,0,573,582,5,1,0,0,574,579,3,94,
  	47,0,575,576,5,85,0,0,576,578,3,94,47,0,577,575,1,0,0,0,578,581,1,0,0,
  	0,579,577,1,0,0,0,579,580,1,0,0,0,580,583,1,0,0,0,581,579,1,0,0,0,582,
  	574,1,0,0,0,582,583,1,0,0,0,583,584,1,0,0,0,584,591,5,2,0,0,585,586,10,
  	21,0,0,586,587,5,3,0,0,587,588,3,94,47,0,588,589,5,4,0,0,589,591,1,0,
  	0,0,590,531,1,0,0,0,590,534,1,0,0,0,590,537,1,0,0,0,590,540,1,0,0,0,590,
  	543,1,0,0,0,590,546,1,0,0,0,590,549,1,0,0,0,590,552,1,0,0,0,590,555,1,
  	0,0,0,590,558,1,0,0,0,590,561,1,0,0,0,590,564,1,0,0,0,590,567,1,0,0,0,
  	590,570,1,0,0,0,590,572,1,0,0,0,590,585,1,0,0,0,591,594,1,0,0,0,592,590,
  	1,0,0,0,592,593,1,0,0,0,593,95,1,0,0,0,594,592,1,0,0,0,595,596,5,19,0,
  	0,596,597,3,94,47,0,597,97,1,0,0,0,598,602,3,100,50,0,599,602,3,102,51,
  	0,600,602,3,104,52,0,601,598,1,0,0,0,601,599,1,0,0,0,601,600,1,0,0,0,
  	602,99,1,0,0,0,603,604,5,25,0,0,604,605,5,1,0,0,605,606,3,66,33,0,606,
  	607,5,2,0,0,607,608,3,90,45,0,608,101,1,0,0,0,609,610,5,24,0,0,610,611,
  	3,90,45,0,611,612,5,25,0,0,612,613,5,1,0,0,613,614,3,66,33,0,614,615,
  	5,2,0,0,615,616,5,96,0,0,616,103,1,0,0,0,617,618,5,23,0,0,618,620,5,1,
  	0,0,619,621,3,66,33,0,620,619,1,0,0,0,620,621,1,0,0,0,621,622,1,0,0,0,
  	622,624,5,96,0,0,623,625,3,66,33,0,624,623,1,0,0,0,624,625,1,0,0,0,625,
  	626,1,0,0,0,626,628,5,96,0,0,627,629,3,66,33,0,628,627,1,0,0,0,628,629,
  	1,0,0,0,629,630,1,0,0,0,630,631,5,2,0,0,631,632,3,90,45,0,632,105,1,0,
  	0,0,633,637,3,108,54,0,634,636,3,110,55,0,635,634,1,0,0,0,636,639,1,0,
  	0,0,637,635,1,0,0,0,637,638,1,0,0,0,638,641,1,0,0,0,639,637,1,0,0,0,640,
  	642,3,112,56,0,641,640,1,0,0,0,641,642,1,0,0,0,642,654,1,0,0,0,643,644,
  	3,114,57,0,644,648,5,5,0,0,645,647,3,116,58,0,646,645,1,0,0,0,647,650,
  	1,0,0,0,648,646,1,0,0,0,648,649,1,0,0,0,649,651,1,0,0,0,650,648,1,0,0,
  	0,651,652,5,6,0,0,652,654,1,0,0,0,653,633,1,0,0,0,653,643,1,0,0,0,654,
  	107,1,0,0,0,655,656,5,20,0,0,656,657,5,1,0,0,657,658,3,66,33,0,658,659,
  	5,2,0,0,659,660,3,90,45,0,660,109,1,0,0,0,661,662,5,21,0,0,662,663,5,
  	1,0,0,663,664,3,66,33,0,664,665,5,2,0,0,665,666,3,90,45,0,666,111,1,0,
  	0,0,667,668,5,22,0,0,668,669,3,90,45,0,669,113,1,0,0,0,670,671,5,28,0,
  	0,671,672,5,1,0,0,672,673,3,66,33,0,673,674,5,2,0,0,674,115,1,0,0,0,675,
  	676,5,29,0,0,676,679,3,118,59,0,677,679,5,30,0,0,678,675,1,0,0,0,678,
  	677,1,0,0,0,679,680,1,0,0,0,680,682,5,86,0,0,681,683,3,90,45,0,682,681,
  	1,0,0,0,682,683,1,0,0,0,683,117,1,0,0,0,684,687,7,13,0,0,685,687,5,95,
  	0,0,686,684,1,0,0,0,686,685,1,0,0,0,687,119,1,0,0,0,688,692,3,122,61,
  	0,689,691,3,124,62,0,690,689,1,0,0,0,691,694,1,0,0,0,692,690,1,0,0,0,
  	692,693,1,0,0,0,693,696,1,0,0,0,694,692,1,0,0,0,695,697,3,126,63,0,696,
  	695,1,0,0,0,696,697,1,0,0,0,697,121,1,0,0,0,698,699,5,31,0,0,699,700,
  	3,90,45,0,700,123,1,0,0,0,701,702,5,32,0,0,702,704,5,1,0,0,703,705,3,
  	66,33,0,704,703,1,0,0,0,704,705,1,0,0,0,705,706,1,0,0,0,706,707,5,2,0,
  	0,707,708,3,90,45,0,708,125,1,0,0,0,709,710,5,33,0,0,710,711,3,90,45,
  	0,711,127,1,0,0,0,712,713,5,43,0,0,713,717,5,5,0,0,714,716,3,130,65,0,
  	715,714,1,0,0,0,716,719,1,0,0,0,717,715,1,0,0,0,717,718,1,0,0,0,718,720,
  	1,0,0,0,719,717,1,0,0,0,720,721,5,6,0,0,721,129,1,0,0,0,722,725,3,132,
  	66,0,723,725,3,142,71,0,724,722,1,0,0,0,724,723,1,0,0,0,725,131,1,0,0,
  	0,726,735,5,95,0,0,727,732,3,134,67,0,728,729,5,85,0,0,729,731,3,134,
  	67,0,730,728,1,0,0,0,731,734,1,0,0,0,732,730,1,0,0,0,732,733,1,0,0,0,
  	733,736,1,0,0,0,734,732,1,0,0,0,735,727,1,0,0,0,735,736,1,0,0,0,736,738,
  	1,0,0,0,737,739,5,96,0,0,738,737,1,0,0,0,738,739,1,0,0,0,739,133,1,0,
  	0,0,740,745,3,136,68,0,741,745,3,138,69,0,742,745,3,140,70,0,743,745,
  	3,142,71,0,744,740,1,0,0,0,744,741,1,0,0,0,744,742,1,0,0,0,744,743,1,
  	0,0,0,745,135,1,0,0,0,746,747,7,13,0,0,747,137,1,0,0,0,748,749,5,91,0,
  	0,749,139,1,0,0,0,750,751,5,3,0,0,751,752,5,95,0,0,752,753,5,4,0,0,753,
  	141,1,0,0,0,754,755,5,86,0,0,755,756,5,95,0,0,756,143,1,0,0,0,90,148,
  	150,164,184,186,192,195,200,215,222,226,228,237,240,243,246,249,252,255,
  	258,261,265,269,271,282,290,292,299,302,310,314,320,324,330,347,353,359,
  	363,370,374,380,385,392,402,406,409,417,427,431,435,446,450,457,459,465,
  	473,477,481,489,500,508,511,515,518,526,529,579,582,590,592,601,620,624,
  	628,637,641,648,653,678,682,686,692,696,704,717,724,732,735,738,744
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

std::vector<HarlequinParser::UsingStmtContext *> HarlequinParser::RootContext::usingStmt() {
  return getRuleContexts<HarlequinParser::UsingStmtContext>();
}

HarlequinParser::UsingStmtContext* HarlequinParser::RootContext::usingStmt(size_t i) {
  return getRuleContext<HarlequinParser::UsingStmtContext>(i);
}

std::vector<HarlequinParser::UsingAliasStmtContext *> HarlequinParser::RootContext::usingAliasStmt() {
  return getRuleContexts<HarlequinParser::UsingAliasStmtContext>();
}

HarlequinParser::UsingAliasStmtContext* HarlequinParser::RootContext::usingAliasStmt(size_t i) {
  return getRuleContext<HarlequinParser::UsingAliasStmtContext>(i);
}

std::vector<HarlequinParser::NamespaceDeclContext *> HarlequinParser::RootContext::namespaceDecl() {
  return getRuleContexts<HarlequinParser::NamespaceDeclContext>();
}

HarlequinParser::NamespaceDeclContext* HarlequinParser::RootContext::namespaceDecl(size_t i) {
  return getRuleContext<HarlequinParser::NamespaceDeclContext>(i);
}

std::vector<HarlequinParser::ClassDeclContext *> HarlequinParser::RootContext::classDecl() {
  return getRuleContexts<HarlequinParser::ClassDeclContext>();
}

HarlequinParser::ClassDeclContext* HarlequinParser::RootContext::classDecl(size_t i) {
  return getRuleContext<HarlequinParser::ClassDeclContext>(i);
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
    setState(150);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072680320) != 0)) {
      setState(148);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(144);
        usingStmt();
        break;
      }

      case 2: {
        setState(145);
        usingAliasStmt();
        break;
      }

      case 3: {
        setState(146);
        namespaceDecl();
        break;
      }

      case 4: {
        setState(147);
        classDecl();
        break;
      }

      default:
        break;
      }
      setState(152);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(153);
    match(HarlequinParser::EOF);
   
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
  enterRule(_localctx, 2, HarlequinParser::RuleCompilerDirective);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(155);
    match(HarlequinParser::Pound);
    setState(156);
    match(HarlequinParser::LineKw);
    setState(157);
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
  enterRule(_localctx, 4, HarlequinParser::RuleQualifiedId);
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
    setState(159);
    match(HarlequinParser::Id);
    setState(164);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Dot) {
      setState(160);
      match(HarlequinParser::Dot);
      setState(161);
      match(HarlequinParser::Id);
      setState(166);
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
  enterRule(_localctx, 6, HarlequinParser::RuleUsingStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(167);
    match(HarlequinParser::UsingKw);
    setState(168);
    qualifiedId();
    setState(169);
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

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::Id() {
  return getToken(HarlequinParser::Id, 0);
}

tree::TerminalNode* HarlequinParser::UsingAliasStmtContext::Term() {
  return getToken(HarlequinParser::Term, 0);
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
  enterRule(_localctx, 8, HarlequinParser::RuleUsingAliasStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(171);
    match(HarlequinParser::UsingKw);
    setState(172);
    qualifiedId();
    setState(173);
    match(HarlequinParser::AsKw);
    setState(174);
    match(HarlequinParser::Id);
    setState(175);
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
  enterRule(_localctx, 10, HarlequinParser::RuleNamespaceDecl);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(177);
    match(HarlequinParser::NamespaceKw);
    setState(178);
    qualifiedId();
    setState(179);
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
  enterRule(_localctx, 12, HarlequinParser::RuleNamespaceDef);
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
    setState(181);
    match(HarlequinParser::LeftBrace);
    setState(186);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072679296) != 0)) {
      setState(184);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case HarlequinParser::NamespaceKw: {
          setState(182);
          namespaceDecl();
          break;
        }

        case HarlequinParser::ClassKw:
        case HarlequinParser::InterfaceKw:
        case HarlequinParser::StaticKw:
        case HarlequinParser::PublicKw:
        case HarlequinParser::ProtectedKw:
        case HarlequinParser::PrivateKw: {
          setState(183);
          classDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(188);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(189);
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
  enterRule(_localctx, 14, HarlequinParser::RuleClassDecl);
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
    setState(192);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072674304) != 0)) {
      setState(191);
      accessBaseSpecifier();
    }
    setState(195);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(194);
      storageSpecifier();
    }
    setState(197);
    classType();
    setState(198);
    match(HarlequinParser::Id);
    setState(200);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ExtendsKw

    || _la == HarlequinParser::ImplementsKw) {
      setState(199);
      classInheritance();
    }
    setState(202);
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
  enterRule(_localctx, 16, HarlequinParser::RuleClassType);
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
  enterRule(_localctx, 18, HarlequinParser::RuleAccessBaseSpecifier);
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
    setState(206);
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
  enterRule(_localctx, 20, HarlequinParser::RuleAccessLimitSpecifier);
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
    setState(208);
    match(HarlequinParser::LimitedKw);
    setState(209);
    match(HarlequinParser::LeftBracket);
    setState(210);
    qualifiedId();
    setState(215);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(211);
      match(HarlequinParser::Comma);
      setState(212);
      qualifiedId();
      setState(217);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(218);
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
  enterRule(_localctx, 22, HarlequinParser::RuleAccessSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(228);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::PublicKw:
      case HarlequinParser::ProtectedKw:
      case HarlequinParser::PrivateKw: {
        enterOuterAlt(_localctx, 1);
        setState(220);
        accessBaseSpecifier();
        setState(222);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::LimitedKw) {
          setState(221);
          accessLimitSpecifier();
        }
        break;
      }

      case HarlequinParser::LimitedKw: {
        enterOuterAlt(_localctx, 2);
        setState(224);
        accessLimitSpecifier();
        setState(226);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 962072674304) != 0)) {
          setState(225);
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
  enterRule(_localctx, 24, HarlequinParser::RuleStorageSpecifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(230);
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
  enterRule(_localctx, 26, HarlequinParser::RuleFunctionSpecifier);
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
    setState(232);
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
  enterRule(_localctx, 28, HarlequinParser::RuleConstQualifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
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
  enterRule(_localctx, 30, HarlequinParser::RuleMethodDeclSpecSeq);
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
    setState(237);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3161095929856) != 0)) {
      setState(236);
      accessSpecifier();
    }
    setState(240);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(239);
      storageSpecifier();
    }
    setState(243);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 106496) != 0)) {
      setState(242);
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
  enterRule(_localctx, 32, HarlequinParser::RuleClassVarDeclSpecSeq);
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
    setState(246);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3161095929856) != 0)) {
      setState(245);
      accessSpecifier();
    }
    setState(249);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(248);
      storageSpecifier();
    }
    setState(252);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(251);
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
  enterRule(_localctx, 34, HarlequinParser::RuleLocalVarDeclSpecSeq);
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
    setState(255);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(254);
      storageSpecifier();
    }
    setState(258);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(257);
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
  enterRule(_localctx, 36, HarlequinParser::RuleArgVarDeclSpecSeq);
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
    setState(261);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(260);
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
  enterRule(_localctx, 38, HarlequinParser::RuleClassInheritance);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(271);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::ExtendsKw: {
        enterOuterAlt(_localctx, 1);
        setState(263);
        classExtends();
        setState(265);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::ImplementsKw) {
          setState(264);
          classImpls();
        }
        break;
      }

      case HarlequinParser::ImplementsKw: {
        enterOuterAlt(_localctx, 2);
        setState(267);
        classImpls();
        setState(269);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::ExtendsKw) {
          setState(268);
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
  enterRule(_localctx, 40, HarlequinParser::RuleClassExtends);

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
    match(HarlequinParser::ExtendsKw);
    setState(274);
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
  enterRule(_localctx, 42, HarlequinParser::RuleClassImpls);
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
    setState(276);
    match(HarlequinParser::ImplementsKw);
    setState(277);
    qualifiedId();
    setState(282);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(278);
      match(HarlequinParser::Comma);
      setState(279);
      qualifiedId();
      setState(284);
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

std::vector<HarlequinParser::ClassDeclContext *> HarlequinParser::ClassDefContext::classDecl() {
  return getRuleContexts<HarlequinParser::ClassDeclContext>();
}

HarlequinParser::ClassDeclContext* HarlequinParser::ClassDefContext::classDecl(size_t i) {
  return getRuleContext<HarlequinParser::ClassDeclContext>(i);
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
  enterRule(_localctx, 44, HarlequinParser::RuleClassDef);
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
    setState(285);
    match(HarlequinParser::LeftBrace);
    setState(292);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3229815534336) != 0) || _la == HarlequinParser::Id) {
      setState(290);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
      case 1: {
        setState(286);
        classDecl();
        break;
      }

      case 2: {
        setState(287);
        classVarDeclStmt();
        break;
      }

      case 3: {
        setState(288);
        methodDecl();
        break;
      }

      case 4: {
        setState(289);
        ctorDecl();
        break;
      }

      default:
        break;
      }
      setState(294);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(295);
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
  enterRule(_localctx, 46, HarlequinParser::RuleArrayTypeDecl);
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
    match(HarlequinParser::LeftBracket);
    setState(299);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
    case 1: {
      setState(298);
      match(HarlequinParser::Comma);
      break;
    }

    default:
      break;
    }
    setState(302);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(301);
      match(HarlequinParser::Comma);
    }
    setState(304);
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
  enterRule(_localctx, 48, HarlequinParser::RuleArrayTypeDef);
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
    setState(306);
    match(HarlequinParser::LeftBracket);
    setState(307);
    expr(0);
    setState(310);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      setState(308);
      match(HarlequinParser::Comma);
      setState(309);
      expr(0);
      break;
    }

    default:
      break;
    }
    setState(314);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(312);
      match(HarlequinParser::Comma);
      setState(313);
      expr(0);
    }
    setState(316);
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
  enterRule(_localctx, 50, HarlequinParser::RuleTypeNameDecl);
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
    setState(318);
    match(HarlequinParser::Id);
    setState(320);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::LeftBracket) {
      setState(319);
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
  enterRule(_localctx, 52, HarlequinParser::RuleBracedInitializer);
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
    setState(322);
    match(HarlequinParser::LeftBrace);
    setState(324);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(323);
      expr(0);
    }
    setState(326);
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
  enterRule(_localctx, 54, HarlequinParser::RuleCtorCall);
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
    setState(328);
    match(HarlequinParser::LeftParen);
    setState(330);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(329);
      expr(0);
    }
    setState(332);
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
  enterRule(_localctx, 56, HarlequinParser::RuleClassVarDeclStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(334);
    classVarDeclSpecSeq();
    setState(335);
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
  enterRule(_localctx, 58, HarlequinParser::RuleLocalVarDeclStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(337);
    localVarDeclSpecSeq();
    setState(338);
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
  enterRule(_localctx, 60, HarlequinParser::RuleVarDeclStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(340);
    varDecl();
    setState(341);
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
  enterRule(_localctx, 62, HarlequinParser::RuleVarDecl);
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
    setState(343);
    typeNameDecl();
    setState(344);
    match(HarlequinParser::Id);
    setState(347);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Assign) {
      setState(345);
      match(HarlequinParser::Assign);
      setState(346);
      expr(0);
    }
    setState(353);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(349);
      match(HarlequinParser::Comma);
      setState(350);
      nextVarDecl();
      setState(355);
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
  enterRule(_localctx, 64, HarlequinParser::RuleNextVarDecl);
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
    setState(356);
    match(HarlequinParser::Id);
    setState(359);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Assign) {
      setState(357);
      match(HarlequinParser::Assign);
      setState(358);
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
  enterRule(_localctx, 66, HarlequinParser::RuleCtrlFlowExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(363);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(361);
      varDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(362);
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
  enterRule(_localctx, 68, HarlequinParser::RuleArrayParamSeq);
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
    setState(365);
    arrayParamElem();
    setState(370);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(366);
        match(HarlequinParser::Comma);
        setState(367);
        arrayParamElem(); 
      }
      setState(372);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx);
    }
    setState(374);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(373);
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
  enterRule(_localctx, 70, HarlequinParser::RuleArrayParamElem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(385);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(376);
      match(HarlequinParser::LeftBrace);
      setState(380);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 80)) & 62991) != 0)) {
        setState(377);
        arrayParamSeq();
        setState(382);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(383);
      match(HarlequinParser::RightBrace);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(384);
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
  enterRule(_localctx, 72, HarlequinParser::RuleLambdaDecl);
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
    setState(387);
    match(HarlequinParser::LambdaKw);
    setState(388);
    typeNameDecl();
    setState(389);
    match(HarlequinParser::Id);
    setState(390);
    match(HarlequinParser::LeftParen);
    setState(392);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(391);
      methodArgSeq();
    }
    setState(394);
    match(HarlequinParser::RightParen);
    setState(409);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::CaptureKw) {
      setState(395);
      match(HarlequinParser::CaptureKw);
      setState(396);
      match(HarlequinParser::LeftBracket);
      setState(397);
      match(HarlequinParser::Id);
      setState(402);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(398);
          match(HarlequinParser::Comma);
          setState(399);
          match(HarlequinParser::Id); 
        }
        setState(404);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx);
      }
      setState(406);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == HarlequinParser::Comma) {
        setState(405);
        match(HarlequinParser::Comma);
      }
      setState(408);
      match(HarlequinParser::RightBracket);
    }
    setState(411);
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
  enterRule(_localctx, 74, HarlequinParser::RuleCtorDecl);
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
    setState(413);
    methodDeclSpecSeq();
    setState(414);
    match(HarlequinParser::ConstructorKw);
    setState(415);
    match(HarlequinParser::LeftParen);
    setState(417);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(416);
      methodArgSeq();
    }
    setState(419);
    match(HarlequinParser::RightParen);
    setState(420);
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
  enterRule(_localctx, 76, HarlequinParser::RuleMethodDecl);
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
    setState(422);
    methodDeclSpecSeq();
    setState(423);
    typeNameDecl();
    setState(424);
    match(HarlequinParser::Id);
    setState(425);
    match(HarlequinParser::LeftParen);
    setState(427);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(426);
      methodArgSeq();
    }
    setState(429);
    match(HarlequinParser::RightParen);
    setState(431);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(430);
      constQualifier();
    }
    setState(435);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftBrace: {
        setState(433);
        codeBlock();
        break;
      }

      case HarlequinParser::Term: {
        setState(434);
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
  enterRule(_localctx, 78, HarlequinParser::RuleMethodQualifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(437);
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
  enterRule(_localctx, 80, HarlequinParser::RuleMethodStorageModifier);
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
    setState(439);
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
  enterRule(_localctx, 82, HarlequinParser::RuleMethodArgSeq);
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
    setState(441);
    methodArg();
    setState(446);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(442);
        match(HarlequinParser::Comma);
        setState(443);
        methodArg(); 
      }
      setState(448);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx);
    }
    setState(450);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(449);
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
  enterRule(_localctx, 84, HarlequinParser::RuleMethodArg);
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
    setState(452);
    argVarDeclSpecSeq();
    setState(453);
    typeNameDecl();
    setState(459);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Id) {
      setState(454);
      match(HarlequinParser::Id);
      setState(457);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == HarlequinParser::Assign) {
        setState(455);
        match(HarlequinParser::Assign);
        setState(456);
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
  enterRule(_localctx, 86, HarlequinParser::RuleCodeBlock);
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
    setState(461);
    match(HarlequinParser::LeftBrace);
    setState(465);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 101174927183906) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 128527) != 0)) {
      setState(462);
      codeBlockStmt();
      setState(467);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(468);
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
  enterRule(_localctx, 88, HarlequinParser::RuleCodeBlockStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(473);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(470);
      codeBlock();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(471);
      localVarDeclStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(472);
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
  enterRule(_localctx, 90, HarlequinParser::RuleCodeStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(477);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(475);
      codeBlock();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(476);
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
  enterRule(_localctx, 92, HarlequinParser::RuleExprStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(489);
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
        setState(481);
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
            setState(479);
            expr(0);
            break;
          }

          case HarlequinParser::ReturnKw: {
            setState(480);
            returnStmt();
            break;
          }

          case HarlequinParser::Term: {
            break;
          }

        default:
          break;
        }
        setState(483);
        match(HarlequinParser::Term);
        break;
      }

      case HarlequinParser::IfKw:
      case HarlequinParser::SwitchKw: {
        enterOuterAlt(_localctx, 2);
        setState(484);
        conditionStmt();
        break;
      }

      case HarlequinParser::ForKw:
      case HarlequinParser::DoKw:
      case HarlequinParser::WhileKw: {
        enterOuterAlt(_localctx, 3);
        setState(485);
        loopStmt();
        break;
      }

      case HarlequinParser::TryKw: {
        enterOuterAlt(_localctx, 4);
        setState(486);
        exceptionHandler();
        break;
      }

      case HarlequinParser::LambdaKw: {
        enterOuterAlt(_localctx, 5);
        setState(487);
        lambdaDecl();
        break;
      }

      case HarlequinParser::AsmIntrinKw: {
        enterOuterAlt(_localctx, 6);
        setState(488);
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
  size_t startState = 94;
  enterRecursionRule(_localctx, 94, HarlequinParser::RuleExpr, precedence);

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
    setState(529);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftParen: {
        _localctx = _tracker.createInstance<ExprParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(492);
        match(HarlequinParser::LeftParen);
        setState(493);
        expr(0);
        setState(494);
        match(HarlequinParser::RightParen);
        break;
      }

      case HarlequinParser::LeftBrace: {
        _localctx = _tracker.createInstance<ExprArrayContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(496);
        match(HarlequinParser::LeftBrace);
        setState(500);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 80)) & 62991) != 0)) {
          setState(497);
          arrayParamSeq();
          setState(502);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(503);
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
        setState(504);
        _la = _input->LA(1);
        if (!(((((_la - 46) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 46)) & 257698037761) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(505);
        expr(18);
        break;
      }

      case HarlequinParser::NewKw: {
        _localctx = _tracker.createInstance<ExprInstantiateContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(506);
        match(HarlequinParser::NewKw);
        setState(508);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx)) {
        case 1: {
          setState(507);
          match(HarlequinParser::Id);
          break;
        }

        default:
          break;
        }
        setState(511);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 61, _ctx)) {
        case 1: {
          setState(510);
          ctorCall();
          break;
        }

        default:
          break;
        }
        setState(515);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 62, _ctx)) {
        case 1: {
          setState(513);
          arrayTypeDef();
          break;
        }

        case 2: {
          setState(514);
          arrayTypeDecl();
          break;
        }

        default:
          break;
        }
        setState(518);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 63, _ctx)) {
        case 1: {
          setState(517);
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
        setState(520);
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
        setState(521);
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
        setState(522);
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
        setState(524); 
        _errHandler->sync(this);
        alt = 1;
        do {
          switch (alt) {
            case 1: {
                  setState(523);
                  match(HarlequinParser::StrLit);
                  break;
                }

          default:
            throw NoViableAltException(this);
          }
          setState(526); 
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx);
        } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
        break;
      }

      case HarlequinParser::Id: {
        _localctx = _tracker.createInstance<ExprIdContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(528);
        match(HarlequinParser::Id);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(592);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(590);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ExprExponentContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(531);

          if (!(precpred(_ctx, 24))) throw FailedPredicateException(this, "precpred(_ctx, 24)");
          setState(532);
          match(HarlequinParser::Expo);
          setState(533);
          expr(24);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ExprDotContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(534);

          if (!(precpred(_ctx, 20))) throw FailedPredicateException(this, "precpred(_ctx, 20)");
          setState(535);
          match(HarlequinParser::Dot);
          setState(536);
          expr(21);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ExprArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(537);

          if (!(precpred(_ctx, 17))) throw FailedPredicateException(this, "precpred(_ctx, 17)");
          setState(538);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 985162418487296) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(539);
          expr(18);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<ExprArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(540);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(541);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::Add

          || _la == HarlequinParser::Sub)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(542);
          expr(17);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<ExprBitShiftContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(543);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");
          setState(544);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 16888498602639360) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(545);
          expr(16);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<ExprEqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(546);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(547);
          _la = _input->LA(1);
          if (!(((((_la - 74) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 74)) & 15) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(548);
          expr(15);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<ExprEqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(549);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(550);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::EqualTo

          || _la == HarlequinParser::NotEqualTo)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(551);
          expr(14);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(552);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(553);
          match(HarlequinParser::BitAnd);
          setState(554);
          expr(13);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(555);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(556);
          match(HarlequinParser::BitXor);
          setState(557);
          expr(12);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(558);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(559);
          match(HarlequinParser::BitOr);
          setState(560);
          expr(11);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<ExprLogicContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(561);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(562);
          match(HarlequinParser::LogicAnd);
          setState(563);
          expr(10);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<ExprLogicContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(564);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(565);
          match(HarlequinParser::LogicOr);
          setState(566);
          expr(9);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<ExprAssignmentContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(567);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(568);
          _la = _input->LA(1);
          if (!(((((_la - 57) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 57)) & 4202495) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(569);
          expr(8);
          break;
        }

        case 14: {
          auto newContext = _tracker.createInstance<ExprUnaryPostfixContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(570);

          if (!(precpred(_ctx, 25))) throw FailedPredicateException(this, "precpred(_ctx, 25)");
          setState(571);
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
          setState(572);

          if (!(precpred(_ctx, 23))) throw FailedPredicateException(this, "precpred(_ctx, 23)");
          setState(573);
          match(HarlequinParser::LeftParen);
          setState(582);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 80)) & 62991) != 0)) {
            setState(574);
            expr(0);
            setState(579);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == HarlequinParser::Comma) {
              setState(575);
              match(HarlequinParser::Comma);
              setState(576);
              expr(0);
              setState(581);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(584);
          match(HarlequinParser::RightParen);
          break;
        }

        case 16: {
          auto newContext = _tracker.createInstance<ExprIndexContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(585);

          if (!(precpred(_ctx, 21))) throw FailedPredicateException(this, "precpred(_ctx, 21)");
          setState(586);
          match(HarlequinParser::LeftBracket);
          setState(587);
          expr(0);
          setState(588);
          match(HarlequinParser::RightBracket);
          break;
        }

        default:
          break;
        } 
      }
      setState(594);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx);
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
  enterRule(_localctx, 96, HarlequinParser::RuleReturnStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(595);
    match(HarlequinParser::ReturnKw);
    setState(596);
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
  enterRule(_localctx, 98, HarlequinParser::RuleLoopStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(601);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::WhileKw: {
        enterOuterAlt(_localctx, 1);
        setState(598);
        loopWhileDef();
        break;
      }

      case HarlequinParser::DoKw: {
        enterOuterAlt(_localctx, 2);
        setState(599);
        loopDoWhileDef();
        break;
      }

      case HarlequinParser::ForKw: {
        enterOuterAlt(_localctx, 3);
        setState(600);
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
  enterRule(_localctx, 100, HarlequinParser::RuleLoopWhileDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(603);
    match(HarlequinParser::WhileKw);
    setState(604);
    match(HarlequinParser::LeftParen);
    setState(605);
    ctrlFlowExpr();
    setState(606);
    match(HarlequinParser::RightParen);
    setState(607);
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
  enterRule(_localctx, 102, HarlequinParser::RuleLoopDoWhileDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(609);
    match(HarlequinParser::DoKw);
    setState(610);
    codeStmt();
    setState(611);
    match(HarlequinParser::WhileKw);
    setState(612);
    match(HarlequinParser::LeftParen);
    setState(613);
    ctrlFlowExpr();
    setState(614);
    match(HarlequinParser::RightParen);
    setState(615);
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
  enterRule(_localctx, 104, HarlequinParser::RuleLoopForDef);
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
    setState(617);
    match(HarlequinParser::ForKw);
    setState(618);
    match(HarlequinParser::LeftParen);
    setState(620);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(619);
      ctrlFlowExpr();
    }
    setState(622);
    match(HarlequinParser::Term);
    setState(624);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(623);
      ctrlFlowExpr();
    }
    setState(626);
    match(HarlequinParser::Term);
    setState(628);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(627);
      ctrlFlowExpr();
    }
    setState(630);
    match(HarlequinParser::RightParen);
    setState(631);
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
  enterRule(_localctx, 106, HarlequinParser::RuleConditionStmt);
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
    setState(653);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::IfKw: {
        enterOuterAlt(_localctx, 1);
        setState(633);
        condIfDef();
        setState(637);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(634);
            condElifDef(); 
          }
          setState(639);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx);
        }
        setState(641);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
        case 1: {
          setState(640);
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
        setState(643);
        condSwitchDef();
        setState(644);
        match(HarlequinParser::LeftBrace);
        setState(648);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == HarlequinParser::CaseKw

        || _la == HarlequinParser::DefaultKw) {
          setState(645);
          condCaseDef();
          setState(650);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(651);
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
  enterRule(_localctx, 108, HarlequinParser::RuleCondIfDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(655);
    match(HarlequinParser::IfKw);
    setState(656);
    match(HarlequinParser::LeftParen);
    setState(657);
    ctrlFlowExpr();
    setState(658);
    match(HarlequinParser::RightParen);
    setState(659);
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
  enterRule(_localctx, 110, HarlequinParser::RuleCondElifDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(661);
    match(HarlequinParser::ElifKw);
    setState(662);
    match(HarlequinParser::LeftParen);
    setState(663);
    ctrlFlowExpr();
    setState(664);
    match(HarlequinParser::RightParen);
    setState(665);
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
  enterRule(_localctx, 112, HarlequinParser::RuleCondElseDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(667);
    match(HarlequinParser::ElseKw);
    setState(668);
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
  enterRule(_localctx, 114, HarlequinParser::RuleCondSwitchDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(670);
    match(HarlequinParser::SwitchKw);
    setState(671);
    match(HarlequinParser::LeftParen);
    setState(672);
    ctrlFlowExpr();
    setState(673);
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
  enterRule(_localctx, 116, HarlequinParser::RuleCondCaseDef);
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
    setState(678);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::CaseKw: {
        setState(675);
        match(HarlequinParser::CaseKw);
        setState(676);
        condCaseResult();
        break;
      }

      case HarlequinParser::DefaultKw: {
        setState(677);
        match(HarlequinParser::DefaultKw);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(680);
    match(HarlequinParser::Colon);
    setState(682);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 101174927163426) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 128527) != 0)) {
      setState(681);
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
  enterRule(_localctx, 118, HarlequinParser::RuleCondCaseResult);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(686);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit: {
        _localctx = _tracker.createInstance<HarlequinParser::CaseLiteralContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(684);
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
        setState(685);
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
  enterRule(_localctx, 120, HarlequinParser::RuleExceptionHandler);

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
    setState(688);
    tryBlockDef();
    setState(692);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 81, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(689);
        catchBlockDef(); 
      }
      setState(694);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 81, _ctx);
    }
    setState(696);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx)) {
    case 1: {
      setState(695);
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
  enterRule(_localctx, 122, HarlequinParser::RuleTryBlockDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(698);
    match(HarlequinParser::TryKw);
    setState(699);
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
  enterRule(_localctx, 124, HarlequinParser::RuleCatchBlockDef);
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
    setState(701);
    match(HarlequinParser::CatchKw);
    setState(702);
    match(HarlequinParser::LeftParen);
    setState(704);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(703);
      ctrlFlowExpr();
    }
    setState(706);
    match(HarlequinParser::RightParen);
    setState(707);
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
  enterRule(_localctx, 126, HarlequinParser::RuleFinallyBlockDef);

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
    match(HarlequinParser::FinallyKw);
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
  enterRule(_localctx, 128, HarlequinParser::RuleAsmBlock);
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
    match(HarlequinParser::AsmIntrinKw);
    setState(713);
    match(HarlequinParser::LeftBrace);
    setState(717);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Colon

    || _la == HarlequinParser::Id) {
      setState(714);
      asmStmt();
      setState(719);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(720);
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
  enterRule(_localctx, 130, HarlequinParser::RuleAsmStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(724);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::Id: {
        enterOuterAlt(_localctx, 1);
        setState(722);
        asmInstr();
        break;
      }

      case HarlequinParser::Colon: {
        enterOuterAlt(_localctx, 2);
        setState(723);
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
  enterRule(_localctx, 132, HarlequinParser::RuleAsmInstr);
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
    setState(726);
    match(HarlequinParser::Id);
    setState(735);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 87, _ctx)) {
    case 1: {
      setState(727);
      asmOperand();
      setState(732);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == HarlequinParser::Comma) {
        setState(728);
        match(HarlequinParser::Comma);
        setState(729);
        asmOperand();
        setState(734);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    default:
      break;
    }
    setState(738);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Term) {
      setState(737);
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
  enterRule(_localctx, 134, HarlequinParser::RuleAsmOperand);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(744);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit: {
        enterOuterAlt(_localctx, 1);
        setState(740);
        asmLit();
        break;
      }

      case HarlequinParser::AsmRegId: {
        enterOuterAlt(_localctx, 2);
        setState(741);
        asmReg();
        break;
      }

      case HarlequinParser::LeftBracket: {
        enterOuterAlt(_localctx, 3);
        setState(742);
        asmVar();
        break;
      }

      case HarlequinParser::Colon: {
        enterOuterAlt(_localctx, 4);
        setState(743);
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
  enterRule(_localctx, 136, HarlequinParser::RuleAsmLit);
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
    setState(746);
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
  enterRule(_localctx, 138, HarlequinParser::RuleAsmReg);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(748);
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
  enterRule(_localctx, 140, HarlequinParser::RuleAsmVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(750);
    match(HarlequinParser::LeftBracket);
    setState(751);
    match(HarlequinParser::Id);
    setState(752);
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
  enterRule(_localctx, 142, HarlequinParser::RuleAsmLabel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(754);
    match(HarlequinParser::Colon);
    setState(755);
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
    case 47: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

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
