#ifndef ROOIBOS_AST_HH
#define ROOIBOS_AST_HH

#include <memory>
#include <vector>

namespace rooibos
{
  struct AssignmentExpressionAST;
  struct BinaryExpressionAST;
  struct BlockStatementAST;
  struct CallExpressionAST;
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
  struct VariableDeclarationAST;
  struct VariableDeclaratorAST;

  class LiteralVisitor
  {
  public:
    ~LiteralVisitor() {};

    virtual void visit(const NumberLiteralAST &) = 0;
    virtual void visit(const StringLiteralAST &) = 0;
  };

  class ExpressionVisitor : LiteralVisitor
  {
  public:
    using LiteralVisitor::visit;

    ~ExpressionVisitor() {};

    virtual void visit(const AssignmentExpressionAST &) = 0;
    virtual void visit(const BinaryExpressionAST &) = 0;
    virtual void visit(const CallExpressionAST &) = 0;
    virtual void visit(const FunctionExpressionAST &) = 0;
    virtual void visit(const IdentifierAST &) = 0;
    virtual void visit(const MemberExpressionAST &) = 0;
    virtual void visit(const NewExpressionAST &) = 0;
    virtual void visit(const ObjectExpressionAST &) = 0;
    virtual void visit(const SubscriptExpressionAST &) = 0;
  };

  class PatternVisitor : public ExpressionVisitor
  {
  public:
    using ExpressionVisitor::visit;

    ~PatternVisitor() {};
  };

  class DeclarationVisitor
  {
  public:
    ~DeclarationVisitor() {};

    virtual void visit(const FunctionDeclarationAST &) = 0;
    virtual void visit(const VariableDeclarationAST &) = 0;
  };

  class StatementVisitor : public DeclarationVisitor
  {
  public:
    using DeclarationVisitor::visit;

    ~StatementVisitor() {};

    virtual void visit(const BlockStatementAST &) = 0;
    virtual void visit(const EmptyStatementAST &) = 0;
    virtual void visit(const ExpressionStatementAST &) = 0;
    virtual void visit(const ReturnStatementAST &) = 0;
  };

  struct ASTNode
  {
    virtual ~ASTNode() {}
  };

  struct ProgramAST : ASTNode
  {
    std::vector<std::shared_ptr<StatementAST>> body;
  };

  struct StatementAST : ASTNode
  {
    virtual void accept(StatementVisitor &) const = 0;
  };

  struct EmptyStatementAST : StatementAST
  {
    void accept(StatementVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct BlockStatementAST : StatementAST
  {
    std::vector<std::shared_ptr<StatementAST>> body;

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

  struct DeclarationAST : StatementAST
  {
    void accept(StatementVisitor & visitor) const override
    {
      accept((DeclarationVisitor &)visitor);
    }

    virtual void accept(DeclarationVisitor &) const = 0;
  };

  struct FunctionDeclarationAST : DeclarationAST
  {
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

    void accept(DeclarationVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
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

  struct ObjectExpressionAST : ExpressionAST
  {
    std::vector<std::shared_ptr<PropertyAST>> props;

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
  };

  struct PropertyAST : ASTNode
  {
    std::shared_ptr<IdentifierAST> key;
    std::shared_ptr<ExpressionAST> value;

    PropertyAST(std::shared_ptr<IdentifierAST> key,
                std::shared_ptr<ExpressionAST> value)
    : key(key), value(value)
    {}
  };

  struct FunctionExpressionAST : ExpressionAST
  {
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

  namespace BinaryOp
  {
    static const std::string SUB = "-";
    static const std::string BITWISE_OR = "|";
    static const std::string SHIFT_RIGHT = ">>";
  }

  struct BinaryExpressionAST : ExpressionAST
  {
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

  struct AssignmentExpressionAST : ExpressionAST
  {
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

  struct NewExpressionAST : ExpressionAST
  {
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

  struct CallExpressionAST : ExpressionAST
  {
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

  struct MemberExpressionAST : ExpressionAST
  {
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

  struct SubscriptExpressionAST : ExpressionAST
  {
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

  struct IdentifierAST : ExpressionAST
  {
    std::string name;

    explicit IdentifierAST(const std::string & name)
    : name(name)
    {}

    void accept(ExpressionVisitor & visitor) const override
    {
      visitor.visit(*this);
    }
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

  struct NumberLiteralAST : LiteralAST
  {
    long value;

    explicit NumberLiteralAST(long value) : value(value) {}

    void accept(LiteralVisitor & visitor) const
    {
      visitor.visit(*this);
    }
  };

  struct StringLiteralAST : LiteralAST
  {
    std::string value;

    explicit StringLiteralAST(const std::string & value) : value(value) {}

    void accept(LiteralVisitor & visitor) const
    {
      visitor.visit(*this);
    }
  };
}

#endif
