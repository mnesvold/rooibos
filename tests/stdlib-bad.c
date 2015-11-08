// Rooibos will quite happily codegen calls to any __rooibos_stdlib_* function
// to a stdlib import without verifying that the call complies with the ASM.JS
// spec. This keeps the code generator from going out of date if the set of
// valid stdlib calls changes.

void __rooibos_stdlib_not__a__real__call(void);

void
call_stdlib(void)
{
  __rooibos_stdlib_not__a__real__call();
}
