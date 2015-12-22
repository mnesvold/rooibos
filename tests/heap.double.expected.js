(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var HEAP64F = new stdlib.Float64Array(heap);
        var SP = 65536;
        function f_load(p_pointer) {
            p_pointer = p_pointer | 0;
            var l_0 = 0.1;
            l_0 = +HEAP64F[(p_pointer | 0) >> 3];
            return +l_0;
        }
        function f_store(p_pointer, p_value) {
            p_pointer = p_pointer | 0;
            p_value = +p_value;
            HEAP64F[(p_pointer | 0) >> 3] = +p_value;
            return;
        }
        function f_stack_allocation() {
            var FP = 0, l_0 = 0;
            FP = SP | 0;
            SP = (SP | 0) - 4 | 0;
            l_0 = SP | 0;
            f_store(l_0 | 0, +3);
            SP = FP | 0;
            return;
        }
        return {
            f_load: f_load,
            f_store: f_store,
            f_stack_allocation: f_stack_allocation
        };
    };
    var ffi = {};
    var heap = new globals.ArrayBuffer(65536);
    var asm = ASM(globals, ffi, heap);
    var adaptors = {
        load: asm.f_load,
        store: asm.f_store,
        stack_allocation: asm.f_stack_allocation
    };
    globals.ASM = adaptors;
}(this));
