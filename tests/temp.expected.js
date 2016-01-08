(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_get_int() {
            return 42 | 0;
        }
        function f_return_int() {
            var l_0 = 0;
            l_0 = f_get_int() | 0;
            return l_0 | 0;
        }
        function f_pick(p_m, p_n) {
            p_m = p_m | 0;
            p_n = p_n | 0;
            return p_m | 0;
        }
        function f_multiple_temps() {
            var l_0 = 0, l_1 = 0, l_2 = 0;
            l_0 = f_get_int() | 0;
            l_1 = f_return_int() | 0;
            l_2 = f_pick(l_0 | 0, l_1 | 0) | 0;
            return l_2 | 0;
        }
        return {
            f_get_int: f_get_int,
            f_return_int: f_return_int,
            f_pick: f_pick,
            f_multiple_temps: f_multiple_temps
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        get_int: asm.f_get_int,
        return_int: asm.f_return_int,
        pick: asm.f_pick,
        multiple_temps: asm.f_multiple_temps
    };
    globals.ASM = adaptors;
}(this));
