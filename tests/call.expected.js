(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_ping() {
            return;
        }
        function f_reping() {
            f_ping();
            f_ping();
            return;
        }
        return {
            f_ping: f_ping,
            f_reping: f_reping
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        ping: asm.f_ping,
        reping: asm.f_reping
    };
    globals.ASM = adaptors;
}(this));
