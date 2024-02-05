#include "SyntaxAnalysis.h"
#include "RuzalLib/include/GraphicDump.h"

bool CheckIfWhileOperator(Node* if_oper) {

    if (if_oper->left == nullptr) {
        printf("if condition is null");

        return false;
    }
        

    if (GET_NODE_TYPE(if_oper->right) != OPERATOR || 
        (GET_OPERATOR_TYPE(if_oper->right) != OPERATOR_END1 && 
         GET_OPERATOR_TYPE(if_oper->right) != OPERATOR_END2)
        )
    {
        printf("if_body is not inctruction");

        return false;
    }

    return true;
}

bool CheckVar(Node* var_node, NameTable* name_table) {

    int assign_count = 0;

    for (int i = 0; i < name_table->size; i++) 
    {
        if (strcmp(var_node->data.value.var.name, name_table->table[i].name) == 0)
            assign_count++;
    }

    if (assign_count == 0) {
        printf("variable definition not found");

        return false;
    }

    if (assign_count > 1) {
        printf("multiply variable definition not found");

        return false;
    }
        
    return true;
}

bool CheckAssignOperator(Node* assign_oper) {

    if (assign_oper->left == nullptr) {
        printf("Assign nullptr variable");

        return false;
    }
    if (GET_NODE_TYPE(assign_oper->right) != VAR)
    {
        printf("l_value in assign not variable");

        return false;
    }
    
    return true;
}

bool CheckNode(Node* node, NameTable* name_table) {
    
    assert(name_table != nullptr);

    if (node == nullptr)
        return true;

    if (GET_NODE_TYPE(node) == OPERATOR) 
    {
        if ((GET_OPERATOR_TYPE(node) == OPERATOR_IF ||
            GET_OPERATOR_TYPE(node) == OPERATOR_WHILE) &&
            !CheckIfWhileOperator(node)) 
        {
            return false;
        }

        if (GET_OPERATOR_TYPE(node) == OPERATOR_ASSIGN && !CheckAssignOperator(node))
            return false;
    }

    if (GET_NODE_TYPE(node) == VAR) {
        if (!CheckVar(node, name_table))
            return false;
    }
    
    if (!CheckNode(node->left, name_table) ||
        !CheckNode(node->right, name_table))
    {
        return false;
    }
    
    return true;
}

bool CheckTree(Tree* tree, NameTable* name_table) {

    if (!CheckNode(tree->root, name_table))
        return false;

    return true;
}

Node* GetOperatorPlus(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    printf("Searching first val\n");

    Node* res_oper = GetOperator(token_array, token_counter, name_table);

    (*token_counter)++;

    Node* res_var = res_oper;
    Node* next_oper = {};
    // ShowGraphicDump(res_oper, "24_example.gv");

    do {
        next_oper      = GetOperator(token_array, token_counter, name_table);

        if (next_oper != nullptr)
            (*token_counter)++;

        res_var->right = next_oper;

        // ShowGraphicDump(res_var, "24_example.gv");

        res_var        = res_var->right;

        // ShowGraphicDump(res_var, "24_example.gv");

    } while ((next_oper != nullptr) && (GET_OPERATOR_TYPE(next_oper) == OPERATOR_END1));

    printf("End\n");

    return res_oper;
}

Node* GetOperatorWithBrackets(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    printf("Searching Op with bracket\n");

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_OPEN_BRACKET_2) {
        printf("Op with brackets is not found\n");
        return nullptr;
    }

    (*token_counter)++;
    
    Node* res_node = GetOperatorPlus(token_array, token_counter, name_table);

    token_array[*token_counter];

    // ShowGraphicDump(res_node, "graph.gv");

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_CLOSE_BRACKET_2)
        assert(!"skipped close bracket");

    (*token_counter)++;

    printf("SUCCESS Op with brackets\n");
    return res_node;
}

