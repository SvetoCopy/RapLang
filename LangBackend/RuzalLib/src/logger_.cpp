#include "../include/logger_.h"

int LogFileCtor(const char* name, LogFile* logger) {
	logger->name = name;
	fopen_s(&(logger->file), logger->name, "w");
	logger->is_opened = true;
	return 0;
}

int LogFileDtor(LogFile* logger) {
	fclose(logger->file);
	logger->file = nullptr;
	logger->name = nullptr;
	return 0;
}
