#ifndef FUZZING_LUA_LLEX_HELPER_H
#define FUZZING_LUA_LLEX_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

extern FILE* LLEX_HELPER_OUTPUT_FILE;

void llex_fuzz(const uint8_t *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif
