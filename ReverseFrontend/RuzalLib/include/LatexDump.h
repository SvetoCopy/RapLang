#ifndef LATEXDUMP_HPP
#define LATEXDUMP_HPP

#include "../include/TreeIO.h"
#include "Differentiator.h"
#include <stdio.h>      
#include <conio.h>  
#include <stdlib.h>  
#include "super_io.h"

const double IMAGE_SCALE = 0.7;

struct ExprLabel {
	char* name;
	char* value;
};

void PrintLatexPackages(FILE* latex_file);

void AddImageLatex(Tree* tree, const char* image_name);

void CreateExprGraph(Tree* first_tree, Tree* second_tree, ExprVar var, const char* graph_name);
void PrintLatexExpr(const Node* node, FILE* latex_logfile);
#endif // !LATEXDUMP_HPP
