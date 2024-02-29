#include "ReverseFrontend.h"
#include "RuzalLib/include/TreeIO.h"
#include "RuzalLib/include/GraphicDump.h"

int main(int argc, const char* argv[])
{
    if (argc < 4) {
        printf("Usage: %s tree.txt backend_tree.gv asm_code.txt \n", argv[0]);

        return -1;
    }

    ProgrammNameTables name_table = {};
    ProgrammNameTablesCtor(&name_table);

    FileInfo file_info = FileInfoCtor(argv[1]);
    Tree tree = {};
    NodeData_t start_data = {};
    TreeCtor(&tree, argv[2], start_data);

    ReadTree(&file_info, &name_table, &tree);

    ShowGraphicDump(tree.root, "24_example.gv");

    FILE* file_asm = {};
    fopen_s(&file_asm, argv[3], "w");

    RetranslateTreeToLang(&tree, file_asm);

    return 0;
}