#ifndef TREE_READ_HPP
#define TREE_READ_HPP

#include "Tree.h"
#include "../../LexicalAnalysis.h"
#include "../../../NameTable.h"

const int   MAX_BD_SIZE		   = 1000;
const int   DEFAULT_TRACK_SIZE = 10;
const int   MAX_NODEINFO_SIZE  = 100;
const int   INDENT_SIZE		   = 2;

constexpr const char* DEFAULT_NIL = ".";

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

void PrintOperatorWithQuotes(Node* oper, FILE* file);
void PrintOperator(Node* oper, FILE* file);
#endif // !TREE_READ_HPP
