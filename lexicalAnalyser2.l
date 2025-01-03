%{
   #include <stdio.h>
   #include "sintaxAndSemantics.tab.h"
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

comma ,
semiColon ;
dot \.

LRoundPara \(
RRoundPara \)

LSquarePara \[
RSquarePara \]

LCourlyPara \{
RCourlyPara \}

SVars START_V\(\)
SFunctions START_F\(\)
SStructs START_S\(\)
SMain START_M\(\)

for for
while while
if if
else else
const const

int ([-]*)(([1-9][0-9]*)|0)
float {int}|([-]*0\.[0-9]+)|([-]*[1-9]+[0-9]*\.[0-9]+)
char \'([a-zA-Z0-9])+\'
bool true|false

intType int
floatType float
boolType bool
charType char

predefinedType {primitiveType}
primitiveType {intType}|{floatType}|{boolType}|{charType}
primitiveValue {int}|{float}|{bool}|{char}

ID [a-zA-Z_][a-zA-Z0-9_]*

%option noyywrap
%%

{plus} return PLUS;
{minus} return MINUS;
{div}  return DIV;
{star} return STAR;
{and}  return AND;
{or}  return OR;
{gt}  return GT;
{gtOrEqual} return GT_OR_EQUAL;
{lt} return LT;
{ltOrEqual} return LT_OR_EQUAL;
{equal} return EQUAL;
{not} return NOT;
{assign} return ASSIGN;

{LRoundPara} return L_ROUND_PARA;
{RRoundPara} return R_ROUND_PARA;

{LSquarePara} return L_SQUARE_PARA;
{RSquarePara} return R_SQUARE_PARA;

{LCourlyPara} return L_COURLY_PARA;
{RCourlyPara} return R_COURLY_PARA;

{comma}  return COMMA;
{semiColon}  return SEMI_COLON;
{dot} return DOT;

{SVars} return START_VARIABLES;
{SFunctions} return START_FUNCTIONS;
{SStructs} return START_STRUCTS;
{SMain} return START_MAIN;

{const} return CONST;

{for} return FOR;
{while} return WHILE;
{if} return IF;
{else} return ELSE;

{int} yylval.intval = 0; return INT;
{float} yylval.intval = 1; return FLOAT;
{char} yylval.intval = 2; return CHAR;
{bool} yylval.intval = 3; return BOOL;
{predefinedType} return PREDEFINED_TYPE;

{ID} strcpy(yylval.strval,yytext); return ID;

%%
