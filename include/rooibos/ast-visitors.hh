#ifndef ROOIBOS_AST_VISITORS_HH
#define ROOIBOS_AST_VISITORS_HH

namespace rooibos
{
  struct AssignmentExpressionAST;
  struct BinaryExpressionAST;
  struct BlockStatementAST;
  struct CallExpressionAST;
  struct ContinueStatementAST;
  struct DoubleLiteralAST;
  struct EmptyStatementAST;
  struct ExpressionAST;
  struct ExpressionStatementAST;
  struct FunctionDeclarationAST;
  struct FunctionExpressionAST;
  struct IdentifierAST;
  struct MemberExpressionAST;
  struct NewExpressionAST;
  struct NumberLiteralAST;
  struct ObjectExpressionAST;
  struct PatternAST;
  struct PropertyAST;
  struct ReturnStatementAST;
  struct StatementAST;
  struct StringLiteralAST;
  struct SubscriptExpressionAST;
  struct SwitchStatementAST;
  struct UnaryExpressionAST;
  struct VariableDeclarationAST;
  struct VariableDeclaratorAST;
  struct WhileStatementAST;

  class LiteralVisitor
  {
  public:
    virtual ~LiteralVisitor() {};

    virtual void visit(const DoubleLiteralAST &) = 0;
    virtual void visit(const NumberLiteralAST &) = 0;
    virtual void visit(const StringLiteralAST &) = 0;
  };

  class ExpressionVisitor : LiteralVisitor
  {
  public:
    using LiteralVisitor::visit;

    virtual ~ExpressionVisitor() {};

    virtual void visit(const AssignmentExpressionAST &) = 0;
    virtual void visit(const BinaryExpressionAST &) = 0;
    virtual void visit(const CallExpressionAST &) = 0;
    virtual void visit(const FunctionExpressionAST &) = 0;
    virtual void visit(const IdentifierAST &) = 0;
    virtual void visit(const MemberExpressionAST &) = 0;
    virtual void visit(const NewExpressionAST &) = 0;
    virtual void visit(const ObjectExpressionAST &) = 0;
    virtual void visit(const SubscriptExpressionAST &) = 0;
    virtual void visit(const UnaryExpressionAST &) = 0;
  };

  class PatternVisitor : public ExpressionVisitor
  {
  public:
    using ExpressionVisitor::visit;

    virtual ~PatternVisitor() {};
  };

  class DeclarationVisitor
  {
  public:
    virtual ~DeclarationVisitor() {};

    virtual void visit(const FunctionDeclarationAST &) = 0;
    virtual void visit(const VariableDeclarationAST &) = 0;
  };

  class StatementVisitor : public DeclarationVisitor
  {
  public:
    using DeclarationVisitor::visit;

    virtual ~StatementVisitor() {};

    virtual void visit(const BlockStatementAST &) = 0;
    virtual void visit(const ContinueStatementAST &) = 0;
    virtual void visit(const EmptyStatementAST &) = 0;
    virtual void visit(const ExpressionStatementAST &) = 0;
    virtual void visit(const ReturnStatementAST &) = 0;
    virtual void visit(const SwitchStatementAST &) = 0;
    virtual void visit(const WhileStatementAST &) = 0;
  };
}

#endif
