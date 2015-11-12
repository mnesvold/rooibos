#include "rooibos/InstCodegenVisitor.hh"

#include <algorithm>

#include "rooibos/codegen.hh"

using std::equal;
using std::make_shared;
using std::shared_ptr;
using std::string;

using llvm::Argument;
using llvm::CallInst;
using llvm::ConstantInt;
using llvm::dyn_cast_or_null;
using llvm::Instruction;
using llvm::ReturnInst;
using llvm::Value;

namespace rooibos
{
  namespace
  {
    bool
    startsWith(const std::string & string, const std::string & prefix)
    {
      return equal(prefix.begin(), prefix.end(), string.begin());
    }

    bool
    isStdlibCall(std::string & name)
    {
      string prefix = "__rooibos_stdlib_";
      if(!startsWith(name, prefix))
      {
        return false;
      }
      name = name.substr(prefix.length());
      return true;
    }
  }

  void
  InstCodegenVisitor::visitCallInst(CallInst & inst)
  {
    auto callee = inst.getCalledFunction();
    string calleeName = callee->getName();
    IdentifierAST::ptr calleeIdent;
    if(isStdlibCall(calleeName))
    {
      _ctx.stdlib.insert(calleeName);
      calleeIdent = _ctx.idents.forStdlibFunc(calleeName);
    }
      else
    {
      calleeIdent = _ctx.idents.forFunction(calleeName);
    }
    auto call = make_shared<CallExpressionAST>(calleeIdent);

    for(auto & arg : inst.arg_operands())
    {
      call->arguments.push_back(codegen(_ctx.idents, arg));
    }

    _emit(inst, call);
  }

  void
  InstCodegenVisitor::visitInstruction(Instruction & inst)
  {
    _stmts.push_back(make_shared<ExpressionStatementAST>(
          make_shared<StringLiteralAST>(inst.getOpcodeName())));
  }

  void
  InstCodegenVisitor::visitReturnInst(ReturnInst & inst)
  {
    auto stmt = make_shared<ReturnStatementAST>();
    Value * value = inst.getReturnValue();
    if(value)
    {
      stmt->argument = codegen(_ctx.idents, value);
    }
    _stmts.push_back(stmt);
  }

  void
  InstCodegenVisitor::_emit(Instruction & inst, ExpressionAST::ptr expr)
  {
    auto stmtExpr = expr;
    if(inst.hasNUsesOrMore(1))
    {
      stmtExpr = _assign(inst, coerce(inst.getType(), expr));
    }
    auto stmt = make_shared<ExpressionStatementAST>(stmtExpr);
    _stmts.push_back(stmt);
  }

  ExpressionAST::ptr
  InstCodegenVisitor::_assign(Instruction & inst,
                              ExpressionAST::ptr expr)
  {
    auto ident = _ctx.idents.forInstruction(inst);
    _vars.push_back(make_shared<VariableDeclaratorAST>(
          ident, codegenDefaultValue(inst.getType())));
    return make_shared<AssignmentExpressionAST>(ident, expr);
  }
}
