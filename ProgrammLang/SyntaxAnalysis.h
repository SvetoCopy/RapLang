#include "RuzalLib/include/Tree.h"
#include "RuzalLib/include/TreeIO.h"
#include "assert.h"
#include "LexicalAnalysis.h"
#include "../NameTable.h"

const int MAX_LINE_NUM = 10000;

bool CheckTree(Tree* tree, NameTable* name_table);

Node* GetOperator(Node** token_array, int* counter, NameTable* name_table);

Node* GetWhile(Node** token_array, int* counter, NameTable* name_table);
Node* GetIf(Node** token_array, int* counter, NameTable* name_table);
Node* GetSimpleOperators(Node** token_array, int* token_counter, NameTable* name_table);

Node* GetAssign(Node** token_array, int* counter, NameTable* name_table);
Node* GetIdentifier(Node** token_array, int* counter);

Node* GetFuncCallOrVar(Node** token_array, int* token_counter, NameTable* name_table);

Node* GetPrimary(Node** token_array, int* counter, NameTable* name_table);
Node* GetPow(Node** token_array, int* token_counter, NameTable* name_table);
Node* GetLogicalOperations(Node** token_array, int* token_counter, NameTable* name_table);
Node* GetExpression(Node** token_array, int* counter, NameTable* name_table);
Node* GetTerm(Node** token_array, int* counter, NameTable* name_table);
Node* GetNum(Node** token_array, int* counter, NameTable* name_table);
Node* GetGeneral(Node** token_array, ProgrammNameTables* table);
