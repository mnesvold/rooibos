(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function _get_answer() {
            return 42 | 0;
        }
        return { _get_answer: _get_answer };
    };
    var asm = ASM(globals);
    var adaptors = { get_answer: asm._get_answer };
    globals.ASM = adaptors;
}(this));
