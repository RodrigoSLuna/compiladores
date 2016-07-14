%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
using namespace std;
struct Atributo{
    string v, t, c;
};
map<string,string> m; 
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
%right '^' '&' '|'
%start S

%%

S : NOME MIOLOS PRINCIPAL 
    { cout << $1.c << $2.c <<  $3.c << endl;}
  ;
   
NOME : _PROGRAM _ID ';'
        { $$.c = "include <stdlib.h>\n"
                 "include <stdio.h>\n\n";
        }
     ;
   
MIOLOS : MIOLO MIOLOS 
        { $$.c = $1.c + $2.c ; }
       | 
        {$$.c = "" ;}
       ;
       
MIOLO : VARS 
      | FUNCTION {}
      ;          
   
FUNCTION : _FUNCTION _ID '(' PARAMETROS ')' ':' TIPO ';' BLOCO ';'
             {$$.c = m[$7.v] + " " + $2.v + "(" + $4.c   + ")" + $9.c  + "\n"; }
         | _FUNCTION _ID ':' TIPO ';' BLOCO ';'
            {$$.c = m[$4.v] + " " + $2.v + "(  )" + $6.c  + "\n"; }   
         ;    
         
PARAMETROS : DECL ';' PARAMETROS
           | DECL
           ;         
   
VARS : _VAR DECLS
     {$$ = $2;}
     ;
     
DECLS : DECL ';' DECLS
      { $$.c =  $1.c + ';' + $3.c;}
      | DECL ';'
      { $$.c = "\n" + $1.c + ";\n" ;}
      ;   
     
DECL : IDS ':' TIPO
     {$$.c =  m[$3.v] + " " + $1.v ;}
     | IDS '[' _CTE_INTEGER ']' ':' TIPO 
     { $$.c = m[$6.v] + " "  + $1.v + '[' + $3.v +']'     ;}
     ;
     
TIPO : _INTEGER
     | _STRING TAM_STRING 
     ;
     
TAM_STRING : '[' _CTE_INTEGER ']'
            { $$.c = '['+ $2.v + ']'; cout << "Here2 " << endl;  }
           |
           ;     
IDS : _ID ',' IDS
    | _ID
    ;      
   
PRINCIPAL : _BEGIN CMDS _END '.'
            { $$.c = "int main() {\n" + $2.c + "}\n";}
          ;
          
CMDS : CMD ';' CMDS { $$.c = $1.c + $3.c;}
     | {$$.c = "";}
     ;                   
 
CMD : SAIDA
    | CMD_IF    {$$.c = $1.c;  }
    | CMD_FOR   {$$.c = $1.c;}
    | CMD_WHILE {$$.c = $1.c;} 
    | BLOCO     {$$.c = $1.c;}
    | CMD_ATRIB {$$.c = $1.c;}
    ;
    
    
CMD_ATRIB : _ID INDICE _ATRIB E
          { $$.c = $1.v + m[$3.v] + $4.v;}
          ;    
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
     ;
     
CMD_WHILE : _WHILE '(' E ')' CMD
            { $$.c = " while( " + $3.v +" )\n" + $5.c + "\n\n";}
          ;
CMD_FOR : _FOR _ID _ATRIB E _TO E _DO CMD
        ;
    
BLOCO : _BEGIN CMDS _END
        { $$.c = m[$1.v] + "\t" +  $2.c + m[$3.v] + "\t"; }
      ;    
    
CMD_IF : _IF E _THEN CMD
            { $$.c = $1.v  + "( " + $2.v + " )" + "\n" + "  " + $4.c + "\n"; }
       | _IF E _THEN  CMD _ELSE CMD
            { $$.c = $1.v + " " + $2.v + "\n" + "  " + $4.c + "\n" + 
              $5.v + " " + $6.c; }

       ;    
    
SAIDA : _WRITE '(' E ')'
        { $$.c = "  printf( \"%"+ $3.t + "\", " + $3.v + " );\n";}
      | _WRITELN '(' E ')' 
        { $$.c = "  printf( \"%"+ $3.t + "\\n\", " + $3.v + " );\n";}
      ;
   
E : E '+' E
    { $$.v = $1.v + $2.v + $3.v;}
  | E '-' E
     { $$.v = $1.v + $2.v + $3.v;}
  | E '*' E
     { $$.v = $1.v + $2.v + $3.v;}
  | E '/' E
     { $$.v = $1.v + $2.v + $3.v;}
  | E '^' E
     { $$.v = $1.v + $2.v + $3.v;}
  | E '>' E
     { $$.v = $1.v + $2.v + $3.v;}
  | E '<' E
     { $$.v = $1.v + $2.v + $3.v;}
  | '(' E ')'
  | F
  ;
  
F : _CTE_STRING { $$ = $1; $$.t = "s"; }
  | _CTE_INTEGER { $$ = $1; $$.t = "d";}
  | _ID         { $$ = $1; $$.t = "s";}
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
  m[":)"] = "{";
  m[":("] = "}";
  m["<-"] = "=";
  m["SemCasaDecimal"] = "int";
  m["CharPointerMelhorado"] = "char";
  yyparse();
}

