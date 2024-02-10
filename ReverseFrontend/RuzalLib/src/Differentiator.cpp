#include "../include/Differentiator.h"
#include "../include/Optimization.h"

double LogBase(double num, double base)
{
	return log(num) / log(base);
}

double Eval(const Node* node) {
	
	if (VerifyNode(node) == NODE_ERROR) 
		assert(!"Verifier failed in Eval");;

	if (!node) 
		return 0;
	
	if (node->data.type == VAR)
		return NODE_VAR_VALUE(node);
	
	if (node->data.type == NUM)
		return NODE_IMM_VALUE(node);

	double left  = Eval(node->left);
	double right = Eval(node->right);

	switch (NODE_CMD_CODE(node)) {
		#define DEF_EXPR_CMD(cmd_name, command, code, priority, args_num, handle, ...) \
		case code:                                                                     \
			return handle;

		#include "../include/def_expr_cmd.h"

		#undef DEF_EXPR_CMD
	}

	assert(!"Eval ERROR");

	return 0;
}

void GetTangentInPoint(Tree* tree, ExprVar diff_var, double point) {

	if (TreeVerify(tree) == TREE_ERROR) return;

	double f_value_in_point = 0;
	SetVarInNode(tree->root, diff_var, point);
	f_value_in_point = Eval(tree->root);

	DiffExprTree(tree, diff_var);
	SetVarInNode(tree->root, diff_var, point);

	// y = f'(x0)*(x - õ0) + f(x0) 
	tree->root = _ADD(
					 _MUL(
						 _IMM(Eval(tree->root)),
						 _SUB(CreateVarNode(diff_var, nullptr, nullptr), _IMM(point))
  						 ),
					 _IMM(f_value_in_point)
					 );
}

void DiffExprTree(Tree* tree, ExprVar diff_var) {

	if (TreeVerify(tree) == TREE_ERROR) return;

	PrintLatexExpr(tree->root, tree->latex_logfile);

	Node* tmp = DiffExprNode(tree->root, diff_var, tree->latex_logfile);
	NodeDtor(tree->root);

	tree->root = tmp;
	ExpressionOptimization(tree);

	PrintLatexExpr(tree->root, tree->latex_logfile);

}

Node* DiffExprNode(Node* node, ExprVar diff_var, FILE* latex_logfile) {

	assert(node != nullptr);
	if (VerifyNode(node) == NODE_ERROR) assert(!"Ìerifier failed in DiffExprNode");
	
	Node* new_node = {};

	if (node->data.type == NUM) {
		new_node = CreateImmNode(0, nullptr, nullptr);
	}

	else if (node->data.type == VAR && !isEqualVar(node->data.value.var, diff_var)) {
		new_node = CreateImmNode(0, nullptr, nullptr);
	}

	else if (node->data.type == VAR && isEqualVar(node->data.value.var, diff_var)) {
		new_node = CreateImmNode(1, nullptr, nullptr);
	}
	else {
		switch (NODE_CMD_CODE(node)) {
		#define DEF_EXPR_CMD(command_name, command_str, int_code, priority, args_num, handle, diff, ...)\
				case int_code: {                                                                        \
					diff;																				\
					break;																				\
				}
		#include "../include/def_expr_cmd.h"
		#undef DEF_EXPR_CMD
		}
	}
	
	return new_node;

}

void DiffExprNTimes(int N, Tree* tree, ExprVar diff_var) {

	if (TreeVerify(tree) == TREE_ERROR) return;

	for (int i = 0; i < N; i++) {
		DiffExprTree(tree, diff_var);
	}
}

int Factorial(int num) {

	if (num == 1 || num == 0)
		return 1;

	return num * Factorial(num - 1);
}

void SetVarInNode(Node* node, ExprVar var, double point) {

	if (VerifyNode(node) == NODE_ERROR) return;

	if (node == nullptr) return;

	if (node->data.type == VAR && isEqualVar(node->data.value.var, var))
		node->data.value.var.imm_value = point;

	SetVarInNode(node->left, var, point);
	SetVarInNode(node->right, var, point);
}

void TaylorExpr(Tree* expr, ExprVar var, double point) {

	if (TreeVerify(expr) == TREE_ERROR) return;

	Node* res = _IMM(0);
	Tree tree_diff = *expr;

	for (int i = 0; i < MAX_TAYLOR_LEN; i++) {

		tree_diff.root = CopyNode(expr->root);

		DiffExprNTimes(i, &tree_diff, var);
		ExpressionOptimization(&tree_diff);
		SetVarInNode(tree_diff.root, var, point);

		//         (n)              n
		// res += (f (x0)/n!)*(x-x0)
		res =  _ADD(
					res,
					_DIV(
						_MUL(
							_IMM(Eval(tree_diff.root)),
							_POW(
								_SUB(
									CreateVarNode(var, nullptr, nullptr),
									_IMM(point)
									),
								_IMM(i)
								)
							),
						_IMM(Factorial(i))
						)
					);
	}
	
	expr->root = res;
	ExpressionOptimization(expr);
}