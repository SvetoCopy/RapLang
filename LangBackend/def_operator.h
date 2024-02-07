
//            operation
//  name -----+    |   +--------------------- code
//			  |    |   |    +------ is char operator
//			  |    |   |    |
//            V    v   v    v
DEF_OPERATOR(MUL, "*", 0,

	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "MUL\n");

)

DEF_OPERATOR(ADD, "+", 1,

	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "ADD\n");

)

DEF_OPERATOR(SUB, "-", 2,

	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "SUB\n");

)

DEF_OPERATOR(DIV, "/", 3,

	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "DIV\n");

)

DEF_OPERATOR(POW, "^", 4,

	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "POW\n");

)

DEF_OPERATOR(OPEN_BRACKET_1, "pisyat", 5, )

DEF_OPERATOR(CLOSE_BRACKET_1, "dva", 6, )

DEF_OPERATOR(END1, "grr", 7, 
	
	fprintf(asm_code, "\n; the next operator has gone from %zu line\n", node->left->line_num);
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

)

DEF_OPERATOR(END2, "yeei", 8, )

DEF_OPERATOR(ASSIGN, "Bless", 9,

	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	fprintf(asm_code, "POP ");
	RetranslateVariable(node->right, asm_code, name_table);
	fprintf(asm_code, "\n");

)

DEF_OPERATOR(ASSIGN_TO, "To", 10, )

DEF_OPERATOR(IF, "LegitCheck", 11, 
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);

	fprintf(asm_code, "PUSH 0\n");
	int next_oper = GetNextOperId();
	fprintf(asm_code, "JE end_if_%d\n", next_oper);
	
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "end_if_%d:\n", next_oper);

)

DEF_OPERATOR(WHILE, "While", 12, 
	
	int cond_id = GetNextOperId();
	fprintf(asm_code, "while_condition_%d:\n", cond_id);

	RetranslateNodeToASM(node->left, asm_code, name_table, table);

	fprintf(asm_code, "PUSH 0\n");
	int next_oper = GetNextOperId();
	fprintf(asm_code, "JE end_while_%d\n", next_oper);
	
	RetranslateNodeToASM(node->right, asm_code, name_table, table);
	fprintf(asm_code, "JMP while_condition_%d\n", cond_id);

	fprintf(asm_code, "end_while_%d:\n", next_oper);

)

DEF_OPERATOR(OPEN_BRACKET_2, "Love", 13, )

DEF_OPERATOR(CLOSE_BRACKET_2, "Sosa", 14, )

DEF_OPERATOR(FUNC_DEFINITION, "VVS", 15,
	
	fprintf(asm_code, "%s:\n", NODE_VAR_NAME(node->left));
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

)

DEF_OPERATOR(RETURN, "FuckTwelve", 16,
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	fprintf(asm_code, "POP RAX\n");
	fprintf(asm_code, "RET\n");

)

DEF_OPERATOR(ARG_SEP, "$", 17, )

DEF_OPERATOR(EQUAL, "is", 18,
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

	int cond_id = GetNextOperId();
	int end_cond_id = GetNextOperId();

	fprintf(asm_code, "JE cond_%d\n", cond_id);
	fprintf(asm_code, "PUSH 0\n");
	fprintf(asm_code, "JMP end_cond%d\n", end_cond_id);
	fprintf(asm_code, "cond_%d:\n", cond_id);
	fprintf(asm_code, "PUSH 1\n");
	fprintf(asm_code, "end_cond%d:\n", end_cond_id);

)
DEF_OPERATOR(MORE, ">", 19,
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

	int cond_id = GetNextOperId();
	int end_cond_id = GetNextOperId();

	fprintf(asm_code, "JA cond_%d\n", cond_id);
	fprintf(asm_code, "PUSH 0\n");
	fprintf(asm_code, "JMP end_cond%d\n", end_cond_id);
	fprintf(asm_code, "cond_%d:\n", cond_id);
	fprintf(asm_code, "PUSH 1\n");
	fprintf(asm_code, "end_cond%d:\n", end_cond_id);

)
DEF_OPERATOR(LESS, "<", 20, 
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

	int cond_id = GetNextOperId();
	int end_cond_id = GetNextOperId();

	fprintf(asm_code, "JB cond_%d\n", cond_id);
	fprintf(asm_code, "PUSH 0\n");
	fprintf(asm_code, "JMP end_cond%d\n", end_cond_id);
	fprintf(asm_code, "cond_%d:\n", cond_id);
	fprintf(asm_code, "PUSH 1\n");
	fprintf(asm_code, "end_cond%d:\n", end_cond_id);

)
DEF_OPERATOR(AND, "&", 21,
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

	int cond_id = GetNextOperId();
	int end_cond_id = GetNextOperId();

	fprintf(asm_code, "MUL\n");
	fprintf(asm_code, "PUSH 0\n");

	fprintf(asm_code, "JE or_%d\n", cond_id);

	fprintf(asm_code, "PUSH 0\n");
	fprintf(asm_code, "JMP end_or_%d\n", end_cond_id);

	fprintf(asm_code, "or_%d:", cond_id);
	fprintf(asm_code, "PUSH 1\n");

	fprintf(asm_code, "end_or_%d:", end_cond_id);

)
DEF_OPERATOR(OR, "|", 22,
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	RetranslateNodeToASM(node->right, asm_code, name_table, table);

	int cond_id = GetNextOperId();
	int end_cond_id = GetNextOperId();

	fprintf(asm_code, "ADD\n");
	fprintf(asm_code, "PUSH 0\n");

	fprintf(asm_code, "JE or_%d\n", cond_id);

	fprintf(asm_code, "PUSH 0\n");
	fprintf(asm_code, "JMP end_or_%d\n", end_cond_id);

	fprintf(asm_code, "or_%d:", cond_id);
	fprintf(asm_code, "PUSH 1\n");

	fprintf(asm_code, "end_or_%d:", end_cond_id);

)

DEF_OPERATOR(PRINT, "ShoutOut", 23,
	
	RetranslateNodeToASM(node->left, asm_code, name_table, table);
	fprintf(asm_code, "OUT\n");
	
)