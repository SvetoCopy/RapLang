#include "NameTable.h"

void NameTableInsert(NameTable* name_table, const char* name, NameTableElemType type) {

	assert(name != nullptr);
	assert(name_table != nullptr);

	NameTableElem elem = {};
	elem.code = name_table->size;
	elem.name = name;
	elem.type = type;

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

void ProgrammNameTablesCtor(ProgrammNameTables* table) {

	table->local_tables = (NameTable*)calloc(INIT_TABLE_CAPACITY, sizeof(ProgrammNameTables));
	
	for (size_t i = 0; i < INIT_TABLE_CAPACITY; i++) {
		NameTableCtor(&(table->local_tables[i]));
	}

	NameTableCtor(&(table->funcs));
}

void ProgrammNameTablesDtor(ProgrammNameTables* table) {

	NameTableDtor(&(table->funcs));
	free(table->local_tables);
}
