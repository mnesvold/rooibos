#ifndef ROOIBOS_AST_DECLARATIONS_HH
#define ROOIBOS_AST_DECLARATIONS_HH

#include <memory>
#include <vector>

#include "rooibos/ast-root.hh"

namespace rooibos
{
  struct FunctionDeclarationAST : DeclarationAST
  {
    ROOIBOS_AST_DEFINE_PTR(FunctionDeclarationAST)

    std::shared_ptr<IdentifierAST> name;
    std::vector<std::shared_ptr<PatternAST>> params;
    std::shared_ptr<BlockStatementAST> body;

    explicit FunctionDeclarationAST(std::shared_ptr<IdentifierAST> name)
    : name(name)
    {
      body = std::make_shared<BlockStatementAST>();
    }

    void accept(DeclarationVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct PropertyAST : ASTNode
  {
    ROOIBOS_AST_DEFINE_PTR(PropertyAST)

    std::shared_ptr<IdentifierAST> key;
    std::shared_ptr<ExpressionAST> value;

    PropertyAST(std::shared_ptr<IdentifierAST> key,
                std::shared_ptr<ExpressionAST> value)
    : key(key), value(value)
    {}
  };

  struct ObjectExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(ObjectExpressionAST)

    std::vector<std::shared_ptr<PropertyAST>> props;

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct VariableDeclaratorAST : ASTNode
  {
    ROOIBOS_AST_DEFINE_PTR(VariableDeclaratorAST)

    std::shared_ptr<PatternAST> id;
    std::shared_ptr<ExpressionAST> init;

    explicit
    VariableDeclaratorAST(std::shared_ptr<PatternAST> id,
                          std::shared_ptr<ExpressionAST> init = nullptr)
    : id(id), init(init)
    {}
  };

  struct VariableDeclarationAST : DeclarationAST
  {
    ROOIBOS_AST_DEFINE_PTR(VariableDeclarationAST)

    std::vector<std::shared_ptr<VariableDeclaratorAST>> decls;

    VariableDeclarationAST()
    {}

    explicit
    VariableDeclarationAST(std::shared_ptr<PatternAST> id,
                           std::shared_ptr<ExpressionAST> init = nullptr)
    {
      decls.push_back(std::make_shared<VariableDeclaratorAST>(id, init));
    }

    void accept(DeclarationVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };
}

#endif
