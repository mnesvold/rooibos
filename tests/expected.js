(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function _ping() {
            return;
        }
        function _reping() {
            _ping();
            _ping();
            return;
        }
        return {
            _ping: _ping,
            _reping: _reping
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        ping: asm._ping,
        reping: asm._reping
    };
    globals.ASM = adaptors;
}(this));
