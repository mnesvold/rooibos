(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_echo(p_value) {
            p_value = p_value | 0;
            return p_value | 0;
        }
        return { f_echo: f_echo };
    };
    var asm = ASM(globals);
    var adaptors = { echo: asm.f_echo };
    globals.ASM = adaptors;
}(this));
