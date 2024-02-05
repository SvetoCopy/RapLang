#define _CRT_SECURE_NO_WARNINGS
#include "../include/Tree.h"

static int CommandPriority(int cmd_code) {

	switch (cmd_code) {
		#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, ...)  \
			case cmd_code:                                                \
				return priority;        
		#include "../include/def_expr_cmd.h"
		#undef DEF_EXPR_CMD
	}
}

void PrintCommand(int cmd_code, FILE* stream) {

	switch (cmd_code) {
		#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, ...)  \
			case cmd_code: {                                              \
				fprintf(stream, " " #command " ");                        \
				break;                                                    \
			}

		#include "../include/def_expr_cmd.h"

		#undef DEF_EXPR_CMD
	}
}

Node* OpNew(NodeData_t data) {

	Node* node = (Node*)calloc(1, sizeof(Node));
	node->data = data;

	return node;
}

void OpDelete(Node* node) {

	assert(node != nullptr);

	free(node);
}

void NodeDtor(Node* node) {

	if (node == nullptr)
		return;

	NodeDtor(node->left);
	NodeDtor(node->right);

	OpDelete(node);
	
	node->left = nullptr;
	node->right = nullptr;
}

void TreeCtor(Tree* tree, const char* graph_logfile_name, 
			 NodeData_t start_data) {

	tree->root = OpNew(start_data);
	tree->graph_logfile = fopen(graph_logfile_name, "w");
	tree->size = 0;

	TreeVerify(tree);
}

void TreeDtor(Tree* tree) {

	TreeVerify(tree);

	NodeDtor(tree->root);

	fclose(tree->graph_logfile);

	fprintf(tree->latex_logfile, "\\end{document}");
	fclose(tree->latex_logfile);
}

static int GetCommandArgsNum(const Node* node) {

	assert(node != nullptr);

	#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, args_num, ...)  \
				if (node->data.value.operator_type == cmd_code) {                \
					return args_num;                                            \
				}
	#include "../include/def_expr_cmd.h"
	#undef DEF_EXPR_CMD
}

int VerifyNode(const Node* node) {
	
	if (node == nullptr) return 0;

	bool error = false;

	if (node->left == nullptr && node->right == nullptr) {
		if (node->data.type != NUM && node->data.type != VAR){
			fprintf(stderr, "Leaf is not NUM\n");
			error = true;
		}
	}

	if (node->data.type == COMMAND) {
		int args_num = GetCommandArgsNum(node);

		if (args_num == 1 && (node->right != nullptr || node->left == nullptr)) {
			fprintf(stderr, "Command with one arg have 0 or 2 arg\n");
			error = true;
		}

		if (args_num == 2 && (node->right == nullptr || node->left == nullptr)) {
			fprintf(stderr, "Command with two arg have 0 or 1 arg\n");
			error = true;
		}
			
	}

	VerifyNode(node->left);
	VerifyNode(node->right);

	return error ? NODE_ERROR : 0;
}

int TreeVerify(Tree* tree) {
		
	if (VerifyNode(tree->root) == NODE_ERROR)
		return TREE_ERROR;
}

Node* CopyNode(const Node* node) {

	if (node == nullptr) return nullptr;

	Node* res = OpNew(node->data);

	res->left  = CopyNode(node->left);
	res->right = CopyNode(node->right);

	return res;
}

int ExprElemCtor(ExprElem* expr_elem, ExprElemType type, Value_t data) {

	assert(expr_elem);

	expr_elem->value = data;
	expr_elem->type = type;

	return 0;
}

ExprElem CreateExprCommand(int cmd_code) {

	ExprElem expr_elem = {};

	Value_t value = {};
	value.operator_type = cmd_code;

	ExprElemCtor(&expr_elem, COMMAND, value);

	return expr_elem;
}

ExprElem CreateExprOperator(int cmd_code) {

	ExprElem expr_elem = {};

	Value_t value = {};
	value.operator_type = cmd_code;

	ExprElemCtor(&expr_elem, OPERATOR, value);

	return expr_elem;
}

ExprElem CreateExprImm(double double_elem) {

	ExprElem data = {};

	Value_t value = {};
	value.imm_value = double_elem;

	ExprElemCtor(&data, NUM, value);

	return data;
}

ExprElem CreateExprVar(char* name) {

	assert(name != nullptr);

	ExprElem data = {};

	Value_t value = {};

	value.var.name = _strdup(name);
	value.var.imm_value = 0;

	data.value = value;
	ExprElemCtor(&data, VAR, value);

	return data;
}

ExprElem CreateExprFunc(char* name) {

	assert(name != nullptr);

	ExprElem data = {};

	Value_t value = {};

	value.var.name = _strdup(name);
	value.var.imm_value = 0;

	data.value = value;
	ExprElemCtor(&data, FUNCTION, value);

	return data;
}

Node* CreateOperatorNode(int command_code, Node* left, Node* right) {

	Node* res = OpNew(CreateExprOperator(command_code));

	res->left  = left;
	res->right = right;

	return res;
}

Node* CreateImmNode(double imm_value, Node* left, Node* right) {

	Node* res = OpNew(CreateExprImm(imm_value));

	res->left = left;
	res->right = right;

	return res;
}

Node* CreateVarNode(ExprVar var, Node* left, Node* right) {

	ExprElem data = {};

	Value_t value = {};
	value.var = var;

	data.value = value;
	ExprElemCtor(&data, VAR, value);

	Node* res = OpNew(data);

	res->left = left;
	res->right = right;

	return res;
}

static bool isNeedBrackets(Node* child, Node* parent) {

	assert(child != nullptr);

	if (parent == nullptr ||
		CommandPriority(NODE_CMD_CODE(child)) <= CommandPriority(NODE_CMD_CODE(parent))) {

		return false;

	}

	return true;
}

static int GetCommandArgsNum(Node* node) {

	assert(node != nullptr);

	#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, args_num, ...)		\
					if (node->data.value.operator_type == cmd_code) {                \
						return args_num;                                            \
					}

	#include "../include/def_expr_cmd.h"

	#undef DEF_EXPR_CMD
}

void PrintNodeExpr(Node* child, Node* parent, FILE* stream) {

	assert(child != nullptr);

	if (child->data.type == VAR) {
		fprintf(stream, "%s", NODE_VAR_NAME(child));
		return;
	}

	if (child->data.type == NUM) {
		fprintf(stream, "%.2lf", NODE_IMM_VALUE(child));
		return;
	}

	if (child->data.type == COMMAND) {
		int args_num = GetCommandArgsNum(child);
		if (args_num == 1) {
			PrintCommand(NODE_CMD_CODE(child), stream);

			fprintf(stream, " ( ");
			PrintNodeExpr(child->left, child, stream);
			fprintf(stream, " ) ");

			return;
		}

		bool is_need_brackets = isNeedBrackets(child, parent);

		if (is_need_brackets)
			fprintf(stream, " ( ");

		PrintNodeExpr(child->left, child, stream);
		PrintCommand(NODE_CMD_CODE(child), stream);
		PrintNodeExpr(child->right, child, stream);

		if (is_need_brackets)
			fprintf(stream, " ) ");
	}
}

void PrintTreeExpr(Tree* tree, FILE* stream) {

	TreeVerify(tree);

	PrintNodeExpr(tree->root, nullptr, stream);
}

bool isEqualVar(ExprVar var1, ExprVar var2) {

	return strcmp(var1.name, var2.name) == 0;
}

bool ÑheckVarInSubTree(Node* node, ExprVar var) {

	VerifyNode(node);

	if (node == nullptr) return false;
	if (node->data.type == VAR) return isEqualVar(node->data.value.var, var);

	if (ÑheckVarInSubTree(node->left, var)) return true;
	if (ÑheckVarInSubTree(node->right, var)) return true;

	return false;
}