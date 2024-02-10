
//         name      priority
//           |          |  +-------------- args_num
//           |          |  |                          Differentiation----------------------------+
//      cmd -|----+  +--|--- cmd code                                                            |
//           |    |  |  |  |             handle                            latex dump            |
//           |    v  v  v  v               |                                   |                 |
DEF_EXPR_CMD(MUL, *, 0, 1, 2,//            |								   |                 |
	left* right, // <----------------------+                                   |                 |
	//		                                                                   |                 |
	// (f(x) * g(x))' = f'(x) * g(x) + f(x) * g'(x)                            |                 |
	new_node = _ADD(                                      //                   |                 |
		_MUL(DIFF_EXPR(LEFT, diff_var), CopyNode(RIGHT)), //<------------------|-----------------+
		_MUL(CopyNode(LEFT), DIFF_EXPR(RIGHT, diff_var))  //	               |                          
	),													  //                   |
	//																		   |
	PRINT_LATEX_LEFT;						  //                               |
	fprintf(file, " \\cdot ");                // <-----------------------------+
	PRINT_LATEX_RIGHT;

)

DEF_EXPR_CMD(ADD, +, 1, 2, 2,
	left + right,

	// (f(x) + g(x))' = f'(x) + g'(x) 
	new_node =  _ADD(DIFF_EXPR(LEFT, diff_var), DIFF_EXPR(RIGHT, diff_var)),

	PRINT_LATEX_LEFT;
	fprintf(file, " + ");
	PRINT_LATEX_RIGHT;

)

DEF_EXPR_CMD(SUB, -, 2, 2, 2,
	left - right,

	// (f(x) - g(x))' = f'(x) - g'(x) 
	new_node =  _SUB(DIFF_EXPR(LEFT, diff_var), DIFF_EXPR(RIGHT, diff_var)),

	PRINT_LATEX_LEFT;
	fprintf(file, " - ");
	PRINT_LATEX_RIGHT;

)

DEF_EXPR_CMD(DIV, / , 3, 1, 2,
	left / right,

	//  f(x)    f'(x) * g(x) - f(x) * g'(x)
	//  ---- =  ---------------------------
	//  g(x)              g(x)^2
	new_node =  _DIV(
					_SUB(
						_MUL(DIFF_EXPR(LEFT, diff_var), CopyNode(RIGHT)),
						_MUL(CopyNode(LEFT), DIFF_EXPR(RIGHT, diff_var))
					),
					_MUL(CopyNode(RIGHT), CopyNode(RIGHT))
					),

	fprintf(file, " \\dfrac{");
	PRINT_LATEX_LEFT;
	fprintf(file, " }{ ");
	PRINT_LATEX_RIGHT;
	fprintf(file, " } ");

)

DEF_EXPR_CMD(POW, ^, 4, 0, 2,
	pow(left, right),

	// 1) (f(x)^C)' = C * (f(x) ^ (C - 1)) 
	if (!СheckVarInSubTree(RIGHT, diff_var))
		new_node =  _MUL(
						_MUL(
							_IMM(NODE_IMM_VALUE(RIGHT)),
							_POW(
								CopyNode(LEFT),
								_SUB(CopyNode(RIGHT), _IMM(1))
								)
							),
						DIFF_EXPR(LEFT, diff_var)
						);

	// 2) (a^f(x))' = a^(f(x)) * ln(a) * f'(x)
	else if (!СheckVarInSubTree(LEFT, diff_var))
		new_node =  _MUL(
						DIFF_EXPR(RIGHT, diff_var),
						_MUL(
							CopyNode(node),
							_LOG(_IMM(EXP), CopyNode(LEFT))
							)
						);

	// 3) (f(x)^g(x))' =  f(x)^g(x) * (g'(x) * ln(f(x)) + g(x) * ( f'(x) / f(x) ) )
	else 
		new_node =  _MUL(
						CopyNode(node),
						_ADD(
							_MUL(
								DIFF_EXPR(RIGHT, diff_var),
								_LOG(
									_IMM(EXP),
									CopyNode(LEFT)
									)
								),
							_DIV(
								_MUL(CopyNode(RIGHT), DIFF_EXPR(LEFT, diff_var)),
								CopyNode(LEFT)
								)
							)
						),

	if (isSqrtExpr(child)) {
		fprintf(file, " \\sqrt[");
		PRINT_LATEX_RIGHT;
		fprintf(file, " ]{ ");
		PRINT_LATEX_LEFT;
		fprintf(file, " } ");
	}
	else {
		fprintf(file, " { ");
		PRINT_LATEX_LEFT;
		fprintf(file, " } ^ {\\small ");
		PRINT_LATEX_RIGHT;
		fprintf(file, " } ");
		break;
	}

)

DEF_EXPR_CMD(LOG, log, 5, 0, 2,
	LogBase(left, right),

	// (log_a(f(x)) = f'(x) / (f(x) * ln(a)) 
	new_node = DIFF_COMPOSITION_FUNC(
									_DIV(
										_IMM(1),
										_MUL(
											CopyNode(RIGHT),
											_LOG(_IMM(EXP), CopyNode(LEFT))
											)
										)
									),
	
	fprintf(file, " \log_{ ");
	PRINT_LATEX_LEFT;

	fprintf(file, " }{ ");
	PRINT_LATEX_RIGHT;

	fprintf(file, " } ");

)

DEF_EXPR_CMD(COS, cos, 6, 0, 1,
	cos(left),

	// (cos(f(x)))' = - sin(f(x)) * f'(x) 
	new_node = DIFF_COMPOSITION_FUNC(
									_MUL(
										_IMM(-1),
										_SIN(CopyNode(LEFT))
										)
									),

	fprintf(file, " \cos( ");
	PRINT_LATEX_LEFT;
	fprintf(file, " ) ");

)

DEF_EXPR_CMD(SIN, sin, 7, 0, 1,
	sin(left),

	// (sin(f(x)))' = cos(f(x)) * f'(x)
	new_node = DIFF_COMPOSITION_FUNC(
									_COS(CopyNode(LEFT)),
									DIFF_EXPR(LEFT, diff_var)
									),

	fprintf(file, " \sin( ");
	PRINT_LATEX_LEFT;
	fprintf(file, " ) ");

)