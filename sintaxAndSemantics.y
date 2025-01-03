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


%start program


%%

program : instructionList { printf("program\n"); }

instructionList : instructionList instruction SEMI_COLON
                | %empty
                ;

instruction : declaration {printf("instructiunea data este o declaratie\n");}
            | functionDefinition
            | functionCall {printf("instructiunea data este un apel de functie\n");}
            | assignment
            | IF L_ROUND_PARA logicExpression R_ROUND_PARA L_COURLY_PARA instructionList R_COURLY_PARA ELSE L_COURLY_PARA instructionList R_COURLY_PARA
            | IF L_ROUND_PARA logicExpression R_ROUND_PARA L_COURLY_PARA instructionList R_COURLY_PARA
            | WHILE L_ROUND_PARA logicExpression R_ROUND_PARA L_COURLY_PARA instructionList R_COURLY_PARA
            | FOR L_ROUND_PARA assignment COMMA logicExpression COMMA algebricExpression R_ROUND_PARA L_COURLY_PARA instructionList R_COURLY_PARA
            ;      

assignment : variableName ASSIGN expression {  }
           | arrayItemAccess ASSIGN expression
           ;

variableName : ID

arrayItemAccess : ID L_SQUARE_PARA INT R_SQUARE_PARA


declaration : type ID { printf("declaration\n"); printf("%s\n",$1); strcpy(types[size],$1); size=size+1; } VARIABLE_ID { if($2==NULL) printf("NULL!!!"); /*printf("inca nu\n");*//*printf("%s\n",$2); printf("%d\n",size); printf("%s\n",$2);  strcpy(names[size-1],$2); printf("am copiat\n");*/ }

type : PREDEFINED_TYPE {printf("correct type\n");}


functionCall : functionName L_ROUND_PARA callParametersList R_ROUND_PARA

functionName : ID 

callParametersList : callParametersList COMMA parameter {printf("de fapt am primit mai multi parametri\n");}
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
%%

int yyerror(char* s){
    printf("eroare %s la linia %d\n",s,yylineno);
}

int main(int argc,char** argv){
yyin = fopen(argv[1],"r");
yyparse();
}

