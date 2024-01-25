#include "../../../TXLib.h"
#include "../include/GraphicDump.h"

void DefineImmNode(Tree* tree, Node* node) {

	fprintf(tree->graph_logfile,
		"\"node_%p\" [shape=Mrecord, style=filled, fillcolor=" IMMNODE_COLOR ", "
		"label = \"{ %s | %lf | line: %zu }\"];\n",
		node,
		"IMM",
		GET_NODE_IMM_VALUE(node),
		node->line_num
		);
}

void DefineVarNode(Tree* tree, Node* node) {

	fprintf(tree->graph_logfile,
		"\"node_%p\" [shape=Mrecord, style=filled, fillcolor=" VARNODE_COLOR ", "
		"label = \"{ %s | %s | line: %zu }\"];\n",
		node,
		"VAR",
		node->data.value.var.name,
		node->line_num
		);
}

void DefineOperNode(Tree* tree, Node* node) {

	fprintf(tree->graph_logfile,
		"\"node_%p\" [shape=Mrecord, style=filled, fillcolor=" OPERNODE_COLOR ", "
		"label = \"{ %s | %d ( ",
		node,
		"OPER",
		GET_OPERATOR_TYPE(node)
		);

	PrintOperator(node, tree->graph_logfile);

	fprintf(tree->graph_logfile, " ) | line: %zu }\"];\n", node->line_num);
}

int DefineNode(Tree* tree, Node* node) {

	if (node == nullptr) return 0;


	if (node->data.type == VAR)
		DefineVarNode(tree, node);

	if (node->data.type == NUM)
		DefineImmNode(tree, node);

	if (node->data.type == OPERATOR)
		DefineOperNode(tree, node);

	DefineNode(tree, node->left);
	DefineNode(tree, node->right);

	return 0;
}

void ConnectNodes(Tree* tree, Node* node) {

	assert(tree != nullptr);

	if (node == nullptr) return;

	if (node->left != nullptr) {
		fprintf(tree->graph_logfile, 
				"node_%p->node_%p\n", 
				node, 
				node->left);
	}

	if (node->right != nullptr) {
		fprintf(tree->graph_logfile,
			"node_%p->node_%p\n",
			node,
			node->right);
	}

	ConnectNodes(tree, node->left);
	ConnectNodes(tree, node->right);
}

void TreeGraphicDump(Tree* tree) {
	fprintf(tree->graph_logfile, 
		    "digraph Tree \n{ bgcolor=\"" BG_COLOR "\";" 
		    "node[margin = \"0.01\"];\n");

	DefineNode(tree, tree->root);

	ConnectNodes(tree, tree->root);

	fprintf(tree->graph_logfile, "}");
}