Node* GetOperator(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);
    
    if (token_array[*token_counter] == nullptr)
        return nullptr;

    // Check If
    Node* if_node = GetIf(token_array, token_counter, name_table);

    if (if_node != nullptr) return if_node;
        
    // Check While
    Node* while_node = GetWhile(token_array, token_counter, name_table);

    if (while_node != nullptr) return while_node;

    // Check Return
    Node* return_node = GetReturn(token_array, token_counter, name_table);

    if (return_node != nullptr) return return_node;

    // Check assignment
    Node* assign_node = GetAssign(token_array, token_counter, name_table);

    if (assign_node != nullptr) {

        if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
            assert(!"skipped end char");

        token_array[*token_counter]->left = assign_node;
        
        return token_array[*token_counter];
    }

    // Check { Op }
    Node* oper_w_brackets = GetOperatorWithBrackets(token_array, token_counter, name_table);

    if (oper_w_brackets == nullptr) return nullptr;

    return oper_w_brackets;
}

Node* GetCondition(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array   != nullptr);
    assert(token_counter != nullptr);
    assert(name_table    != nullptr);

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_OPEN_BRACKET_1)
        return nullptr;

    (*token_counter)++;

    Node* condition = GetExpression(token_array, token_counter, name_table);

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_CLOSE_BRACKET_1)
        assert(!"skipped close bracket");

    (*token_counter)++;

    return condition;
}

Node* GetIf(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    printf("Searching IF\n");

    int old_counter = *token_counter;
    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_IF) {
        printf("IF NOT FOUND\n");
        return nullptr;
    }

    printf("IF FOUND\n");

    (*token_counter)++;

    printf("Searching CONDITION\n");
    Node* condition = GetCondition(token_array, token_counter, name_table);

    if (condition == nullptr) {
        printf("IF NOT FOUND\n");
        return nullptr;
    }
        
    printf("Searching CONDITION BODY\n");
    Node* condition_body = GetOperator(token_array, token_counter, name_table);

    token_array[old_counter]->left = condition;
    token_array[old_counter]->right = condition_body;
    printf("SUCCESS IF\n");

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
        assert(!"skipped end char");

    token_array[*token_counter]->left = token_array[old_counter];

    return token_array[*token_counter];
}

Node* GetWhile(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    printf("Searching WHILE\n");

    int old_counter = *token_counter;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_WHILE) {
        printf("WHILE IS NOT FOUND\n");
        return nullptr;
    }
    (*token_counter)++;

    Node* condition = GetCondition(token_array, token_counter, name_table);

    if (condition == nullptr)
        return nullptr;

    Node* condition_body = GetOperator(token_array, token_counter, name_table);

    token_array[old_counter]->left  = condition;
    token_array[old_counter]->right = condition_body;

    printf("SUCCESS WHILE\n");

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
        assert(!"skipped end char");

    token_array[*token_counter]->left = token_array[old_counter];

    return token_array[*token_counter];
}

Node* GetReturn(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    printf("Searching Return\n");

    int old_counter = *token_counter;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_RETURN) {
        printf("Return IS NOT FOUND\n");
        return nullptr;
    }

    (*token_counter)++;

    Node* return_node = GetExpression(token_array, token_counter, name_table);

    token_array[old_counter]->left = return_node;

    return token_array[old_counter];
}


Node* GetFunc(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(name_table != nullptr);

    int old_counter = *token_counter;

    if (token_array[*token_counter] == nullptr)
        return nullptr;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_FUNC_DEFINITION)
        return nullptr;

    (*token_counter)++;
    
    Node* func_name = GetIdentifier(token_array, token_counter);
    func_name->data.type = FUNCTION;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_OPEN_BRACKET_1)
        return nullptr;

    (*token_counter)++;

    while (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_CLOSE_BRACKET_1) {

        NameTableInsertVar(name_table, token_array[*token_counter]->data.value.var.name);
        func_name->left = token_array[*token_counter];

        (*token_counter)++;
    }

    (*token_counter)++;
    
    Node* func_body = GetOperator(token_array, token_counter, name_table);

    token_array[old_counter]->left = func_name;
    token_array[old_counter]->right = func_body;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
        assert(!"skipped end char");

    token_array[*token_counter]->left = token_array[old_counter];

    return token_array[*token_counter];
}

