#include "../include/TreeIO.h"
#include "../../../TXLib.h"

int ReadExprElem(char* str, Node** node) {

	assert(str != nullptr);

	double  double_elem	   = 0;
	int     elem_read_size = 0;
	size_t	line_num	   = 0;
	int     read_arg_count = sscanf(str, " %lf[%zu] %n", &double_elem, &line_num, &elem_read_size);

	if (read_arg_count == 0) {

		elem_read_size = 0;
		char str_elem[MAX_NODEINFO_SIZE] = {};
		read_arg_count = sscanf(str, " %[^[][%zu] %n", &str_elem, &line_num, &elem_read_size);

		if (strcmp(str_elem, DEFAULT_NIL) == 0) {
			*node = nullptr;
		}

	#define DEF_OPERATOR(cmd_name, str_command, cmd_code, ...)  \
															    \
        else if (strcmp(str_elem,  str_command) == 0) {         \
			ExprElem data = CreateExprOperator(cmd_code);       \
			*node = OpNew(data);                                \
		}

	#include "../../def_operator.h"
	#undef DEF_EXPR_CMD
		else {
			ExprElem data = CreateExprVar(str_elem);
			*node = OpNew(data);
		}
	}
	else {
		ExprElem data = CreateExprImm(double_elem);
		*node = OpNew(data);
	}

	(*node)->line_num = line_num;

	return elem_read_size;
}

static int DeleteCloseBracket(char* str) {

	char symbol = {};
	int end_read_size = 0;
	sscanf(str, " %c %n", &symbol, &end_read_size);

	if (symbol == ')') {
		return end_read_size;
	}

	return 0;
}

static int SkipNil(char* str) {

	char read_str[MAX_NODEINFO_SIZE] = {};
	int read_nil_size = 0;
	sscanf(str, " %s %n", &read_str, &read_nil_size);

	if (strcmp(read_str, ".") == 0) return read_nil_size;

	return 0;
}

#define MoveStr(num) res_size += num;\
					 str      += num;   

#define ReadChild(child) Node* child = nullptr;                                \
						 int child ## _size = ReadNodePRE(str, &child);        \
						 MoveStr(child ## _size);                              \
                                                                               \
					 	 close_bracket_size = DeleteCloseBracket(str);		   \
						 MoveStr(close_bracket_size);      

// return read size
int ReadNodePRE(char* str, Node** res) {

	assert(str != nullptr);

	if (strcmp(str, "") == 0) return 0;

	int res_size = 0;
	char symbol = {};
	int first_read_size = 0;
	sscanf(str, " %c %n", &symbol, &first_read_size);

	if (symbol == '(') {
		MoveStr(first_read_size);
	}

	int nil_size = SkipNil(str);
	if (nil_size != 0)
	{
		MoveStr(nil_size);
		*res = nullptr;
		res_size += DeleteCloseBracket(str);

		return res_size;
	}

	int read_root_size = ReadExprElem(str, res);
	MoveStr(read_root_size);

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
		#define DEF_OPERATOR(name, command, code, ...) \
		case code:									   \
			fprintf(file, command);					   \
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
		fprintf(file, "%s", NODE_VAR_NAME(node));
	}

	else if (GET_NODE_TYPE(node) == OPERATOR) {
		PrintOperator(node, file);
	}

	else if (GET_NODE_TYPE(node) == NUM) {
		fprintf(file, "%d", NODE_IMM_VALUE(node));
	}

	fprintf(file, "\n");

	SaveNode(node->left, file, rec_level + 1);
	SaveNode(node->right, file, rec_level + 1);

	fprintf(file, "%*s", rec_level * INDENT_SIZE, "");
	fprintf(file, ")\n");
}

void SaveNameTable(NameTable* name_table, FILE* file) {

	assert(name_table != nullptr);

	fprintf(file, "NameTable [%d] {\n", name_table->size);

	for (int i = 0; i < name_table->size; i++) {
		fprintf(file, "    [\"%s\", %d, %d]\n",
					  name_table->table[i].name,
					  name_table->table[i].code,
					  name_table->table[i].type);
	}

	fprintf(file, "}\n\n");
}

int ReadNameTable(const char* str, NameTable* name_table, size_t name_table_size) {
	
	assert(str != nullptr);
	
	int res_size = 0;

	while (*str != '{') {
		str++;
		res_size++;
	}

	char name_table_str[MAX_NAMETABLE_SIZE] = "";

	int table_str_size = 0;
	sscanf(str, "{%[^}]%n", &name_table_str, &table_str_size);

	res_size += table_str_size;

	int name_table_iter = 0;

	for (size_t i = 0; i < name_table_size; i++) {

		char			  name[MAX_NAME_SIZE] = "";
		int				  code				  = 0;
		NameTableElemType type				  = KEYWORD;

		int read_size = 0;
		sscanf(name_table_str + name_table_iter, " [\"%[^\"]\", %d, %d%n", &name, &code, &type, &read_size);

		name_table->table[i].name    = _strdup(name);
		name_table->table[i].code    = code;
		name_table->table[i].type    = type;
		name_table->table[i].address = code;

		name_table_iter += read_size + 1;
	}

	return res_size + 1;
}

void ReadTree(FileInfo* file, NameTable* name_table, Tree* tree) {

	assert(name_table != nullptr);
	assert(file != nullptr);
	assert(tree != nullptr);
	
	ReadFile(file);

	size_t table_size = 0;
	int read_size = 0;
	sscanf(file->buff, "NameTable [%d%n", &table_size, &read_size);

	file->buff += read_size;
	file->buff += ReadNameTable(file->buff, name_table, table_size);
	name_table->size = table_size;

	while (*(file->buff) != 'T') file->buff++;

	char tree_str[MAX_TREE_STR_SIZE] = "";
	sscanf(file->buff, "Tree: {%" QUOTE(MAX_TREE_STR_SIZE) "[^}]", tree_str);

	ReadNodePRE(tree_str, &(tree->root));
}

void SaveTree(Tree* tree, FILE* file, NameTable* name_table) {

	assert(name_table != nullptr);
	assert(file != nullptr);
	assert(tree != nullptr);

	SaveNameTable(name_table, file);

	fprintf(file, "Tree: {\n");
	SaveNode(tree->root, file, 1);
	fprintf(file, "}");
}