
// Generated from D:/dev/oss/Harlequin/Harlequin.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  HarlequinLexer : public antlr4::Lexer {
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

  explicit HarlequinLexer(antlr4::CharStream *input);

  ~HarlequinLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

