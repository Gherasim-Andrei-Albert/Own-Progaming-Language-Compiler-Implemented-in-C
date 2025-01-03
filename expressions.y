%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdbool.h>


    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;


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
        INT_TYPE, FLOAT_TYPE, BOOL_TYPE, CHAR_TYPE
    } Type;

    typedef struct Tree
    {
        char* root;
        struct Tree* leftTree;
        struct Tree* rightTree;
        RootType rootType;
        enum Type type;
    } Tree;

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


%type <treeType> expressionTerm expression expressionsList arrayAccessList
    codeBlock assignment instruction instructionList vDeclaration

%start mainBlock

%%

// test : expression SEMI_COLON {
//     printf("Tree available here in $1. For example it's root is %s\n", $1->root);
// };

mainBlock : instructionList

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

vDeclaration : PREDEFINED_TYPE ID
               {
                   Tree *typeTree = malloc(sizeof(struct Tree));
                   buildAST($1, NULL, NULL, VARIABLE_ID_ROOT_TYPE, typeTree);

                   Tree *IDTree = malloc(sizeof(struct Tree));
                   buildAST($2, NULL, NULL, VARIABLE_ID_ROOT_TYPE, IDTree);

                   $$ = malloc(sizeof(struct Tree));
                   buildAST("decl", typeTree, IDTree, INSTR_ROOT_TYPE, $$);

                   printf("%s <- %s -> %s\n", $$->leftTree->root, $$->root, $$->rightTree->root);
               }
             ;

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

                 evalAST($3);

                 printf("\n-------------------------------------------\n\n");
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

