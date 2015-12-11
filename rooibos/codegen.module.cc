#include "rooibos/codegen.hh"

#include "rooibos/util.hh"

using std::set;
using std::string;
using std::unique_ptr;
using std::vector;

using llvm::Module;

namespace {
  using namespace rooibos;

  vector<StatementAST::ptr>
  generateStdlibImports(Identifiers & idents, const set<string> & names)
  {
    vector<StatementAST::ptr> imports;
    for(auto name : names)
    {
      auto lhs = idents.forStdlibFunc(name);
      ExpressionAST::ptr rhs = idents.stdlib;
      string::size_type idx;
      while((idx = name.find("__")) != string::npos)
      {
        auto head = name.substr(0, idx);
        name = name.substr(idx + 2);
        auto member = IdentifierAST::create(head);
        rhs = MemberExpressionAST::create(rhs, member);
      }
      rhs = MemberExpressionAST::create(rhs, IdentifierAST::create(name));
      auto stmt = VariableDeclarationAST::create(lhs, rhs);
      imports.push_back(stmt);
    }
    return imports;
  }

}

namespace rooibos {

  unique_ptr<ProgramAST>
  codegen(Module & module)
  {
    Identifiers idents;
    auto STACK_SIZE = 64 * 1024;

    auto program = make_unique<ProgramAST>();
    auto iifeFunc = FunctionExpressionAST::create();
    iifeFunc->params.push_back(idents.globals);
    auto iifeCall = CallExpressionAST::create(iifeFunc);
    iifeCall->arguments.push_back(idents.this_);
    auto iifeCallStmt = ExpressionStatementAST::create(iifeCall);
    program->body.push_back(iifeCallStmt);

    auto asmFunc = FunctionExpressionAST::create();
    auto & asmBody = asmFunc->body->body;
    asmFunc->params.push_back(idents.stdlib);
    asmFunc->params.push_back(idents.ffi);
    asmFunc->params.push_back(idents.heap);
    asmBody.push_back(ExpressionStatementAST::create(
          StringLiteralAST::create("use asm")));
    auto asmRetVal = ObjectExpressionAST::create();
    iifeFunc->body->body.push_back(VariableDeclarationAST::create(
         idents.ASM, asmFunc));

    auto asmCall = CallExpressionAST::create(idents.ASM);
    asmCall->arguments.push_back(idents.globals);

    auto adaptors = ObjectExpressionAST::create();
    set<string> stdlibSymbols;
    bool needsHeap32 = false;
    bool needsSP = false;
    vector<StatementAST::ptr> impls;

    CodegenContext ctx { idents, stdlibSymbols, needsHeap32, needsSP };
    for(auto & func : module.getFunctionList())
    {
      idents.clearInstructionMap();
      codegen(func, ctx, impls, asmRetVal, adaptors);
    }

    auto stdlibImports = generateStdlibImports(idents, stdlibSymbols);
    asmBody.insert(asmBody.end(), stdlibImports.begin(), stdlibImports.end());
    if(needsHeap32)
    {
      auto ctor = MemberExpressionAST::create(
          idents.stdlib, idents.Int32Array);
      auto call = NewExpressionAST::create(ctor);
      call->arguments.push_back(idents.heap);
      auto decl = VariableDeclarationAST::create(
          idents.HEAP32, call);
      asmBody.push_back(decl);
    }
    if(needsSP)
    {
      asmBody.push_back(VariableDeclarationAST::create(
          idents.SP, NumberLiteralAST::create(STACK_SIZE)));
    }
    asmBody.insert(asmBody.end(), impls.begin(), impls.end());
    asmBody.push_back(ReturnStatementAST::create(asmRetVal));

    if(needsHeap32)
    {
      iifeFunc->body->body.push_back(VariableDeclarationAST::create(
          idents.ffi, ObjectExpressionAST::create()));

      auto ctor = MemberExpressionAST::create(
          idents.globals, idents.ArrayBuffer);
      auto call = NewExpressionAST::create(ctor);
      call->arguments.push_back(NumberLiteralAST::create(STACK_SIZE));
      iifeFunc->body->body.push_back(VariableDeclarationAST::create(
          idents.heap, call));

      asmCall->arguments.push_back(idents.ffi);
      asmCall->arguments.push_back(idents.heap);
    }

    iifeFunc->body->body.push_back(VariableDeclarationAST::create(
          idents.asm_, asmCall));
    iifeFunc->body->body.push_back(VariableDeclarationAST::create(
          idents.adaptors, adaptors));
    iifeFunc->body->body.push_back(ExpressionStatementAST::create(
          AssignmentExpressionAST::create(
            MemberExpressionAST::create(idents.globals, idents.asmExtern),
            idents.adaptors)));

    return program;
  }
}
