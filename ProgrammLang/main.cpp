#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "RuzalLib/include/Differentiator.h"
#include "RuzalLib/include/GraphicDump.h"

#include <locale.h>

int main(int argc, const char* argv[])
{                              
    if (argc < 3) {
        printf("Usage: %s code.txt save_tree.txt\n", argv[0]);

        return -1;
    }

    setlocale(LC_CTYPE, "");
    
    FileInfo file = FileInfoCtor(argv[1]);
    ReadFile(&file);

    Node** node_array = ReadTokenArray(file.buff);
    PrintTokenArray(node_array);


    NameTable table = {};
    NameTableCtor(&table);

    Node* node = GetGeneral(node_array, &table);
    Tree tree = {};
    NodeData_t start_data = {};

    TreeCtor(&tree, "frontend_dump.gv", start_data);
    tree.root = node;

    //printf("%d", CheckTree(&tree, &table));

    FILE* file_save = {};
    fopen_s(&file_save, argv[2], "w");

    SaveTree(&tree, file_save, &table);

    TreeGraphicDump(&tree);
}