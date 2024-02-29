#include "Backend.h"
    
int GetNextOperId() {

	static int n = 0;
	n++;

	return n;
}   

void PushAllVariables(FILE* asm_code, NameTable name_table) {

	assert(asm_code != nullptr);

	if (name_table.size == 0)
		return;

	for (size_t i = 0; i < name_table.size; i++) {

		fprintf(asm_code, "PUSH [%d] ; saving %zu arg\n", name_table.table[i].address, i);
	}
}

void PopAllVariables(FILE* asm_code, NameTable name_table) {

	assert(asm_code != nullptr);

	if (name_table.size == 0)
		return;

	for (int i = name_table.size - 1; i >= 0; i--) {

		fprintf(asm_code, "POP [%d]; getting old %d arg\n", name_table.table[i].address, i);
	}
}

void GiveArg(FILE* asm_code, Node* arg, NameTableElem param, NameTable name_table, ProgrammNameTables table) {

	assert(asm_code != nullptr);
	assert(arg != nullptr);

	RetranslateNodeToASM(arg->left, asm_code, name_table, table);
	fprintf(asm_code, "\n");

	fprintf(asm_code, "POP [%d]\n", param.address);
}
    
int GetArgsCount(Node* args) {

	assert(args != nullptr);

	int iter = 0;
	Node* var = args;

	while (var != nullptr) {
		iter++;
		var = var->right;
	}

	return iter;
}

void GiveArgs(Node* func, FILE* asm_code, ProgrammNameTables table, NameTable name_table) {
	    
	assert(asm_code != nullptr);
	assert(func != nullptr);     
	    
	for (size_t i = 0; i < table.size; i++) {
		
		if (strcmp(table.funcs.table[i].name, NODE_VAR_NAME(func)) == 0) {
			
			int   iter = 0;
			Node* arg  = func->left;

			while (table.local_tables[i].table[iter].type == ARGUMENT && iter < table.local_tables[i].size) {
				
				if (arg == nullptr)
					assert(!"Fuck, give me enough arguments, nigga");

				GiveArg(asm_code, arg, table.local_tables[i].table[iter], name_table, table);
				arg = arg->right;

				iter++;
			}
		}
	}   
	     
}       
    
void RetranslateVariable(Node* node, FILE* asm_code, NameTable name_table) {

	assert(asm_code != nullptr);
	assert(node != nullptr);

	for (size_t i = 0; i < name_table.size; i++) {
		if (strcmp(name_table.table[i].name, NODE_VAR_NAME(node)) == 0) {

			fprintf(asm_code, "[%d]", name_table.table[i].address);

			return;
		}
	}

	assert(!"Unknown variable");
}

void RetranslateNodeToASM(Node* node, FILE* asm_code, NameTable name_table, ProgrammNameTables table) {
	
	assert(asm_code != nullptr);

	if (node == nullptr)
		return;

	if (GET_NODE_TYPE(node) == NUM) {

		fprintf(asm_code, "PUSH %lf\n", GET_NODE_IMM_VALUE(node));

		return;
	}

	if (GET_NODE_TYPE(node) == VAR) {

		fprintf(asm_code, "PUSH ");
		RetranslateVariable(node, asm_code, name_table);
		fprintf(asm_code, "\n");
		
		return;
	}

	if (GET_NODE_TYPE(node) == FUNCTION) {

		PushAllVariables(asm_code, name_table);

		GiveArgs(node, asm_code, table, name_table);
		fprintf(asm_code, "CALL %s\n", node->data.value.var.name);

		PopAllVariables(asm_code, name_table);

		fprintf(asm_code, "PUSH RAX\n");
		
		return;
	}

	#define DEF_OPERATOR(name, operation, code, asm_code)		\
			if (NODE_CMD_CODE(node) == code){					\
				asm_code;										\
			}
	#include "def_operator.h"
	#undef DEF_OPERATOR
}

void RetranslateTreeToASM(Tree* tree, FILE* asm_code, ProgrammNameTables table) {

	assert(tree != nullptr);
	assert(asm_code != nullptr);

	fprintf(asm_code, "CALL main\n");
	fprintf(asm_code, "HLT\n");

	Node* var_node = tree->root;

	for (int i = 0; i < table.size; i++) {
		printf("local table number %d\n", i);
		RetranslateNodeToASM(var_node->left, asm_code, table.local_tables[i], table);
		var_node = var_node->right;
	}
	
}