%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define YYSTYPE string

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
%right '^' '&' '|'
%start S

%%

S : NOME MIOLOS PRINCIPAL { cout << "Aceito " << $1 << endl; 
                            cout << "Funcoes" << endl <<$2;    }
  ;
   
NOME : _PROGRAM _ID ';' { $$ = $2;}
     ;
   
MIOLOS : MIOLO MIOLOS { $$ = $1 + $2 ;}
       | 
       {$$ = "" ;}
       ;
       
MIOLO : VARS { $$ = "" ;}
      | FUNCTION //{ $$ = $1 + "\n";}
      ;          
   
FUNCTION : _FUNCTION _ID '(' PARAMETROS ')' ':' TIPO ';' BLOCO ';'
        { $$ = $2 + "\n"; }
         | _FUNCTION _ID ':' TIPO ';' BLOCO ';'
         { $$ = $2 + "\n"; }
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
     | IDS '[' _CTE_INTEGER ']' ':' TIPO
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
          ;
          
CMDS : CMD ';' CMDS
     |
     ;                   
 
CMD : SAIDA
    | CMD_IF
    | CMD_FOR
    | CMD_WHILE
    | BLOCO
    | CMD_ATRIB
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
          ;
CMD_FOR : _FOR _ID _ATRIB E _TO E _DO CMD
        ;
    
BLOCO : _BEGIN CMDS _END
      ;    
    
CMD_IF : _IF E _THEN CMD
       | _IF E _THEN  CMD _ELSE CMD
       ;    
    
SAIDA : _WRITE '(' E ')'
      | _WRITELN '(' E ')' 
      ;
   
E : E '+' E
  | E '-' E
  | E '*' E
  | E '/' E
  | E '^' E
  | E '>' E
  | E '<' E
  | E '&' E
  | E '|' E
  | E '%' E
  | '(' E ')'
  | F
  ;
  
F : _CTE_STRING
  | _CTE_INTEGER
  | _ID
  ;     
 
%%

#include "lex.yy.c"

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     printf( "%s\nNo final do arquivo\n", st );
   else  
   
     printf( "%s\nProximo a: %s\nlinha: %d, col: %d\n", st, yytext, yylineno,yyrowno );
}

int main( int argc, char* argv[] )
{
  yyparse();
}

