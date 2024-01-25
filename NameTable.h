#ifndef NAME_TABLE_HPP
#define NAME_TABLE_HPP
#include "assert.h"
#include <stdlib.h>

const int INIT_TABLE_CAPACITY = 100;

enum NameTableElemType {
	KEYWORD  = 0,
	VARIABLE = 1,
	FUNCTION = 2
};

struct NameTableElem {
	const char*		  name;
	int				  code;
	int				  address;
	NameTableElemType type;
};

struct NameTable {
	NameTableElem* table;
	size_t		   size;
};

void NameTableCtor(NameTable* name_table);
void NameTableDtor(NameTable* name_table);

void NameTableInsertVar(NameTable* name_table, const char* name);
#endif // !