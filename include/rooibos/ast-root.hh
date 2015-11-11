#ifndef ROOIBOS_AST_ROOT_HH
#define ROOIBOS_AST_ROOT_HH

#include <memory>
#include <vector>

#include "rooibos/ast-visitors.hh"

namespace rooibos
{
  struct ASTNode
  {
    virtual ~ASTNode() {};
  };

  struct ProgramAST : ASTNode
  {
    std::vector<std::shared_ptr<StatementAST>> body;
  };

  struct StatementAST : ASTNode
  {
    virtual void accept(StatementVisitor &) const = 0;
  };

  struct DeclarationAST : StatementAST
  {
    void accept(StatementVisitor & visitor) const override
    {
      accept((DeclarationVisitor &)visitor);
    }

    virtual void accept(DeclarationVisitor &) const = 0;
  };

  struct PatternAST : ASTNode
  {
    virtual void accept(PatternVisitor & visitor) const = 0;
  };

  struct ExpressionAST : PatternAST
  {
    void accept(PatternVisitor & visitor) const override
    {
      return accept((ExpressionVisitor &)visitor);
    }

    virtual void accept(ExpressionVisitor & visitor) const = 0;
  };

  struct LiteralAST : ExpressionAST
  {
    using ExpressionAST::accept;

    void accept(ExpressionVisitor & visitor) const override
    {
      accept((LiteralVisitor &)visitor);
    }

    virtual void accept(LiteralVisitor & visitor) const = 0;
  };
}

#endif
