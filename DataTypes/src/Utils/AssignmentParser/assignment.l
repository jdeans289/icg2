/* recognize tokens for the assignment and print them out */
%{
# include "assignment.tab.h"
%}

%%
"="    { return EQ; }
"&"    { return AMP; }
";"    { return SEMICOLON; }

-?[0-9]+ { printf ("Text of int literal: %s\n", yytext); yylval.ival = atoi(yytext); return INTEGER_LITERAL; }
0x[a-f0-9]+ { printf ("Text of int literal: %s\n", yytext); yylval.ival = (int)strtol(yytext, NULL, 0); return INTEGER_LITERAL; }

-?[0-9]*\.[0-9]+ { printf ("Text of float literal: %s\n", yytext); yylval.fval = atof(yytext); return FLOAT_LITERAL; }

[_A-Za-z][_A-Za-z0-9\.\[\]]* { printf ("Text of identifier: %s\n", yytext); yylval.sval = strdup(yytext); return IDENTIFIER; }

\".*\" { printf ("Text of string literal: %s\n", yytext); yylval.sval = strdup(yytext); return STRING_LITERAL; }

\n     { return EOL; }
[ \t]  { /* ~~nothing~~ */ }
.      { printf("Mystery character %c\n", *yytext); }
%%
