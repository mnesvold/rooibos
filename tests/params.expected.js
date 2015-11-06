(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function _echo(_value) {
            _value = _value | 0;
            return _value | 0;
        }
        return { _echo: _echo };
    };
    var asm = ASM(globals);
    var adaptors = { echo: asm._echo };
    globals.ASM = adaptors;
}(this));
