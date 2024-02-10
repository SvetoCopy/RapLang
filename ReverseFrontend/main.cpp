#include "ReverseFrontend.h"
#include "RuzalLib/include/TreeIO.h"
#include "RuzalLib/include/GraphicDump.h"

int main()
{
    ProgrammNameTables name_table = {};
    ProgrammNameTablesCtor(&name_table);

    FileInfo file_info = FileInfoCtor("../tree.txt");
    Tree tree = {};
    NodeData_t start_data = {};
    TreeCtor(&tree, "backend_tree.gv", start_data);

    printf("aaa");
    ReadTree(&file_info, &name_table, &tree);

    ShowGraphicDump(tree.root, "24_example.gv");

    FILE* file_lang = {};
    fopen_s(&file_lang, "lang_code.txt", "w");

    FILE* file_save = {};
    fopen_s(&file_save, "tes_saving.txt", "w");

    SaveTree(&tree, file_save, &name_table);

    RetranslateNodeToLang(tree.root, stdout);

    return 0;
}