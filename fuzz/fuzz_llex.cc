#include "llex_helper.h"

#ifndef FUZZ_ENTRY
#define FUZZ_ENTRY LLVMFuzzerTestOneInput
#endif

extern "C" int FUZZ_ENTRY(const uint8_t *data, size_t size)
{
	llex_fuzz(data, size);
	return 0;
}
