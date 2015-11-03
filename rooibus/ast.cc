#include "rooibus/ast.hh"

using std::ostream;

namespace rooibus
{
  ostream &
  operator << (ostream & out, const ASTNode & node)
  {
    node.writeTo(out);
    return out;
  }

  void
  ProgramAST::writeTo(ostream & out) const
  {
    out << "{\"type\":\"Program\",\"body\":[";
    for(auto & stmt : body)
    {
      stmt->writeTo(out);
    }
    out << "]}";
  }

  void
  EmptyStatementAST::writeTo(ostream & out) const
  {
    out << "{\"type\":\"EmptyStatement\"}";
  }
}
