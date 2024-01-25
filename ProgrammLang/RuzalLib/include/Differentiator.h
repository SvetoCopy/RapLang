#ifndef DIFFERCIATOR_DED
#define DIFFERCIATOR_DED
#include "../include/TreeIO.h"
#include <math.h>
#include <cstdlib>
#include "LatexDump.h"

#define _ADD(left, ...)	CreateOperatorNode(ADD, left, __VA_ARGS__)
#define _SUB(left, ...) CreateOperatorNode(SUB, left, __VA_ARGS__) 
#define _MUL(left, ...) CreateOperatorNode(MUL, left, __VA_ARGS__)
#define _DIV(left, ...) CreateOperatorNode(DIV, left, __VA_ARGS__)
#define _POW(left, ...) CreateOperatorNode(POW, left, __VA_ARGS__)
#define _LOG(left, ...) CreateOperatorNode(LOG, left, __VA_ARGS__)
#define _COS(left)      CreateOperatorNode(COS, left, nullptr)
#define _SIN(left)      CreateOperatorNode(SIN, left, nullptr)
#define _IMM(num)		CreateImmNode(num, nullptr, nullptr)

#define DIFF_EXPR(node, var) DiffExprNode(node, var, latex_logfile)

#define DIFF_COMPOSITION_FUNC(expr) _MUL(expr, DIFF_EXPR(LEFT, diff_var))

#define LEFT  node->left
#define RIGHT node->right

#define PRINT_LATEX_LEFT  PrintLatexNode(child->left, child, file);
#define PRINT_LATEX_RIGHT PrintLatexNode(child->right, child, file);
const int	 MAX_TAYLOR_LEN = 5;
const double EXP			= 2.72;
const double EPSILON		= 0.01;

void SetVarInNode(Node* node, ExprVar var, double point);

double LogBase(double num, double base);
double Eval(const Node* node);

int Factorial(int num);
void TaylorExpr(Tree* expr, ExprVar var, double point);
void GetTangentInPoint(Tree* tree, ExprVar diff_var, double point);

void  DiffExprNTimes(int N, Tree* tree, ExprVar diff_var);
Node* DiffExprNode(Node* node, ExprVar diff_var, FILE* latex_logfile);
void  DiffExprTree(Tree* tree, ExprVar diff_var);
#endif // !DIFFERCIATOR_DED
