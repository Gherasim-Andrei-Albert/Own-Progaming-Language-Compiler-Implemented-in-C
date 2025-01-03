%{
   #include <stdio.h>
   #include "y.tab.h"
%}

plus \+
minus \-
div \/
star \*
and \&
or \|
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

{plus} yylval=yytext; return PLUS;
{minus} yylval=yytext;  return MINUS;
{div} yylval=yytext;  return DIV;
{star} yylval=yytext; return STAR;
{and} yylval=yytext;  return AND;
{or} yylval=yytext;  return OR;
{gt} yylval=yytext;  return GT;
{gtOrEqual} yylval=yytext; return GT_OR_EQUAL;
{lt} yylval=yytext; return LT;
{ltOrEqual} yylval=yytext; return LT_OR_EQUAL;
{equal} yylval=yytext; return EQUAL;
{not} yylval=yytext; return NOT;
{assign} yylval=yytext; return ASSIGN;

{LRoundPara} yylval=yytext; { return L_ROUND_PARA; }
{RRoundPara} yylval=yytext; { return R_ROUND_PARA; }

{LSquarePara} yylval=yytext; return L_SQUARE_PARA;
{RSquarePara} yylval=yytext; return R_SQUARE_PARA;

{LCourlyPara} yylval=yytext; return L_COURLY_PARA;
{RCourlyPara} yylval=yytext; return R_COURLY_PARA;

{comma} yylval=yytext;  return COMMA;
{semiColon} yylval=yytext;  return SEMI_COLON;
{dot} yylval=yytext; return DOT;

{const} yylval=yytext; return CONST;

{for} yylval=yytext; return FOR;
{while} yylval=yytext; return WHILE;
{if} yylval=yytext; return IF;
{else} yylval=yytext; return ELSE;

{int} yylval=yytext; return INT;
{float} yylval=yytext; return FLOAT;
{char} yylval=yytext; return CHAR;
{bool} yylval=yytext; return BOOL;
{predefinedType} yylval=yytext; return PREDEFINED_TYPE;

{ID} yylval=yytext; return ID;


   
%%

