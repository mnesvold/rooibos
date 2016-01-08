(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var stdlib_Math__min = stdlib.Math.min;
        var stdlib_not__a__real__call = stdlib.not.a.real.call;
        function f_get_lesser(p_m, p_n) {
            p_m = p_m | 0;
            p_n = p_n | 0;
            var l_0 = 0;
            l_0 = stdlib_Math__min(p_m | 0, p_n | 0) | 0;
            return l_0 | 0;
        }
        function f_call_missing() {
            stdlib_not__a__real__call();
            return;
        }
        return {
            f_get_lesser: f_get_lesser,
            f_call_missing: f_call_missing
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        get_lesser: asm.f_get_lesser,
        call_missing: asm.f_call_missing
    };
    globals.ASM = adaptors;
}(this));
