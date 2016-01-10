(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_select_int() {
            var l_0 = 0;
            l_0 = (1 | 0 ? 10 | 0 : 20 | 0) | 0;
            return l_0 | 0;
        }
        function f_select_double() {
            var l_0 = 0.1;
            l_0 = +(0 | 0 ? +10 : +20);
            return +l_0;
        }
        return {
            f_select_int: f_select_int,
            f_select_double: f_select_double
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        select_int: asm.f_select_int,
        select_double: asm.f_select_double
    };
    globals.ASM = adaptors;
}(this));
