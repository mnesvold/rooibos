(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_add_doubles(p_a, p_b) {
            p_a = +p_a;
            p_b = +p_b;
            var l_0 = 0.1;
            l_0 = +(+p_a + +p_b);
            return +l_0;
        }
        function f_sub_doubles(p_a, p_b) {
            p_a = +p_a;
            p_b = +p_b;
            var l_0 = 0.1;
            l_0 = +(+p_a - +p_b);
            return +l_0;
        }
        function f_mul_doubles(p_a, p_b) {
            p_a = +p_a;
            p_b = +p_b;
            var l_0 = 0.1;
            l_0 = +(+p_a * +p_b);
            return +l_0;
        }
        function f_div_doubles(p_a, p_b) {
            p_a = +p_a;
            p_b = +p_b;
            var l_0 = 0.1;
            l_0 = +(+p_a / +p_b);
            return +l_0;
        }
        function f_rem_doubles(p_a, p_b) {
            p_a = +p_a;
            p_b = +p_b;
            var l_0 = 0.1;
            l_0 = +(+p_a % +p_b);
            return +l_0;
        }
        return {
            f_add_doubles: f_add_doubles,
            f_sub_doubles: f_sub_doubles,
            f_mul_doubles: f_mul_doubles,
            f_div_doubles: f_div_doubles,
            f_rem_doubles: f_rem_doubles
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        add_doubles: asm.f_add_doubles,
        sub_doubles: asm.f_sub_doubles,
        mul_doubles: asm.f_mul_doubles,
        div_doubles: asm.f_div_doubles,
        rem_doubles: asm.f_rem_doubles
    };
    globals.ASM = adaptors;
}(this));