Node* GetAssign(Node** token_array, int* token_counter, NameTable* name_table) {
    
    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* equal_node = token_array[*token_counter];

    printf("Searching ASSIGN\n");
    if (GET_OPERATOR_TYPE(equal_node) != OPERATOR_ASSIGN) {
        printf("ASSIGN IS NOT FOUND\n");
        return nullptr;
    }
        
    (*token_counter)++;

    Node* l_value = GetIdentifier(token_array, token_counter);
    // ShowGraphicDump(l_value, "graph.gv");
    if (l_value == nullptr)
        return nullptr;

    Node* to_node = token_array[*token_counter];

    if (GET_OPERATOR_TYPE(to_node) != OPERATOR_ASSIGN_TO)
        return nullptr;

    (*token_counter)++;

    Node* r_value = GetExpression(token_array, token_counter, name_table);
    // ShowGraphicDump(r_value, "graph.gv");

    if (r_value == nullptr)
        return nullptr;

    NameTableInsertVar(name_table, l_value->data.value.var.name);
    equal_node->left  = r_value;
    equal_node->right = l_value;

    printf("SUCCESS ASSIGN\n");
    printf("equal_node.code: %d", equal_node->data.value.operator_type);
    // ShowGraphicDump(equal_node, "graph.gv");

    return equal_node;
}

Node* GetIdentifier(Node** token_array, int* token_counter) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);

    if (GET_NODE_TYPE(token_array[*token_counter]) == VAR) {

        Node* res = token_array[*token_counter];
        (*token_counter)++;

        return res;
    }

    return nullptr;
}

Node* GetPrimary(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* val = 0;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_OPEN_BRACKET_1) {
        (*token_counter)++;

        val = GetExpression(token_array, token_counter, name_table);

        (*token_counter)++;
        return val;
    }

    int old_counter = *token_counter;
    val = GetNum(token_array, token_counter, name_table);

    if (old_counter == *token_counter) {
        val = GetIdentifier(token_array, token_counter);
        
        if (val != nullptr) {

            if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_OPEN_BRACKET_1)
                return val;

            (*token_counter)++;

            old_counter = *token_counter;

            Node* arg = GetExpression(token_array, token_counter, name_table);

            val->data.type = FUNCTION;
            val->left = arg;

            if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_CLOSE_BRACKET_1)
                assert(!"Skipped close bracket");

            (*token_counter)++;
        }
    }

    return val;
}

Node* GetTerm(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* first_val = GetPrimary(token_array, token_counter, name_table);

    while (GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_MUL ||
           GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_DIV)
    {
        Node* oper_node = token_array[*token_counter];

        (*token_counter)++;
        Node* second_val = GetPrimary(token_array, token_counter, name_table);

        oper_node->left = first_val;
        oper_node->right = second_val;

        first_val = oper_node;
    }

    return first_val;
}

Node* GetExpression(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* first_val = GetTerm(token_array, token_counter, name_table);

    while (GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_ADD ||
           GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_SUB)
    {

        Node* oper_node = token_array[*token_counter];

        (*token_counter)++;
        Node* second_val = GetTerm(token_array, token_counter, name_table);

        oper_node->left  = first_val;
        oper_node->right = second_val;

        first_val = oper_node;
    }

    return first_val;
}

Node* GetNum(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    if (token_array[*token_counter]->data.type == NUM)
    {
        Node* node = token_array[*token_counter];
        (*token_counter)++;

        return node;
    }

    return nullptr;
}

Node* GetGeneral(Node** token_array, ProgrammNameTables* table) {

    assert(table != nullptr);
    assert(token_array != nullptr);

    int token_counter = 0;

    Node* res_oper = GetFunc(token_array, &token_counter, &(table->local_tables[0]));

    NameTableInsertVar(&(table->funcs), res_oper->left->left->data.value.var.name);
    token_counter++;

    Node* res_var = res_oper;
    Node* next_oper = {};
    int func_counter = 1;

    do {
        next_oper = GetFunc(token_array, &token_counter, &(table->local_tables[func_counter]));
        
        if (next_oper != nullptr) {

            NameTableInsertVar(&(table->funcs), next_oper->left->left->data.value.var.name);
            func_counter++;
            token_counter++;
        }

        res_var->right = next_oper;
        res_var = res_var->right;

    } while ((next_oper != nullptr) && (GET_OPERATOR_TYPE(next_oper) == OPERATOR_END1));

    printf("End\n");

    table->size = func_counter;
    ShowGraphicDump(res_oper, "24_example.gv");

    return res_oper;
}