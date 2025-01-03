%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>


    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;


    typedef enum RootType
    {
        INT_VAL_ROOT_TYPE, VARIABLE_ID_ROOT_TYPE, OPERATOR_ROOT_TYPE,
        PARANTHESIS_ROOT_TYPE, CALL_ROOT_TYPE, ARRAY_ACCESS_ROOT_TYPE
    } RootType;

    typedef struct Tree
    {
        char* root;
        struct Tree* leftTree;
        struct Tree* rightTree;
        RootType rootType;
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
        int sizes[2] = {7, 3};
        char* precedenceTable[100][100]= 
        {
            { "*", "/", "&&", "!" },
            { "+", "-", "||" }
        };
        int line;
        for(line = 0; line < 2; ++ line)
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
    }

    void buildASTForOperator(char* root, Tree* leftTree, Tree* rightTree, RootType rootType, Tree* tree) {
        printf("%s\n", root);
        if(leftTree->rootType == OPERATOR_ROOT_TYPE 
            && precedence(leftTree->root) > precedence(root))
        {
            // printf("found bigger precedence beneath\n");

            buildAST(root, leftTree->rightTree, rightTree, OPERATOR_ROOT_TYPE, tree);
            leftTree->rightTree = tree;

            printf("%s <- %s -> %s\n", tree->leftTree->root, tree->root, tree->rightTree->root);
            printf("%s <- %s -> %s\n", leftTree->leftTree->root, leftTree->root, leftTree->rightTree->root);
        }
        else
        {
            //  printf("precedence ok\n");

            buildAST(root, leftTree, rightTree, OPERATOR_ROOT_TYPE, tree);

            printf("%s <- %s -> %s\n", tree->leftTree->root, tree->root, tree->rightTree->root);
        }
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
    ASSIGN
    COMMA SEMI_COLON DOT 
    CONST PREDEFINED_TYPE

%type <treeType> expressionTerm expression

%start expression

%%

expression : expressionTerm {$$=$1; printf("term %s",$1->root);}
           | expression PLUS expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression MINUS expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression STAR expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression DIV expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression OR expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression AND expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression EQUAL expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression GT expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression GT_OR_EQUAL expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression LT expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           | expression LT_OR_EQUAL expressionTerm
             {
                 $$ = malloc(sizeof(struct Tree));
                 buildASTForOperator($2, $1, $3, OPERATOR_ROOT_TYPE, $$);
             }
           ;

expressionTerm : INT
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, INT_VAL_ROOT_TYPE, $$);
                     printf("INT %s\n", $$->root);
                 }
               | ID 
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, INT_VAL_ROOT_TYPE, $$);
                     printf("VARIABLE_ID %s\n", $$->root);
                 }
               | L_ROUND_PARA expression R_ROUND_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST("()", NULL, $2, PARANTHESIS_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | ID L_ROUND_PARA expression R_ROUND_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, $3, CALL_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | ID L_ROUND_PARA R_ROUND_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, NULL, CALL_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | ID L_SQUARE_PARA expression R_SQUARE_PARA
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, $3, ARRAY_ACCESS_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
                 }
               | NOT expressionTerm
                 {
                     $$ = malloc(sizeof(struct Tree));
                     buildAST($1, NULL, $2, OPERATOR_ROOT_TYPE, $$);
                     printf("NULL <- %s -> %s\n", $$->root, $$->rightTree->root);
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

