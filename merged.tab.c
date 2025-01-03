/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "merged.y" /* yacc.c:339  */

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


#line 838 "merged.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "merged.tab.h".  */
#ifndef YY_YY_MERGED_TAB_H_INCLUDED
# define YY_YY_MERGED_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    FLOAT = 259,
    INT = 260,
    CHAR = 261,
    BOOL = 262,
    PLUS = 263,
    MINUS = 264,
    DIV = 265,
    STAR = 266,
    GT = 267,
    GT_OR_EQUAL = 268,
    LT = 269,
    LT_OR_EQUAL = 270,
    EQUAL = 271,
    AND = 272,
    OR = 273,
    NOT = 274,
    L_ROUND_PARA = 275,
    R_ROUND_PARA = 276,
    L_SQUARE_PARA = 277,
    R_SQUARE_PARA = 278,
    L_COURLY_PARA = 279,
    R_COURLY_PARA = 280,
    FOR = 281,
    IF = 282,
    WHILE = 283,
    ELSE = 284,
    PREDEFINED_TYPE = 285,
    ASSIGN = 286,
    COMMA = 287,
    SEMI_COLON = 288,
    DOT = 289,
    CONST = 290,
    START_MAIN = 291,
    START_VARIABLES = 292,
    START_STRUCTS = 293,
    START_FUNCTIONS = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 773 "merged.y" /* yacc.c:355  */

    char* text;
    struct Tree* treeType;

#line 923 "merged.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MERGED_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 938 "merged.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   151

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  132

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   806,   806,   806,   808,   809,   812,   813,   816,   816,
     817,   820,   821,   821,   822,   822,   825,   826,   829,   830,
     833,   833,   834,   837,   850,   850,   864,   865,   868,   869,
     872,   872,   874,   875,   878,   879,   879,   881,   882,   885,
     895,   901,   902,   903,   908,   937,   985,   986,   992,   998,
    1004,  1010,  1016,  1022,  1028,  1034,  1040,  1046,  1054,  1060,
    1066,  1072,  1078,  1084,  1090,  1096,  1102,  1108,  1116,  1117,
    1125,  1131
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "FLOAT", "INT", "CHAR", "BOOL",
  "PLUS", "MINUS", "DIV", "STAR", "GT", "GT_OR_EQUAL", "LT", "LT_OR_EQUAL",
  "EQUAL", "AND", "OR", "NOT", "L_ROUND_PARA", "R_ROUND_PARA",
  "L_SQUARE_PARA", "R_SQUARE_PARA", "L_COURLY_PARA", "R_COURLY_PARA",
  "FOR", "IF", "WHILE", "ELSE", "PREDEFINED_TYPE", "ASSIGN", "COMMA",
  "SEMI_COLON", "DOT", "CONST", "START_MAIN", "START_VARIABLES",
  "START_STRUCTS", "START_FUNCTIONS", "$accept", "program", "$@1",
  "structBlock", "structDeclarations", "sDecl", "$@2", "sContent", "$@3",
  "$@4", "elementType", "variableBlock", "gvariableDeclarations", "$@5",
  "vDeclaration", "@6", "functionBlock", "functionDeclarations",
  "functionDeclaration", "$@7", "parameterDeclarations",
  "parameterDeclaration", "$@8", "mainBlock", "instructionList",
  "instruction", "codeBlock", "assignment", "expression", "expressionTerm",
  "expressionsList", "arrayAccessList", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

#define YYPACT_NINF -76

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-76)))

