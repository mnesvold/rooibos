#include "rooibus/codegen.hh"

using std::forward;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

using llvm::Function;
using llvm::Module;

namespace rooibus {
namespace {

  template<class T, class... Args>
  unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }

  struct Identifiers
  {
    const shared_ptr<IdentifierAST>
      adaptors = make_shared<IdentifierAST>("adaptors"),
      ASM = make_shared<IdentifierAST>("ASM"),
      asm_ = make_shared<IdentifierAST>("asm"),
      asmExtern = make_shared<IdentifierAST>("ASM"),
      ffi = make_shared<IdentifierAST>("ffi"),
      globals = make_shared<IdentifierAST>("globals"),
      heap = make_shared<IdentifierAST>("heap"),
      stdlib = make_shared<IdentifierAST>("stdlib"),
      this_ = make_shared<IdentifierAST>("this");

    shared_ptr<IdentifierAST> forFunction(const string & name)
    {
      return make_shared<IdentifierAST>("_" + name);
    }

    shared_ptr<IdentifierAST> forFunctionExtern(const string & name)
    {
      return make_shared<IdentifierAST>(name);
    }
  };

  void
  codegen(Function & func,
          Identifiers & idents,
          shared_ptr<FunctionExpressionAST> asmFunc,
          shared_ptr<ObjectExpressionAST> asmRet,
          shared_ptr<ObjectExpressionAST> adaptors)
  {
      auto funcIdent = idents.forFunction(func.getName());
      auto externIdent = idents.forFunctionExtern(func.getName());

      auto impl = make_shared<FunctionDeclarationAST>(funcIdent);
      asmFunc->body.push_back(impl);

      asmRet->props.push_back(make_shared<PropertyAST>(funcIdent, funcIdent));

      adaptors->props.push_back(make_shared<PropertyAST>(
            externIdent,
            make_shared<MemberExpressionAST>(idents.asm_, funcIdent)));
  }

} /* nested anonymous namespace */

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
    asmFunc->params.push_back(idents.stdlib);
    asmFunc->params.push_back(idents.ffi);
    asmFunc->params.push_back(idents.heap);
    asmFunc->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<LiteralAST>("use asm")));
    auto asmRetVal = make_shared<ObjectExpressionAST>();
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
         idents.ASM, asmFunc));

    auto asmCall = make_shared<CallExpressionAST>(idents.ASM);
    asmCall->arguments.push_back(idents.globals);
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
          idents.asm_, asmCall));

    auto adaptors = make_shared<ObjectExpressionAST>();
    iifeFunc->body.push_back(make_shared<VariableDeclarationAST>(
          idents.adaptors, adaptors));
    iifeFunc->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<AssignmentExpressionAST>(
            make_shared<MemberExpressionAST>(idents.globals, idents.asmExtern),
            idents.adaptors)));

    for(auto & func : module.getFunctionList())
    {
      codegen(func, idents, asmFunc, asmRetVal, adaptors);
    }
    asmFunc->body.push_back(make_shared<ReturnStatementAST>(asmRetVal));

    return program;
  }
}
