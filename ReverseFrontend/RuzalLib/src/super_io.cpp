#include "../include/super_io.h"

FileInfo FileInfoCtor(const char* file_name) {

	FileInfo file = {};
	file.name = file_name;

	fopen_s(&file.input_file, file.name, "r");
	
	assert(file.input_file != NULL);

	file.buff_size = GetFileSize(&file);
	file.buff	   = (char*)calloc(file.buff_size + 1, sizeof(char));

	file.n_lines = GetLinesCount(&file);
	file.text = (char**)(calloc(GetFileSize(&file) + 1, sizeof(char*)));

	return file;
}

void FileInfoDtor(FileInfo* file) {
	free(file->text);
	//free(file->buff);


	file->buff_size = 0;
	file->n_lines = 0;
	file->buff = NULL;
}

void ReadBuff(FileInfo* file) {
	size_t sym_count = 0;
	size_t str_count = 0;

	assert(file != NULL);

	file->text[str_count] = file->buff;
	str_count++;

	while (sym_count < file->buff_size) {
		if (file->buff[sym_count] == '\n') {
			file->text[str_count] = file->buff + sym_count + 1;

			str_count++;
			file->buff[sym_count] = '\0';
			
		}
		//printf("%c", file->buff[sym_count]);
		sym_count++;
	}
}

void PrintData(char* data[], int size) {
	printf("\n");

	for (int i = 0; i < size; i++) {
		printf("<%s>\n", data[i]);
	}
	

}

void DebugPrint(char* data[], int left, int right, int size, int mid) {
	int flag = 0;
	for (int i = 0; i < (size); i++) {
		if (i == left) {
			printf("<");
		}
		if (i == (right + 1)) {
			printf(">");
			flag++;
		}

		printf(" %s ", data[i]);
	}
	if (!flag) printf(">");
	printf("left = %d right = %d mid = %d, data[mid] = %s", left, right, mid, data[mid]);
	printf("\n");
}


size_t GetFileSize(FileInfo* file) {

	assert(file != NULL);

	fseek(file->input_file, 0, SEEK_END);
	size_t file_size = ftell(file->input_file);
	fseek(file->input_file, 0, SEEK_SET);

	return file_size;
}

size_t GetLinesCount(FileInfo* file) {
	size_t count = 1;
	size_t i = 0;

	assert(file != NULL);
	
	while (i < file->buff_size) {
		if (file->buff[i] == '\n') {
			count++;
		}
		i++;
	}
	return count;
}

void ReadFile(FileInfo* file) {

	fopen_s(&file->input_file, file->name, "r");

	if (file->input_file == NULL)
		file->name;

	assert(file->input_file != NULL);
	assert(file->buff != NULL);

	size_t size = fread(file->buff, sizeof(char), file->buff_size, file->input_file);
	file->buff[size] = '\0';
}