#define YYTABLE_NINF -11

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -76,    14,   -33,   -76,    -5,   -17,    -1,    -2,   -16,   -76,
       2,    -9,     8,    20,     9,    22,   -76,    -1,   -76,   -76,
      45,    24,   -76,     8,    27,   -76,     8,   -76,    30,   -76,
      21,    50,    33,    26,   -76,     5,    74,    73,     8,   -76,
     -76,     8,     1,   -76,     8,    57,   -76,   -76,    60,   -76,
      51,   -76,   -76,   -76,    48,   -76,   -76,    78,    52,    61,
       8,    69,     4,   -76,   -76,   -76,   -76,    83,    66,    70,
      -7,   -76,   -76,   -76,   -76,    69,    69,   128,   -76,   -76,
      80,    79,    93,     8,    36,    69,    96,   -76,   114,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      71,   115,    94,   -76,   -76,   128,   -15,    82,    69,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,   -76,   -76,   -76,    69,   -76,    98,   110,   128,
     -76,   -76
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     5,     1,     0,    19,     7,     0,    27,     8,
       0,     0,    22,     0,    38,     0,     4,     7,    17,    16,
       0,     0,    20,    29,     0,     3,     0,     6,    23,    18,
       0,     0,     0,     0,    40,     0,     0,     0,    22,    30,
      26,    29,     0,     9,    11,    12,    24,    21,     0,    28,
      17,    40,    37,    42,     0,    41,    43,     0,     0,     0,
      33,     0,     0,    39,    14,    13,    25,     0,     0,     0,
      62,    59,    58,    61,    60,     0,     0,    45,    46,    44,
       0,    34,     0,     0,     0,     0,    66,    67,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    32,    64,    68,     0,     0,     0,    63,
      47,    48,    50,    49,    54,    55,    56,    57,    53,    52,
      51,    15,    35,    31,    65,     0,    70,     0,     0,    69,
      71,    36
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -76,   -76,   -76,   -76,   117,   -76,   -76,   121,   -76,   -76,
     -23,   -76,   111,   -76,   -26,   -76,   -76,   107,   -76,   -76,
     -76,    67,   -76,   -76,   100,   -76,   -76,   -76,   -75,   -28,
     -76,   -76
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,    10,    11,    15,    44,    58,    80,
      20,     8,    21,    30,    22,    59,    14,    32,    33,    48,
      68,    69,   128,    25,    42,    54,    55,    56,    77,    78,
     106,    86
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,    88,     9,    36,    50,     4,   124,    50,    18,   105,
     107,    18,    36,    84,     3,    85,    53,   125,    31,     6,
       7,    36,    12,    13,    17,    51,    52,    16,    51,    79,
      43,    19,   -10,   127,    19,    19,    53,    67,    19,    70,
      71,    72,    73,    74,    23,    24,    26,    87,    28,    29,
     129,    34,    37,    39,    38,    75,    76,   104,    40,    41,
      67,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    70,    71,    72,    73,    74,    45,    46,    57,
      60,    63,    61,    64,    66,    65,    81,    82,    75,    76,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   101,    83,   100,   121,   126,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   102,   108,   123,
     122,   130,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   131,    27,   109,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    35,    49,    47,
     103,    62
};

