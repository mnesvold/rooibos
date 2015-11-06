# Rooibos
> Highly experimental LLVM IR → ASM.JS compiler

Rooibus converts [LLVM](http://llvm.org/) bitcode (most commonly produced with `clang -c -emit-llvm ...`) into [ASM.JS](http://asmjs.org/) code*. In theory, this means that any language that has an LLVM-compatible frontend (including C and dialects, thanks to Clang) can be used to write a web app frontend. (In practice, there's the small question of porting runtime libraries, but that's another matter.)

*Technically, Rooibus generates a [SpiderMonkey](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/Parser_API)-compatible JSON representation of the AST for ASM.JS-compliant code. You'll need another tool to actually generate the JavaScript; see below.

## Installation

Roobius has some dependencies that you'll need to wire up before you can compile it.

1. Make sure you have [Tup](http://gittup.org/tup/) and [npm](https://www.npmjs.com/) installed.
1. Clone this repository into some directory `$ROOT`.
2. `mkdir $ROOT/tools; cd $ROOT/tools`
3. Somewhere, probably out of the Rooibus directory, build LLVM using the instructions [here](http://llvm.org/docs/GettingStarted.html#getting-started-quickly-a-summary). Link these LLVM directories into your new `tools` directory so Rooibus can find them:
  1. `ln -s where-you-want-to-build-llvm/bin ./bin` — built binaries
  2. `ln -s where-you-want-to-build-llvm/lib ./lib` — built libraries
  3. `ln -s where-you-want-to-build-llvm/include ./include-build` — build-specific headers
  4. `ln -s where-you-want-llvm-to-live/include ./include-src` — build-independent headers
4. Clone [Niels Lohmann's JSON library](https://github.com/nlohmann/json): `cd $ROOT/tools; git clone https://github.com/nlohmann/json.git`
5. Install [escodegen](https://github.com/estools/escodegen) and [UglifyJS](https://github.com/mishoo/UglifyJS): `cd $ROOT/tools; npm install escodegen ugilfy-js`
6. Pick a variant: `tup variant configs/{default,release}.conf` (Use `default` to include debug symbols; use `release` to optimize Rooibus.)
6. Run Tup: `cd $ROOT; tup`

If all dependencies are in place, this will build Rooibus and run the integration tests by running Rooibus against the C files in `tests` and `diff`ing against their expected results. If Tup comes back green and "`Updated`", you're good to go.

## Usage

There's a working example in `tests/Tupfile`, but the gist of it is:

```bash
$ edit foo.c
$ clang -c -emit-llvm -o foo.bc foo.c
$ $BUILD/rooibus/rooibus-codegen < foo.bc > foo.ast
$ $TOOLS/node_modules/.bin/esgenerate foo.ast > foo.js
```

(For debugging's and curiosity's sakes, the Tupfile also generates human-readable `.ll` files for each test and minifies the resulting JavaScript into a separate `.min.js` file.)

## Using the results

The produced ASM.JS function is itself wrapped in a non-ASM.JS adaptor, which is responsible for setting up the heap, bridging native JavaScript and ASM.JS types, and so on. Currently, all functions defined in the IR are exposed through the addaptor which is attached to the global variable (`window` in browsers) under the name `ASM`. For instance, a source file

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

Rooibus is meant to be not overly verbose in its output, partially inspired by C++'s "don't pay for what you don't use" philosophy. If no source code references the heap, for instance, then the runtime won't contain the code to set up the heap in the first place.

## Implementation status

* IR support
  * [X] Functions
  * [ ] Global variables
  * [X] Parameters
  * [X] Return values
  * [X] Calls to other ASM functions
  * [ ] Calls to the ASM.JS `stdlib`
  * [ ] Calls to the ASM.JS foreign function interface
  * [ ] Heap access
  * [ ] `asm` support
  * [ ] All other IR instructions
* Type support
  * [X] Integers
  * [ ] Floats
  * [ ] Doubles
* Dependency system integrations
  * [X] Direct global assignment (`window.ASM = ...`)
  * [ ] CommonJS (`require([], function() { ... })`)
  * [ ] Browserify (`module.exports = ...`)
* JS integrations
  * [ ] Manipulating JS objects from ASM.JS code (via FFI)
  * [ ] Converting JS strings to `const char *`s and vice-versa
* Tool support
  * [ ] Rooibus driver
  * [ ] Source maps
  * [ ] C headers for browser environment
  * [ ] C++ runtime(?)
