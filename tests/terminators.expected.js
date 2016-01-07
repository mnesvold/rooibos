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
        function f_one_thing() {
            return;
        }
        function f_another_thing() {
            return;
        }
        function f_choice(p_x) {
            p_x = p_x | 0;
            var PC = 0, l_0 = 0;
            while (1) {
                switch (PC | 0) {
                case 0:
                    l_0 = ((p_x | 0) != (0 | 0) ? 1 : 0) | 0;
                    PC = l_0 | 0 ? 1 : 2;
                    continue;
                case 1:
                    f_one_thing();
                    PC = 3 | 0;
                    continue;
                case 2:
                    f_another_thing();
                    PC = 3 | 0;
                    continue;
                case 3:
                    return;
                }
            }
        }
        return {
            f_loop_forever: f_loop_forever,
            f_one_thing: f_one_thing,
            f_another_thing: f_another_thing,
            f_choice: f_choice
        };
    };
    var asm = ASM(globals);
    var adaptors = {
        loop_forever: asm.f_loop_forever,
        one_thing: asm.f_one_thing,
        another_thing: asm.f_another_thing,
        choice: asm.f_choice
    };
    globals.ASM = adaptors;
}(this));
