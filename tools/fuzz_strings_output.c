#include "llex_helper.h"
#include "tools_utils.h"

#include "lobject.h"
#include <lua.h>
#include <lauxlib.h>
#include <llex.h>
#include <lzio.h>
#include <lstring.h>
#include <lparser.h>
#include <lcode.h>
#include <ldo.h>
#include <lfunc.h>
#include <ltable.h>

FILE* OUTPUT_FILE;

static void protected_lex(lua_State *L, void *ud)
{
        (void)L; // fix warning
        LexState* lexstate = (LexState*)ud;
        luaX_next(lexstate);
        fwrite(getstr(lexstate->t.seminfo.ts), 1, lexstate->t.seminfo.ts->tsv.len, OUTPUT_FILE);
}

int main(int argc, const char* argv[])
{
	if (argc <= 2) {
		printf("usage: %s input_dir output_dir\n  (no trailing path separators)\n", argv[0]);
		return 0;
	}
	static uint8_t buf[1024 * 1024];
	static char pathbuf[512];
	const char* input_dir = argv[1];
	const char* output_dir = argv[2];

	strcpy(pathbuf, output_dir);
	int mkdir_result = mkdir_recursive(pathbuf, 0755);
        if (mkdir_result != 0 && errno != EEXIST) {
                sprintf(pathbuf, "Failed to create output dir \"%s\"", output_dir);
                perror(pathbuf);
                return 1;
        }

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (input_dir)) == NULL) {
		sprintf(pathbuf, "Failed to open input dir \"%s\"", input_dir);
		perror(pathbuf);
		return 1;
	}
        printf("Reading inputs from: %s\n", input_dir);
	while ((ent = readdir (dir)) != NULL) {
		if (ent->d_type == DT_DIR) continue;

		sprintf(pathbuf, "%s/%s", input_dir, ent->d_name);
		size_t size = read_file(pathbuf, buf, sizeof(buf));

		sprintf(pathbuf, "%s/%s", output_dir, ent->d_name);
		OUTPUT_FILE = fopen(pathbuf, "wb");

		llex_fuzz_custom(buf, size, protected_lex);

		fclose(OUTPUT_FILE);
  	}
  	closedir (dir);
	printf("Results written to: %s\n", output_dir);
	return 0;
}
