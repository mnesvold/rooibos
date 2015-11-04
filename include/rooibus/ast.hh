#ifndef ROOIBUS_AST_HH
#define ROOIBUS_AST_HH

#include <memory>
#include <vector>

#include <json.hpp>

namespace rooibus
{
  struct ExpressionAST;
  struct IdentifierAST;
  struct PatternAST;
  struct PropertyAST;
  struct StatementAST;
  struct VariableDeclaratorAST;

  struct ASTNode
  {
    virtual ~ASTNode() {}
    virtual nlohmann::json toJSON() const = 0;
  };

  struct ProgramAST : ASTNode
  {
    std::vector<std::shared_ptr<StatementAST>> body;
    nlohmann::json toJSON() const override;
  };

  struct StatementAST : ASTNode
  {
  };

  struct EmptyStatementAST : StatementAST
  {
    nlohmann::json toJSON() const override;
  };

  struct ExpressionStatementAST : StatementAST
  {
    std::shared_ptr<ExpressionAST> expression;

    explicit ExpressionStatementAST(std::shared_ptr<ExpressionAST> expr)
    : expression(std::move(expr))
    {}

    nlohmann::json toJSON() const override;
  };

  struct ReturnStatementAST : StatementAST
  {
    std::shared_ptr<ExpressionAST> argument;

    explicit ReturnStatementAST(std::shared_ptr<ExpressionAST> arg=nullptr)
    : argument(arg)
    {}

    nlohmann::json toJSON() const override;
  };

  struct DeclarationAST : StatementAST
  {
  };

  struct VariableDeclarationAST : DeclarationAST
  {
    std::vector<std::shared_ptr<VariableDeclaratorAST>> decls;

    VariableDeclarationAST()
    {}

    explicit
    VariableDeclarationAST(std::shared_ptr<PatternAST> id,
                           std::shared_ptr<ExpressionAST> init = nullptr)
    {
      decls.push_back(std::make_shared<VariableDeclaratorAST>(id, init));
    }

    nlohmann::json toJSON() const override;
  };

  struct VariableDeclaratorAST : ASTNode
  {
    std::shared_ptr<PatternAST> id;
    std::shared_ptr<ExpressionAST> init;

    explicit
    VariableDeclaratorAST(std::shared_ptr<PatternAST> id,
                          std::shared_ptr<ExpressionAST> init = nullptr)
    : id(id), init(init)
    {}

    nlohmann::json toJSON() const override;
  };

  struct ExpressionAST : ASTNode
  {
  };

  struct ObjectExpressionAST : ExpressionAST
  {
    std::vector<std::shared_ptr<PropertyAST>> props;

    nlohmann::json toJSON() const override;
  };

  struct PropertyAST : ASTNode
  {
    std::shared_ptr<IdentifierAST> key;
    std::shared_ptr<ExpressionAST> value;

    nlohmann::json toJSON() const override;
  };

  struct FunctionExpressionAST : ExpressionAST
  {
    std::vector<std::shared_ptr<PatternAST>> params;
    std::vector<std::shared_ptr<StatementAST>> body;

    nlohmann::json toJSON() const override;
  };

  struct CallExpressionAST : ExpressionAST
  {
    std::shared_ptr<ExpressionAST> callee;
    std::vector<std::shared_ptr<ExpressionAST>> arguments;

    explicit CallExpressionAST(std::shared_ptr<ExpressionAST> callee)
    : callee(std::move(callee))
    {}

    nlohmann::json toJSON() const override;
  };

  struct PatternAST : ASTNode
  {
  };

  struct IdentifierAST : ExpressionAST, PatternAST
  {
    std::string name;
    
    explicit IdentifierAST(const std::string & name)
    : name(name)
    {}

    nlohmann::json toJSON() const override;
  };

  struct LiteralAST : ExpressionAST
  {
    explicit LiteralAST(const std::string & value) : value(value) {}

    nlohmann::json toJSON() const override;

  private:
    nlohmann::json value;
  };
}

#endif