static const yytype_uint8 yycheck[] =
{
      23,    76,     3,    26,     3,    38,    21,     3,     3,    84,
      85,     3,    35,    20,     0,    22,    42,    32,    41,    24,
      37,    44,    24,    39,    33,    24,    25,    25,    24,    25,
      25,    30,    33,   108,    30,    30,    62,    60,    30,     3,
       4,     5,     6,     7,    24,    36,    24,    75,     3,    25,
     125,    24,    22,     3,    33,    19,    20,    21,    25,    33,
      83,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,     3,     4,     5,     6,     7,     3,     5,    22,
      20,    33,    31,     5,    23,    33,     3,    21,    19,    20,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    22,    32,    23,    33,    23,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    24,    22,    25,
       5,    23,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    23,    17,    21,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    26,    41,    38,
      83,    51
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    41,    42,     0,    38,    43,    24,    37,    51,     3,
      44,    45,    24,    39,    56,    46,    25,    33,     3,    30,
      50,    52,    54,    24,    36,    63,    24,    44,     3,    25,
      53,    50,    57,    58,    24,    47,    50,    22,    33,     3,
      25,    33,    64,    25,    47,     3,     5,    52,    59,    57,
       3,    24,    25,    54,    65,    66,    67,    22,    48,    55,
      20,    31,    64,    33,     5,    33,    23,    50,    60,    61,
       3,     4,     5,     6,     7,    19,    20,    68,    69,    25,
      49,     3,    21,    32,    20,    22,    71,    69,    68,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      23,    22,    24,    61,    21,    68,    70,    68,    22,    21,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    33,     5,    25,    21,    32,    23,    68,    62,    68,
      23,    23
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    42,    41,    43,    43,    44,    44,    46,    45,
      45,    47,    48,    47,    49,    47,    50,    50,    51,    51,
      53,    52,    52,    54,    55,    54,    56,    56,    57,    57,
      59,    58,    60,    60,    61,    62,    61,    63,    63,    64,
      64,    65,    65,    65,    66,    67,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    70,    70,
      71,    71
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     5,     4,     0,     3,     0,     0,     5,
       0,     2,     0,     4,     0,     7,     1,     1,     4,     0,
       0,     4,     0,     2,     0,     6,     4,     0,     3,     0,
       0,     8,     3,     0,     2,     0,     6,     4,     0,     3,
       0,     1,     1,     1,     3,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     3,     3,     4,     2,     2,     1,     3,
       3,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 806 "merged.y" /* yacc.c:1646  */
    { init(); }
#line 2118 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 806 "merged.y" /* yacc.c:1646  */
    { verif_all(); }
#line 2124 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 816 "merged.y" /* yacc.c:1646  */
    { duplicate_error((void*)user_structs, 2, -1, (yyvsp[0].text)); (user_structs + current_structs)->elements = NULL; (user_structs + current_structs)->element_count = 0; }
#line 2130 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 816 "merged.y" /* yacc.c:1646  */
    { grow_struct(2); }
#line 2136 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 821 "merged.y" /* yacc.c:1646  */
    { add_element((void*)user_structs, 2, global_temp, 1, (yyvsp[0].text)); }
#line 2142 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 822 "merged.y" /* yacc.c:1646  */
    { add_element((void*)user_structs, 2, global_temp, atoi(yytext), (yyvsp[-2].text)); }
#line 2148 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 825 "merged.y" /* yacc.c:1646  */
    { get_type_predefined(yytext); }
#line 2154 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 826 "merged.y" /* yacc.c:1646  */
    { get_type_custom(yytext); }
#line 2160 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 833 "merged.y" /* yacc.c:1646  */
    { grow_struct(0); }
#line 2166 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 837 "merged.y" /* yacc.c:1646  */
    { 
        add_variable(global_temp, 1, (yyvsp[0].text)); 
        Tree *typeTree = malloc(sizeof(struct Tree));
        buildAST(global_string, NULL, NULL, VARIABLE_ID_ROOT_TYPE, typeTree);

        Tree *IDTree = malloc(sizeof(struct Tree));
        buildAST((yyvsp[0].text), NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);

        (yyval.treeType) = malloc(sizeof(struct Tree));
        buildAST("decl", typeTree, IDTree, INSTR_ROOT_TYPE, (yyval.treeType));

        printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
    }
#line 2184 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 850 "merged.y" /* yacc.c:1646  */
    {
                add_variable(global_temp, atoi(yytext), (yyvsp[-2].text));
                Tree *typeTree = malloc(sizeof(struct Tree));
                buildAST(global_string, NULL, NULL, VARIABLE_ID_ROOT_TYPE, typeTree);

                Tree *IDTree = malloc(sizeof(struct Tree));
                buildAST((yyvsp[-2].text), NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);

                (yyval.treeType) = malloc(sizeof(struct Tree));
                buildAST("decl", typeTree, IDTree, INSTR_ROOT_TYPE, (yyval.treeType));

                printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
    }
#line 2202 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 872 "merged.y" /* yacc.c:1646  */
    { duplicate_error((void*)functions, 1, -1, (yyvsp[0].text)); (functions + current_functions)->elements = NULL; (functions + current_functions)->element_count = 0; (functions + current_functions)->type = global_temp; }
#line 2208 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 872 "merged.y" /* yacc.c:1646  */
    { grow_struct(1); }
#line 2214 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 878 "merged.y" /* yacc.c:1646  */
    { add_element((void*)(functions + current_functions), 1, global_temp, 1, (yyvsp[0].text)); }
#line 2220 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 879 "merged.y" /* yacc.c:1646  */
    { add_element((void*)(functions + current_functions), 1, global_temp, atoi(yytext), (yyvsp[-2].text)); }
#line 2226 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 886 "merged.y" /* yacc.c:1646  */
    {
                      (yyval.treeType) = malloc(sizeof(struct Tree));
                      buildAST(";", (yyvsp[-2].treeType), (yyvsp[-1].treeType), INSTR_ROOT_TYPE, (yyval.treeType));

                      if((yyval.treeType)->leftTree!=NULL && (yyval.treeType)->rightTree!=NULL) 
                        printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                      else
                        printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                  }
#line 2240 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 896 "merged.y" /* yacc.c:1646  */
    {
                      (yyval.treeType) = NULL;
                  }
#line 2248 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 909 "merged.y" /* yacc.c:1646  */
    {
                (yyval.treeType) = malloc(sizeof(struct Tree));
                buildAST("{}", NULL, (yyvsp[-1].treeType), INSTR_ROOT_TYPE, (yyval.treeType));

                if((yyval.treeType)->leftTree!=NULL && (yyval.treeType)->rightTree!=NULL) 
                    printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                if((yyval.treeType)->rightTree!=NULL) 
                    printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                else
                    printf("NULL <- %s -> NULL\n", (yyval.treeType)->root);
            }
#line 2264 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 938 "merged.y" /* yacc.c:1646  */
    {printf("!!!!!!search var to assign\n");printf("!!!!!!search var to assign\n");printf("!!!!!!search var to assign\n");printf("!!!!!!search var to assign\n");
                 Tree *IDTree = malloc(sizeof(struct Tree));
                 buildAST((yyvsp[-2].text), NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);
                 
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 buildAST("=", (yyvsp[0].treeType), IDTree, INSTR_ROOT_TYPE, (yyval.treeType));

                 printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);

                 
                 printf("\n-------------------------------------------\n\n");
                 printf("typssdafe sdfsdfofsdfasdf expression:\n");
                 printf("----------------\n\n");

                 typeOf((yyvsp[0].treeType));

                 printf("\n-------------------------------------------\n\n");
                 

                 printf("\n-------------------------------------------\n\n");
                 printf("eval sdfsadfsdafexpression:\n");
                 printf("!!!!!!search var to assign\n");
                 printf("----------------\n\n");

                 struct EvalASTResult* evalResult = evalAST((yyvsp[0].treeType));

                 

                 printf("\n-------------------------------------------\n\n");


                 int i; for(i = 0; i < current_variables; ++i) {
                     printf("iterat |%s|\n", (variables + i)->name);
                     if(strcmp((variables + i)->name, tree->root) == 0) {
                         printf("found %s\n", (variables + i)->name);
                         if((variables + i)->type == INT_TYPE || (variables + i)->type == FLOAT_TYPE)
                         {
                             printf("found %s\n", (variables + i)->name);
                             *((float*)((variables + i)->addr)) = floatVal;
                             printf("variable val %f\n", floatVal);
                         }
                     }
                 }

             }
