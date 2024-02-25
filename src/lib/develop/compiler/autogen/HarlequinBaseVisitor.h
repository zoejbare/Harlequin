
// Generated from D:/dev/oss/Harlequin/Harlequin.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "HarlequinVisitor.h"


/**
 * This class provides an empty implementation of HarlequinVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  HarlequinBaseVisitor : public HarlequinVisitor {
public:

  virtual std::any visitRoot(HarlequinParser::RootContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompilerDirective(HarlequinParser::CompilerDirectiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitQualifiedId(HarlequinParser::QualifiedIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUsingStmt(HarlequinParser::UsingStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamespaceDecl(HarlequinParser::NamespaceDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamespaceDef(HarlequinParser::NamespaceDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassDecl(HarlequinParser::ClassDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassType(HarlequinParser::ClassTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAccessBaseSpecifier(HarlequinParser::AccessBaseSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAccessLimitSpecifier(HarlequinParser::AccessLimitSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAccessSpecifier(HarlequinParser::AccessSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStorageSpecifier(HarlequinParser::StorageSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionSpecifier(HarlequinParser::FunctionSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstQualifier(HarlequinParser::ConstQualifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodDeclSpecSeq(HarlequinParser::MethodDeclSpecSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassVarDeclSpecSeq(HarlequinParser::ClassVarDeclSpecSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocalVarDeclSpecSeq(HarlequinParser::LocalVarDeclSpecSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgVarDeclSpecSeq(HarlequinParser::ArgVarDeclSpecSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassInheritance(HarlequinParser::ClassInheritanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassExtends(HarlequinParser::ClassExtendsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassImpls(HarlequinParser::ClassImplsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassDef(HarlequinParser::ClassDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayTypeDecl(HarlequinParser::ArrayTypeDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayTypeDef(HarlequinParser::ArrayTypeDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeNameDecl(HarlequinParser::TypeNameDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBracedInitializer(HarlequinParser::BracedInitializerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCtorCall(HarlequinParser::CtorCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocalVarDeclStmt(HarlequinParser::LocalVarDeclStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDeclStmt(HarlequinParser::VarDeclStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarDecl(HarlequinParser::VarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNextVarDecl(HarlequinParser::NextVarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCtrlFlowExpr(HarlequinParser::CtrlFlowExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayParamSeq(HarlequinParser::ArrayParamSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArrayParamElem(HarlequinParser::ArrayParamElemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLambdaDecl(HarlequinParser::LambdaDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCtorDecl(HarlequinParser::CtorDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodDecl(HarlequinParser::MethodDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodQualifier(HarlequinParser::MethodQualifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodStorageModifier(HarlequinParser::MethodStorageModifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodArgSeq(HarlequinParser::MethodArgSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMethodArg(HarlequinParser::MethodArgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCodeBlock(HarlequinParser::CodeBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCodeBlockStmt(HarlequinParser::CodeBlockStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCodeStmt(HarlequinParser::CodeStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStmt(HarlequinParser::ExprStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprIntrinsic(HarlequinParser::ExprIntrinsicContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprEquality(HarlequinParser::ExprEqualityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprControlFlow(HarlequinParser::ExprControlFlowContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprLiteral(HarlequinParser::ExprLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprDot(HarlequinParser::ExprDotContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprArithmetic(HarlequinParser::ExprArithmeticContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprParen(HarlequinParser::ExprParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprUnaryPostfix(HarlequinParser::ExprUnaryPostfixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprInstantiate(HarlequinParser::ExprInstantiateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprExponent(HarlequinParser::ExprExponentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprCall(HarlequinParser::ExprCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprId(HarlequinParser::ExprIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprAssignment(HarlequinParser::ExprAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprIndex(HarlequinParser::ExprIndexContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprLogic(HarlequinParser::ExprLogicContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprArray(HarlequinParser::ExprArrayContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprBitwise(HarlequinParser::ExprBitwiseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprUnaryPrefix(HarlequinParser::ExprUnaryPrefixContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStrLiteral(HarlequinParser::ExprStrLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprBitShift(HarlequinParser::ExprBitShiftContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturnStmt(HarlequinParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoopStmt(HarlequinParser::LoopStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoopWhileDef(HarlequinParser::LoopWhileDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoopDoWhileDef(HarlequinParser::LoopDoWhileDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLoopForDef(HarlequinParser::LoopForDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionStmt(HarlequinParser::ConditionStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondIfDef(HarlequinParser::CondIfDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondElifDef(HarlequinParser::CondElifDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondElseDef(HarlequinParser::CondElseDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondSwitchDef(HarlequinParser::CondSwitchDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondCaseDef(HarlequinParser::CondCaseDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCaseLiteral(HarlequinParser::CaseLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCaseId(HarlequinParser::CaseIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExceptionHandler(HarlequinParser::ExceptionHandlerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTryBlockDef(HarlequinParser::TryBlockDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCatchBlockDef(HarlequinParser::CatchBlockDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFinallyBlockDef(HarlequinParser::FinallyBlockDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmBlock(HarlequinParser::AsmBlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmStmt(HarlequinParser::AsmStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmInstr(HarlequinParser::AsmInstrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmOperand(HarlequinParser::AsmOperandContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmLit(HarlequinParser::AsmLitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmReg(HarlequinParser::AsmRegContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmVar(HarlequinParser::AsmVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAsmLabel(HarlequinParser::AsmLabelContext *ctx) override {
    return visitChildren(ctx);
  }


};

