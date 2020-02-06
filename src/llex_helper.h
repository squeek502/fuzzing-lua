#ifndef FUZZING_LUA_LLEX_HELPER_H
#define FUZZING_LUA_LLEX_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "ldo.h"

extern FILE* LLEX_HELPER_OUTPUT_FILE;

void llex_fuzz(const uint8_t *data, size_t size);
void llex_fuzz_custom(const uint8_t *data, size_t size, Pfunc func);

#ifdef __cplusplus
}
#endif

#endif