#line 2314 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 987 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2324 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 993 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2334 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 999 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2344 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1005 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2354 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1011 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2364 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1017 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2374 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1023 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2384 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1029 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2394 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1035 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2404 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1041 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2414 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1047 "merged.y" /* yacc.c:1646  */
    {
                 (yyval.treeType) = malloc(sizeof(struct Tree));
                 Tree *aux = malloc(sizeof(struct Tree));
                 buildASTForOperator((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType), aux);
             }
#line 2424 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1055 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[0].text), NULL, NULL, INT_VAL_ROOT_TYPE, (yyval.treeType));
                     printf("INT %s\n", (yyval.treeType)->root);
                 }
#line 2434 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1061 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[0].text), NULL, NULL, FLOAT_VAL_ROOT_TYPE, (yyval.treeType));
                     printf("FLOAT %s\n", (yyval.treeType)->root);
                 }
#line 2444 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1067 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[0].text), NULL, NULL, BOOL_VAL_ROOT_TYPE, (yyval.treeType));
                     printf("BOOL %s\n", (yyval.treeType)->root);
                 }
#line 2454 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1073 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[0].text), NULL, NULL, CHAR_VAL_ROOT_TYPE, (yyval.treeType));
                     printf("CHAR %s\n", (yyval.treeType)->root);
                 }
