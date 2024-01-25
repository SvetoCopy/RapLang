#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "RuzalLib/include/Tree.h"
#include "../NameTable.h"

void RetranslateTreeToASM(Tree* tree, FILE* asm_code, NameTable name_table);

#endif 