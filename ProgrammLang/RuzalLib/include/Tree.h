#define _CRT_SECURE_NO_WARNINGS
#ifndef TREE_DED
#define TREE_DED

#include "stdio.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include "../../../TXLib.h"

const int TREE_ERROR = -1;
const int NODE_ERROR = -1;

#define GET_OPERATOR_TYPE(node)  node->data.value.operator_type
#define GET_NODE_TYPE(node)      node->data.type
#define GET_NODE_IMM_VALUE(node) node->data.value.imm_value

#define NODE_IMM_VALUE(node) node->data.value.imm_value
#define NODE_CMD_CODE(node)  node->data.value.operator_type

#define NODE_VAR_VALUE(node) node->data.value.var.imm_value
#define NODE_VAR_NAME(node)  node->data.value.var.name

#define _IMM(num)		CreateImmNode(num, nullptr, nullptr)

enum ExprOperators {

	#define DEF_OPERATOR(cmd_name, command, code, ...) cmd_name = code,

	#include "../../def_operator.h"

	#undef DEF_OPERATOR

};

enum ExprElemType {
	NUM		 = 0,
	COMMAND  = 1,
	VAR		 = 2,
	OPERATOR = 3,
	FUNCTION = 4,
	DELETED  = -1
};

struct Func_t {
	const char* name;
	size_t      args_count;
};

struct ExprVar {
	const char* name;
	double      imm_value;
};

union Value_t {
	ExprVar var;
	int     operator_type;
	double  imm_value;
	Func_t  func;
};

struct ExprElem {
	ExprElemType type;
	Value_t      value;
};

typedef ExprElem NodeData_t;

struct Node {
	size_t     line_num;
	NodeData_t data;
	Node*      left;
	Node*      right;
};

struct Tree {
	Node*  root;
	size_t size;
	FILE*  graph_logfile;
	FILE*  latex_logfile;
};

Node* OpNew(NodeData_t data);
void  OpDelete(Node* node);

Node* CopyNode(const Node* node);
void  NodeDtor(Node* node);

void TreeCtor(Tree* tree, const char* graph_logfile_name, NodeData_t start_data);
void TreeDtor(Tree* tree);
int  TreeVerify(Tree* tree);

int  VerifyNode(const Node* node);

bool ÑheckVarInSubTree(Node* node, ExprVar var);

Node* CreateOperatorNode(int command_code, Node* left, Node* right);
Node* CreateImmNode(double imm_value, Node* left, Node* right);
Node* CreateVarNode(ExprVar var, Node* left, Node* right);

ExprElem CreateExprCommand(int cmd_code);
ExprElem CreateExprOperator(int cmd_code);
ExprElem CreateExprImm(double double_elem);
ExprElem CreateExprVar(char* name);

void PrintTreeExpr(Tree* tree, FILE* stream);
bool isEqualVar(ExprVar var1, ExprVar var2);
#endif // !TREE_DED
