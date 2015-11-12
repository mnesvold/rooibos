#ifndef ROOIBOS_AST_ROOT_HH
#define ROOIBOS_AST_ROOT_HH

#include <memory>
#include <vector>

#include "rooibos/ast-visitors.hh"

namespace rooibos
{
  #define ROOIBOS_AST_DEFINE_PTR(NAME) \
    typedef std::shared_ptr<NAME> ptr; \
    template<class... Args> \
    static ptr create(Args&&... args) \
    { \
      return std::make_shared<NAME>(std::forward<Args>(args)...); \
    }

  struct ASTNode
  {
    virtual ~ASTNode() {};
  };

  struct ProgramAST : ASTNode
  {
    ROOIBOS_AST_DEFINE_PTR(ProgramAST)

    std::vector<std::shared_ptr<StatementAST>> body;
  };

  struct StatementAST : ASTNode
  {
    typedef std::shared_ptr<StatementAST> ptr;

    virtual void accept(StatementVisitor &) const = 0;
  };

  struct DeclarationAST : StatementAST
  {
    typedef std::shared_ptr<DeclarationAST> ptr;

    void accept(StatementVisitor & visitor) const override
    {
      accept((DeclarationVisitor &)visitor);
    }

    virtual void accept(DeclarationVisitor &) const = 0;
  };

  struct PatternAST : ASTNode
  {
    typedef std::shared_ptr<PatternAST> ptr;

    virtual void accept(PatternVisitor & visitor) const = 0;
  };

  struct ExpressionAST : PatternAST
  {
    typedef std::shared_ptr<ExpressionAST> ptr;

    void accept(PatternVisitor & visitor) const override
    {
      return accept((ExpressionVisitor &)visitor);
    }

    virtual void accept(ExpressionVisitor & visitor) const = 0;
  };

  struct LiteralAST : ExpressionAST
  {
    using ExpressionAST::accept;

    typedef std::shared_ptr<LiteralAST> ptr;

    void accept(ExpressionVisitor & visitor) const override
    {
      accept((LiteralVisitor &)visitor);
    }

    virtual void accept(LiteralVisitor & visitor) const = 0;
  };
}

#endif
