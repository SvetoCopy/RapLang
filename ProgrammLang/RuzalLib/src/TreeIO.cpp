#include "../include/TreeIO.h"

int ReadExprElem(char* str, Node** node) {

	assert(str != nullptr);

	double double_elem = 0;
	int    elem_read_size = 0;
	int    read_arg_count = sscanf(str, " %lf %n", &double_elem, &elem_read_size);

	if (read_arg_count == 0) {

		elem_read_size = 0;
		char str_elem[MAX_NODEINFO_SIZE] = {};
		read_arg_count = sscanf(str, " %s %n", &str_elem, &elem_read_size);

		if (strcmp(str_elem, DEFAULT_NIL) == 0) {
			*node = nullptr;
		}

	#define DEF_OPERATOR(cmd_name, str_command, cmd_code, ...) \
															   \
        else if (strcmp(str_elem,  str_command) == 0){         \
			ExprElem data = CreateExprCommand(cmd_code);       \
			*node = OpNew(data);                               \
		}

	#include "../../def_operator.h"
	#undef DEF_OPERATOR
		else {
			ExprElem data = CreateExprVar(str_elem);
			*node = OpNew(data);
		}
	}
	else {
		ExprElem data = CreateExprImm(double_elem);
		*node = OpNew(data);
	}

	return elem_read_size;
}

static int DeleteCloseBracket(char* str) {

	assert(str != nullptr);

	char symbol = {};
	int end_read_size = 0;

	sscanf(str, " %c %n", &symbol, &end_read_size);

	if (symbol == ')') {
		return end_read_size;
	}

	return 0;
}

#define MoveStr(num) \
	res_size += num; \
	str      += num;   

#define ReadChild(child_name)                                                \
	symbol = {};                                                             \
	open_bracket_size = 0;                                                   \
	sscanf(str, " %c %n", &symbol, &open_bracket_size);                      \
																			 \
	Node* child_name = {};                                                   \
	int child_name ## _size = 0;                                             \
																		     \
	if (symbol == '(') child_name ## _size = ReadNodePRE(str, &child_name);  \
	else               child_name ## _size = ReadExprElem(str, &child_name); \
		                                                                     \
	MoveStr(child_name ## _size);                                            \
		                                                                     \
	close_bracket_size = DeleteCloseBracket(str);                            \
	MoveStr(close_bracket_size);                 

int ReadNodePRE(char* str, Node** res) {

	assert(str != nullptr);

	int res_size = 0;

	char symbol = {};
	int open_bracket_size = 0;
	sscanf(str, " %c %n", &symbol, &open_bracket_size);

	if (symbol == '(') {
		MoveStr(open_bracket_size);
	}

	int root_size = ReadExprElem(str, res);
	MoveStr(root_size);
	
	int close_bracket_size = 0;
	ReadChild(left);

	ReadChild(right);

	(*res)->left = left;
	(*res)->right = right;

	return res_size;
}

void PrintOperator(Node* oper, FILE* file) {

	assert(file != nullptr);

	switch (NODE_CMD_CODE(oper)) {
		#define DEF_OPERATOR(name, command, code, ...)									\
		case code:																		\
			fprintf(file, command " line = %zu type = %d", oper->line_num, OPERATOR);	\
			break;
		#include "../../def_operator.h"
		#undef DEF_OPERATOR
	}
}

void SaveNode(Node* node, FILE* file, int rec_level) {
	
	assert(file != nullptr);

	fprintf(file, "%*s", rec_level * INDENT_SIZE, "");

	if (node == nullptr) {
		fprintf(file, " %s\n", DEFAULT_NIL);
		
		return;
	}
	
	fprintf(file, "(");

	if (GET_NODE_TYPE(node) == VAR) {
		fprintf(file, "%s line = %zu type = %d", NODE_VAR_NAME(node), node->line_num, VAR);
	}

	else if (GET_NODE_TYPE(node) == OPERATOR) {
		PrintOperator(node, file);
	}

	else if (GET_NODE_TYPE(node) == NUM) {
		fprintf(file, "%lf line = %zu type = %d", NODE_IMM_VALUE(node), node->line_num, NUM);
	}

	else if (GET_NODE_TYPE(node) == FUNCTION) {
		fprintf(file, "%s line = %zu type = %d", NODE_VAR_NAME(node), node->line_num, FUNCTION);
	}

	fprintf(file, "\n");

	SaveNode(node->left, file, rec_level + 1);
	SaveNode(node->right, file, rec_level + 1);

	fprintf(file, "%*s", rec_level * INDENT_SIZE, "");
	fprintf(file, ")\n");
}

void SaveNameTable(NameTable* name_table, FILE* file) {

	assert(name_table != nullptr);

	for (int i = 0; i < name_table->size; i++) {
		fprintf(file, "    [\"%s\", %d, %d]\n",
					  name_table->table[i].name,
					  name_table->table[i].code,
					  name_table->table[i].type);
	}
}

void SaveTree(Tree* tree, FILE* file, ProgrammNameTables* table) {

	assert(table != nullptr);
	assert(file != nullptr);
	assert(tree != nullptr);

	fprintf(file, "Functions size: %zu {\n", table->funcs.size);

	SaveNameTable(&(table->funcs), file);

	fprintf(file, "}\n\n");
	
	for (size_t i = 0; i < table->size; i++) {
		fprintf(file, "%s size: %zu {\n", table->funcs.table[i].name, table->local_tables[i].size);
		SaveNameTable(&(table->local_tables[i]), file);
		fprintf(file, "}\n\n");

	}

	fprintf(file, "Tree: {\n");
	SaveNode(tree->root, file, 1);
	fprintf(file, "}");
}