#ifndef SUPER_IO
#define SUPER_IO
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

struct FileInfo {
	FILE* input_file;
	const char* name;
	char** text;
	char* buff;
	size_t n_lines;
	size_t buff_size;
};

void PrintData(char* data[], int size);
void DebugPrint(char* data[], int left, int right, int size, int mid);

size_t GetLinesCount(FileInfo* file);
size_t GetFileSize(FileInfo* file);

void FileInfoDtor(FileInfo* file);
FileInfo FileInfoCtor(const char* file_name);

void ReadFile(FileInfo* file);
void ReadBuff(FileInfo* file);

#endif
