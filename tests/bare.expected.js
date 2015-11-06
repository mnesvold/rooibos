(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_ping() {
            return;
        }
        return { f_ping: f_ping };
    };
    var asm = ASM(globals);
    var adaptors = { ping: asm.f_ping };
    globals.ASM = adaptors;
}(this));
