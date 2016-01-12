(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_return_coercion() {
            return 1 | 0;
        }
        function f_param_coercion(p_n) {
            p_n = p_n | 0;
            return;
        }
        function f_arg_coercion() {
            f_param_coercion(1 | 0);
            return;
        }
        function f_locals() {
            var l_0 = 0;
            l_0 = f_return_coercion() | 0;
            f_param_coercion(l_0 | 0);
            return;
        }
        return {
            f_return_coercion: f_return_coercion,
            f_param_coercion: f_param_coercion,
            f_arg_coercion: f_arg_coercion,
            f_locals: f_locals
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        return_coercion: asm.f_return_coercion,
        param_coercion: asm.f_param_coercion,
        arg_coercion: asm.f_arg_coercion,
        locals: asm.f_locals
    };
    globals.ASM = adaptors;
}(this));
