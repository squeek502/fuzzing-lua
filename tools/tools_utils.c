#include "tools_utils.h"

size_t read_file(const char* path, uint8_t* buf, size_t bufsize)
{
	FILE* f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	if (size < 0) {
		fclose(f);
		perror("ftell failed when reading file");
		return 0;
	}
	rewind(f);
	if ((size_t)size > bufsize) {
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
