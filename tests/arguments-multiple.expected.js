(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_echo(p_m, p_n) {
            p_m = p_m | 0;
            p_n = p_n | 0;
            return p_n | 0;
        }
        function f_pass_to_echo(p_n) {
            p_n = p_n | 0;
            var l_0 = 0;
            l_0 = f_echo(p_n | 0, p_n | 0) | 0;
            return l_0 | 0;
        }
        return {
            f_echo: f_echo,
            f_pass_to_echo: f_pass_to_echo
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        echo: asm.f_echo,
        pass_to_echo: asm.f_pass_to_echo
    };
    globals.ASM = adaptors;
}(this));
