#include "rooibos/codegen.hh"

#include "rooibos/InstCodegenVisitor.hh"
#include "rooibos/InstContextVisitor.hh"

using std::map;
using std::set;
using std::string;
using std::vector;

using llvm::BasicBlock;
using llvm::Function;

namespace rooibos
{
  namespace
  {
    VariableDeclaratorAST::ptr
    codegen_fp_var_decl(CodegenContext & ctx)
    {
      return VariableDeclaratorAST::create(ctx.idents.FP,
          NumberLiteralAST::create(0));
    }

    ExpressionStatementAST::ptr
    codegen_fp_prologue(CodegenContext & ctx)
    {
      auto coercedFP = BinaryExpressionAST::create(ctx.idents.SP,
          BinaryOp::BITWISE_OR, NumberLiteralAST::create(0));
      auto init = AssignmentExpressionAST::create(ctx.idents.FP, coercedFP);
      return ExpressionStatementAST::create(init);
    }

    ExpressionStatementAST::ptr
    codegen_fp_epilogue(CodegenContext & ctx)
    {
      return ExpressionStatementAST::create(
          AssignmentExpressionAST::create(
            ctx.idents.SP,
            BinaryExpressionAST::create(
              ctx.idents.FP,
              BinaryOp::BITWISE_OR,
              NumberLiteralAST::create(0))));
    }
  }

  void
  codegen(Function & func,
          CodegenContext & ctx,
          vector<StatementAST::ptr> & impls,
          ObjectExpressionAST::ptr asmRet,
          ObjectExpressionAST::ptr adaptors)
  {
      auto & idents = ctx.idents;

      if(func.isDeclaration())
      {
        return;
      }
      auto funcIdent = idents.forFunction(func.getName());
      auto externIdent = idents.forFunctionExtern(func.getName());

      InstContextVisitor reqs(func);

      auto impl = FunctionDeclarationAST::create(funcIdent);
      auto & body = impl->body->body;
      vector<StatementAST::ptr> paramCoercions, prologue, core, epilogue;
      vector<VariableDeclaratorAST::ptr> vars;

      if(reqs.needsFramePointer())
      {
        ctx.needsStackPointer = true;
        vars.insert(vars.begin(), codegen_fp_var_decl(ctx));
        prologue.push_back(codegen_fp_prologue(ctx));
        epilogue.push_back(codegen_fp_epilogue(ctx));
      }

      for(auto & param : func.getArgumentList())
      {
        auto ident = idents.forParameter(param.getName());
        impl->params.push_back(ident);
        auto typeExpr = codegen(idents, &param);
        auto paramType = AssignmentExpressionAST::create(ident, typeExpr);
        paramCoercions.push_back(ExpressionStatementAST::create(paramType));
      }

      map<BasicBlock *, int> bbIndices;
      map<int, BlockStatementAST::ptr> bbBodies;
      for(auto & bbRef : func.getBasicBlockList())
      {
        auto bbIndex = bbIndices.size();
        bbIndices.emplace(&bbRef, bbIndex);
      }
      for(auto & pair : bbIndices) // no codegen until they're ALL indexed
      {
        BasicBlock * bb = pair.first;
        auto bbIndex = pair.second;
        auto bbBody = BlockStatementAST::create();
        bbBodies.emplace(bbIndex, bbBody);
        InstCodegenVisitor instVisitor(
            ctx, vars, bbIndices, bbBody->body, epilogue);
        instVisitor.visit(bb);
      }

      if(reqs.needsProgramCounter())
      {
        vars.push_back(VariableDeclaratorAST::create(idents.PC,
              NumberLiteralAST::create(0)));
        auto pcWhile = WhileStatementAST::create(NumberLiteralAST::create(1));
        auto pcSwitch = SwitchStatementAST::create(coerceToInt(idents.PC));
        for(auto & pair : bbBodies)
        {
          auto caseTest = NumberLiteralAST::create(pair.first);
          auto bbCase = SwitchCaseAST::create(caseTest, pair.second->body);
          pcSwitch->cases.push_back(bbCase);
        }
        pcWhile->body->body.push_back(pcSwitch);
        core.push_back(pcWhile);
      }
        else
      {
        auto onlyBlock = bbBodies[0];
        auto & onlyBody = onlyBlock->body;
        core.insert(core.end(), onlyBody.begin(), onlyBody.end());
      }

      body.insert(body.begin(), paramCoercions.begin(), paramCoercions.end());
      if(!vars.empty())
      {
        auto decl = VariableDeclarationAST::create();
        decl->decls.insert(decl->decls.begin(), vars.begin(), vars.end());
        body.push_back(decl);
      }
      body.insert(body.end(), prologue.begin(), prologue.end());
      body.insert(body.end(), core.begin(), core.end());

      impls.push_back(impl);
      asmRet->props.push_back(PropertyAST::create(funcIdent, funcIdent));
      adaptors->props.push_back(PropertyAST::create(
            externIdent,
            MemberExpressionAST::create(idents.asm_, funcIdent)));
  }
}
