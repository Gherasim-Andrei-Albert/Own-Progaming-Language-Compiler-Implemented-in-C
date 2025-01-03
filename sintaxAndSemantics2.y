%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    int yylex();
    int yyerror(char* s);

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
        for(int i = 0; i < current_structs; ++i) {
            if(!strcmp((user_structs + i)->name, type_name)) {
                global_temp = 4 + i;
                return;
            }
        }
        yyerror("tip date invalid");
        exit(-3);
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
        for(int i = 0; i < number; ++i) {
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
        for(int i = 0; i < current_variables; ++i) {
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
        for(int i = 0; i < number; ++i) {
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
                for(int i = 0; i < current_variables; ++i) {
                    printf("name: %s  -  type: %d  -  size: %d  -  mem: %d\n",
                    (variables + i)->name, (variables + i)->type, (variables + i)->size, (variables + i)->mem);
                }
                break;
            case 1:
                printf("\nVERIF FUNCTIONS: %d\n", current_functions);
                variable* ptr;
                for(int i = 0; i < current_functions; ++i) {
                    printf("name: %s  -  param_count: %d  -  type: %d\n",
                    (functions + i)->name, (functions + i)->element_count, (functions + i)->type);
                    ptr = (functions + i)->elements;
                    for(int j = 0; j < (functions + i)->element_count; ++j) {
                        printf("param - name: %s  -  type: %d  -  size: %d  -  mem: %d\n", ptr->name, ptr->type, ptr->size, ptr->mem);
                        ++ptr;
                    }
                }
                break;
            case 2:
                printf("\nVERIF STRUCTS: %d\n", current_structs);
                for(int i = 0; i < current_structs; ++i) {
                    printf("name: %s  -  elem_count: %d  -  mem: %d\n",
                    (user_structs + i)->name, (user_structs + i)->element_count, (user_structs + i)->mem);
                    for(int j = 0; j < (user_structs + i)->element_count; ++j) {
                        variable* s2 = (user_structs + i)->elements;
                        printf("name: %s  -  size: %d  -  type: %d  -  mem: %d\n", (s2 + j)->name, (s2 + j)->size, (s2 + j)->type, (s2 + j)->mem);
                    }
                }
                break;
        }
    }

    void verif_all() {
        print_verif(2);
        print_verif(0);
        print_verif(1);
    }

%}
%union {
    int intval;
    char strval[100];
}

%token START_VARIABLES
%token START_FUNCTIONS
%token START_STRUCTS
%token START_MAIN

%token L_ROUND_PARA 
%token R_ROUND_PARA

%token L_SQUARE_PARA
%token R_SQUARE_PARA

%token L_COURLY_PARA
%token R_COURLY_PARA

%token COMMA
%token SEMI_COLON
%token DOT

%token FLOAT
%token INT
%token CHAR
%token BOOL

%token CONST


%token PREDEFINED_TYPE

%token PLUS
%token MINUS
%token DIV
%token STAR
%token AND
%token OR
%token GT
%token GT_OR_EQUAL
%token LT
%token LT_OR_EQUAL
%token EQUAL
%token NOT
%token ASSIGN

%token FOR
%token IF
%token WHILE
%token ELSE

%token <strval>ID

%start program

%%

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

vDeclaration : elementType ID { add_variable(global_temp, 1, $2); }
             | elementType ID L_SQUARE_PARA INT { add_variable(global_temp, atoi(yytext), $2); } R_SQUARE_PARA

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

instructionList : instructionList instruction SEMI_COLON
                | %empty
                ;

instruction : %empty
            | L_COURLY_PARA instructionList 

/*
instruction : functionCall {printf("instructiunea data este un apel de functie\n");}
            | assignment
            | arrayItemAccess
            | ifInstruction
            | whileInstruction
            | forInstruction
            ;

ifInstruction : IF L_ROUND_PARA expression R_ROUND_PARA L_COURLY_PARA loop_body R_COURLY_PARA
              | IF L_ROUND_PARA expression R_ROUND_PARA L_COURLY_PARA loop_body R_COURLY_PARA ELSE L_COURLY_PARA loop_body R_COURLY_PARA

whileInstruction : WHILE L_ROUND_PARA expression R_ROUND_PARA L_COURLY_PARA loop_body R_COURLY_PARA

forInstruction : FOR L_ROUND_PARA expression SEMI_COLON expression SEMI_COLON expression R_ROUND_PARA L_COURLY_PARA loop_body R_COURLY_PARA

loop_body : instruction loop_body
          | instruction
          ;

assignment : variableName ASSIGN expression {  }
           | arrayItemAccess ASSIGN expression
           | %empty;
           ;

variableName : ID

arrayItemAccess : ID L_SQUARE_PARA INT {$$ = 2;} R_SQUARE_PARA

param_declaration : param_type ID { functions[func_size].params[functions[func_size].has_params].size = 1; printf("declaration %s\n", yytext); strcpy(functions[func_size].params[functions[func_size].has_params].name, yytext);}
                  //| param_type L_SQUARE_PARA R_SQUARE_PARA
                  ;

type : PREDEFINED_TYPE {printf("correct type %s\n", yytext); strcpy(variables[var_size].type, yytext); }

func_type : PREDEFINED_TYPE {printf("correct function type %s\n", yytext); strcpy(functions[func_size].type[func_size], yytext); }

param_type : PREDEFINED_TYPE {printf("correct param type %s\n", yytext); strcpy(functions[func_size].params[functions[func_size].has_params].type, yytext); }


functionCall : functionName L_ROUND_PARA callParametersList R_ROUND_PARA

functionName : ID 

callParametersList : callParametersList ',' parameter {printf("de fapt am primit mai multi parametri\n");}
                   | parameter {printf("am primit un singur argument\n");}
                   | %empty
                   ;

parameter : expression

expression : L_ROUND_PARA expression R_ROUND_PARA
           | variableName {printf("argumentul este un variable id\n");}
           | functionCall {printf("argumentul este un apel\n");}
           | arrayItemAccess
           | INT
           | FLOAT
           | BOOL { if(strcmp($1,"true\0")){$$=1;} if(strcmp($1,"false\0")){$$=0;} }
           | CHAR
           | algebricExpression
           | logicExpression
           ;

algebricExpression : L_ROUND_PARA algebricExpression R_ROUND_PARA
                   | expression PLUS expression {printf("argum %d",$2);$$=$$+$2; printf("Evaluarea este %d",$1+$2);}
                   | expression MINUS expression {$$=$1-$1;}
                   | expression STAR expression {$$=$1*$1;}
                   | expression DIV expression {$$=$1/$1;}
                   ;

logicExpression : L_ROUND_PARA logicExpression R_ROUND_PARA {$$=$1;}
                | NOT logicExpression {$$=!$1;}
                | logicExpression AND logicExpression {$$=$1&&$2;}
                | logicExpression OR logicExpression {$$=$1||$2;}
                | algebricExpression LT algebricExpression {$$=$1<$2;}
                | algebricExpression GT algebricExpression {$$=$1>$2;}
                | algebricExpression LT_OR_EQUAL algebricExpression {$$<=$1&&$2;}
                | algebricExpression GT_OR_EQUAL algebricExpression {$$>=$1&&$2;}
                | algebricExpression EQUAL algebricExpression {$$==$1&&$2;}
                ;
*/
%%

int yyerror(char* s){
    printf("eroare %s la linia %d\n",s,yylineno);
}

int main(int argc,char** argv){
    yyin = fopen(argv[1],"r");
    yyparse();
}

