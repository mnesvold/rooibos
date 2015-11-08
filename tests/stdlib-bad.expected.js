(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var stdlib_not__a__real__call = stdlib.not.a.real.call;
        function f_call_stdlib() {
            stdlib_not__a__real__call();
            return;
        }
        return { f_call_stdlib: f_call_stdlib };
    };
    var asm = ASM(globals);
    var adaptors = { call_stdlib: asm.f_call_stdlib };
    globals.ASM = adaptors;
}(this));
