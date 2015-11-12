#ifndef ROOIBOS_AST_EXPRESSIONS_HH
#define ROOIBOS_AST_EXPRESSIONS_HH

#include <memory>
#include <vector>

#include "rooibos/ast-root.hh"

namespace rooibos
{
  namespace BinaryOp
  {
    static const std::string SUB = "-";
    static const std::string BITWISE_OR = "|";
    static const std::string SHIFT_RIGHT = ">>";
  }

  struct AssignmentExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(AssignmentExpressionAST)

    std::shared_ptr<PatternAST> lhs;
    std::shared_ptr<ExpressionAST> rhs;

    AssignmentExpressionAST(std::shared_ptr<PatternAST> lhs,
                            std::shared_ptr<ExpressionAST> rhs)
    : lhs(lhs), rhs(rhs)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct BinaryExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(BinaryExpressionAST)

    std::shared_ptr<ExpressionAST> lhs;
    std::string op;
    std::shared_ptr<ExpressionAST> rhs;

    BinaryExpressionAST(std::shared_ptr<ExpressionAST> lhs,
                        const std::string & op,
                        std::shared_ptr<ExpressionAST> rhs)
    : lhs(lhs), op(op), rhs(rhs)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct CallExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(CallExpressionAST)

    std::shared_ptr<ExpressionAST> callee;
    std::vector<std::shared_ptr<ExpressionAST>> arguments;

    explicit CallExpressionAST(std::shared_ptr<ExpressionAST> callee)
    : callee(callee)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct FunctionExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(FunctionExpressionAST)

    std::vector<std::shared_ptr<PatternAST>> params;
    std::shared_ptr<BlockStatementAST> body;

    FunctionExpressionAST()
    {
      body = std::make_shared<BlockStatementAST>();
    }

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct IdentifierAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(IdentifierAST)

    std::string name;

    explicit IdentifierAST(const std::string & name)
    : name(name)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct MemberExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(MemberExpressionAST)

    std::shared_ptr<ExpressionAST> object;
    std::shared_ptr<IdentifierAST> property;

    MemberExpressionAST(std::shared_ptr<ExpressionAST> object,
                        std::shared_ptr<IdentifierAST> property)
    : object(object), property(property)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct NewExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(NewExpressionAST)

    std::shared_ptr<ExpressionAST> callee;
    std::vector<std::shared_ptr<ExpressionAST>> arguments;

    explicit NewExpressionAST(std::shared_ptr<ExpressionAST> callee)
    : callee(callee)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct SubscriptExpressionAST : ExpressionAST
  {
    ROOIBOS_AST_DEFINE_PTR(SubscriptExpressionAST)

    std::shared_ptr<ExpressionAST> object;
    std::shared_ptr<ExpressionAST> subscript;

    SubscriptExpressionAST(std::shared_ptr<ExpressionAST> object,
                           std::shared_ptr<ExpressionAST> subscript)
    : object(object), subscript(subscript)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };
}

#endif
