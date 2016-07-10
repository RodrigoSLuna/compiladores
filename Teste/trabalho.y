%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct Atributo {
  string v, t, c;
};

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);

%}

%token _ID _PROGRAM _BEGIN _END _WRITELN _WRITE _VAR _IF _THEN _ELSE
%token _FOR _TO _DO _ATRIB _FUNCTION _WHILE
%token _INTEGER _STRING

%token _CTE_STRING _CTE_INTEGER

%nonassoc '>' '<' '='
%left '+' '-'
%left '*' '/'

%start S

%%

S : NOME MIOLOS PRINCIPAL 
  { cout << $1.c << $3.c << endl; }
  ;
   
NOME : _PROGRAM _ID ';' 
       { $$.c = "#include <stdlib.h>\n"
                "#include <stdio.h>\n\n";
       }              
     ;   
   
MIOLOS : MIOLO MIOLOS 
         { $$.c = $1.c + $2.c; }
       | 
         { $$.c = ""; }
       ;
       
MIOLO : VARS  
      | FUNCTION 
      ;          
   
FUNCTION : _FUNCTION _ID '(' PARAMETROS ')' ':' TIPO ';' BLOCO ';'
         | _FUNCTION _ID ':' TIPO ';' BLOCO ';'
         ;    
         
PARAMETROS : DECL ';' PARAMETROS
           | DECL
           ;         
   
VARS : _VAR DECLS
     ;
     
DECLS : DECL ';' DECLS
      | DECL ';'
      ;   
     
DECL : IDS ':' TIPO        
     ;
     
TIPO : _INTEGER
     | _STRING TAM_STRING 
     ;
     
TAM_STRING : '[' _CTE_INTEGER ']'
           |
           ;     
     
IDS : _ID ',' IDS
    | _ID
    ;      
   
PRINCIPAL : _BEGIN CMDS _END '.'
            { $$.c = "int main() {\n" + $2.c + "}\n"; }
          ;
          
CMDS : CMD ';' CMDS { $$.c = $1.c + $3.c; }
     | { $$.c = ""; }
     ;                   
 
CMD : SAIDA
    | CMD_IF
    | CMD_FOR
    | BLOCO
    | CMD_ATRIB
    | CMD_WHILE
    ;
    
CMD_ATRIB : _ID INDICE _ATRIB E
          ;    
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
     ;        

CMD_WHILE : _WHILE '(' E ')' CMD 

CMD_FOR : _FOR _ID _ATRIB E _TO E _DO CMD
        ;
    
BLOCO : _BEGIN CMDS _END
      ;    
    
CMD_IF : _IF E _THEN CMD
       | _IF E _THEN CMD _ELSE CMD
       ;    
    
SAIDA : _WRITE '(' E ')'
        { $$.c = "  printf( \"%"+ $3.t + "\", " + $3.v + " );\n"; }
      | _WRITELN '(' E ')'
        { $$.c = "  printf( \"%"+ $3.t + "\\n\", " + $3.v + " );\n"; }
      ;
   
E : E '+' E
  | E '>' E
  | E '-' E
  | E '*' E
  | E '/' E
  | E '<' E
  | E '=' E
  | F
  ;
  
F : _CTE_STRING  { $$ = $1; $$.t = "s"; }
  | _CTE_INTEGER  { $$ = $1; $$.t = "d"; }
  | _ID
  ;     
 
%%

#include "lex.yy.c"

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     printf( "%s\nNo final do arquivo\n", st );
   else  
     printf( "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st, 
             yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}

int main( int argc, char* argv[] )
{
  yyparse();
}

