#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "RuzalLib/include/Differentiator.h"
#include "RuzalLib/include/GraphicDump.h"

#include <locale.h>

int main(int argc, const char* argv[])
{                              

    if (argc < 4) {
        printf("Usage: %s code.txt frontend_dump.gv save_tree.txt \n", argv[0]);

        return -1;
    }
    
    FileInfo file = FileInfoCtor(argv[1]);
    ReadFile(&file);

    Node** node_array = ReadTokenArray(file.buff);
    PrintTokenArray(node_array);

    ProgrammNameTables table = {};
    ProgrammNameTablesCtor(&table);

    Node* node = GetGeneral(node_array, &table);
    Tree tree = {};
    NodeData_t start_data = {};

    TreeCtor(&tree, argv[2], start_data);
    tree.root = node;

    FILE* file_save = {};
    fopen_s(&file_save, argv[3], "w");

    SaveTree(&tree, file_save, &table);

    TreeGraphicDump(&tree);
}