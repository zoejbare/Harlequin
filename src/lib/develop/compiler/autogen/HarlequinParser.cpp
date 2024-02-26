
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
  	4,1,99,767,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
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
  	3,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,3,5,182,8,5,1,6,1,6,1,6,5,6,
  	187,8,6,10,6,12,6,190,9,6,1,6,1,6,1,7,3,7,195,8,7,1,7,3,7,198,8,7,1,7,
  	1,7,1,7,3,7,203,8,7,1,7,1,7,3,7,207,8,7,1,8,1,8,1,9,1,9,1,10,1,10,1,10,
  	1,10,1,10,5,10,218,8,10,10,10,12,10,221,9,10,1,10,1,10,1,11,1,11,3,11,
  	227,8,11,1,11,1,11,3,11,231,8,11,3,11,233,8,11,1,12,1,12,1,13,1,13,1,
  	14,1,14,1,15,3,15,242,8,15,1,15,3,15,245,8,15,1,15,3,15,248,8,15,1,16,
  	3,16,251,8,16,1,16,3,16,254,8,16,1,16,3,16,257,8,16,1,17,3,17,260,8,17,
  	1,17,3,17,263,8,17,1,18,3,18,266,8,18,1,19,1,19,3,19,270,8,19,1,19,1,
  	19,3,19,274,8,19,3,19,276,8,19,1,20,1,20,1,20,1,21,1,21,1,21,1,21,5,21,
  	285,8,21,10,21,12,21,288,9,21,1,22,1,22,1,22,1,22,1,22,5,22,295,8,22,
  	10,22,12,22,298,9,22,1,22,1,22,1,23,1,23,3,23,304,8,23,1,23,3,23,307,
  	8,23,1,23,1,23,1,24,1,24,1,24,1,24,3,24,315,8,24,1,24,1,24,3,24,319,8,
  	24,1,24,1,24,1,25,1,25,3,25,325,8,25,1,26,1,26,3,26,329,8,26,1,26,1,26,
  	1,27,1,27,3,27,335,8,27,1,27,1,27,1,28,1,28,1,28,1,29,1,29,1,29,1,30,
  	1,30,1,30,1,31,1,31,1,31,1,31,3,31,352,8,31,1,31,1,31,5,31,356,8,31,10,
  	31,12,31,359,9,31,1,32,1,32,1,32,3,32,364,8,32,1,33,1,33,3,33,368,8,33,
  	1,34,1,34,1,34,5,34,373,8,34,10,34,12,34,376,9,34,1,34,3,34,379,8,34,
  	1,35,1,35,5,35,383,8,35,10,35,12,35,386,9,35,1,35,1,35,3,35,390,8,35,
  	1,36,1,36,1,36,1,36,1,36,3,36,397,8,36,1,36,1,36,1,36,1,36,1,36,1,36,
  	5,36,405,8,36,10,36,12,36,408,9,36,1,36,3,36,411,8,36,1,36,3,36,414,8,
  	36,1,36,1,36,1,37,1,37,1,37,1,37,3,37,422,8,37,1,37,1,37,1,37,3,37,427,
  	8,37,1,38,1,38,1,38,1,38,1,38,3,38,434,8,38,1,38,1,38,3,38,438,8,38,1,
  	38,1,38,3,38,442,8,38,1,38,3,38,445,8,38,1,39,1,39,1,40,1,40,1,41,1,41,
  	1,41,5,41,454,8,41,10,41,12,41,457,9,41,1,41,3,41,460,8,41,1,42,1,42,
  	1,42,1,42,1,42,3,42,467,8,42,3,42,469,8,42,1,43,1,43,5,43,473,8,43,10,
  	43,12,43,476,9,43,1,43,1,43,1,44,1,44,1,44,3,44,483,8,44,1,45,1,45,3,
  	45,487,8,45,1,46,1,46,3,46,491,8,46,1,46,1,46,1,46,1,46,1,46,1,46,3,46,
  	499,8,46,1,47,1,47,1,47,1,47,1,47,1,47,1,47,5,47,508,8,47,10,47,12,47,
  	511,9,47,1,47,1,47,1,47,1,47,1,47,3,47,518,8,47,1,47,3,47,521,8,47,1,
  	47,1,47,3,47,525,8,47,1,47,3,47,528,8,47,1,47,1,47,1,47,1,47,4,47,534,
  	8,47,11,47,12,47,535,1,47,3,47,539,8,47,1,47,1,47,1,47,1,47,1,47,1,47,
  	1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,
  	1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,
  	1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,1,47,5,47,587,
  	8,47,10,47,12,47,590,9,47,3,47,592,8,47,1,47,1,47,1,47,1,47,1,47,1,47,
  	5,47,600,8,47,10,47,12,47,603,9,47,1,48,1,48,1,48,1,49,1,49,1,49,3,49,
  	611,8,49,1,50,1,50,1,50,1,50,1,50,1,50,1,51,1,51,1,51,1,51,1,51,1,51,
  	1,51,1,51,1,52,1,52,1,52,3,52,630,8,52,1,52,1,52,3,52,634,8,52,1,52,1,
  	52,3,52,638,8,52,1,52,1,52,1,52,1,53,1,53,5,53,645,8,53,10,53,12,53,648,
  	9,53,1,53,3,53,651,8,53,1,53,1,53,1,53,5,53,656,8,53,10,53,12,53,659,
  	9,53,1,53,1,53,3,53,663,8,53,1,54,1,54,1,54,1,54,1,54,1,54,1,55,1,55,
  	1,55,1,55,1,55,1,55,1,56,1,56,1,56,1,57,1,57,1,57,1,57,1,57,1,58,1,58,
  	1,58,3,58,688,8,58,1,58,1,58,3,58,692,8,58,1,59,1,59,3,59,696,8,59,1,
  	60,1,60,5,60,700,8,60,10,60,12,60,703,9,60,1,60,3,60,706,8,60,1,61,1,
  	61,1,61,1,62,1,62,1,62,3,62,714,8,62,1,62,1,62,1,62,1,63,1,63,1,63,1,
  	64,1,64,1,64,5,64,725,8,64,10,64,12,64,728,9,64,1,64,1,64,1,65,1,65,3,
  	65,734,8,65,1,66,1,66,1,66,1,66,5,66,740,8,66,10,66,12,66,743,9,66,3,
  	66,745,8,66,1,66,3,66,748,8,66,1,67,1,67,1,67,1,67,3,67,754,8,67,1,68,
  	1,68,1,69,1,69,1,70,1,70,1,70,1,70,1,71,1,71,1,71,1,71,0,1,94,72,0,2,
  	4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,
  	52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,
  	98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,
  	134,136,138,140,142,0,14,1,0,8,9,1,0,37,39,2,0,13,13,15,16,2,0,46,46,
  	80,83,1,0,26,27,2,0,89,90,92,93,1,0,47,49,1,0,45,46,1,0,50,53,1,0,74,
  	77,1,0,72,73,2,0,57,69,79,79,1,0,80,81,2,0,89,90,92,94,823,0,150,1,0,
  	0,0,2,155,1,0,0,0,4,159,1,0,0,0,6,167,1,0,0,0,8,171,1,0,0,0,10,177,1,
  	0,0,0,12,183,1,0,0,0,14,194,1,0,0,0,16,208,1,0,0,0,18,210,1,0,0,0,20,
  	212,1,0,0,0,22,232,1,0,0,0,24,234,1,0,0,0,26,236,1,0,0,0,28,238,1,0,0,
  	0,30,241,1,0,0,0,32,250,1,0,0,0,34,259,1,0,0,0,36,265,1,0,0,0,38,275,
  	1,0,0,0,40,277,1,0,0,0,42,280,1,0,0,0,44,289,1,0,0,0,46,301,1,0,0,0,48,
  	310,1,0,0,0,50,322,1,0,0,0,52,326,1,0,0,0,54,332,1,0,0,0,56,338,1,0,0,
  	0,58,341,1,0,0,0,60,344,1,0,0,0,62,347,1,0,0,0,64,360,1,0,0,0,66,367,
  	1,0,0,0,68,369,1,0,0,0,70,389,1,0,0,0,72,391,1,0,0,0,74,417,1,0,0,0,76,
  	428,1,0,0,0,78,446,1,0,0,0,80,448,1,0,0,0,82,450,1,0,0,0,84,461,1,0,0,
  	0,86,470,1,0,0,0,88,482,1,0,0,0,90,486,1,0,0,0,92,498,1,0,0,0,94,538,
  	1,0,0,0,96,604,1,0,0,0,98,610,1,0,0,0,100,612,1,0,0,0,102,618,1,0,0,0,
  	104,626,1,0,0,0,106,662,1,0,0,0,108,664,1,0,0,0,110,670,1,0,0,0,112,676,
  	1,0,0,0,114,679,1,0,0,0,116,687,1,0,0,0,118,695,1,0,0,0,120,697,1,0,0,
  	0,122,707,1,0,0,0,124,710,1,0,0,0,126,718,1,0,0,0,128,721,1,0,0,0,130,
  	733,1,0,0,0,132,735,1,0,0,0,134,753,1,0,0,0,136,755,1,0,0,0,138,757,1,
  	0,0,0,140,759,1,0,0,0,142,763,1,0,0,0,144,149,3,6,3,0,145,149,3,8,4,0,
  	146,149,3,10,5,0,147,149,3,14,7,0,148,144,1,0,0,0,148,145,1,0,0,0,148,
  	146,1,0,0,0,148,147,1,0,0,0,149,152,1,0,0,0,150,148,1,0,0,0,150,151,1,
  	0,0,0,151,153,1,0,0,0,152,150,1,0,0,0,153,154,5,0,0,1,154,1,1,0,0,0,155,
  	156,5,87,0,0,156,157,5,88,0,0,157,158,5,92,0,0,158,3,1,0,0,0,159,164,
  	5,95,0,0,160,161,5,84,0,0,161,163,5,95,0,0,162,160,1,0,0,0,163,166,1,
  	0,0,0,164,162,1,0,0,0,164,165,1,0,0,0,165,5,1,0,0,0,166,164,1,0,0,0,167,
  	168,5,10,0,0,168,169,3,4,2,0,169,170,5,96,0,0,170,7,1,0,0,0,171,172,5,
  	10,0,0,172,173,3,4,2,0,173,174,5,11,0,0,174,175,5,95,0,0,175,176,5,96,
  	0,0,176,9,1,0,0,0,177,178,5,7,0,0,178,179,3,4,2,0,179,181,3,12,6,0,180,
  	182,5,96,0,0,181,180,1,0,0,0,181,182,1,0,0,0,182,11,1,0,0,0,183,188,5,
  	5,0,0,184,187,3,10,5,0,185,187,3,14,7,0,186,184,1,0,0,0,186,185,1,0,0,
  	0,187,190,1,0,0,0,188,186,1,0,0,0,188,189,1,0,0,0,189,191,1,0,0,0,190,
  	188,1,0,0,0,191,192,5,6,0,0,192,13,1,0,0,0,193,195,3,18,9,0,194,193,1,
  	0,0,0,194,195,1,0,0,0,195,197,1,0,0,0,196,198,3,24,12,0,197,196,1,0,0,
  	0,197,198,1,0,0,0,198,199,1,0,0,0,199,200,3,16,8,0,200,202,5,95,0,0,201,
  	203,3,38,19,0,202,201,1,0,0,0,202,203,1,0,0,0,203,204,1,0,0,0,204,206,
  	3,44,22,0,205,207,5,96,0,0,206,205,1,0,0,0,206,207,1,0,0,0,207,15,1,0,
  	0,0,208,209,7,0,0,0,209,17,1,0,0,0,210,211,7,1,0,0,211,19,1,0,0,0,212,
  	213,5,41,0,0,213,214,5,3,0,0,214,219,3,4,2,0,215,216,5,85,0,0,216,218,
  	3,4,2,0,217,215,1,0,0,0,218,221,1,0,0,0,219,217,1,0,0,0,219,220,1,0,0,
  	0,220,222,1,0,0,0,221,219,1,0,0,0,222,223,5,4,0,0,223,21,1,0,0,0,224,
  	226,3,18,9,0,225,227,3,20,10,0,226,225,1,0,0,0,226,227,1,0,0,0,227,233,
  	1,0,0,0,228,230,3,20,10,0,229,231,3,18,9,0,230,229,1,0,0,0,230,231,1,
  	0,0,0,231,233,1,0,0,0,232,224,1,0,0,0,232,228,1,0,0,0,233,23,1,0,0,0,
  	234,235,5,12,0,0,235,25,1,0,0,0,236,237,7,2,0,0,237,27,1,0,0,0,238,239,
  	5,14,0,0,239,29,1,0,0,0,240,242,3,22,11,0,241,240,1,0,0,0,241,242,1,0,
  	0,0,242,244,1,0,0,0,243,245,3,24,12,0,244,243,1,0,0,0,244,245,1,0,0,0,
  	245,247,1,0,0,0,246,248,3,26,13,0,247,246,1,0,0,0,247,248,1,0,0,0,248,
  	31,1,0,0,0,249,251,3,22,11,0,250,249,1,0,0,0,250,251,1,0,0,0,251,253,
  	1,0,0,0,252,254,3,24,12,0,253,252,1,0,0,0,253,254,1,0,0,0,254,256,1,0,
  	0,0,255,257,3,28,14,0,256,255,1,0,0,0,256,257,1,0,0,0,257,33,1,0,0,0,
  	258,260,3,24,12,0,259,258,1,0,0,0,259,260,1,0,0,0,260,262,1,0,0,0,261,
  	263,3,28,14,0,262,261,1,0,0,0,262,263,1,0,0,0,263,35,1,0,0,0,264,266,
  	3,28,14,0,265,264,1,0,0,0,265,266,1,0,0,0,266,37,1,0,0,0,267,269,3,40,
  	20,0,268,270,3,42,21,0,269,268,1,0,0,0,269,270,1,0,0,0,270,276,1,0,0,
  	0,271,273,3,42,21,0,272,274,3,40,20,0,273,272,1,0,0,0,273,274,1,0,0,0,
  	274,276,1,0,0,0,275,267,1,0,0,0,275,271,1,0,0,0,276,39,1,0,0,0,277,278,
  	5,17,0,0,278,279,3,4,2,0,279,41,1,0,0,0,280,281,5,18,0,0,281,286,3,4,
  	2,0,282,283,5,85,0,0,283,285,3,4,2,0,284,282,1,0,0,0,285,288,1,0,0,0,
  	286,284,1,0,0,0,286,287,1,0,0,0,287,43,1,0,0,0,288,286,1,0,0,0,289,296,
  	5,5,0,0,290,295,3,14,7,0,291,295,3,56,28,0,292,295,3,76,38,0,293,295,
  	3,74,37,0,294,290,1,0,0,0,294,291,1,0,0,0,294,292,1,0,0,0,294,293,1,0,
  	0,0,295,298,1,0,0,0,296,294,1,0,0,0,296,297,1,0,0,0,297,299,1,0,0,0,298,
  	296,1,0,0,0,299,300,5,6,0,0,300,45,1,0,0,0,301,303,5,3,0,0,302,304,5,
  	85,0,0,303,302,1,0,0,0,303,304,1,0,0,0,304,306,1,0,0,0,305,307,5,85,0,
  	0,306,305,1,0,0,0,306,307,1,0,0,0,307,308,1,0,0,0,308,309,5,4,0,0,309,
  	47,1,0,0,0,310,311,5,3,0,0,311,314,3,94,47,0,312,313,5,85,0,0,313,315,
  	3,94,47,0,314,312,1,0,0,0,314,315,1,0,0,0,315,318,1,0,0,0,316,317,5,85,
  	0,0,317,319,3,94,47,0,318,316,1,0,0,0,318,319,1,0,0,0,319,320,1,0,0,0,
  	320,321,5,4,0,0,321,49,1,0,0,0,322,324,5,95,0,0,323,325,3,46,23,0,324,
  	323,1,0,0,0,324,325,1,0,0,0,325,51,1,0,0,0,326,328,5,5,0,0,327,329,3,
  	94,47,0,328,327,1,0,0,0,328,329,1,0,0,0,329,330,1,0,0,0,330,331,5,6,0,
  	0,331,53,1,0,0,0,332,334,5,1,0,0,333,335,3,94,47,0,334,333,1,0,0,0,334,
  	335,1,0,0,0,335,336,1,0,0,0,336,337,5,2,0,0,337,55,1,0,0,0,338,339,3,
  	32,16,0,339,340,3,60,30,0,340,57,1,0,0,0,341,342,3,34,17,0,342,343,3,
  	60,30,0,343,59,1,0,0,0,344,345,3,62,31,0,345,346,5,96,0,0,346,61,1,0,
  	0,0,347,348,3,50,25,0,348,351,5,95,0,0,349,350,5,57,0,0,350,352,3,94,
  	47,0,351,349,1,0,0,0,351,352,1,0,0,0,352,357,1,0,0,0,353,354,5,85,0,0,
  	354,356,3,64,32,0,355,353,1,0,0,0,356,359,1,0,0,0,357,355,1,0,0,0,357,
  	358,1,0,0,0,358,63,1,0,0,0,359,357,1,0,0,0,360,363,5,95,0,0,361,362,5,
  	57,0,0,362,364,3,94,47,0,363,361,1,0,0,0,363,364,1,0,0,0,364,65,1,0,0,
  	0,365,368,3,62,31,0,366,368,3,94,47,0,367,365,1,0,0,0,367,366,1,0,0,0,
  	368,67,1,0,0,0,369,374,3,70,35,0,370,371,5,85,0,0,371,373,3,70,35,0,372,
  	370,1,0,0,0,373,376,1,0,0,0,374,372,1,0,0,0,374,375,1,0,0,0,375,378,1,
  	0,0,0,376,374,1,0,0,0,377,379,5,85,0,0,378,377,1,0,0,0,378,379,1,0,0,
  	0,379,69,1,0,0,0,380,384,5,5,0,0,381,383,3,68,34,0,382,381,1,0,0,0,383,
  	386,1,0,0,0,384,382,1,0,0,0,384,385,1,0,0,0,385,387,1,0,0,0,386,384,1,
  	0,0,0,387,390,5,6,0,0,388,390,3,94,47,0,389,380,1,0,0,0,389,388,1,0,0,
  	0,390,71,1,0,0,0,391,392,5,34,0,0,392,393,3,50,25,0,393,394,5,95,0,0,
  	394,396,5,1,0,0,395,397,3,82,41,0,396,395,1,0,0,0,396,397,1,0,0,0,397,
  	398,1,0,0,0,398,413,5,2,0,0,399,400,5,35,0,0,400,401,5,3,0,0,401,406,
  	5,95,0,0,402,403,5,85,0,0,403,405,5,95,0,0,404,402,1,0,0,0,405,408,1,
  	0,0,0,406,404,1,0,0,0,406,407,1,0,0,0,407,410,1,0,0,0,408,406,1,0,0,0,
  	409,411,5,85,0,0,410,409,1,0,0,0,410,411,1,0,0,0,411,412,1,0,0,0,412,
  	414,5,4,0,0,413,399,1,0,0,0,413,414,1,0,0,0,414,415,1,0,0,0,415,416,3,
  	86,43,0,416,73,1,0,0,0,417,418,3,30,15,0,418,419,5,36,0,0,419,421,5,1,
  	0,0,420,422,3,82,41,0,421,420,1,0,0,0,421,422,1,0,0,0,422,423,1,0,0,0,
  	423,424,5,2,0,0,424,426,3,86,43,0,425,427,5,96,0,0,426,425,1,0,0,0,426,
  	427,1,0,0,0,427,75,1,0,0,0,428,429,3,30,15,0,429,430,3,50,25,0,430,431,
  	5,95,0,0,431,433,5,1,0,0,432,434,3,82,41,0,433,432,1,0,0,0,433,434,1,
  	0,0,0,434,435,1,0,0,0,435,437,5,2,0,0,436,438,3,28,14,0,437,436,1,0,0,
  	0,437,438,1,0,0,0,438,444,1,0,0,0,439,441,3,86,43,0,440,442,5,96,0,0,
  	441,440,1,0,0,0,441,442,1,0,0,0,442,445,1,0,0,0,443,445,5,96,0,0,444,
  	439,1,0,0,0,444,443,1,0,0,0,445,77,1,0,0,0,446,447,5,14,0,0,447,79,1,
  	0,0,0,448,449,7,2,0,0,449,81,1,0,0,0,450,455,3,84,42,0,451,452,5,85,0,
  	0,452,454,3,84,42,0,453,451,1,0,0,0,454,457,1,0,0,0,455,453,1,0,0,0,455,
  	456,1,0,0,0,456,459,1,0,0,0,457,455,1,0,0,0,458,460,5,85,0,0,459,458,
  	1,0,0,0,459,460,1,0,0,0,460,83,1,0,0,0,461,462,3,36,18,0,462,468,3,50,
  	25,0,463,466,5,95,0,0,464,465,5,57,0,0,465,467,3,94,47,0,466,464,1,0,
  	0,0,466,467,1,0,0,0,467,469,1,0,0,0,468,463,1,0,0,0,468,469,1,0,0,0,469,
  	85,1,0,0,0,470,474,5,5,0,0,471,473,3,88,44,0,472,471,1,0,0,0,473,476,
  	1,0,0,0,474,472,1,0,0,0,474,475,1,0,0,0,475,477,1,0,0,0,476,474,1,0,0,
  	0,477,478,5,6,0,0,478,87,1,0,0,0,479,483,3,86,43,0,480,483,3,58,29,0,
  	481,483,3,92,46,0,482,479,1,0,0,0,482,480,1,0,0,0,482,481,1,0,0,0,483,
  	89,1,0,0,0,484,487,3,86,43,0,485,487,3,92,46,0,486,484,1,0,0,0,486,485,
  	1,0,0,0,487,91,1,0,0,0,488,491,3,94,47,0,489,491,3,96,48,0,490,488,1,
  	0,0,0,490,489,1,0,0,0,490,491,1,0,0,0,491,492,1,0,0,0,492,499,5,96,0,
  	0,493,499,3,106,53,0,494,499,3,98,49,0,495,499,3,120,60,0,496,499,3,72,
  	36,0,497,499,3,128,64,0,498,490,1,0,0,0,498,493,1,0,0,0,498,494,1,0,0,
  	0,498,495,1,0,0,0,498,496,1,0,0,0,498,497,1,0,0,0,499,93,1,0,0,0,500,
  	501,6,47,-1,0,501,502,5,1,0,0,502,503,3,94,47,0,503,504,5,2,0,0,504,539,
  	1,0,0,0,505,509,5,5,0,0,506,508,3,68,34,0,507,506,1,0,0,0,508,511,1,0,
  	0,0,509,507,1,0,0,0,509,510,1,0,0,0,510,512,1,0,0,0,511,509,1,0,0,0,512,
  	539,5,6,0,0,513,514,7,3,0,0,514,539,3,94,47,18,515,517,5,42,0,0,516,518,
  	5,95,0,0,517,516,1,0,0,0,517,518,1,0,0,0,518,520,1,0,0,0,519,521,3,54,
  	27,0,520,519,1,0,0,0,520,521,1,0,0,0,521,524,1,0,0,0,522,525,3,48,24,
  	0,523,525,3,46,23,0,524,522,1,0,0,0,524,523,1,0,0,0,524,525,1,0,0,0,525,
  	527,1,0,0,0,526,528,3,52,26,0,527,526,1,0,0,0,527,528,1,0,0,0,528,539,
  	1,0,0,0,529,539,7,4,0,0,530,539,5,44,0,0,531,539,7,5,0,0,532,534,5,94,
  	0,0,533,532,1,0,0,0,534,535,1,0,0,0,535,533,1,0,0,0,535,536,1,0,0,0,536,
  	539,1,0,0,0,537,539,5,95,0,0,538,500,1,0,0,0,538,505,1,0,0,0,538,513,
  	1,0,0,0,538,515,1,0,0,0,538,529,1,0,0,0,538,530,1,0,0,0,538,531,1,0,0,
  	0,538,533,1,0,0,0,538,537,1,0,0,0,539,601,1,0,0,0,540,541,10,24,0,0,541,
  	542,5,78,0,0,542,600,3,94,47,24,543,544,10,20,0,0,544,545,5,84,0,0,545,
  	600,3,94,47,21,546,547,10,17,0,0,547,548,7,6,0,0,548,600,3,94,47,18,549,
  	550,10,16,0,0,550,551,7,7,0,0,551,600,3,94,47,17,552,553,10,15,0,0,553,
  	554,7,8,0,0,554,600,3,94,47,16,555,556,10,14,0,0,556,557,7,9,0,0,557,
  	600,3,94,47,15,558,559,10,13,0,0,559,560,7,10,0,0,560,600,3,94,47,14,
  	561,562,10,12,0,0,562,563,5,54,0,0,563,600,3,94,47,13,564,565,10,11,0,
  	0,565,566,5,56,0,0,566,600,3,94,47,12,567,568,10,10,0,0,568,569,5,55,
  	0,0,569,600,3,94,47,11,570,571,10,9,0,0,571,572,5,70,0,0,572,600,3,94,
  	47,10,573,574,10,8,0,0,574,575,5,71,0,0,575,600,3,94,47,9,576,577,10,
  	7,0,0,577,578,7,11,0,0,578,600,3,94,47,8,579,580,10,25,0,0,580,600,7,
  	12,0,0,581,582,10,23,0,0,582,591,5,1,0,0,583,588,3,94,47,0,584,585,5,
  	85,0,0,585,587,3,94,47,0,586,584,1,0,0,0,587,590,1,0,0,0,588,586,1,0,
  	0,0,588,589,1,0,0,0,589,592,1,0,0,0,590,588,1,0,0,0,591,583,1,0,0,0,591,
  	592,1,0,0,0,592,593,1,0,0,0,593,600,5,2,0,0,594,595,10,21,0,0,595,596,
  	5,3,0,0,596,597,3,94,47,0,597,598,5,4,0,0,598,600,1,0,0,0,599,540,1,0,
  	0,0,599,543,1,0,0,0,599,546,1,0,0,0,599,549,1,0,0,0,599,552,1,0,0,0,599,
  	555,1,0,0,0,599,558,1,0,0,0,599,561,1,0,0,0,599,564,1,0,0,0,599,567,1,
  	0,0,0,599,570,1,0,0,0,599,573,1,0,0,0,599,576,1,0,0,0,599,579,1,0,0,0,
  	599,581,1,0,0,0,599,594,1,0,0,0,600,603,1,0,0,0,601,599,1,0,0,0,601,602,
  	1,0,0,0,602,95,1,0,0,0,603,601,1,0,0,0,604,605,5,19,0,0,605,606,3,94,
  	47,0,606,97,1,0,0,0,607,611,3,100,50,0,608,611,3,102,51,0,609,611,3,104,
  	52,0,610,607,1,0,0,0,610,608,1,0,0,0,610,609,1,0,0,0,611,99,1,0,0,0,612,
  	613,5,25,0,0,613,614,5,1,0,0,614,615,3,66,33,0,615,616,5,2,0,0,616,617,
  	3,90,45,0,617,101,1,0,0,0,618,619,5,24,0,0,619,620,3,90,45,0,620,621,
  	5,25,0,0,621,622,5,1,0,0,622,623,3,66,33,0,623,624,5,2,0,0,624,625,5,
  	96,0,0,625,103,1,0,0,0,626,627,5,23,0,0,627,629,5,1,0,0,628,630,3,66,
  	33,0,629,628,1,0,0,0,629,630,1,0,0,0,630,631,1,0,0,0,631,633,5,96,0,0,
  	632,634,3,66,33,0,633,632,1,0,0,0,633,634,1,0,0,0,634,635,1,0,0,0,635,
  	637,5,96,0,0,636,638,3,66,33,0,637,636,1,0,0,0,637,638,1,0,0,0,638,639,
  	1,0,0,0,639,640,5,2,0,0,640,641,3,90,45,0,641,105,1,0,0,0,642,646,3,108,
  	54,0,643,645,3,110,55,0,644,643,1,0,0,0,645,648,1,0,0,0,646,644,1,0,0,
  	0,646,647,1,0,0,0,647,650,1,0,0,0,648,646,1,0,0,0,649,651,3,112,56,0,
  	650,649,1,0,0,0,650,651,1,0,0,0,651,663,1,0,0,0,652,653,3,114,57,0,653,
  	657,5,5,0,0,654,656,3,116,58,0,655,654,1,0,0,0,656,659,1,0,0,0,657,655,
  	1,0,0,0,657,658,1,0,0,0,658,660,1,0,0,0,659,657,1,0,0,0,660,661,5,6,0,
  	0,661,663,1,0,0,0,662,642,1,0,0,0,662,652,1,0,0,0,663,107,1,0,0,0,664,
  	665,5,20,0,0,665,666,5,1,0,0,666,667,3,66,33,0,667,668,5,2,0,0,668,669,
  	3,90,45,0,669,109,1,0,0,0,670,671,5,21,0,0,671,672,5,1,0,0,672,673,3,
  	66,33,0,673,674,5,2,0,0,674,675,3,90,45,0,675,111,1,0,0,0,676,677,5,22,
  	0,0,677,678,3,90,45,0,678,113,1,0,0,0,679,680,5,28,0,0,680,681,5,1,0,
  	0,681,682,3,66,33,0,682,683,5,2,0,0,683,115,1,0,0,0,684,685,5,29,0,0,
  	685,688,3,118,59,0,686,688,5,30,0,0,687,684,1,0,0,0,687,686,1,0,0,0,688,
  	689,1,0,0,0,689,691,5,86,0,0,690,692,3,90,45,0,691,690,1,0,0,0,691,692,
  	1,0,0,0,692,117,1,0,0,0,693,696,7,13,0,0,694,696,5,95,0,0,695,693,1,0,
  	0,0,695,694,1,0,0,0,696,119,1,0,0,0,697,701,3,122,61,0,698,700,3,124,
  	62,0,699,698,1,0,0,0,700,703,1,0,0,0,701,699,1,0,0,0,701,702,1,0,0,0,
  	702,705,1,0,0,0,703,701,1,0,0,0,704,706,3,126,63,0,705,704,1,0,0,0,705,
  	706,1,0,0,0,706,121,1,0,0,0,707,708,5,31,0,0,708,709,3,90,45,0,709,123,
  	1,0,0,0,710,711,5,32,0,0,711,713,5,1,0,0,712,714,3,66,33,0,713,712,1,
  	0,0,0,713,714,1,0,0,0,714,715,1,0,0,0,715,716,5,2,0,0,716,717,3,90,45,
  	0,717,125,1,0,0,0,718,719,5,33,0,0,719,720,3,90,45,0,720,127,1,0,0,0,
  	721,722,5,43,0,0,722,726,5,5,0,0,723,725,3,130,65,0,724,723,1,0,0,0,725,
  	728,1,0,0,0,726,724,1,0,0,0,726,727,1,0,0,0,727,729,1,0,0,0,728,726,1,
  	0,0,0,729,730,5,6,0,0,730,129,1,0,0,0,731,734,3,132,66,0,732,734,3,142,
  	71,0,733,731,1,0,0,0,733,732,1,0,0,0,734,131,1,0,0,0,735,744,5,95,0,0,
  	736,741,3,134,67,0,737,738,5,85,0,0,738,740,3,134,67,0,739,737,1,0,0,
  	0,740,743,1,0,0,0,741,739,1,0,0,0,741,742,1,0,0,0,742,745,1,0,0,0,743,
  	741,1,0,0,0,744,736,1,0,0,0,744,745,1,0,0,0,745,747,1,0,0,0,746,748,5,
  	96,0,0,747,746,1,0,0,0,747,748,1,0,0,0,748,133,1,0,0,0,749,754,3,136,
  	68,0,750,754,3,138,69,0,751,754,3,140,70,0,752,754,3,142,71,0,753,749,
  	1,0,0,0,753,750,1,0,0,0,753,751,1,0,0,0,753,752,1,0,0,0,754,135,1,0,0,
  	0,755,756,7,13,0,0,756,137,1,0,0,0,757,758,5,91,0,0,758,139,1,0,0,0,759,
  	760,5,3,0,0,760,761,5,95,0,0,761,762,5,4,0,0,762,141,1,0,0,0,763,764,
  	5,86,0,0,764,765,5,95,0,0,765,143,1,0,0,0,94,148,150,164,181,186,188,
  	194,197,202,206,219,226,230,232,241,244,247,250,253,256,259,262,265,269,
  	273,275,286,294,296,303,306,314,318,324,328,334,351,357,363,367,374,378,
  	384,389,396,406,410,413,421,426,433,437,441,444,455,459,466,468,474,482,
  	486,490,498,509,517,520,524,527,535,538,588,591,599,601,610,629,633,637,
  	646,650,657,662,687,691,695,701,705,713,726,733,741,744,747,753
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

