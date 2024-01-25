#ifndef LEXICAL_ANALYSIS_HPP
#define LEXICAL_ANALYSIS_HPP

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "RuzalLib/include/Tree.h"
#include <wchar.h>
#include "../../../TXLib.h"

const int MAX_OPERATOR_SIZE   = 100;
const int MAX_VAR_SIZE        = 100;
const int CHAR_OPERATION_SIZE = 1;

enum Operators {
	#define DEF_OPERATOR(name, oper, code, ...) OPERATOR_ ## name = code,

	#include "def_operator.h"

	#undef DEF_OPERATOR
};

void PrintTokenArray(Node** token_array);

Node** ReadTokenArray(const char* str);

#endif