#include "Backend.h"

int GetNextOperId() {

	static int n = 0;
	n++;

	return n;
}

void RetranslateVariable(Node* node, FILE* asm_code, NameTable name_table) {

	assert(asm_code != nullptr);
	assert(node != nullptr);

	for (size_t i = 0; i < name_table.size; i++) {
		if (strcmp(name_table.table[i].name, NODE_VAR_NAME(node)) == 0) {
			fprintf(asm_code, "[%d]", name_table.table[i].address);
		}
	}
}

void RetranslateNodeToASM(Node* node, FILE* asm_code, NameTable name_table) {
	
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

	#define DEF_OPERATOR(name, operation, code, asm_code)		\
			if (NODE_CMD_CODE(node) == code){					\
				asm_code;										\
			}
	#include "def_operator.h"
	#undef DEF_OPERATOR

}

void RetranslateTreeToASM(Tree* tree, FILE* asm_code, NameTable name_table) {

	assert(tree != nullptr);
	assert(asm_code != nullptr);

	RetranslateNodeToASM(tree->root, asm_code, name_table);
}