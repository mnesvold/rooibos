(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var stdlib_Math__min = stdlib.Math.min;
        function f_get_lesser(p_m, p_n) {
            p_m = p_m | 0;
            p_n = p_n | 0;
            var l_0 = 0;
            l_0 = stdlib_Math__min(p_m | 0, p_n | 0) | 0;
            return l_0 | 0;
        }
        return { f_get_lesser: f_get_lesser };
    };
    var asm = ASM(globals);
    var adaptors = { get_lesser: asm.f_get_lesser };
    globals.ASM = adaptors;
}(this));
