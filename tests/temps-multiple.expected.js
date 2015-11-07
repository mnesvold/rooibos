(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_get_int() {
            return 42 | 0;
        }
        function f_echo(p_n) {
            p_n = p_n | 0;
            return p_n | 0;
        }
        function f_return_int() {
            var l_0 = 0, l_1 = 0;
            l_0 = f_get_int() | 0;
            l_1 = f_echo(l_0 | 0) | 0;
            return l_1 | 0;
        }
        return {
            f_get_int: f_get_int,
            f_echo: f_echo,
            f_return_int: f_return_int
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        get_int: asm.f_get_int,
        echo: asm.f_echo,
        return_int: asm.f_return_int
    };
    globals.ASM = adaptors;
}(this));
