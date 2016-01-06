#ifndef ROOIBOS_AST_STATEMENTS_HH
#define ROOIBOS_AST_STATEMENTS_HH

#include <memory>
#include <vector>

#include "rooibos/ast-root.hh"

namespace rooibos
{
  struct BlockStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(BlockStatementAST)

    std::vector<std::shared_ptr<StatementAST>> body;

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct ContinueStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(ContinueStatementAST)

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct EmptyStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(EmptyStatementAST)

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct ExpressionStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(ExpressionStatementAST)

    std::shared_ptr<ExpressionAST> expression;

    explicit ExpressionStatementAST(std::shared_ptr<ExpressionAST> expr)
    : expression(expr)
    {}

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct ReturnStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(ReturnStatementAST)

    std::shared_ptr<ExpressionAST> argument;

    explicit ReturnStatementAST(std::shared_ptr<ExpressionAST> arg=nullptr)
    : argument(arg)
    {}

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct SwitchCaseAST : ASTNode
  {
    ROOIBOS_AST_DEFINE_PTR(SwitchCaseAST)

    std::shared_ptr<ExpressionAST> test;
    std::vector<std::shared_ptr<StatementAST>> consequent;

    SwitchCaseAST(std::shared_ptr<ExpressionAST> test,
                  const std::vector<std::shared_ptr<StatementAST>> & cons)
    : test(test), consequent(cons)
    {}
  };

  struct SwitchStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(SwitchStatementAST)

    std::shared_ptr<ExpressionAST> discriminant;
    std::vector<std::shared_ptr<SwitchCaseAST>> cases;

    explicit SwitchStatementAST(std::shared_ptr<ExpressionAST> discriminant)
    : discriminant(discriminant)
    {}

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct WhileStatementAST : StatementAST
  {
    ROOIBOS_AST_DEFINE_PTR(WhileStatementAST)

    std::shared_ptr<ExpressionAST> test;
    std::shared_ptr<BlockStatementAST> body;

    explicit WhileStatementAST(std::shared_ptr<ExpressionAST> test)
    : test(test), body(BlockStatementAST::create())
    {}

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };
}

#endif
