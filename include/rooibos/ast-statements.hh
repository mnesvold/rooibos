#ifndef ROOIBOS_AST_STATEMENTS_HH
#define ROOIBOS_AST_STATEMENTS_HH

#include <memory>
#include <vector>

#include "rooibos/ast-root.hh"

namespace rooibos
{
  struct BlockStatementAST : StatementAST
  {
    std::vector<std::shared_ptr<StatementAST>> body;

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct EmptyStatementAST : StatementAST
  {
    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct ExpressionStatementAST : StatementAST
  {
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
    std::shared_ptr<ExpressionAST> argument;

    explicit ReturnStatementAST(std::shared_ptr<ExpressionAST> arg=nullptr)
    : argument(arg)
    {}

    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };
}

#endif
