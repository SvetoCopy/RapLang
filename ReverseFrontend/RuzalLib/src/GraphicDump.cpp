#define _CRT_SECURE_NO_WARNINGS
#include "../../../TXLib.h"
#include "../include/GraphicDump.h"

void DefineImmNode(Tree* tree, Node* node) {

	fprintf(tree->graph_logfile,
		"\"node_%p\" [shape=Mrecord, style=filled, fillcolor=" IMMNODE_COLOR ", "
		"label = \"{ %s | %lf }\"];\n",
		node,
		"IMM",
		GET_NODE_IMM_VALUE(node)
		);
}

void DefineVarNode(Tree* tree, Node* node) {

	fprintf(tree->graph_logfile,
		"\"node_%p\" [shape=Mrecord, style=filled, fillcolor=" VARNODE_COLOR ", "
		"label = \"{ %s | %s }\"];\n",
		node,
		"VAR",
		node->data.value.var.name
		);
}

void DefineFuncNode(Tree* tree, Node* node) {

	fprintf(tree->graph_logfile,
		"\"node_%p\" [shape=Mrecord, style=filled, fillcolor=" FUNCNODE_COLOR ", "
		"label = \"{ %s | %s }\"];\n",
		node,
		"FUNC",
		node->data.value.var.name
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

	fprintf(tree->graph_logfile, " ) }\"];\n");
}

int DefineNode(Tree* tree, Node* node) {

	if (node == nullptr) return 0;

	if (node->data.type == VAR)
		DefineVarNode(tree, node);

	if (node->data.type == NUM)
		DefineImmNode(tree, node);

	if (node->data.type == OPERATOR)
		DefineOperNode(tree, node);

	if (node->data.type == FUNCTION)
		DefineFuncNode(tree, node);

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

void ShowGraphicDump(Node* node, const char* filename) {

	Tree tree = {};
	tree.root = node;

	FILE* file_graph = {};
	fopen_s(&file_graph, filename, "w");

	tree.graph_logfile = file_graph;

	TreeGraphicDump(&tree);
	fclose(file_graph);
	char command1[MAX_COMMAND_SIZE] = "";
	sprintf(command1, "dot -Tpng %s -o example111.png", filename);

	system(command1);
	system("start example111.png");
	system("pause");
}