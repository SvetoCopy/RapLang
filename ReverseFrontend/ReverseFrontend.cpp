#include "ReverseFrontend.h"

void RetranslateNodeToLang(Node* node, FILE* lang_file) {

	assert(lang_file != nullptr);

	if (node == nullptr)
		return;

	if (GET_NODE_TYPE(node) == VAR) {
		
		fprintf(lang_file, "%s", NODE_VAR_NAME(node));
		
		return;
	}

	if (GET_NODE_TYPE(node) == FUNCTION) {

		fprintf(lang_file, "%s pisyat ", NODE_VAR_NAME(node));
		RetranslateNodeToLang(node->left, lang_file);
		fprintf(lang_file, " dva ");

		return;
	}

	if (GET_NODE_TYPE(node) == NUM) {

		fprintf(lang_file, "%d", NODE_IMM_VALUE(node));

		return;
	}

	if (GET_NODE_TYPE(node) == OPERATOR) {

		#define DEF_OPERATOR(name, operation, code, lang_code)		\
			if (NODE_CMD_CODE(node) == code){						\
				lang_code;											\
			}

		#include "def_operator.h"
		#undef DEF_OPERATOR

		return;
	}

	else {
		assert(0);
	}
}