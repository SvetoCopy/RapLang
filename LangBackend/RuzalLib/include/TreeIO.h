#ifndef TREE_READ_HPP
#define TREE_READ_HPP

#include "Tree.h"
#include "../../../NameTable.h"
#include "super_io.h"

#define MAX_TREE_STR_SIZE 10000

const int	MAX_NAME_SIZE	   = 1000;
const int	OPEN_BRACKET_SIZE  = 1;
const int	MAX_NAMETABLE_SIZE = 10000;
const int   MAX_NODEINFO_SIZE  = 1000;
const int   INDENT_SIZE		   = 2;

constexpr const char* DEFAULT_NIL = ".";

#define QUOTE(value) LL_QUOTE(value)
#define LL_QUOTE(value) #value

enum ParseStatus {
	FOUND = 1,
	UNFOUND = 2
};

enum Order {
	ORDER_POST = 1,
	ORDER_IN = 2,
	ORDER_PRE = 3
};

enum NodeElem {
	LEFT_ELEM = 0,
	ROOT_ELEM = 1,
	RIGHT_ELEM = 2
};

int  ReadNodePRE(char* str, Node** res);
void SaveTree(Tree* tree, FILE* file, ProgrammNameTables* table);

void PrintOperator(Node* oper, FILE* file);

void ReadTree(FileInfo* file, ProgrammNameTables* table, Tree* tree);
#endif // !TREE_READ_HPP
