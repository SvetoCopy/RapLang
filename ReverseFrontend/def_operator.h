
//            operation
//  name -----+    |   +--------------------- code
//			  |    |   |    +------ is char operator
//			  |    |   |    |
//            V    v   v    v
DEF_OPERATOR(MUL, "*", 0,

	fprintf(lang_file, " pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " * ");

	RetranslateNodeToLang(node->right, lang_file, tab_count);
	fprintf(lang_file, " dva ");

)

DEF_OPERATOR(ADD, "+", 1,

	fprintf(lang_file, " pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " + ");

	RetranslateNodeToLang(node->right, lang_file, tab_count);
	fprintf(lang_file, " dva ");

)

DEF_OPERATOR(SUB, "-", 2,

	fprintf(lang_file, " pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " - ");

	RetranslateNodeToLang(node->right, lang_file, tab_count);
	fprintf(lang_file, " dva ");

)

DEF_OPERATOR(DIV, "/", 3,

	fprintf(lang_file, " pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " / ");

	RetranslateNodeToLang(node->right, lang_file, tab_count);
	fprintf(lang_file, " dva ");

)

DEF_OPERATOR(POW, "^", 4,

	fprintf(lang_file, " pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, "^");

	RetranslateNodeToLang(node->right, lang_file, tab_count);
	fprintf(lang_file, " dva ");

)

DEF_OPERATOR(OPEN_BRACKET_1, "pisyat", 5, )

DEF_OPERATOR(CLOSE_BRACKET_1, "dva", 6, )

DEF_OPERATOR(END1, "grr", 7, 
	
	RetranslateNodeToLang(node->left, lang_file, tab_count);
	fprintf(lang_file, " grr\n");
	RetranslateNodeToLang(node->right, lang_file, tab_count);

)

DEF_OPERATOR(END2, "yeei", 8, )

DEF_OPERATOR(ASSIGN, "Bless", 9,

	AlignStr(lang_file, tab_count);

	fprintf(lang_file, "Bless ");
	RetranslateNodeToLang(node->right, lang_file, tab_count);

	fprintf(lang_file, " To ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

)

DEF_OPERATOR(ASSIGN_TO, "To", 10, )

DEF_OPERATOR(IF, "LegitCheck", 11, 
	
	AlignStr(lang_file, tab_count);

	fprintf(lang_file, "LegitCheck pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " dva Love \n");

	RetranslateNodeToLang(node->right, lang_file, tab_count + 1);

	AlignStr(lang_file, tab_count);
	fprintf(lang_file, "Sosa ");

)

DEF_OPERATOR(WHILE, "Sesh", 12, 
	
	AlignStr(lang_file, tab_count);
	tab_count++;
	
	fprintf(lang_file, "Sesh pisyat ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " dva Love \n");

	RetranslateNodeToLang(node->right, lang_file, tab_count + 1);

	AlignStr(lang_file, tab_count);
	fprintf(lang_file, "Sosa ");

)

DEF_OPERATOR(OPEN_BRACKET_2, "Love", 13, )

DEF_OPERATOR(CLOSE_BRACKET_2, "Sosa", 14, )

DEF_OPERATOR(FUNC_DEFINITION, "VVS", 15,
	
	fprintf(lang_file, "VVS ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	fprintf(lang_file, " Love\n");
	RetranslateNodeToLang(node->right, lang_file, tab_count + 1);

	AlignStr(lang_file, tab_count);
	fprintf(lang_file, "Sosa ");

)

DEF_OPERATOR(RETURN, "FuckTwelve", 16,
	
	AlignStr(lang_file, tab_count);

	fprintf(lang_file, "FuckTwelve ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

)

DEF_OPERATOR(ARG_SEP, "$", 17, 
	
	RetranslateNodeToLang(node->left, lang_file, tab_count);

	if (node->right != nullptr)
		fprintf(lang_file, " $ ");
	
	RetranslateNodeToLang(node->right, lang_file, tab_count);
)

DEF_OPERATOR(EQUAL, "is", 18,
	
	fprintf(lang_file, " is ");

)
DEF_OPERATOR(MORE, ">", 19,
	
	fprintf(lang_file, " > ");

)
DEF_OPERATOR(LESS, "<", 20, 
	
	fprintf(lang_file, " < ");

)
DEF_OPERATOR(AND, "&", 21,
	
	fprintf(lang_file, " & ");

)
DEF_OPERATOR(OR, "|", 22,
	
	fprintf(lang_file, " | ");

)

DEF_OPERATOR(PRINT, "ShoutOut", 23,
	
	AlignStr(lang_file, tab_count);

	fprintf(lang_file, "ShoutOut ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);
	
)

DEF_OPERATOR(INPUT_NUM, "PullUp", 24,
	
	AlignStr(lang_file, tab_count);

	fprintf(lang_file, "PullUp ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

)

DEF_OPERATOR(VRAM_PRINT, "PawPaw", 25, 
	
	AlignStr(lang_file, tab_count);

	fprintf(lang_file, "PawPaw ");
	RetranslateNodeToLang(node->left, lang_file, tab_count);

)