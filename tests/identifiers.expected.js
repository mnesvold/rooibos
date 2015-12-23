(function (globals) {
    var ASM = function (stdlib, ffi, heap) {
        'use asm';
        function f_identifiers() {
            f_lettersAndDigitsAreNotEnc0ded();
            f_neither_are_underscores();
            f_but_$$_is();
            f_as$2dare$2dhyphens();
            f_also$20spaces();
            f_and$2eperiods();
            f_and_things_like_$01();
            return;
        }
        return { f_identifiers: f_identifiers };
    };
    var asm = ASM(globals);
    var adaptors = { identifiers: asm.f_identifiers };
    globals.ASM = adaptors;
}(this));
