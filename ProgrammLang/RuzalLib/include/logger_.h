#ifndef DED_LOGGER
#define DED_LOGGER
#include "stdio.h"

struct LogFile {
	FILE* file;
	const char* name;
	bool is_opened;
};

int LogFileCtor(const char* name, LogFile* logger);
// LoggerPrint()
// 
int LogFileDtor(LogFile* logger);


#endif // !DED_LOGGER