tree::TerminalNode* HarlequinParser::NamespaceDeclContext::Term() {
  return getToken(HarlequinParser::Term, 0);
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
    setState(177);
    match(HarlequinParser::NamespaceKw);
    setState(178);
    qualifiedId();
    setState(179);
    namespaceDef();
    setState(181);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Term) {
      setState(180);
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
    setState(183);
    match(HarlequinParser::LeftBrace);
    setState(188);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072679296) != 0)) {
      setState(186);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case HarlequinParser::NamespaceKw: {
          setState(184);
          namespaceDecl();
          break;
        }

        case HarlequinParser::ClassKw:
        case HarlequinParser::InterfaceKw:
        case HarlequinParser::StaticKw:
        case HarlequinParser::PublicKw:
        case HarlequinParser::ProtectedKw:
        case HarlequinParser::PrivateKw: {
          setState(185);
          classDecl();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(190);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(191);
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

tree::TerminalNode* HarlequinParser::ClassDeclContext::Term() {
  return getToken(HarlequinParser::Term, 0);
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
    setState(194);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 962072674304) != 0)) {
      setState(193);
      accessBaseSpecifier();
    }
    setState(197);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(196);
      storageSpecifier();
    }
    setState(199);
    classType();
    setState(200);
    match(HarlequinParser::Id);
    setState(202);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ExtendsKw

    || _la == HarlequinParser::ImplementsKw) {
      setState(201);
      classInheritance();
    }
    setState(204);
    classDef();
    setState(206);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Term) {
      setState(205);
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
    setState(208);
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
    setState(210);
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
    setState(212);
    match(HarlequinParser::LimitedKw);
    setState(213);
    match(HarlequinParser::LeftBracket);
    setState(214);
    qualifiedId();
    setState(219);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(215);
      match(HarlequinParser::Comma);
      setState(216);
      qualifiedId();
      setState(221);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(222);
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
    setState(232);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::PublicKw:
      case HarlequinParser::ProtectedKw:
      case HarlequinParser::PrivateKw: {
        enterOuterAlt(_localctx, 1);
        setState(224);
        accessBaseSpecifier();
        setState(226);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::LimitedKw) {
          setState(225);
          accessLimitSpecifier();
        }
        break;
      }

      case HarlequinParser::LimitedKw: {
        enterOuterAlt(_localctx, 2);
        setState(228);
        accessLimitSpecifier();
        setState(230);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 962072674304) != 0)) {
          setState(229);
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
    setState(234);
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
    setState(236);
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
    setState(238);
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
    setState(241);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3161095929856) != 0)) {
      setState(240);
      accessSpecifier();
    }
    setState(244);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(243);
      storageSpecifier();
    }
    setState(247);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 106496) != 0)) {
      setState(246);
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
    setState(250);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3161095929856) != 0)) {
      setState(249);
      accessSpecifier();
    }
    setState(253);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(252);
      storageSpecifier();
    }
    setState(256);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(255);
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
    setState(259);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::StaticKw) {
      setState(258);
      storageSpecifier();
    }
    setState(262);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(261);
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
    setState(265);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(264);
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
    setState(275);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::ExtendsKw: {
        enterOuterAlt(_localctx, 1);
        setState(267);
        classExtends();
        setState(269);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::ImplementsKw) {
          setState(268);
          classImpls();
        }
        break;
      }

      case HarlequinParser::ImplementsKw: {
        enterOuterAlt(_localctx, 2);
        setState(271);
        classImpls();
        setState(273);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::ExtendsKw) {
          setState(272);
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
    setState(277);
    match(HarlequinParser::ExtendsKw);
    setState(278);
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
    setState(280);
    match(HarlequinParser::ImplementsKw);
    setState(281);
    qualifiedId();
    setState(286);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(282);
      match(HarlequinParser::Comma);
      setState(283);
      qualifiedId();
      setState(288);
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
    setState(289);
    match(HarlequinParser::LeftBrace);
    setState(296);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3229815534336) != 0) || _la == HarlequinParser::Id) {
      setState(294);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
      case 1: {
        setState(290);
        classDecl();
        break;
      }

      case 2: {
        setState(291);
        classVarDeclStmt();
        break;
      }

      case 3: {
        setState(292);
        methodDecl();
        break;
      }

      case 4: {
        setState(293);
        ctorDecl();
        break;
      }

      default:
        break;
      }
      setState(298);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(299);
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
    setState(301);
    match(HarlequinParser::LeftBracket);
    setState(303);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      setState(302);
      match(HarlequinParser::Comma);
      break;
    }

    default:
      break;
    }
    setState(306);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(305);
      match(HarlequinParser::Comma);
    }
    setState(308);
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
    setState(310);
    match(HarlequinParser::LeftBracket);
    setState(311);
    expr(0);
    setState(314);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      setState(312);
      match(HarlequinParser::Comma);
      setState(313);
      expr(0);
      break;
    }

    default:
      break;
    }
    setState(318);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(316);
      match(HarlequinParser::Comma);
      setState(317);
      expr(0);
    }
    setState(320);
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
    setState(322);
    match(HarlequinParser::Id);
    setState(324);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::LeftBracket) {
      setState(323);
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
    setState(326);
    match(HarlequinParser::LeftBrace);
    setState(328);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(327);
      expr(0);
    }
    setState(330);
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
    setState(332);
    match(HarlequinParser::LeftParen);
    setState(334);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(333);
      expr(0);
    }
    setState(336);
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
    setState(338);
    classVarDeclSpecSeq();
    setState(339);
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
    setState(341);
    localVarDeclSpecSeq();
    setState(342);
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
    setState(344);
    varDecl();
    setState(345);
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
    setState(347);
    typeNameDecl();
    setState(348);
    match(HarlequinParser::Id);
    setState(351);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Assign) {
      setState(349);
      match(HarlequinParser::Assign);
      setState(350);
      expr(0);
    }
    setState(357);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Comma) {
      setState(353);
      match(HarlequinParser::Comma);
      setState(354);
      nextVarDecl();
      setState(359);
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
    setState(360);
    match(HarlequinParser::Id);
    setState(363);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Assign) {
      setState(361);
      match(HarlequinParser::Assign);
      setState(362);
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
    setState(367);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(365);
      varDecl();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(366);
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
    setState(369);
    arrayParamElem();
    setState(374);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(370);
        match(HarlequinParser::Comma);
        setState(371);
        arrayParamElem(); 
      }
      setState(376);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx);
    }
    setState(378);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(377);
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
    setState(389);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(380);
      match(HarlequinParser::LeftBrace);
      setState(384);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 80)) & 62991) != 0)) {
        setState(381);
        arrayParamSeq();
        setState(386);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(387);
      match(HarlequinParser::RightBrace);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(388);
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
    setState(391);
    match(HarlequinParser::LambdaKw);
    setState(392);
    typeNameDecl();
    setState(393);
    match(HarlequinParser::Id);
    setState(394);
    match(HarlequinParser::LeftParen);
    setState(396);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(395);
      methodArgSeq();
    }
    setState(398);
    match(HarlequinParser::RightParen);
    setState(413);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::CaptureKw) {
      setState(399);
      match(HarlequinParser::CaptureKw);
      setState(400);
      match(HarlequinParser::LeftBracket);
      setState(401);
      match(HarlequinParser::Id);
      setState(406);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(402);
          match(HarlequinParser::Comma);
          setState(403);
          match(HarlequinParser::Id); 
        }
        setState(408);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx);
      }
      setState(410);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == HarlequinParser::Comma) {
        setState(409);
        match(HarlequinParser::Comma);
      }
      setState(412);
      match(HarlequinParser::RightBracket);
    }
    setState(415);
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