#line 2464 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1079 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[0].text), NULL, NULL, VARIABLE_ID_ROOT_TYPE, (yyval.treeType));
                     printf("VARIABLE_ID %s\n", (yyval.treeType)->root);
                 }
#line 2474 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1085 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST("()", NULL, (yyvsp[-1].treeType), PARANTHESIS_ROOT_TYPE, (yyval.treeType));
                     printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                 }
#line 2484 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1091 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[-2].text), NULL, NULL, CALL_ROOT_TYPE, (yyval.treeType));
                     printf("NULL <- %s -> NULL\n", (yyval.treeType)->root);
                 }
#line 2494 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1097 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[-3].text), NULL, (yyvsp[-1].treeType), CALL_ROOT_TYPE, (yyval.treeType));
                     printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                 }
#line 2504 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1103 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[-1].text), NULL, (yyvsp[0].treeType), ARRAY_ACCESS_ROOT_TYPE, (yyval.treeType));
                     printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                 }
#line 2514 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1109 "merged.y" /* yacc.c:1646  */
    {
                     (yyval.treeType) = malloc(sizeof(struct Tree));
                     buildAST((yyvsp[-1].text), NULL, (yyvsp[0].treeType), OPERATOR_ROOT_TYPE, (yyval.treeType));
                     printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                 }
#line 2524 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1118 "merged.y" /* yacc.c:1646  */
    {
                      (yyval.treeType) = malloc(sizeof(struct Tree));
                      buildAST((yyvsp[-1].text), (yyvsp[-2].treeType), (yyvsp[0].treeType), COMMA_ROOT_TYPE, (yyval.treeType));
                      printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                  }
#line 2534 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1126 "merged.y" /* yacc.c:1646  */
    {
                      (yyval.treeType) = malloc(sizeof(struct Tree));
                      buildAST("[]", NULL, (yyvsp[-1].treeType), ARRAY_INDEXING_ROOT_TYPE, (yyval.treeType));
                      printf("NULL <- %s -> %s\n", (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                  }
#line 2544 "merged.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1132 "merged.y" /* yacc.c:1646  */
    {
                      (yyval.treeType) = malloc(sizeof(struct Tree));
                      buildAST("[]", (yyvsp[-3].treeType), (yyvsp[-1].treeType), ARRAY_INDEXING_ROOT_TYPE, (yyval.treeType));
                      printf("%s <- %s -> %s\n", (yyval.treeType)->leftTree->root, (yyval.treeType)->root, (yyval.treeType)->rightTree->root);
                  }
#line 2554 "merged.tab.c" /* yacc.c:1646  */
    break;


#line 2558 "merged.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1140 "merged.y" /* yacc.c:1906  */


int yyerror(char* s){
    printf("eroare %s la linia %d\n",s,yylineno);
}

int main(int argc,char** argv){
    yyin = fopen(argv[1],"r");
    yyparse();
}

