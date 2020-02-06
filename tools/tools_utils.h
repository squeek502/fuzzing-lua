#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>

size_t read_file(const char* path, uint8_t* buf, size_t bufsize);
int mkdir_recursive(char *path, mode_t mode);
