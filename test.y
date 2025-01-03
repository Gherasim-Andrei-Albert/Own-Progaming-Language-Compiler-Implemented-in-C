%{
    #include <stdio.h>
          #include <string.h>

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    
   
        char names[100][100];
        char types[100][100];
        char values[100][100];  
 

    
        int size =0;

    
/*

expression : undeterminedTypeExpressionTerm {printf("variable as expresion\n");}
           | algebricExpression {printf("algebric\n");}
           ;

undeterminedTypeExpressionTerm : variableName

algebricExpression : FLOAT {printf("float\n");}

rest : rest PLUS FLOAT
     | rest PLUS undeterminedTypeExpressionTerm {printf("variable as algebric\n");}
     | FLOAT
     | undeterminedTypeExpressionTerm {printf("variable as algebric\n");}
     ;

*/
    
    
%}




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

%token ID


%start expression


%%

variableName : ID ;

expression : undeterminedTypeExpressionTerm {printf("variable as expresion\n");}
           | algebricExpression {printf("algebric\n");}
           ;

undeterminedTypeExpressionTerm : variableName

algebricExpression : FLOAT {printf("float\n");}
                   | FLOAT + algebricExpression
                   | undeterminedTypeExpressionTerm + algebricExpression





expression : undeterminedTypeExpressionTerm {printf("variable as expresion\n");}
           | algebricExpression {printf("algebric\n");}
           ;

undeterminedTypeExpressionTerm : variableName

algebricExpression : FLOAT {printf("float\n");}

rest : rest PLUS FLOAT
     | rest PLUS undeterminedTypeExpressionTerm {printf("variable as algebric\n");}
     | FLOAT
     | undeterminedTypeExpressionTerm {printf("variable as algebric\n");}
     ;
%%

int yyerror(char* s){
    printf("eroare %s la linia %d\n",s,yylineno);
}

int main(int argc,char** argv){
yyin = fopen(argv[1],"r");
yyparse();
}

