#include <stdio.h>

#include "llex_helper.h"

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

size_t read_file(const char* path, uint8_t* buf, size_t bufsize)
{
	FILE* f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);
	if (size > bufsize) {
		fclose(f);
		fprintf(stderr, "file '%s' too big (%ld)", path, size);
		return 0;
	}
	size_t bytesRead = fread(buf, 1, size, f);
	fclose(f);
	return bytesRead;
}

int mkdir_recursive(char *path, mode_t mode)
{
	char *last_sep = strrchr(path, '/');
	if (last_sep && last_sep != path)
	{
		// temporarily shorten the path
		*last_sep = 0;
		int result = mkdir_recursive(path, mode);
		*last_sep = '/';
		if (result != 0 && errno != EEXIST)
		{
			return result;
		}
	}
	return mkdir(path, mode);
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
	strncpy(pathbuf, output_dir, 512);
	int mkdir_result = mkdir_recursive(pathbuf, 0755);
	if (mkdir_result != 0 && errno != EEXIST) {
		sprintf(pathbuf, "Failed to create output dir \"%s\"", output_dir);
		perror(pathbuf);
		return 1;
	}
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (input_dir)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (strlen(ent->d_name) <= 2) continue;

			sprintf(pathbuf, "%s/%s", input_dir, ent->d_name);
			size_t size = read_file(pathbuf, buf, sizeof(buf));

			sprintf(pathbuf, "%s/%s", output_dir, ent->d_name);
			LLEX_HELPER_OUTPUT_FILE = fopen(pathbuf, "w");

    			llex_fuzz(buf, size);

			fclose(LLEX_HELPER_OUTPUT_FILE);
  		}
  		closedir (dir);
	} else {
		perror ("");
		return EXIT_FAILURE;
	}
	printf("Results written to: %s\n", output_dir);
	return 0;
}
