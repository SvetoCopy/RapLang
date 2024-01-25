#include "NameTable.h"

void NameTableInsertKeyWord(NameTable* name_table, const char* name, int code) {

	assert(name != nullptr);
	assert(name_table != nullptr);

	NameTableElem elem = {};
	elem.code = code;
	elem.name = name;
	elem.type = KEYWORD;

	name_table->table[name_table->size] = elem;
	name_table->size++;
}

void NameTableInsertVar(NameTable* name_table, const char* name) {

	assert(name != nullptr);
	assert(name_table != nullptr);

	NameTableElem elem = {};
	elem.code = name_table->size;
	elem.name = name;
	elem.type = VARIABLE;

	name_table->table[name_table->size] = elem;

	name_table->size++;
}

void NameTableCtor(NameTable* name_table) {

	name_table->table = (NameTableElem*)calloc(INIT_TABLE_CAPACITY, sizeof(NameTableElem));

	NameTableElem elem = {};
}

void NameTableDtor(NameTable* name_table) {

	free(name_table->table);
	name_table->size = 0;
}