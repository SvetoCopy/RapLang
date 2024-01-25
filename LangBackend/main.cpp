﻿#include "RuzalLib/include/TreeIO.h"
#include "RuzalLib/include/GraphicDump.h"
#include "Backend.h"

int main()
{
    NameTable name_table = {};
    NameTableCtor(&name_table);

    FileInfo file_info = FileInfoCtor("../tree.txt");
    Tree tree = {};
    NodeData_t start_data = {};
    TreeCtor(&tree, "backend_tree.gv", start_data);

    printf("aaa");
    ReadTree(&file_info, &name_table, &tree);

    TreeGraphicDump(&tree);

    FILE* file_asm = {};
    fopen_s(&file_asm, "asm_code.txt", "w");

    RetranslateTreeToASM(&tree, file_asm, name_table);

    return 0;
}