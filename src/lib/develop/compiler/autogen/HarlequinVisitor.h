
// Generated from D:/dev/oss/Harlequin/Harlequin.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "HarlequinParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by HarlequinParser.
 */
class  HarlequinVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by HarlequinParser.
   */
    virtual std::any visitRoot(HarlequinParser::RootContext *context) = 0;

    virtual std::any visitCompilerDirective(HarlequinParser::CompilerDirectiveContext *context) = 0;

    virtual std::any visitQualifiedId(HarlequinParser::QualifiedIdContext *context) = 0;

    virtual std::any visitUsingStmt(HarlequinParser::UsingStmtContext *context) = 0;

    virtual std::any visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext *context) = 0;

    virtual std::any visitNamespaceDecl(HarlequinParser::NamespaceDeclContext *context) = 0;

    virtual std::any visitNamespaceDef(HarlequinParser::NamespaceDefContext *context) = 0;

    virtual std::any visitClassDecl(HarlequinParser::ClassDeclContext *context) = 0;

    virtual std::any visitClassType(HarlequinParser::ClassTypeContext *context) = 0;

    virtual std::any visitAccessBaseSpecifier(HarlequinParser::AccessBaseSpecifierContext *context) = 0;

    virtual std::any visitAccessLimitSpecifier(HarlequinParser::AccessLimitSpecifierContext *context) = 0;

    virtual std::any visitAccessSpecifier(HarlequinParser::AccessSpecifierContext *context) = 0;

    virtual std::any visitStorageSpecifier(HarlequinParser::StorageSpecifierContext *context) = 0;

    virtual std::any visitFunctionSpecifier(HarlequinParser::FunctionSpecifierContext *context) = 0;

    virtual std::any visitConstQualifier(HarlequinParser::ConstQualifierContext *context) = 0;

    virtual std::any visitMethodDeclSpecSeq(HarlequinParser::MethodDeclSpecSeqContext *context) = 0;

    virtual std::any visitClassVarDeclSpecSeq(HarlequinParser::ClassVarDeclSpecSeqContext *context) = 0;

    virtual std::any visitLocalVarDeclSpecSeq(HarlequinParser::LocalVarDeclSpecSeqContext *context) = 0;

    virtual std::any visitArgVarDeclSpecSeq(HarlequinParser::ArgVarDeclSpecSeqContext *context) = 0;

    virtual std::any visitClassInheritance(HarlequinParser::ClassInheritanceContext *context) = 0;

    virtual std::any visitClassExtends(HarlequinParser::ClassExtendsContext *context) = 0;

    virtual std::any visitClassImpls(HarlequinParser::ClassImplsContext *context) = 0;

    virtual std::any visitClassDef(HarlequinParser::ClassDefContext *context) = 0;

    virtual std::any visitArrayTypeDecl(HarlequinParser::ArrayTypeDeclContext *context) = 0;

    virtual std::any visitArrayTypeDef(HarlequinParser::ArrayTypeDefContext *context) = 0;

    virtual std::any visitTypeNameDecl(HarlequinParser::TypeNameDeclContext *context) = 0;

    virtual std::any visitBracedInitializer(HarlequinParser::BracedInitializerContext *context) = 0;

    virtual std::any visitCtorCall(HarlequinParser::CtorCallContext *context) = 0;

    virtual std::any visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext *context) = 0;

    virtual std::any visitLocalVarDeclStmt(HarlequinParser::LocalVarDeclStmtContext *context) = 0;

    virtual std::any visitVarDeclStmt(HarlequinParser::VarDeclStmtContext *context) = 0;

    virtual std::any visitVarDecl(HarlequinParser::VarDeclContext *context) = 0;

    virtual std::any visitNextVarDecl(HarlequinParser::NextVarDeclContext *context) = 0;

    virtual std::any visitCtrlFlowExpr(HarlequinParser::CtrlFlowExprContext *context) = 0;

    virtual std::any visitArrayParamSeq(HarlequinParser::ArrayParamSeqContext *context) = 0;

    virtual std::any visitArrayParamElem(HarlequinParser::ArrayParamElemContext *context) = 0;

    virtual std::any visitLambdaDecl(HarlequinParser::LambdaDeclContext *context) = 0;

    virtual std::any visitCtorDecl(HarlequinParser::CtorDeclContext *context) = 0;

    virtual std::any visitMethodDecl(HarlequinParser::MethodDeclContext *context) = 0;

    virtual std::any visitMethodQualifier(HarlequinParser::MethodQualifierContext *context) = 0;

    virtual std::any visitMethodStorageModifier(HarlequinParser::MethodStorageModifierContext *context) = 0;

    virtual std::any visitMethodArgSeq(HarlequinParser::MethodArgSeqContext *context) = 0;

    virtual std::any visitMethodArg(HarlequinParser::MethodArgContext *context) = 0;

    virtual std::any visitCodeBlock(HarlequinParser::CodeBlockContext *context) = 0;

    virtual std::any visitCodeBlockStmt(HarlequinParser::CodeBlockStmtContext *context) = 0;

    virtual std::any visitCodeStmt(HarlequinParser::CodeStmtContext *context) = 0;

    virtual std::any visitExprStmt(HarlequinParser::ExprStmtContext *context) = 0;

    virtual std::any visitExprIntrinsic(HarlequinParser::ExprIntrinsicContext *context) = 0;

    virtual std::any visitExprEquality(HarlequinParser::ExprEqualityContext *context) = 0;

    virtual std::any visitExprControlFlow(HarlequinParser::ExprControlFlowContext *context) = 0;

    virtual std::any visitExprLiteral(HarlequinParser::ExprLiteralContext *context) = 0;

    virtual std::any visitExprDot(HarlequinParser::ExprDotContext *context) = 0;

    virtual std::any visitExprArithmetic(HarlequinParser::ExprArithmeticContext *context) = 0;

    virtual std::any visitExprParen(HarlequinParser::ExprParenContext *context) = 0;

    virtual std::any visitExprUnaryPostfix(HarlequinParser::ExprUnaryPostfixContext *context) = 0;

    virtual std::any visitExprInstantiate(HarlequinParser::ExprInstantiateContext *context) = 0;

    virtual std::any visitExprExponent(HarlequinParser::ExprExponentContext *context) = 0;

    virtual std::any visitExprCall(HarlequinParser::ExprCallContext *context) = 0;

    virtual std::any visitExprId(HarlequinParser::ExprIdContext *context) = 0;

    virtual std::any visitExprAssignment(HarlequinParser::ExprAssignmentContext *context) = 0;

    virtual std::any visitExprIndex(HarlequinParser::ExprIndexContext *context) = 0;

    virtual std::any visitExprLogic(HarlequinParser::ExprLogicContext *context) = 0;

    virtual std::any visitExprArray(HarlequinParser::ExprArrayContext *context) = 0;

    virtual std::any visitExprBitwise(HarlequinParser::ExprBitwiseContext *context) = 0;

    virtual std::any visitExprUnaryPrefix(HarlequinParser::ExprUnaryPrefixContext *context) = 0;

    virtual std::any visitExprStrLiteral(HarlequinParser::ExprStrLiteralContext *context) = 0;

    virtual std::any visitExprBitShift(HarlequinParser::ExprBitShiftContext *context) = 0;

    virtual std::any visitReturnStmt(HarlequinParser::ReturnStmtContext *context) = 0;

    virtual std::any visitLoopStmt(HarlequinParser::LoopStmtContext *context) = 0;

    virtual std::any visitLoopWhileDef(HarlequinParser::LoopWhileDefContext *context) = 0;

    virtual std::any visitLoopDoWhileDef(HarlequinParser::LoopDoWhileDefContext *context) = 0;

    virtual std::any visitLoopForDef(HarlequinParser::LoopForDefContext *context) = 0;

    virtual std::any visitConditionStmt(HarlequinParser::ConditionStmtContext *context) = 0;

    virtual std::any visitCondIfDef(HarlequinParser::CondIfDefContext *context) = 0;

    virtual std::any visitCondElifDef(HarlequinParser::CondElifDefContext *context) = 0;

    virtual std::any visitCondElseDef(HarlequinParser::CondElseDefContext *context) = 0;

    virtual std::any visitCondSwitchDef(HarlequinParser::CondSwitchDefContext *context) = 0;

    virtual std::any visitCondCaseDef(HarlequinParser::CondCaseDefContext *context) = 0;

    virtual std::any visitCaseLiteral(HarlequinParser::CaseLiteralContext *context) = 0;

    virtual std::any visitCaseId(HarlequinParser::CaseIdContext *context) = 0;

    virtual std::any visitExceptionHandler(HarlequinParser::ExceptionHandlerContext *context) = 0;

    virtual std::any visitTryBlockDef(HarlequinParser::TryBlockDefContext *context) = 0;

    virtual std::any visitCatchBlockDef(HarlequinParser::CatchBlockDefContext *context) = 0;

    virtual std::any visitFinallyBlockDef(HarlequinParser::FinallyBlockDefContext *context) = 0;

    virtual std::any visitAsmBlock(HarlequinParser::AsmBlockContext *context) = 0;

    virtual std::any visitAsmStmt(HarlequinParser::AsmStmtContext *context) = 0;

    virtual std::any visitAsmInstr(HarlequinParser::AsmInstrContext *context) = 0;

    virtual std::any visitAsmOperand(HarlequinParser::AsmOperandContext *context) = 0;

    virtual std::any visitAsmLit(HarlequinParser::AsmLitContext *context) = 0;

    virtual std::any visitAsmReg(HarlequinParser::AsmRegContext *context) = 0;

    virtual std::any visitAsmVar(HarlequinParser::AsmVarContext *context) = 0;

    virtual std::any visitAsmLabel(HarlequinParser::AsmLabelContext *context) = 0;


};

