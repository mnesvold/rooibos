(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_compare_ints(p_x, p_y) {
            p_x = p_x | 0;
            p_y = p_y | 0;
            (p_x | 0) == (p_y | 0) ? 1 : 0;
            (p_x | 0) != (p_y | 0) ? 1 : 0;
            (p_x | 0) > (p_y | 0) ? 1 : 0;
            (p_x | 0) >= (p_y | 0) ? 1 : 0;
            (p_x | 0) < (p_y | 0) ? 1 : 0;
            (p_x | 0) <= (p_y | 0) ? 1 : 0;
            return;
        }
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
            f_compare_ints: f_compare_ints,
            f_select_int: f_select_int,
            f_select_double: f_select_double
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        compare_ints: asm.f_compare_ints,
        select_int: asm.f_select_int,
        select_double: asm.f_select_double
    };
    globals.ASM = adaptors;
}(this));
