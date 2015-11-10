(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var HEAP32 = new stdlib.Int32Array(heap);
        var SP = 65536;
        function f_load(p_pointer) {
            p_pointer = p_pointer | 0;
            var l_0 = 0;
            l_0 = HEAP32[(p_pointer | 0) >> 2] | 0;
            return l_0 | 0;
        }
        function f_store(p_pointer, p_value) {
            p_pointer = p_pointer | 0;
            p_value = p_value | 0;
            HEAP32[(p_pointer | 0) >> 2] = p_value | 0;
            return;
        }
        function f_stack_allocation() {
            var FP = 0, l_0 = 0;
            FP = SP | 0;
            SP = (SP | 0) - 4 | 0;
            l_0 = SP | 0;
            f_store(l_0 | 0, 3 | 0);
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
