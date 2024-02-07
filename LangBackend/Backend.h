#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "RuzalLib/include/Tree.h"
#include "../NameTable.h"

void RetranslateNodeToASM(Node* node, FILE* asm_code, NameTable name_table, ProgrammNameTables table);
void RetranslateTreeToASM(Tree* tree, FILE* asm_code, ProgrammNameTables table);

#endif 