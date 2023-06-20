/* simplest version of assignment */
%{
#include <stdio.h>
#include <math.h>

void yyerror(char * s);
int yylex();

%}

%union {
  int ival;
  double fval;
  char * sval;
}

/* declare tokens */
%token EQ
%token AMP
%token SEMICOLON
%token<ival> INTEGER_LITERAL
%token<fval> FLOAT_LITERAL
%token<sval> STRING_LITERAL
%token<sval> IDENTIFIER
%token EOL

%%

calclist:                                     
 | calclist varname EQ expr SEMICOLON EOL
;

varname: IDENTIFIER
;

expr: INTEGER_LITERAL { printf ("Got an int: %d\n", $1);}
    | FLOAT_LITERAL     { printf ("Got a float: %f\n", $1);}
    | STRING_LITERAL     { printf ("Got a string: %s\n", $1);}
    | AMP IDENTIFIER    { printf ("Got a refernce: &%s\n", $2);}
;

%%
int main(int argc, char **argv)
{
  yyparse();
}

void yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}
