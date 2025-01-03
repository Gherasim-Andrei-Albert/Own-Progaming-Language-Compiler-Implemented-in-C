%{
   #include <stdio.h>
   #include "y.tab.h"
%}

plus \+
minus \-
div \/
star \*
and \&\&
or \|\|
gt \>
gtOrEqual \>\=
lt \<
ltOrEqual \<\=
equal \=\=
not \!
assign \=

LRoundPara \(
RRoundPara \)

LSquarePara \[
RSquarePara \]

LCourlyPara \{
RCourlyPara \}

comma ,
semiColon ;
dot \.


do do
for for
while while
if if
else else
const const


int ([-]*)(([1-9][0-9]*)|0)
float {int}|([-]*0{dot}[0-9]+)|([-]*[1-9]+[0-9]*{dot}[0-9]+)
char \'([a-zA-Z0-9])+\'
bool true|false


intType int
floatType float
boolType bool
charType char


predefinedType {primitiveType}
primitiveType {intType}|{floatType}|{boolType}|{charType}
primitiveValue {int}|{float}|{bool}|{char}


ID [a-zA-Z][a-zA-Z0-9]*


%option noyywrap


%%

{plus} yylval.text = strdup(yytext); return PLUS;
{minus} yylval.text = strdup(yytext);  return MINUS;
{div} yylval.text = strdup(yytext);  return DIV;
{star} yylval.text = strdup(yytext); return STAR;
{and} yylval.text = strdup(yytext);  return AND;
{or} yylval.text = strdup(yytext);  return OR;
{gt} yylval.text = strdup(yytext);  return GT;
{gtOrEqual} yylval.text = strdup(yytext); return GT_OR_EQUAL;
{lt} yylval.text = strdup(yytext); return LT;
{ltOrEqual} yylval.text = strdup(yytext); return LT_OR_EQUAL;
{equal} yylval.text = strdup(yytext); return EQUAL;
{not} yylval.text = strdup(yytext); return NOT;
{assign} yylval.text = strdup(yytext); return ASSIGN;

{LRoundPara} yylval.text = strdup(yytext); { return L_ROUND_PARA; }
{RRoundPara} yylval.text = strdup(yytext); { return R_ROUND_PARA; }

{LSquarePara} yylval.text = strdup(yytext); return L_SQUARE_PARA;
{RSquarePara} yylval.text = strdup(yytext); return R_SQUARE_PARA;

{LCourlyPara} yylval.text = strdup(yytext); return L_COURLY_PARA;
{RCourlyPara} yylval.text = strdup(yytext); return R_COURLY_PARA;

{comma} yylval.text = strdup(yytext);  return COMMA;
{semiColon} yylval.text = strdup(yytext);  return SEMI_COLON;
{dot} yylval.text = strdup(yytext); return DOT;

{const} yylval.text = strdup(yytext); return CONST;

{for} yylval.text = strdup(yytext); return FOR;
{while} yylval.text = strdup(yytext); return WHILE;
{if} yylval.text = strdup(yytext); return IF;
{else} yylval.text = strdup(yytext); return ELSE;

{int} yylval.text = strdup(yytext); return INT;
{float} yylval.text = strdup(yytext); return FLOAT;
{char} yylval.text = strdup(yytext); return CHAR;
{bool} yylval.text = strdup(yytext); return BOOL;
{predefinedType} yylval.text = strdup(yytext); return PREDEFINED_TYPE;

{ID} yylval.text = strdup(yytext); return ID;


   
%%

