(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_get_int() {
            return 42 | 0;
        }
        function f_return_int() {
            var l_0 = 0;
            l_0 = f_get_int() | 0;
            return l_0 | 0;
        }
        return {
            f_get_int: f_get_int,
            f_return_int: f_return_int
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        get_int: asm.f_get_int,
        return_int: asm.f_return_int
    };
    globals.ASM = adaptors;
}(this));
