#ifndef GRAPHIC_DUMP
#define GRAPHIC_DUMP
#include "Tree.h"
#include "TreeIO.h"
#include "../../../TXLib.h"

#define BG_COLOR        "cornsilk1"
#define IMMNODE_COLOR   "darksalmon"
#define VARNODE_COLOR   "bisque"
#define OPERNODE_COLOR  "darkolivegreen1"

const size_t MAX_COMMAND_SIZE = 100;

void TreeGraphicDump(Tree* tree);
void ShowGraphicDump(Node* node, const char* filename);

#endif // !GRAPHIC_DUMP
