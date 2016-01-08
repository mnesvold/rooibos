# Rooibos
> Highly experimental LLVM IR → ASM.JS compiler

Rooibos converts [LLVM](http://llvm.org/) bitcode (most commonly produced with `clang -c -emit-llvm ...`) into [ASM.JS](http://asmjs.org/) code¹. In theory, this means that any language that has an LLVM-compatible frontend (including C and dialects, thanks to Clang) can be used to write a web app frontend. (In practice, there's the small question of porting runtime libraries, but that's another matter.)

¹ Technically, Rooibos generates a [SpiderMonkey](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/Parser_API)-compatible JSON representation of the AST for ASM.JS-compliant code. You'll need another tool to actually generate the JavaScript; see below.

## Installation

Roobius has some dependencies that you'll need to wire up before you can compile it.

1. Make sure you have [Tup](http://gittup.org/tup/) and [npm](https://www.npmjs.com/) installed.
1. Clone this repository into some directory `$ROOT`.
2. `mkdir $ROOT/tools; cd $ROOT/tools`
3. Somewhere, probably out of the Rooibos directory, build LLVM using the instructions [here](http://llvm.org/docs/GettingStarted.html#getting-started-quickly-a-summary). Link these LLVM directories into your new `tools` directory so Rooibos can find them:
  1. `ln -s where-you-want-to-build-llvm/bin ./bin` — built binaries
  2. `ln -s where-you-want-to-build-llvm/lib ./lib` — built libraries
  3. `ln -s where-you-want-to-build-llvm/include ./include-build` — build-specific headers
  4. `ln -s where-you-want-llvm-to-live/include ./include-src` — build-independent headers
4. Clone [Niels Lohmann's JSON library](https://github.com/nlohmann/json): `cd $ROOT/tools; git clone https://github.com/nlohmann/json.git`
5. Install [escodegen](https://github.com/estools/escodegen) and [UglifyJS](https://github.com/mishoo/UglifyJS): `cd $ROOT/tools; npm install escodegen ugilfy-js`
6. Pick a variant: `tup variant configs/{default,release}.conf`
  * `default` includes debug symbols and produces some extra test output files (human-readable `.ll` and minified JS) to aid development
  * `release` runs optimization passes on Rooibos itself and skips the extra test output
6. Run Tup: `cd $ROOT; tup`

If all dependencies are in place, this will build Rooibos and run the integration tests by running Rooibos against the C files in `tests` and `diff`ing against their expected results. If Tup comes back green and `Updated`, you're good to go.

## Usage

Rooibos picks up from LLVM bitcode (`.bc` files). There's a working example in `tests/Tupfile`, but the gist of it is:

```bash
$ generate foo.bc
$ $BUILD/rooibos/rooibos-codegen < foo.bc > foo.ast
$ $TOOLS/node_modules/.bin/esgenerate foo.ast > foo.js
```

If you're starting from C(++) sources, the command you want is `clang -c -emit-llvm -o foo.bc foo.c`.

(For curiosity's sake, the Tupfile also minifies the resulting JavaScript into a separate `.min.js` file if you're building the `default` variant.)

## Using the results

The produced ASM.JS function is itself wrapped in a non-ASM.JS adaptor, which is responsible for setting up the heap, bridging native JavaScript and ASM.JS types, and so on. Currently, all functions defined in the IR are exposed through the adaptor which is attached to the global variable (`window` in browsers) under the name `ASM`. For instance, a source file

```c
int echo(int value)
{
  return value;
}
```

can be used like this:

```js
console.log(ASM.echo(42));
```

## Design goals

Rooibos is meant to be not overly verbose in its output, partially inspired by C++'s "don't pay for what you don't use" philosophy. If no source code references the heap, for instance, then the runtime won't contain the code to set up the heap in the first place. As another example, branch instructions are implemented by emulating a program counter with a local `PC` variable and a `switch` nested in a `while (1)` loop. Functions with only one basic block don't need to emulate the program counter, so they don't.

## Implementation status

* IR support
  * [X] Functions
  * [ ] Global variables
  * [X] Parameters
  * [X] Return values
  * [X] Calls to other ASM functions
  * [X] Calls to the ASM.JS `stdlib`
  * [ ] Calls to the ASM.JS foreign function interface
  * [X] Heap access
  * [ ] `asm` support
  * [ ] All other IR instructions (see `README.instructions.md`)
* Type support
  * [X] Integers
  * [ ] Floats
  * [X] Doubles
* Dependency system integrations
  * [X] Direct global assignment (`window.ASM = ...`)
  * [ ] CommonJS (`require([], function() { ... })`)
  * [ ] Browserify (`module.exports = ...`)
* JS integrations
  * [ ] Manipulating JS objects from ASM.JS code (via FFI)
  * [ ] Converting JS strings to `const char *`s and vice-versa
* Tool support
  * [ ] Rooibos driver
  * [ ] Source maps
  * [ ] C headers for browser environment
  * [ ] C++ runtime(?)
