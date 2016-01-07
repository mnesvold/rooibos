#include "rooibos/InstCodegenVisitor.hh"

#include "rooibos/codegen.hh"
#include "rooibos/util.hh"

using std::map;
using std::vector;

using llvm::BasicBlock;
using llvm::BranchInst;
using llvm::ReturnInst;
using llvm::Value;

namespace rooibos
{
  namespace
  {
    void
    codegen_unconditional(BranchInst & inst,
                          const map<BasicBlock *, int> & bbIndices,
                          Identifiers & idents,
                          vector<StatementAST::ptr> & stmts)
    {
      auto nextPC = bbIndices.at(inst.getSuccessor(0));
      auto pcUpdate = ExpressionStatementAST::create(
          AssignmentExpressionAST::create(
            idents.PC,
            coerceToInt(NumberLiteralAST::create(nextPC))));
      stmts.push_back(pcUpdate);
    }

    void
    codegen_conditional(BranchInst & inst,
                        const map<BasicBlock *, int> & bbIndices,
                        Identifiers & idents,
                        vector<StatementAST::ptr> & stmts)
    {
      auto truePC = bbIndices.at(inst.getSuccessor(0));
      auto falsePC = bbIndices.at(inst.getSuccessor(1));
      auto condition = codegen(idents, inst.getCondition());
      auto conditional = ConditionalExpressionAST::create(
          condition,
          NumberLiteralAST::create(truePC),
          NumberLiteralAST::create(falsePC));
      auto pcUpdate = ExpressionStatementAST::create(
          AssignmentExpressionAST::create(
            idents.PC, conditional));
      stmts.push_back(pcUpdate);
    }
  }

  void
  InstCodegenVisitor::visitBranchInst(BranchInst & inst)
  {
    if(inst.isConditional())
    {
      codegen_conditional(inst, _bbIndices, _ctx.idents, _stmts);
    }
      else
    {
      codegen_unconditional(inst, _bbIndices, _ctx.idents, _stmts);
    }
    auto continue_stmt = ContinueStatementAST::create();
    _stmts.push_back(continue_stmt);
  }

  void
  InstCodegenVisitor::visitReturnInst(ReturnInst & inst)
  {
    auto stmt = ReturnStatementAST::create();
    Value * value = inst.getReturnValue();
    if(value)
    {
      stmt->argument = codegen(_ctx.idents, value);
    }
    _stmts.insert(_stmts.end(), _epilogue.begin(), _epilogue.end());
    _stmts.push_back(stmt);
  }
}
