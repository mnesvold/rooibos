#include "rooibos/codegen.hh"

using std::forward;
using std::make_shared;
using std::move;
using std::set;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

using llvm::Module;

namespace {
  using namespace rooibos;

  template<class T, class... Args>
  unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }

  vector<shared_ptr<StatementAST>>
  generateStdlibImports(Identifiers & idents, const set<string> & names)
  {
    vector<shared_ptr<StatementAST>> imports;
    for(auto name : names)
    {
      auto lhs = idents.forStdlibFunc(name);
      shared_ptr<ExpressionAST> rhs = idents.stdlib;
      string::size_type idx;
      while((idx = name.find("__")) != string::npos)
      {
        auto head = name.substr(0, idx);
        name = name.substr(idx + 2);
        auto member = make_shared<IdentifierAST>(head);
        rhs = make_shared<MemberExpressionAST>(rhs, member);
      }
      rhs = make_shared<MemberExpressionAST>(rhs, make_shared<IdentifierAST>(name));
      auto stmt = make_shared<VariableDeclarationAST>(lhs, rhs);
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

    auto program = make_unique<ProgramAST>();
    auto iifeFunc = make_shared<FunctionExpressionAST>();
    iifeFunc->params.push_back(idents.globals);
    auto iifeCall = make_shared<CallExpressionAST>(iifeFunc);
    iifeCall->arguments.push_back(idents.this_);
    auto iifeCallStmt = make_shared<ExpressionStatementAST>(iifeCall);
    program->body.push_back(iifeCallStmt);

    auto asmFunc = make_shared<FunctionExpressionAST>();
    auto & asmBody = asmFunc->body->body;
    asmFunc->params.push_back(idents.stdlib);
    asmFunc->params.push_back(idents.ffi);
    asmFunc->params.push_back(idents.heap);
    asmBody.push_back(make_shared<ExpressionStatementAST>(
          make_shared<StringLiteralAST>("use asm")));
    auto asmRetVal = make_shared<ObjectExpressionAST>();
    iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
         idents.ASM, asmFunc));

    auto asmCall = make_shared<CallExpressionAST>(idents.ASM);
    asmCall->arguments.push_back(idents.globals);

    auto adaptors = make_shared<ObjectExpressionAST>();
    set<string> stdlibSymbols;
    bool needsHeap32 = false;
    vector<shared_ptr<StatementAST>> impls;

    CodegenContext ctx { idents, stdlibSymbols, needsHeap32 };
    for(auto & func : module.getFunctionList())
    {
      codegen(func, ctx, impls, asmRetVal, adaptors);
    }

    auto stdlibImports = generateStdlibImports(idents, stdlibSymbols);
    asmBody.insert(asmBody.end(), stdlibImports.begin(), stdlibImports.end());
    if(needsHeap32)
    {
      auto ctor = make_shared<MemberExpressionAST>(
          idents.stdlib, idents.Int32Array);
      auto call = make_shared<NewExpressionAST>(ctor);
      call->arguments.push_back(idents.heap);
      auto decl = make_shared<VariableDeclarationAST>(
          idents.HEAP32, call);
      asmBody.push_back(decl);
    }
    asmBody.insert(asmBody.end(), impls.begin(), impls.end());
    asmBody.push_back(make_shared<ReturnStatementAST>(asmRetVal));

    if(needsHeap32)
    {
      iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
          idents.ffi, make_shared<ObjectExpressionAST>()));

      auto ctor = make_shared<MemberExpressionAST>(
          idents.globals, idents.ArrayBuffer);
      auto call = make_shared<NewExpressionAST>(ctor);
      call->arguments.push_back(make_shared<NumberLiteralAST>(64 * 1024));
      iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
          idents.heap, call));

      asmCall->arguments.push_back(idents.ffi);
      asmCall->arguments.push_back(idents.heap);
    }

    iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
          idents.asm_, asmCall));
    iifeFunc->body->body.push_back(make_shared<VariableDeclarationAST>(
          idents.adaptors, adaptors));
    iifeFunc->body->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<AssignmentExpressionAST>(
            make_shared<MemberExpressionAST>(idents.globals, idents.asmExtern),
            idents.adaptors)));

    return program;
  }
}
