(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function _ping() {
        }
        return { _ping: _ping };
    };
    var asm = ASM(globals);
    var adaptors = { ping: asm._ping };
    globals.ASM = adaptors;
}(this));
