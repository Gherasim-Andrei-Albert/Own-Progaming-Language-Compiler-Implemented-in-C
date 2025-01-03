%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>


    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    int yylex();
    int yyerror(char* s);


    typedef enum RootType
    {
        INT_VAL_ROOT_TYPE, FLOAT_VAL_ROOT_TYPE,
        BOOL_VAL_ROOT_TYPE, CHAR_VAL_ROOT_TYPE,
        VARIABLE_ID_ROOT_TYPE, 
        OPERATOR_ROOT_TYPE, 
        PARANTHESIS_ROOT_TYPE, CALL_ROOT_TYPE, ARRAY_ACCESS_ROOT_TYPE,
        COMMA_ROOT_TYPE, ARRAY_INDEXING_ROOT_TYPE,
        INSTR_ROOT_TYPE
    } RootType;

    typedef enum Type
    {
        INT_TYPE, FLOAT_TYPE, CHAR_TYPE, BOOL_TYPE 
    } Type;

    typedef struct Tree
    {
        char* root;
        struct Tree* leftTree;
        struct Tree* rightTree;
        RootType rootType;
        enum Type type;
    } Tree;

    typedef struct _variable {
        char* name;
        //int constant;
        // tip variabila: 0 - int, 1 - float, 2 - char, 3 - bool, 4+ - structuri facute de user
        int type;
        // 1 sau dim. array
        int size;
        // sizeof
        int mem;
        void* addr;
    } variable;

    typedef struct _user_struct {
        char* name;
        int element_count;
        variable* elements;
        int size;
        int mem;
    } user_struct;

    typedef struct _function {
        char* name;
        int element_count;
        variable* elements;
        int type;
    } function;

    int max_variables = 10;
    int current_variables = 0;
    int max_functions = 10;
    int current_functions = 0;
    int max_structs = 10;
    int current_structs = 0;
    int global_temp;
    char* global_string = NULL;

    variable* variables;
    function* functions;
    user_struct* user_structs;

    void init() {
        variables = malloc(max_variables * sizeof(variable));
        functions = malloc(max_functions * sizeof(function));
        user_structs = malloc(max_structs * sizeof(user_struct));
        if(!(variables && functions && user_structs)) {
            yyerror("malloc init");
            exit(-1);
        }
    }

    void get_type_predefined(const char* type_name) {
        free(global_string);
        global_string = strdup(type_name);
        if(!strcmp(type_name, "int")) {
            global_temp = 0;
        } else if(!strcmp(type_name, "float")) {
            global_temp = 1;
        } else if(!strcmp(type_name, "char")) {
            global_temp = 2;
        } else if(!strcmp(type_name, "bool")) {
            global_temp = 3;
        }
    }

    void get_type_custom(const char* type_name) {
        free(global_string);
        global_string = strdup(type_name);
        int i; for(i = 0; i < current_structs; ++i) {
            if(!strcmp((user_structs + i)->name, type_name)) {
                global_temp = 4 + i;
                return;
            }
        }
        yyerror("tip date invalid");
        exit(-3);
    }

    void get_type_string(int type) {
        free(global_string);
        switch(type) {
            case 0:
                global_string = strdup("int");
                break;
            case 1:
                global_string = strdup("float");
                break;
            case 2:
                global_string = strdup("char");
                break;
            case 3:
                global_string = strdup("bool");
                break;
            default:
                type -= 4;
                global_string = strdup((user_structs + type)->name);
        }
    }

    void grow_struct(int var_func_struct) {
        void* temp;
        switch(var_func_struct) {
            case 0:
                ++current_variables;
                if(current_variables == max_variables) {
                    max_variables += 10;
                    temp = (variable*)realloc(variables, max_variables * sizeof(variable));
                    variables = temp;
                    if(!temp) {
                        yyerror("malloc grow_struct");
                        exit(-1);
                    }
                }
                break;
            case 1:
                ++current_functions;
                if(current_functions == max_functions) {
                    max_functions += 10;
                    if(!temp) {
                        yyerror("malloc grow_struct");
                        exit(-1);
                    }
                    temp = (function*)realloc(functions, max_functions * sizeof(function));
                    functions = temp;
                    if(!temp) {
                        yyerror("malloc grow_struct");
                        exit(-1);
                    }
                }
                break;
            case 2:
                ++current_structs;
                if(current_structs == max_structs) {
                    max_structs += 10;
                    temp = (user_struct*)realloc(user_structs, max_structs * sizeof(user_struct));
                    functions = temp;
                    if(!temp) {
                        yyerror("malloc grow_struct");
                        exit(-1);
                    }
                }
                break;
        }
    }

    void step_size(int struct_type, int* number, int* multipl) {
        switch(struct_type) {
            case 0:
                *number = current_variables;
                *multipl = sizeof(variable);
                break;
            case 1:
                *number = current_functions;
                *multipl = sizeof(function);
                break;
            case 2:
                *number = current_structs;
                *multipl = sizeof(user_struct);
                break;
        }
    }

    void duplicate_error(void* pool, int struct_type, int count, const char* name) {
        int number;
        int multipl;
        step_size(struct_type, &number, &multipl);
        if(count >= 0) {
            number = count;
        }
        int i; for(i = 0; i < number; ++i) {
            if(!strcmp(name, ((variable*)(pool + i * multipl))->name)) {
                yyerror("nume duplicat");
                exit(-2);
            }
        }
        ((variable*)(pool + number * multipl))->name = strdup(name);
    }

    int get_variable_mem(int type) {
        switch(type) {
            case 0:
            case 1:
                return 4;
            case 2:
            case 3:
                return 1;
        }
        return (user_structs + type - 4)->mem;
    }

    void add_variable(int variable_type, int variable_size, const char* name) {
        variable* var = (variables + current_variables);
        if(variable_size <= 0) {
            yyerror("marime invalida array");
            exit(-4);
        }
        int i; for(i = 0; i < current_variables; ++i) {
            if(!strcmp((variables + i)->name, name)) {
                yyerror("nume duplicat variabila");
                exit(-2);
            }
        }
        var->name = strdup(name);
        var->type = variable_type;
        var->size = variable_size;
        var->mem = get_variable_mem(variable_type) * variable_size;
        var->addr = malloc(var->mem);
    }

    void add_element(void* pool, int struct_type, int element_type, int element_size, const char* name) {
        if(element_size <= 0) {
            yyerror("marime invalida array");
            exit(-4);
        }
        int number;
        int multipl;
        void* temp;
        step_size(struct_type, &number, &multipl);
        user_struct* p = (user_struct*)(pool + number * multipl);
        number = p->element_count;
        int i; for(i = 0; i < number; ++i) {
            if(!strcmp(p->elements->name, name)) {
                yyerror("nume duplicat element");
                exit(-2);
            }
        }
        ++p->element_count;
        temp = realloc(p->elements, sizeof(variable) * p->element_count);
        p->elements = temp;
        if(!temp) {
            yyerror("malloc add_element");
            exit(-1);
        }
        number = get_variable_mem(element_type);
        if(struct_type == 2) {
            p->mem += number;
        }
        (p->elements + p->element_count - 1)->type = element_type;
        (p->elements + p->element_count - 1)->size = element_size;
        (p->elements + p->element_count - 1)->mem = number;
        (p->elements + p->element_count - 1)->name = strdup(name);
    }

    void print_verif(int type) {
        switch(type) {
            case 0:
                printf("\nVERIF VARIABLES: %d\n", current_variables);
                int i; for(i = 0; i < current_variables; ++i) {
                    get_type_string((variables + i)->type);
                    printf("name: %s  -  type: %s  -  size: %d  -  mem: %d\n",
                        (variables + i)->name, global_string, (variables + i)->size, (variables + i)->mem);
                }
                break;
            case 1:
                printf("\nVERIF FUNCTIONS: %d\n", current_functions);
                variable* ptr;
                for(i = 0; i < current_functions; ++i) {
                    printf("name: %s  -  param_count: %d  -  type: %d\n",
                    (functions + i)->name, (functions + i)->element_count, (functions + i)->type);
                    ptr = (functions + i)->elements;
                    int j; for(j = 0; j < (functions + i)->element_count; ++j) {
                        get_type_string(ptr->type);
                        printf("param - name: %s  -  type: %s  -  size: %d  -  mem: %d\n", ptr->name, global_string, ptr->size, ptr->mem);
                        ++ptr;
                    }
                    printf("\n");
                }
                break;
            case 2:
                printf("\nVERIF STRUCTS: %d\n", current_structs);
                for(i = 0; i < current_structs; ++i) {
                    printf("name: %s  -  elem_count: %d  -  mem: %d\n",
                    (user_structs + i)->name, (user_structs + i)->element_count, (user_structs + i)->mem);
                    int j; for(j = 0; j < (user_structs + i)->element_count; ++j) {
                        variable* s2 = (user_structs + i)->elements;
                        get_type_string((s2 + j)->type);
                        printf("elem name: %s  -  size: %d  -  type: %s  -  mem: %d\n", (s2 + j)->name, (s2 + j)->size, global_string, (s2 + j)->mem);
                    }
                    printf("\n");
                }
                break;
        }
    }

    void symbol_tables() {
        FILE* f;
        f = fopen("symbol_table.txt", "w");
        int i1; for(i1 = 0; i1 < current_variables; ++i1) {
            get_type_string((variables + i1)->type);
            fprintf(f, "name: %s  -  type: %s  -  size: %d  -  mem: %d\n",
                (variables + i1)->name, global_string, (variables + i1)->size, (variables + i1)->mem);
        }
        fclose(f);
        f = fopen("symbol_table_functions.txt", "w");
        variable* ptr;
        int i2; for(i2 = 0; i2 < current_functions; ++i2) {
            fprintf(f, "name: %s  -  param_count: %d  -  type: %d\n",
            (functions + i2)->name, (functions + i2)->element_count, (functions + i2)->type);
            ptr = (functions + i2)->elements;
            int j; for(j = 0; j < (functions + i2)->element_count; ++j) {
                get_type_string(ptr->type);
                fprintf(f, "param - name: %s  -  type: %s  -  size: %d  -  mem: %d\n", ptr->name, global_string, ptr->size, ptr->mem);
                ++ptr;
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }

    void verif_all() {
        print_verif(2);
        print_verif(0);
        print_verif(1);
        symbol_tables();
    }

    void buildAST(char* root, Tree* leftTree, Tree* rightTree, RootType rootType, Tree* tree)
    {
        tree->root = root;
        tree->leftTree = leftTree;
        tree->rightTree = rightTree;
        tree->rootType = rootType;
    }

    int precedence(char* operator)
    {
        int sizes[3] = {5, 4, 3};
        char* precedenceTable[100][100]= 
        {
            { "<", ">", "==", ">=", "<="},
            { "*", "/", "&&", "!" },
            { "+", "-", "||" }
        };
        int line;
        for(line = 0; line < 3; ++ line)
        {
            int col;
            for(col = 0; col < sizes[line]; ++ col)
            {
                if(strcmp(precedenceTable[line][col], operator) == 0)
                {
                    return line;
                }
            }
        }
        return 0;
    }

    void buildASTForOperator(char* root, Tree* leftTree, Tree* rightTree, RootType rootType, Tree* tree, Tree* aux) {
        printf("%s\n", root);
        if(leftTree->rootType == OPERATOR_ROOT_TYPE 
            && precedence(leftTree->root) > precedence(root))
        {
            printf("found bigger precedence beneath\n");
            printf("precedence root:%d left:%d\n", precedence(leftTree->root), precedence(root));

            buildAST(root, leftTree->rightTree, rightTree, OPERATOR_ROOT_TYPE, tree);
            leftTree->rightTree = tree;


            aux->root = strdup(tree->root);
            aux->leftTree = tree->leftTree;
            aux->rightTree = tree->rightTree;
            aux->rootType = tree->rootType;
            aux->type = tree->type;

            leftTree->rightTree = aux;
            
            tree->root = strdup(leftTree->root);
            tree->leftTree = leftTree->leftTree;
            tree->rightTree = leftTree->rightTree;
            tree->rootType = leftTree->rootType;

            printf("%s <- %s -> %s\n", tree->leftTree->root, tree->root, tree->rightTree->root);
            printf("%s <- %s -> %s\n", tree->rightTree->leftTree->root, tree->rightTree->root, tree->rightTree->rightTree->root);
        }
        else
        {
            printf("precedence ok\n");
            printf("precedence root:%d left:%d\n", precedence(leftTree->root), precedence(root));

            buildAST(root, leftTree, rightTree, OPERATOR_ROOT_TYPE, tree);

            printf("%s <- %s -> %s\n", tree->leftTree->root, tree->root, tree->rightTree->root);
        }
    }

    enum Type typeOf(Tree *tree)
    {
        enum Type type;

        if(tree->rootType == INT_VAL_ROOT_TYPE) {
            type = INT_TYPE;
            printf("int type %d\n", type);
        }
        if(tree->rootType == FLOAT_VAL_ROOT_TYPE) {
            type = FLOAT_TYPE;
            printf("float type %d\n", type);
        }
        if(tree->rootType == BOOL_VAL_ROOT_TYPE) {
            type = BOOL_TYPE;
            printf("bool type %d\n", type);
        }
        if(tree->rootType == CHAR_TYPE) {
            type = CHAR_TYPE;
            printf("char type %d\n", type);
        }

        if(tree->rootType == VARIABLE_ID_ROOT_TYPE) {
            printf("check type for variable\n");
            int i; for(i = 0; i < current_variables; ++i) {
                if(strcmp((variables + i)->name, tree->root)) {
                    if((variables + i)->type <= 3)
                    {
                        printf("found %d\n", (variables + i)->type);
                        tree->type = (variables + i)->type;
                    }
                }
            }
        }
        
        if(tree->rootType == OPERATOR_ROOT_TYPE)
        {
            if(strcmp("+", tree->root) == 0)
            {
                type = INT_TYPE;
                int rightType = typeOf(tree->rightTree);
                int leftType = typeOf(tree->leftTree);
                if(rightType == FLOAT_TYPE || leftType == FLOAT_TYPE)
                    type = FLOAT_TYPE;
                printf("+ type %d\n", type);
            }

            if(strcmp("-", tree->root) == 0)
            {
                type = INT_TYPE;
                int rightType = typeOf(tree->rightTree);
                int leftType = typeOf(tree->leftTree);
                if(rightType == FLOAT_TYPE || leftType == FLOAT_TYPE)
                    type = FLOAT_TYPE;
                printf("- type %d\n", type);
            }

            if(strcmp("*", tree->root) == 0)
            {
                type = INT_TYPE;
                int rightType = typeOf(tree->rightTree);
                int leftType = typeOf(tree->leftTree);
                if(rightType == FLOAT_TYPE || leftType == FLOAT_TYPE)
                    type = FLOAT_TYPE;
                printf("* type %d\n", type);
            }

            if(strcmp("/", tree->root) == 0)
            {
                type = INT_TYPE;
                int rightType = typeOf(tree->rightTree);
                int leftType = typeOf(tree->leftTree);
                if(rightType == FLOAT_TYPE || leftType == FLOAT_TYPE)
                    type = FLOAT_TYPE;
                printf("/ type %d\n", type);
            }

            if(strcmp("<", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("< type %d\n", type);
            }

            if(strcmp("<=", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("<= type %d\n", type);
            }

            if(strcmp(">", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("> type %d\n", type);
            }

            if(strcmp(">=", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf(">= type %d\n", type);
            }

            if(strcmp("==", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("== type %d\n", type);
            }

            if(strcmp("&&", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("&& type %d\n", type);
            }

            if(strcmp("||", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("|| type %d\n", type);
            }

            if(strcmp("!", tree->root) == 0)
            {
                type = BOOL_TYPE;
                printf("! type %d\n", type);
            }    
        }

        if(tree->rootType == PARANTHESIS_ROOT_TYPE)
        {
            if(strcmp("()", tree->root) == 0)
            {
                type = typeOf(tree->rightTree);
                printf("() type %d\n", type);
            }
        }

        tree->type = type;
        return type;
    }

    typedef struct EvalASTResult
    {
        float floatVal;
        bool boolVal;
        char charVal;
        enum Type type;
    } EvalASTResult;

    struct EvalASTResult evalAST(Tree* tree)
    {
        // printf("%s <- %s -> %s\n", 
        //     tree->leftTree ? tree->leftTree->root : "NULL", 
        //     tree->root, 
        //     tree->rightTree ? tree->rightTree->root : "NULL"
        // );

        int resultType;

        float floatVal = 0;
        bool boolVal = false;
        char charVal = ' ';

        if(tree->rootType == INT_VAL_ROOT_TYPE) {
            floatVal = atoi(tree->root);
            printf("int val %f\n", floatVal);
        }

        if(tree->rootType == FLOAT_VAL_ROOT_TYPE) {
            floatVal = atof(tree->root);
            printf("float val %f\n", floatVal);
        }

        if(tree->rootType == BOOL_VAL_ROOT_TYPE) {
            boolVal = strcmp("true", tree->root) == 0;
            printf("bool val %d\n", boolVal);
        }

        if(tree->rootType == CHAR_VAL_ROOT_TYPE) {
            charVal = (tree->root)[1];
            printf("char val %c\n", charVal);
        }

        if(tree->rootType == VARIABLE_ID_ROOT_TYPE) {
            printf("search for var |%s|\n", tree->root);
            int i; for(i = 0; i < current_variables; ++i) {
                printf("iterat |%s|\n", (variables + i)->name);
                if(strcmp((variables + i)->name, tree->root) == 0) {
                    printf("found %s\n", (variables + i)->name);
                    if((variables + i)->type == INT_TYPE || (variables + i)->type == FLOAT_TYPE)
                    {
                        printf("found %s\n", (variables + i)->name);
                        floatVal = *((float*)((variables + i)->addr));
                        
                        printf("found variable val %f\n", *((float*)((variables + i)->addr)));
                        printf("variable val %f\n", floatVal);
                    }
                }
            }
            
            printf("variable val %c\n", charVal);
        }

        if(tree->rootType == OPERATOR_ROOT_TYPE)
        {
            if(strcmp("+", tree->root) == 0)
            {
                floatVal =
                    evalAST(tree->leftTree).floatVal + evalAST(tree->rightTree).floatVal;

                printf("+ val %f\n", floatVal);
            }

            if(strcmp("-", tree->root) == 0)
            {
                floatVal =
                    evalAST(tree->leftTree).floatVal - evalAST(tree->rightTree).floatVal;

                printf("- val %f\n", floatVal);
            }

            if(strcmp("*", tree->root) == 0)
            {
                floatVal =
                    evalAST(tree->leftTree).floatVal * evalAST(tree->rightTree).floatVal;

                printf("* val %f\n", floatVal);
            }

            if(strcmp("/", tree->root) == 0)
            {
                
                floatVal =
                    evalAST(tree->leftTree).floatVal / evalAST(tree->rightTree).floatVal;

                printf("/ val %f\n", floatVal);
            }

            if(strcmp("<", tree->root) == 0)
            {
                boolVal =
                    evalAST(tree->leftTree).floatVal < evalAST(tree->rightTree).floatVal;

                printf("< val %d\n", boolVal);
            }

            if(strcmp("<=", tree->root) == 0)
            {
                boolVal = 
                    evalAST(tree->leftTree).floatVal <= evalAST(tree->rightTree).floatVal;
                printf("<= val %d\n", boolVal);
            }

            if(strcmp(">", tree->root) == 0)
            {
                boolVal = 
                    evalAST(tree->leftTree).floatVal > evalAST(tree->rightTree).floatVal;
                printf("> val %d\n", boolVal);
            }

            if(strcmp(">", tree->root) == 0)
            {
                boolVal = 
                    evalAST(tree->leftTree).floatVal > evalAST(tree->rightTree).floatVal;
                printf("> val %d\n", boolVal);
            }

            if(strcmp(">=", tree->root) == 0)
            {
                boolVal = 
                    evalAST(tree->leftTree).floatVal >= evalAST(tree->rightTree).floatVal;
                printf(">= val %d\n", boolVal);
            }

            if(strcmp("==", tree->root) == 0)
            {  
                if(tree->leftTree->type == CHAR_TYPE)
                    boolVal = 
                        evalAST(tree->leftTree).charVal == evalAST(tree->rightTree).charVal;

                if(tree->leftTree->type == BOOL_TYPE)
                    boolVal = 
                        evalAST(tree->leftTree).boolVal == evalAST(tree->rightTree).boolVal;

                if(tree->leftTree->type == INT_TYPE || tree->leftTree->type == FLOAT_TYPE)
                    boolVal = 
                        evalAST(tree->leftTree).floatVal == evalAST(tree->rightTree).floatVal;
                
                printf("== val %d\n", boolVal);
            }

            if(strcmp("&&", tree->root) == 0)
            {
                boolVal = 
                    evalAST(tree->leftTree).boolVal && evalAST(tree->rightTree).boolVal;
                printf("&& val %d\n", boolVal);
            }

            if(strcmp("||", tree->root) == 0)
            {
                boolVal = 
                    evalAST(tree->leftTree).boolVal || evalAST(tree->rightTree).boolVal;
                printf("|| val %d\n", boolVal);
            }

            if(strcmp("!", tree->root) == 0)
            {
                boolVal = ! evalAST(tree->rightTree).boolVal;
                printf("! val %d\n", boolVal);
            }    
        }

        if(tree->rootType == PARANTHESIS_ROOT_TYPE)
        {
            if(strcmp("()", tree->root) == 0)
            {
                if(tree->rightTree->type == CHAR_TYPE)
                {
                    charVal = (evalAST(tree->rightTree).charVal);
                    printf("() val %d\n", charVal);
                }

                if(tree->rightTree->type == BOOL_TYPE)
                {
                    boolVal = (evalAST(tree->rightTree).boolVal);
                    printf("() val %d\n", boolVal);
                }

                if(tree->rightTree->type == INT_TYPE || tree->rightTree->type == FLOAT_TYPE)
                {
                    floatVal = (evalAST(tree->rightTree).floatVal);
                    printf("() val %f\n", floatVal);
                }
            }
        }

        struct EvalASTResult result;
        result.floatVal = floatVal;
        result.boolVal = boolVal;
        result.charVal = charVal;
        result.type = tree->type;
        return result;
    }
    
/*



*/

%}

%union {
    char* text;
    struct Tree* treeType;
}

%token <text> ID FLOAT INT CHAR BOOL
    PLUS MINUS DIV STAR 
    GT GT_OR_EQUAL LT LT_OR_EQUAL EQUAL 
    AND OR NOT
    L_ROUND_PARA R_ROUND_PARA L_SQUARE_PARA R_SQUARE_PARA
    L_COURLY_PARA R_COURLY_PARA 
    FOR IF WHILE ELSE
    PREDEFINED_TYPE
    ASSIGN
    COMMA SEMI_COLON DOT 
    CONST
    START_MAIN
    START_VARIABLES
    START_STRUCTS
    START_FUNCTIONS


%type <treeType> expressionTerm expression expressionsList arrayAccessList
    codeBlock assignment instruction instructionList vDeclaration

%start program

%%

// test : expression SEMI_COLON {
//     printf("Tree available here in $1. For example it's root is %s\n", $1->root);
// };

program : { init(); } structBlock variableBlock functionBlock mainBlock { verif_all(); }

structBlock : START_STRUCTS L_COURLY_PARA structDeclarations R_COURLY_PARA
            | %empty
            ;

structDeclarations : sDecl SEMI_COLON structDeclarations
                   | %empty
                   ;

sDecl : ID { duplicate_error((void*)user_structs, 2, -1, $1); (user_structs + current_structs)->elements = NULL; (user_structs + current_structs)->element_count = 0; } L_COURLY_PARA sContent R_COURLY_PARA { grow_struct(2); }
      | %empty
      ;

sContent : sContent sContent
         | elementType ID { add_element((void*)user_structs, 2, global_temp, 1, $2); } SEMI_COLON
         | elementType ID L_SQUARE_PARA INT { add_element((void*)user_structs, 2, global_temp, atoi(yytext), $2); } R_SQUARE_PARA SEMI_COLON
         ;

elementType : PREDEFINED_TYPE { get_type_predefined(yytext); }
            | ID { get_type_custom(yytext); }
            ;

variableBlock : START_VARIABLES L_COURLY_PARA gvariableDeclarations R_COURLY_PARA
              | %empty
              ;

gvariableDeclarations : vDeclaration { grow_struct(0); } SEMI_COLON gvariableDeclarations
                      | %empty
                      ;

vDeclaration : elementType ID { 
        add_variable(global_temp, 1, $2); 
        Tree *typeTree = malloc(sizeof(struct Tree));
        buildAST(global_string, NULL, NULL, VARIABLE_ID_ROOT_TYPE, typeTree);

        Tree *IDTree = malloc(sizeof(struct Tree));
        buildAST($2, NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);

        $<treeType>$ = malloc(sizeof(struct Tree));
        buildAST("decl", typeTree, IDTree, INSTR_ROOT_TYPE, $<treeType>$);

        printf("%s <- %s -> %s\n", $<treeType>$->leftTree->root, $<treeType>$->root, $<treeType>$->rightTree->root);
    }
             | elementType ID L_SQUARE_PARA INT {
                add_variable(global_temp, atoi(yytext), $2);
                Tree *typeTree = malloc(sizeof(struct Tree));
                buildAST(global_string, NULL, NULL, VARIABLE_ID_ROOT_TYPE, typeTree);

                Tree *IDTree = malloc(sizeof(struct Tree));
                buildAST($2, NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);

                $<treeType>$ = malloc(sizeof(struct Tree));
                buildAST("decl", typeTree, IDTree, INSTR_ROOT_TYPE, $<treeType>$);

                printf("%s <- %s -> %s\n", $<treeType>$->leftTree->root, $<treeType>$->root, $<treeType>$->rightTree->root);
    } R_SQUARE_PARA

functionBlock : START_FUNCTIONS L_COURLY_PARA functionDeclarations R_COURLY_PARA
              | %empty
              ;

functionDeclarations : functionDeclaration SEMI_COLON functionDeclarations
                     | %empty
                     ;

functionDeclaration : elementType ID { duplicate_error((void*)functions, 1, -1, $2); (functions + current_functions)->elements = NULL; (functions + current_functions)->element_count = 0; (functions + current_functions)->type = global_temp; } L_ROUND_PARA parameterDeclarations R_ROUND_PARA L_COURLY_PARA R_COURLY_PARA { grow_struct(1); }

parameterDeclarations : parameterDeclaration COMMA parameterDeclaration
                      | %empty
                      ;

parameterDeclaration : elementType ID { add_element((void*)(functions + current_functions), 1, global_temp, 1, $2); }
                     | elementType ID L_SQUARE_PARA INT { add_element((void*)(functions + current_functions), 1, global_temp, atoi(yytext), $2); } R_SQUARE_PARA

mainBlock : START_MAIN L_COURLY_PARA instructionList R_COURLY_PARA
          | %empty;
          ;

instructionList : instructionList instruction SEMI_COLON
                  {
                      $$ = malloc(sizeof(struct Tree));
                      buildAST(";", $1, $2, INSTR_ROOT_TYPE, $$);

                      if($$->leftTree!=NULL && $$->rightTree!=NULL) 
                        printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);
                      else
                        printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                  }
                | %empty
                  {
                      $$ = NULL;
                  }
                ;

instruction : codeBlock
            | vDeclaration
            | assignment
            // | ifControlFlow
            // | whileControlFlow
            ;

codeBlock : L_COURLY_PARA instructionList R_COURLY_PARA
            {
                $$ = malloc(sizeof(struct Tree));
                buildAST("{}", NULL, $2, INSTR_ROOT_TYPE, $$);

                if($$->leftTree!=NULL && $$->rightTree!=NULL) 
                    printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);
                if($$->rightTree!=NULL) 
                    printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                else
                    printf("NULL <- %s -> NULL\n", $$->root);
            }
          ;

// vDeclaration : PREDEFINED_TYPE ID
//                {
//                    Tree *typeTree = malloc(sizeof(struct Tree));
//                    buildAST($1, NULL, NULL, VARIABLE_ID_ROOT_TYPE, typeTree);

//                    Tree *IDTree = malloc(sizeof(struct Tree));
//                    buildAST($2, NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);

//                    $$ = malloc(sizeof(struct Tree));
//                    buildAST("decl", typeTree, IDTree, INSTR_ROOT_TYPE, $$);

//                    printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);
//                }
//              ;

assignment : ID ASSIGN expression
             {
                 Tree *IDTree = malloc(sizeof(struct Tree));
                 buildAST($1, NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);
                 
                 $$ = malloc(sizeof(struct Tree));
                 buildAST("=", $3, IDTree, INSTR_ROOT_TYPE, $$);

                 printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);

                 
                 printf("\n-------------------------------------------\n\n");
                 printf("type of expression:\n");
                 printf("----------------\n\n");

                 typeOf($3);

                 printf("\n-------------------------------------------\n\n");
                 

                 printf("\n-------------------------------------------\n\n");
                 printf("eval expression:\n");
                 printf("----------------\n\n");

                 struct EvalASTResult evalResult = evalAST($3);

                 printf("\n-------------------------------------------\n\n");

                 
                 int i; for(i = 0; i < current_variables; ++i) {
                     printf("iterat |%s|\n", (variables + i)->name);
                     if(strcmp((variables + i)->name, $1) == 0) {
                         printf("found %s\n", (variables + i)->name);
                         if((variables + i)->type == INT_TYPE || (variables + i)->type == FLOAT_TYPE)
                         {
                             printf("found %s\n", (variables + i)->name);
                             *((float*)((variables + i)->addr)) = evalResult.floatVal;
                             printf("variable val %f\n", *((float*)((variables + i)->addr)));
                         }
                     }
                 }

             }
           ;

expression : expressionTerm
           | expression PLUS expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression MINUS expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression STAR expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression DIV expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression OR expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression AND expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression EQUAL expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression GT expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression GT_OR_EQUAL expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression LT expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           | expression LT_OR_EQUAL expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$, aux);
             }
           ;

expressionTerm : INT
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, INT_VAL_ROOT_TYPE, $$);
                     printf("INT %s\n", $$->root);
                 }
               | FLOAT
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, FLOAT_VAL_ROOT_TYPE, $$);
                     printf("FLOAT %s\n", $$->root);
                 }
               | BOOL
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, BOOL_VAL_ROOT_TYPE, $$);
                     printf("BOOL %s\n", $$->root);
                 }
               | CHAR
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, CHAR_VAL_ROOT_TYPE, $$);
                     printf("CHAR %s\n", $$->root);
                 }
               | ID 
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, VARIABLE_ID_ROOT_TYPE, $$);
                     printf("VARIABLE_ID %s\n", $$->root);
                 }
               | L_ROUND_PARA expression R_ROUND_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST("()", NULL, $2, PARANTHESIS_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | ID L_ROUND_PARA R_ROUND_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, CALL_ROOT_TYPE, $$);
                     printf("NULL <- %s -> NULL\n", $$->root);
                 }
               | ID L_ROUND_PARA expressionsList R_ROUND_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, $3, CALL_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | ID arrayAccessList
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, $2, ARRAY_ACCESS_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | NOT expressionTerm
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, $2, OPERATOR_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               ;

expressionsList : expression
                | expressionsList COMMA expression
                  {
                      $$ = malloc(sizeof(struct Tree));
                      buildAST($2, $1, $3, COMMA_ROOT_TYPE, $$);
                      printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);
                  }
               ;

arrayAccessList : L_SQUARE_PARA expression R_SQUARE_PARA
                  {
                      $$ = malloc(sizeof(struct Tree));
                      buildAST("[]", NULL, $2, ARRAY_INDEXING_ROOT_TYPE, $$);
                      printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                  }
                | arrayAccessList L_SQUARE_PARA expression R_SQUARE_PARA
                  {
                      $$ = malloc(sizeof(struct Tree));
                      buildAST("[]", $1, $3, ARRAY_INDEXING_ROOT_TYPE, $$);
                      printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);
                  }
               ;


%%

int yyerror(char* s){
    printf("eroare %s la linia %d\n",s,yylineno);
}

int main(int argc,char** argv){
    yyin = fopen(argv[1],"r");
    yyparse();
}

