#ifndef ROOIBOS_AST_LITERALS_HH
#define ROOIBOS_AST_LITERALS_HH

#include <string>

#include "rooibos/ast-root.hh"

namespace rooibos
{
  struct NumberLiteralAST : LiteralAST
  {
    ROOIBOS_AST_DEFINE_PTR(NumberLiteralAST)

    long value;

    explicit NumberLiteralAST(long value) : value(value) {}

    void accept(LiteralVisitor & visitor) const
    {
      visitor.visit(*this);
    }
  };

  struct StringLiteralAST : LiteralAST
  {
    ROOIBOS_AST_DEFINE_PTR(StringLiteralAST)

    std::string value;

    explicit StringLiteralAST(const std::string & value) : value(value) {}

    void accept(LiteralVisitor & visitor) const
    {
      visitor.visit(*this);
    }
  };
}

#endif
