Fuzzing Lua
===========

Fuzz testing for various parts of the Lua interpreter, mostly for use as a test-case generator for alternate Lua implementations (e.g. [Zua](https://github.com/squeek502/zua)). Uses [libFuzzer](https://llvm.org/docs/LibFuzzer.html) via the Clang compiler. A pre-generated corpus and corresponding outputs can be found on the [releases page](https://github.com/squeek502/fuzzing-lua/releases) for quick use as a data set for testing other Lua implementations.

For a writeup of how this was used to test [Zua](https://github.com/squeek502/zua)'s lexer implementation, see:
- [Fuzzing As A Test Case Generator](https://www.ryanliptak.com/blog/fuzzing-as-test-case-generator/)

Currently supports:
- Lua 5.1.5
  + Lexer (llex.c)

## Setup

- Make sure you have [CMake](https://cmake.org), Clang, and Clang++ installed.
- Run `./setup.sh` to generate `build.fuzz`, `build.cov`, and `build.tools` directories via CMake.

## Fuzzing

First, `cd build.fuzz`, then, from within that directory:

- To run the fuzzer: `make fuzz_llex_run`
- To minimize the fuzzer's corpus: `make fuzz_llex_minimize`

## Tools

### fuzz_llex_output

The lexer fuzzer has an accompanying tool to output the tokens generated when lexing each file in the corpus. This set of result files can then be used to test alternate Lua lexer implementations to ensure that they generate the exact same set of tokens.

The format of the output file is a space-separated list of tokens, printed via `luaX_token2str`. For example, if the input file was:

```lua
local hello = 'world'
```

then the output file would be:

```
local <name> = <string> <eof>
```

If there is an error while lexing, then the error appears on the second line of the file. For example, if the last `'` in the `'world'` string in the previous example were omitted, then the output would look like:

```
local <name> =
[string "fuzz"]:3: unfinished string near '<eof>'
```

#### Usage

First, run the fuzzer (see Fuzzing above) to create a comprehensive corpus. Then, to generate the output files for each file in the corpus:

```
cd build.tools
make fuzz_llex_output_run
```

The files will be in `build.tools/tools/output/fuzz_llex/` and the file names will correspond with the file names of the corpus input files.

## Coverage

This is currently untested. **TODO**: Adapt [this guide](https://github.com/squeek502/d2itemreader/tree/master/tests/fuzz#checking-coverage-of-a-fuzzer).
