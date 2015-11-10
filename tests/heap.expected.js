(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        var HEAP32 = new stdlib.Int32Array(heap);
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
        return {
            f_load: f_load,
            f_store: f_store
        };
    };
    var ffi = {};
    var heap = new globals.ArrayBuffer(65536);
    var asm = ASM(globals, ffi, heap);
    var adaptors = {
        load: asm.f_load,
        store: asm.f_store
    };
    globals.ASM = adaptors;
}(this));
