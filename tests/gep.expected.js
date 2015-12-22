(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var stdlib_Math__imul = stdlib.Math.imul;
        function f_singleGEP(p_array, p_index) {
            p_array = p_array | 0;
            p_index = p_index | 0;
            var l_0 = 0;
            l_0 = (p_array | 0) + stdlib_Math__imul(4, p_index | 0) | 0;
            return l_0 | 0;
        }
        function f_doubleGEP(p_foo) {
            p_foo = p_foo | 0;
            var l_0 = 0;
            l_0 = (p_foo | 0) + stdlib_Math__imul(16, 0 | 0) + 12 | 0;
            return l_0 | 0;
        }
        function f_unusualGEPSize(p_foo) {
            p_foo = p_foo | 0;
            var l_0 = 0;
            l_0 = (p_foo | 0) + stdlib_Math__imul(16, 1 | 0) | 0;
            return l_0 | 0;
        }
        return {
            f_singleGEP: f_singleGEP,
            f_doubleGEP: f_doubleGEP,
            f_unusualGEPSize: f_unusualGEPSize
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        singleGEP: asm.f_singleGEP,
        doubleGEP: asm.f_doubleGEP,
        unusualGEPSize: asm.f_unusualGEPSize
    };
    globals.ASM = adaptors;
}(this));
