(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_unconditional_branch() {
            var PC = 0;
            while (1) {
                switch (PC | 0) {
                case 0:
                    PC = 1 | 0;
                    continue;
                case 1:
                    return;
                }
            }
        }
        function f_conditional_branch(p_x) {
            p_x = p_x | 0;
            var PC = 0, l_0 = 0;
            while (1) {
                switch (PC | 0) {
                case 0:
                    l_0 = ((p_x | 0) != (0 | 0) ? 1 : 0) | 0;
                    PC = l_0 | 0 ? 1 : 2;
                    continue;
                case 1:
                    return 0 | 0;
                case 2:
                    return p_x | 0;
                }
            }
        }
        return {
            f_unconditional_branch: f_unconditional_branch,
            f_conditional_branch: f_conditional_branch
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        unconditional_branch: asm.f_unconditional_branch,
        conditional_branch: asm.f_conditional_branch
    };
    globals.ASM = adaptors;
}(this));
