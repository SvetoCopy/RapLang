#include "RuzalLib/include/Tree.h"
#include "RuzalLib/include/TreeIO.h"
#include "assert.h"
#include "LexicalAnalysis.h"
#include "../NameTable.h"

bool CheckTree(Tree* tree, NameTable* name_table);

Node* GetOperator(Node** token_array, int* counter, NameTable* name_table);

Node* GetWhile(Node** token_array, int* counter, NameTable* name_table);
Node* GetIf(Node** token_array, int* counter, NameTable* name_table);

Node* GetAssign(Node** token_array, int* counter, NameTable* name_table);
Node* GetIdentifier(Node** token_array, int* counter, NameTable* name_table);

Node* GetPrimary(Node** token_array, int* counter, NameTable* name_table);
Node* GetExpression(Node** token_array, int* counter, NameTable* name_table);
Node* GetTerm(Node** token_array, int* counter, NameTable* name_table);
Node* GetNum(Node** token_array, int* counter, NameTable* name_table);
Node* GetGeneral(Node** token_array, NameTable* name_table);