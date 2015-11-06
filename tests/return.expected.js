(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_get_answer() {
            return 42 | 0;
        }
        return { f_get_answer: f_get_answer };
    };
    var asm = ASM(globals);
    var adaptors = { get_answer: asm.f_get_answer };
    globals.ASM = adaptors;
}(this));
