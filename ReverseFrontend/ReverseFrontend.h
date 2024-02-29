#pragma once
#include "RuzalLib/include/Tree.h"

void RetranslateNodeToLang(Node* node, FILE* lang_file, int tab_count);
void RetranslateTreeToLang(Tree* tree, FILE* lang_file);