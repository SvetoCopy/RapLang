#include "../include/TreeIO.h"
#include "../../../TXLib.h"

int ReadExprElem(char* str, Node** node) {

	assert(str != nullptr);

	double  double_elem	   = 0;
	int     elem_read_size = 0;
	size_t	line_num	   = 0;
	int     node_type	   = 0;
	int     read_arg_count = sscanf(str, " %lf line = %zu type = %d%n", 
									&double_elem, 
									&line_num, 
									&node_type,
									&elem_read_size);

	if (read_arg_count == 0) {

		elem_read_size = 0;
		char str_elem[MAX_NODEINFO_SIZE] = {};
		read_arg_count = sscanf(str, " %s line = %zu type = %d%n", 
								&str_elem, 
								&line_num,
								&node_type,
								&elem_read_size);

		if (strcmp(str_elem, DEFAULT_NIL) == 0) {
			*node = nullptr;
			return 1;
		}

		#define DEF_OPERATOR(cmd_name, str_command, cmd_code, ...)  \
															    \
        else if (strcmp(str_elem,  str_command) == 0) {         \
			ExprElem data = CreateExprOperator(cmd_code);       \
			*node = OpNew(data);                                \
		}

		#include "../../def_operator.h"
		#undef DEF_EXPR_CMD

		else if (node_type == VAR) {
			ExprElem data = CreateExprVar(str_elem);
			*node = OpNew(data);
		}

		else if (node_type == FUNCTION) {
			ExprElem data = CreateExprFunc(str_elem);
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

int ReadNameTable(const char* str, NameTable* name_table, size_t name_table_size, int start_address) {
	
	assert(str != nullptr);
	
	int res_size = 0;

	int name_table_iter = 0;

	for (size_t i = 0; i < name_table_size; i++) {

		char			  name[MAX_NAME_SIZE] = "";
		int				  code				  = 0;
		NameTableElemType type				  = ARGUMENT;

		int read_size = 0;
		sscanf(str + name_table_iter, " [\"%[^\"]\", %d, %d%n", &name, &code, &type, &read_size);

		name_table->table[i].name    = _strdup(name);
		name_table->table[i].code    = code;
		name_table->table[i].type    = type;
		name_table->table[i].address = start_address + code;

		name_table->size += 1;
		name_table_iter += read_size + 1;
	}

	return res_size;
}

void ReadTree(FileInfo* file, ProgrammNameTables* table, Tree* tree) {

	assert(table != nullptr);
	assert(file != nullptr);
	assert(tree != nullptr);
	
	ReadFile(file);

	// Read func table
	char   func_table_str[MAX_NAMETABLE_SIZE] = "";
	int	   read_size					  = 0;
	size_t func_table_size				  = 0;

	sscanf(file->buff, "Functions size: %zu { %[^}] }%n", &func_table_size, func_table_str, &read_size);

	ReadNameTable(func_table_str, &(table->funcs), func_table_size, 0);
	file->buff += read_size;
	
	int start_address = 0;

	for (int i = 0; i < func_table_size; i++) {

		char   local_table_name[MAX_NAMETABLE_SIZE] = "";
		char   local_table_str [MAX_NAMETABLE_SIZE] = "";
		size_t local_table_size					    = 0;

		read_size = 0;

		sscanf(file->buff, "%s size: %zu { %[^}] }%n", local_table_name, &local_table_size, local_table_str, &read_size);

		ReadNameTable(local_table_str, &(table->local_tables[i]), local_table_size, start_address);

		table->size++;
		start_address += local_table_size;

		file->buff += read_size;
	}

	table->size = func_table_size;

	while (*(file->buff) != 'T') file->buff++;

	char tree_str[MAX_TREE_STR_SIZE] = "";
	sscanf(file->buff, "Tree: {%" QUOTE(MAX_TREE_STR_SIZE) "[^}]", tree_str);

	ReadNodePRE(tree_str, &(tree->root));
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

	fprintf(file, "Functions [%zu] {\n", table->funcs.size);

	SaveNameTable(&(table->funcs), file);

	fprintf(file, "}\n\n");

	for (size_t i = 0; i < table->size; i++) {
		fprintf(file, "%s [%zu] {\n", table->funcs.table[i].name, table->local_tables[i].size);
		SaveNameTable(&(table->local_tables[i]), file);
		fprintf(file, "}\n\n");

	}

	fprintf(file, "Tree: {\n");
	SaveNode(tree->root, file, 1);
	fprintf(file, "}");
}