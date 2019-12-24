Fuzzing Lua
===========

Fuzz testing for various parts of the Lua interpreter, mostly for use as a test-case generator for alternate Lua implementations (e.g. [Zua](https://github.com/squeek502/zua)).

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

The lexer fuzzer has an accompanying tool to output the tokens generated when lexing each file in the corpus. This set of result files can then be used to compare alternate Lua lexer implementations to ensure that they generate the exact same set of tokens.

The format of the output file is a space-separate list of tokens, printed via `luaX_token2str`. For example, if the input file was:

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

To generate the output files, first `cd build.tools`, then from within that directory run:

```
make fuzz_llex_output_run
```

The files will be in `build.tools/output/fuzz_llex/` and the file names will correspond with the file names of the corpus input files.

## Coverage

This is currently untested. **TODO**: Adapt [this guide](https://github.com/squeek502/d2itemreader/tree/master/tests/fuzz#checking-coverage-of-a-fuzzer).
