(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_loop_forever() {
            var PC = 0;
            while (1) {
                switch (PC | 0) {
                case 0:
                    PC = 1 | 0;
                    continue;
                case 1:
                    PC = 1 | 0;
                    continue;
                case 2:
                    return;
                }
            }
        }
        return { f_loop_forever: f_loop_forever };
    };
    var asm = ASM(globals);
    var adaptors = { loop_forever: asm.f_loop_forever };
    globals.ASM = adaptors;
}(this));
