#define _CRT_SECURE_NO_WARNINGS

#include "LexicalAnalysis.h"

// return value size
static int ReadIntFromStr(const char* str, double* value) {

	assert(str != nullptr);

	int    iter = 0;
	double res  = 0;

	while (isdigit(*str)) {
		res *= 10;
		res += *str - '0';
		iter++;
		str++;
	}

	*value = res;

	return iter;
}

bool isOperator(const char* str, int* operator_type) {

	assert(str != nullptr);

	#define DEF_OPERATOR(oper_name, oper_str, oper_code, ...)		  \
		if (strcmp(str, oper_str) == 0 && strlen(oper_str) != 1) {	  \
			*operator_type = oper_code;                               \
			return true;               								  \
		}

	#include "def_operator.h"

	#undef DEF_OPERATOR

	return false;
}

int ReadCharToken(const char* str, Node** node) {
	
	#define DEF_OPERATOR(oper_name, oper_str, oper_code, ...)		  \
		if (*str == *oper_str && strlen(oper_str) == 1) {			  \
			*node = CreateOperatorNode(oper_code, nullptr, nullptr);  \
			return CHAR_OPERATION_SIZE;								  \
		}

	#include "def_operator.h"

	#undef DEF_OPERATOR

	return 0;
}

int ReadStrToken(const char* str, Node** node) {

	assert(str != nullptr);
	assert(node != nullptr);

	int   iter = 0;
	char* ident = (char*)calloc(MAX_VAR_SIZE, sizeof(char));

	while (
		('A' <= str[iter] && str[iter] <= 'Z') ||
		('a' <= str[iter] && str[iter] <= 'z') ||
		(str[iter] == '_')					   ||
		(isdigit(str[iter]))
		)
	{
		ident[iter] = str[iter];
		iter++;
	}

	int oper_code = 0;
	if (isOperator(ident, &oper_code)) {
		*node = CreateOperatorNode(oper_code, nullptr, nullptr);
	}

	else {
		ExprVar var = {};
		var.name = ident;

		*node = CreateVarNode(var, nullptr, nullptr);
	}

	return iter;
}

void PrintTokenArray(Node** token_array) {
	
	int iter = 0;

	while (token_array[iter] != nullptr) 
	{
		if (GET_NODE_TYPE(token_array[iter]) == NUM)
			printf("%d) [num - %lf] \n", iter, GET_NODE_IMM_VALUE(token_array[iter]));

		else if (GET_NODE_TYPE(token_array[iter]) == OPERATOR)
			printf("%d) [command - %d] \n", iter, GET_OPERATOR_TYPE(token_array[iter]));

		else if (GET_NODE_TYPE(token_array[iter]) == VAR)
			printf("%d) [var - %s] \n", iter, token_array[iter]->data.value.var.name);

		iter++;
	}

}

Node** ReadTokenArray(const char* str) {

	Node** node_array = (Node**)calloc(strlen(str), sizeof(Node*));
	int	   iter       = 0;
	size_t line_iter  = 1;

	while (*str != '\0') {

		if (isspace(*str) || *str == '\t') {
			str++;
		}

		else if (*str == '\n') {
			line_iter++;
		}

		else if (ReadCharToken(str, &(node_array[iter])) != 0) {
			node_array[iter]->line_num = line_iter;

			iter++;
			str++;
		}

		else if (isdigit(*str)) {
			double number = 0;
			str += ReadIntFromStr(str, &number);

			node_array[iter] = _IMM(number);
			node_array[iter]->line_num = line_iter;

			iter++;
		}

		else {
			Node* node = {};
			int str_token_size = ReadStrToken(str, &node);

			node_array[iter] = node;
			node_array[iter]->line_num = line_iter;

			iter++;
			str += str_token_size;
		}
	}

	return node_array;
}