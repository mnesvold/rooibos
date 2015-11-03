#ifndef ROOIBUS_AST_HH
#define ROOIBUS_AST_HH

#include <memory>
#include <ostream>
#include <vector>

namespace rooibus
{
  struct StatementAST;

  struct ASTNode
  {
    virtual ~ASTNode() {}
    virtual void writeTo(std::ostream & out) const = 0;
  };

  std::ostream & operator<<(std::ostream & out, const ASTNode & node);

  struct ProgramAST : ASTNode
  {
    std::vector<std::unique_ptr<StatementAST>> body;
    void writeTo(std::ostream & out) const override;
  };

  struct StatementAST : ASTNode
  {
  };

  struct EmptyStatementAST : StatementAST
  {
    void writeTo(std::ostream & out) const override;
  };
}

#endif