tree::TerminalNode* HarlequinParser::CtorDeclContext::Term() {
  return getToken(HarlequinParser::Term, 0);
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
    setState(417);
    methodDeclSpecSeq();
    setState(418);
    match(HarlequinParser::ConstructorKw);
    setState(419);
    match(HarlequinParser::LeftParen);
    setState(421);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(420);
      methodArgSeq();
    }
    setState(423);
    match(HarlequinParser::RightParen);
    setState(424);
    codeBlock();
    setState(426);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Term) {
      setState(425);
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
    setState(428);
    methodDeclSpecSeq();
    setState(429);
    typeNameDecl();
    setState(430);
    match(HarlequinParser::Id);
    setState(431);
    match(HarlequinParser::LeftParen);
    setState(433);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw || _la == HarlequinParser::Id) {
      setState(432);
      methodArgSeq();
    }
    setState(435);
    match(HarlequinParser::RightParen);
    setState(437);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::ConstKw) {
      setState(436);
      constQualifier();
    }
    setState(444);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftBrace: {
        setState(439);
        codeBlock();
        setState(441);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == HarlequinParser::Term) {
          setState(440);
          match(HarlequinParser::Term);
        }
        break;
      }

      case HarlequinParser::Term: {
        setState(443);
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
    setState(446);
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
    setState(448);
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
    setState(450);
    methodArg();
    setState(455);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(451);
        match(HarlequinParser::Comma);
        setState(452);
        methodArg(); 
      }
      setState(457);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 54, _ctx);
    }
    setState(459);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Comma) {
      setState(458);
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
    setState(461);
    argVarDeclSpecSeq();
    setState(462);
    typeNameDecl();
    setState(468);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Id) {
      setState(463);
      match(HarlequinParser::Id);
      setState(466);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == HarlequinParser::Assign) {
        setState(464);
        match(HarlequinParser::Assign);
        setState(465);
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
    setState(470);
    match(HarlequinParser::LeftBrace);
    setState(474);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 101174927183906) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 128527) != 0)) {
      setState(471);
      codeBlockStmt();
      setState(476);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(477);
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
    setState(482);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 59, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(479);
      codeBlock();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(480);
      localVarDeclStmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(481);
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
    setState(486);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 60, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(484);
      codeBlock();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(485);
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
    setState(498);
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
        setState(490);
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
            setState(488);
            expr(0);
            break;
          }

          case HarlequinParser::ReturnKw: {
            setState(489);
            returnStmt();
            break;
          }

          case HarlequinParser::Term: {
            break;
          }

        default:
          break;
        }
        setState(492);
        match(HarlequinParser::Term);
        break;
      }

      case HarlequinParser::IfKw:
      case HarlequinParser::SwitchKw: {
        enterOuterAlt(_localctx, 2);
        setState(493);
        conditionStmt();
        break;
      }

      case HarlequinParser::ForKw:
      case HarlequinParser::DoKw:
      case HarlequinParser::WhileKw: {
        enterOuterAlt(_localctx, 3);
        setState(494);
        loopStmt();
        break;
      }

      case HarlequinParser::TryKw: {
        enterOuterAlt(_localctx, 4);
        setState(495);
        exceptionHandler();
        break;
      }

      case HarlequinParser::LambdaKw: {
        enterOuterAlt(_localctx, 5);
        setState(496);
        lambdaDecl();
        break;
      }

      case HarlequinParser::AsmIntrinKw: {
        enterOuterAlt(_localctx, 6);
        setState(497);
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
    setState(538);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::LeftParen: {
        _localctx = _tracker.createInstance<ExprParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(501);
        match(HarlequinParser::LeftParen);
        setState(502);
        expr(0);
        setState(503);
        match(HarlequinParser::RightParen);
        break;
      }

      case HarlequinParser::LeftBrace: {
        _localctx = _tracker.createInstance<ExprArrayContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(505);
        match(HarlequinParser::LeftBrace);
        setState(509);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 80)) & 62991) != 0)) {
          setState(506);
          arrayParamSeq();
          setState(511);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(512);
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
        setState(513);
        _la = _input->LA(1);
        if (!(((((_la - 46) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 46)) & 257698037761) != 0))) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(514);
        expr(18);
        break;
      }

      case HarlequinParser::NewKw: {
        _localctx = _tracker.createInstance<ExprInstantiateContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(515);
        match(HarlequinParser::NewKw);
        setState(517);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 64, _ctx)) {
        case 1: {
          setState(516);
          match(HarlequinParser::Id);
          break;
        }

        default:
          break;
        }
        setState(520);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
        case 1: {
          setState(519);
          ctorCall();
          break;
        }

        default:
          break;
        }
        setState(524);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 66, _ctx)) {
        case 1: {
          setState(522);
          arrayTypeDef();
          break;
        }

        case 2: {
          setState(523);
          arrayTypeDecl();
          break;
        }

        default:
          break;
        }
        setState(527);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 67, _ctx)) {
        case 1: {
          setState(526);
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
        setState(529);
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
        setState(530);
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
        setState(531);
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
        setState(533); 
        _errHandler->sync(this);
        alt = 1;
        do {
          switch (alt) {
            case 1: {
                  setState(532);
                  match(HarlequinParser::StrLit);
                  break;
                }

          default:
            throw NoViableAltException(this);
          }
          setState(535); 
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx);
        } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
        break;
      }

      case HarlequinParser::Id: {
        _localctx = _tracker.createInstance<ExprIdContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(537);
        match(HarlequinParser::Id);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(601);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(599);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 72, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ExprExponentContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(540);

          if (!(precpred(_ctx, 24))) throw FailedPredicateException(this, "precpred(_ctx, 24)");
          setState(541);
          match(HarlequinParser::Expo);
          setState(542);
          expr(24);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ExprDotContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(543);

          if (!(precpred(_ctx, 20))) throw FailedPredicateException(this, "precpred(_ctx, 20)");
          setState(544);
          match(HarlequinParser::Dot);
          setState(545);
          expr(21);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ExprArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(546);

          if (!(precpred(_ctx, 17))) throw FailedPredicateException(this, "precpred(_ctx, 17)");
          setState(547);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 985162418487296) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(548);
          expr(18);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<ExprArithmeticContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(549);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(550);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::Add

          || _la == HarlequinParser::Sub)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(551);
          expr(17);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<ExprBitShiftContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(552);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");
          setState(553);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 16888498602639360) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(554);
          expr(16);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<ExprEqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(555);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(556);
          _la = _input->LA(1);
          if (!(((((_la - 74) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 74)) & 15) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(557);
          expr(15);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<ExprEqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(558);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(559);
          _la = _input->LA(1);
          if (!(_la == HarlequinParser::EqualTo

          || _la == HarlequinParser::NotEqualTo)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(560);
          expr(14);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(561);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(562);
          match(HarlequinParser::BitAnd);
          setState(563);
          expr(13);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(564);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(565);
          match(HarlequinParser::BitXor);
          setState(566);
          expr(12);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<ExprBitwiseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(567);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(568);
          match(HarlequinParser::BitOr);
          setState(569);
          expr(11);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<ExprLogicContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(570);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(571);
          match(HarlequinParser::LogicAnd);
          setState(572);
          expr(10);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<ExprLogicContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(573);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(574);
          match(HarlequinParser::LogicOr);
          setState(575);
          expr(9);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<ExprAssignmentContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(576);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(577);
          _la = _input->LA(1);
          if (!(((((_la - 57) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 57)) & 4202495) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(578);
          expr(8);
          break;
        }

        case 14: {
          auto newContext = _tracker.createInstance<ExprUnaryPostfixContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(579);

          if (!(precpred(_ctx, 25))) throw FailedPredicateException(this, "precpred(_ctx, 25)");
          setState(580);
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
          setState(581);

          if (!(precpred(_ctx, 23))) throw FailedPredicateException(this, "precpred(_ctx, 23)");
          setState(582);
          match(HarlequinParser::LeftParen);
          setState(591);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 80)) & 62991) != 0)) {
            setState(583);
            expr(0);
            setState(588);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == HarlequinParser::Comma) {
              setState(584);
              match(HarlequinParser::Comma);
              setState(585);
              expr(0);
              setState(590);
              _errHandler->sync(this);
              _la = _input->LA(1);
            }
          }
          setState(593);
          match(HarlequinParser::RightParen);
          break;
        }

        case 16: {
          auto newContext = _tracker.createInstance<ExprIndexContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(594);

          if (!(precpred(_ctx, 21))) throw FailedPredicateException(this, "precpred(_ctx, 21)");
          setState(595);
          match(HarlequinParser::LeftBracket);
          setState(596);
          expr(0);
          setState(597);
          match(HarlequinParser::RightBracket);
          break;
        }

        default:
          break;
        } 
      }
      setState(603);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx);
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
    setState(604);
    match(HarlequinParser::ReturnKw);
    setState(605);
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
    setState(610);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::WhileKw: {
        enterOuterAlt(_localctx, 1);
        setState(607);
        loopWhileDef();
        break;
      }

      case HarlequinParser::DoKw: {
        enterOuterAlt(_localctx, 2);
        setState(608);
        loopDoWhileDef();
        break;
      }

      case HarlequinParser::ForKw: {
        enterOuterAlt(_localctx, 3);
        setState(609);
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
    setState(612);
    match(HarlequinParser::WhileKw);
    setState(613);
    match(HarlequinParser::LeftParen);
    setState(614);
    ctrlFlowExpr();
    setState(615);
    match(HarlequinParser::RightParen);
    setState(616);
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
    setState(618);
    match(HarlequinParser::DoKw);
    setState(619);
    codeStmt();
    setState(620);
    match(HarlequinParser::WhileKw);
    setState(621);
    match(HarlequinParser::LeftParen);
    setState(622);
    ctrlFlowExpr();
    setState(623);
    match(HarlequinParser::RightParen);
    setState(624);
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
    setState(626);
    match(HarlequinParser::ForKw);
    setState(627);
    match(HarlequinParser::LeftParen);
    setState(629);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(628);
      ctrlFlowExpr();
    }
    setState(631);
    match(HarlequinParser::Term);
    setState(633);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(632);
      ctrlFlowExpr();
    }
    setState(635);
    match(HarlequinParser::Term);
    setState(637);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(636);
      ctrlFlowExpr();
    }
    setState(639);
    match(HarlequinParser::RightParen);
    setState(640);
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
    setState(662);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::IfKw: {
        enterOuterAlt(_localctx, 1);
        setState(642);
        condIfDef();
        setState(646);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(643);
            condElifDef(); 
          }
          setState(648);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
        }
        setState(650);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 79, _ctx)) {
        case 1: {
          setState(649);
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
        setState(652);
        condSwitchDef();
        setState(653);
        match(HarlequinParser::LeftBrace);
        setState(657);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == HarlequinParser::CaseKw

        || _la == HarlequinParser::DefaultKw) {
          setState(654);
          condCaseDef();
          setState(659);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(660);
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
    setState(664);
    match(HarlequinParser::IfKw);
    setState(665);
    match(HarlequinParser::LeftParen);
    setState(666);
    ctrlFlowExpr();
    setState(667);
    match(HarlequinParser::RightParen);
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
    setState(670);
    match(HarlequinParser::ElifKw);
    setState(671);
    match(HarlequinParser::LeftParen);
    setState(672);
    ctrlFlowExpr();
    setState(673);
    match(HarlequinParser::RightParen);
    setState(674);
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
    setState(676);
    match(HarlequinParser::ElseKw);
    setState(677);
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
    setState(679);
    match(HarlequinParser::SwitchKw);
    setState(680);
    match(HarlequinParser::LeftParen);
    setState(681);
    ctrlFlowExpr();
    setState(682);
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
    setState(687);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::CaseKw: {
        setState(684);
        match(HarlequinParser::CaseKw);
        setState(685);
        condCaseResult();
        break;
      }

      case HarlequinParser::DefaultKw: {
        setState(686);
        match(HarlequinParser::DefaultKw);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(689);
    match(HarlequinParser::Colon);
    setState(691);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 101174927163426) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 128527) != 0)) {
      setState(690);
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
    setState(695);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit: {
        _localctx = _tracker.createInstance<HarlequinParser::CaseLiteralContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(693);
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
        setState(694);
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
    setState(697);
    tryBlockDef();
    setState(701);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 85, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(698);
        catchBlockDef(); 
      }
      setState(703);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 85, _ctx);
    }
    setState(705);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 86, _ctx)) {
    case 1: {
      setState(704);
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
    setState(707);
    match(HarlequinParser::TryKw);
    setState(708);
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
    setState(710);
    match(HarlequinParser::CatchKw);
    setState(711);
    match(HarlequinParser::LeftParen);
    setState(713);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 92359178059810) != 0) || ((((_la - 80) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 80)) & 62991) != 0)) {
      setState(712);
      ctrlFlowExpr();
    }
    setState(715);
    match(HarlequinParser::RightParen);
    setState(716);
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
    setState(718);
    match(HarlequinParser::FinallyKw);
    setState(719);
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
    setState(721);
    match(HarlequinParser::AsmIntrinKw);
    setState(722);
    match(HarlequinParser::LeftBrace);
    setState(726);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == HarlequinParser::Colon

    || _la == HarlequinParser::Id) {
      setState(723);
      asmStmt();
      setState(728);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(729);
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
    setState(733);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::Id: {
        enterOuterAlt(_localctx, 1);
        setState(731);
        asmInstr();
        break;
      }

      case HarlequinParser::Colon: {
        enterOuterAlt(_localctx, 2);
        setState(732);
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
    setState(735);
    match(HarlequinParser::Id);
    setState(744);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx)) {
    case 1: {
      setState(736);
      asmOperand();
      setState(741);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == HarlequinParser::Comma) {
        setState(737);
        match(HarlequinParser::Comma);
        setState(738);
        asmOperand();
        setState(743);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    default:
      break;
    }
    setState(747);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == HarlequinParser::Term) {
      setState(746);
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
    setState(753);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case HarlequinParser::NullLit:
      case HarlequinParser::BoolLit:
      case HarlequinParser::IntLit:
      case HarlequinParser::RealLit:
      case HarlequinParser::StrLit: {
        enterOuterAlt(_localctx, 1);
        setState(749);
        asmLit();
        break;
      }

      case HarlequinParser::AsmRegId: {
        enterOuterAlt(_localctx, 2);
        setState(750);
        asmReg();
        break;
      }

      case HarlequinParser::LeftBracket: {
        enterOuterAlt(_localctx, 3);
        setState(751);
        asmVar();
        break;
      }

      case HarlequinParser::Colon: {
        enterOuterAlt(_localctx, 4);
        setState(752);
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
    setState(755);
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
    setState(757);
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
    setState(759);
    match(HarlequinParser::LeftBracket);
    setState(760);
    match(HarlequinParser::Id);
    setState(761);
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
    setState(763);
    match(HarlequinParser::Colon);
    setState(764);
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
