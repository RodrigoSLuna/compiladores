%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;


struct Range{
    int inicio,fim;
};


struct Tipo {
  string nome;  // O nome na sua linguagem
  string decl;  // A declaração correspondente em c-assembly
  string fmt;   // O formato para "printf"
  vector< Range > dim; // Dimensões (se não for array, fica vazio)
};
 

Tipo Integer = { "integer", "int", "d" };
Tipo Real =    { "real", "float", "f" };
Tipo Double =  { "double", "double", "lf" };
Tipo Boolean = { "boolean", "int", "d" };
Tipo String =  { "string", "char", "s" };
Tipo Char =    { "char", "char", "c" };

struct Atributo {
  string v, c;
  Tipo t;
  vector<string> lst;
};

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);
void erro( string );


map<string,Tipo> ts;
map< string, map< string, Tipo > > tro; // tipo_resultado_operacao;

// contadores para variáveis temporariras
map< string, int > temp_global;
map< string, int > temp_local;
map< string, int > nlabel;
bool escopo_local = false;

string toString( int n ) {
  char buf[256] = "";
  
  sprintf( buf, "%d", n );
  
  return buf;
}

int toInt( string valor ) {
  int aux = 0;
  
  sscanf( valor.c_str(), "%d", &aux );
  
  return aux;
}

string gera_nome_var( Tipo t ) {
  return "t_" + t.nome + "_" + 
   toString( ++(escopo_local ? temp_local : temp_global)[t.nome] );
}

string gera_nome_label( string cmd ) {
  return "L_" + cmd + "_" + toString( ++nlabel[cmd] );
}

ostream& operator << ( ostream& o, const vector<string>& st ) {
  cout << "[ "; 
  for( vector<string>::const_iterator itr = st.begin();
       itr != st.end(); ++itr )
    cout << *itr << " "; 
       
  cout << "]";
  return o;     
}

string trata_dimensoes_decl_var( Tipo t ) {
  string aux;
  
  for( int i = 0; i < t.dim.size(); i++ )
    aux += "[" + toString( t.dim[i].fim - t.dim[i].inicio + 1 )+ "]";
           
  return aux;         
}

// 'Atributo&': o '&' significa passar por referência (modifica).
void declara_variavel( Atributo& ss, 
                       const Atributo& s1, const Atributo& s3 ) {
  ss.c = "";
  for( int i = 0; i < s1.lst.size(); i++ ) {
    if( ts.find( s1.lst[i] ) != ts.end() ) 
      erro( "Variável já declarada: " + s1.lst[i] );
    else {
      ts[ s1.lst[i] ] = s3.t; 
      ss.c += s3.t.decl + " " + s1.lst[i] 
              + trata_dimensoes_decl_var( s3.t ) + ";\n"; 
    }  
  }
}

void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts.find( s1.v ) == ts.end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ s1.v ];
    ss.v = s1.v;
  }
}

void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {
  if( s1.t.nome == s3.t.nome )
      {
    ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }
  else if( s1.t.nome == s3.t.nome &&  s1.t.nome == "string" ) {
    ss.c = s1.c + s3.c + "  " 
           + "strncpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim[0].fim ) + " );\n";
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss, 
                           const Atributo& s1, 
                           const Atributo& s2, 
                           const Atributo& s3 ) {
  if( tro.find( s2.v ) != tro.end() ) {
    if( tro[s2.v].find( par( s1.t, s3.t ) ) != tro[s2.v].end() ) {
      ss.t = tro[s2.v][par( s1.t, s3.t )];
      ss.v = gera_nome_var( ss.t );      
      ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v 
             + ";\n";
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";
   
  for( int i = 1; i <= qtde; i++ )
    aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";
    
  return aux;  
}

string declara_var_temp( map< string, int >& temp ) {
  string decls = "// variáveis temporárias \n" + 
    declara_nvar_temp( Integer, temp[Integer.nome] ) +
    declara_nvar_temp( Real, temp[Real.nome] ) +
    declara_nvar_temp( Double, temp[Double.nome] ) +
    declara_nvar_temp( String, temp[String.nome] ) +
    declara_nvar_temp( Char, temp[Char.nome] ) +
    declara_nvar_temp( Boolean, temp[Boolean.nome] ) +
    "\n";
  
  temp.clear();
  
  return decls;
}

void gera_cmd_if( Atributo& ss, 
                  const Atributo& exp, 
                  const Atributo& cmd_then, 
                  const Atributo& cmd_else ) { 
  string lbl_then = gera_nome_label( "then" );
  string lbl_end_if = gera_nome_label( "end_if" );
  
  if( exp.t.nome != Boolean.nome )
    erro( "A expressão do IF deve ser booleana!" );
    
  ss.c = exp.c + 
         "\nif( " + exp.v + " ) goto " + lbl_then + ";\n" +
         cmd_else.c + "  goto " + lbl_end_if + ";\n\n" +
         lbl_then + ":;\n" + 
         cmd_then.c + "\n" +
         lbl_end_if + ":;\n"; 
}

%}

%token _ID _PROGRAM _BEGIN _END _WRITELN _WRITE _VAR _IF _THEN _ELSE
%token _FOR _TO _DO _ATRIB _FUNCTION _WHILE _MOD
%token _INTEGER _STRING _REAL _BOOLEAN _DOUBLE

%token _CTE_STRING _CTE_INTEGER

%nonassoc '>' '<' '='
%left '+' '-'
%left '*' '/' _MOD
%right '^' '&' '|'

%start S

%%

S : NOME MIOLOS PRINCIPAL 
    { cout << $1.c << declara_var_temp( temp_global )
           << $2.c <<  $3.c << endl;}
  ;
   
NOME : _PROGRAM _ID ';'
        { $$.c = "#include <stdlib.h>\n"
                "#include <string.h>\n" 
                "#include <stdio.h>\n\n";
        }
     ;
   
MIOLOS : MIOLO MIOLOS 
        { $$.c = $1.c + $2.c ; }
       | 
        {$$.c = "" ;}
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
     {$$.c = $2.c;}
     ;
     
DECLS : DECL ';' DECLS
      { $$.c =  $1.c + $3.c;}
      | DECL ';'
      ;   
     
DECL : IDS ':' TIPO
     { declara_variavel( $$, $1, $3); }
     ;
     
TIPO : _INTEGER { $$.t = Integer;}
     | _REAL { $$.t = Real; }
     | _DOUBLE { $$.t = Double; }
     | _BOOLEAN { $$.t = Boolean;}
     | _STRING TAM_STRING { $$.t = $2.t ; }
     ;
     
TAM_STRING : '[' _CTE_INTEGER ']'
           { $$.t = String; $$.t.dim[0].fim = toInt( $2.v ); }
           | { $$.t = String; }
           ;     
IDS : _ID ',' IDS { $$.lst = $1.lst; $$.lst.push_back( $3.v ); }
    | _ID   { $$.lst.push_back( $1.v ); }
    ;      
   
PRINCIPAL : _BEGIN CMDS _END '.'
            { $$.c = "int main() {\n" + $2.c + "}\n";}
          ;
          
CMDS : CMD ';' CMDS { $$.c = $1.c + $3.c;}
     | {$$.c = "";}
     ;                   
 
CMD : SAIDA
    | CMD_IF  
    | CMD_FOR   
    | CMD_WHILE 
    | BLOCO     
    | CMD_ATRIB 
    ;
    
    
CMD_ATRIB : LVALUE INDICE _ATRIB E
          | LVALUE _ATRIB E
            { gera_codigo_atribuicao( $$, $1, $3 ); }
          ;
            

LVALUE: _ID { busca_tipo_da_variavel( $$, $1 ); }
        ;
          
INDICE : '[' EXPS ']' '[' EXPS ']'
       | '[' EXPS ']'
       ;          
       
EXPS : E ',' EXPS
     | E
     ;
     
CMD_WHILE : _WHILE '(' E ')' CMD
          ;
CMD_FOR : _FOR _ID _ATRIB E _TO E _DO CMD
        ;
    
BLOCO : _BEGIN CMDS _END
        { $$ = $2; }
      ;    
    
CMD_IF : _IF E _THEN CMD
       | _IF E _THEN  CMD  _ELSE CMD
        { gera_cmd_if ($$ , $2, $4, $6); }
       ;    
    
SAIDA : _WRITE '(' E ')'
        { $$.c = "  printf( \"%"+ $3.t.fmt + "\", " + $3.v + " );\n";}
      | _WRITELN '(' E ')' 
        { $$.c = "  printf( \"%"+ $3.t.fmt + "\\n\", " + $3.v + " );\n";}
      ;
   
E : E '+' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '-' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '*' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '/' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '>' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '<' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E '=' E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | E _MOD E { gera_codigo_operador( $$, $1, $2, $3 ); }
  | F
  ;
  
F : _CTE_STRING { $$ = $1; $$.t = String; }
  | _CTE_INTEGER { $$ = $1; $$.t = Integer; }
  | _ID         { busca_tipo_da_variavel( $$, $1 ); }
  | '(' E ')'   {$$ = $2;}
  ;     
 
%%

#include "lex.yy.c"

void erro( string st ) {
  yyerror( st.c_str() );
  exit( 1 );
}

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     fprintf( stderr, "%s\nNo final do arquivo\n", st );
   else  
     fprintf( stderr, "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st, 
              yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}


void inicializa_tabela_de_resultado_de_operacoes() {
  map< string, Tipo > r;
  
  // OBS: a ordem é muito importante!!  
  r[par(Integer, Integer)] = Integer;    
  tro[ "%" ] = r;    
  
  r[par(Integer, Real)] = Real;    
  r[par(Integer, Double)] = Double;    
  r[par(Real, Integer)] = Real;    
  r[par(Real, Real)] = Real;    
  r[par(Real, Double)] = Double;    
  r[par(Double, Integer)] = Double;    
  r[par(Double, Real)] = Double;    
  r[par(Double, Double)] = Double;    

  tro[ "-" ] = r; 
  tro[ "*" ] = r; 
  tro[ "/" ] = r; 

  r[par(Char, Char)] = String;      
  r[par(String, Char)] = String;      
  r[par(Char, String)] = String;    
  r[par(String, String)] = String;    
  tro[ "+" ] = r; 
  
  r.clear();
  r[par(Integer, Integer)] = Boolean; 
  r[par(Real, Real)] = Boolean;    
  r[par(Real, Double)] = Boolean;    
  r[par(Double, Real)] = Boolean;    
  r[par(Double, Double)] = Boolean;    
  r[par(Char, Char)] = Boolean;      
  r[par(String, Char)] = Boolean;      
  r[par(Char, String)] = Boolean;    
  r[par(String, String)] = Boolean;    
  r[par(Boolean, Boolean)] = Boolean;    
  tro["=="] = r;
  tro["!="] = r;
  tro[">="] = r;
  tro[">"] = r;
  tro["<"] = r;
  tro["<="] = r;
}

void inicializa_tipos() {
  Range r = { 0, 255 };
  
  String.dim.push_back( r );
}

int main( int argc, char* argv[] )
{
  inicializa_tipos();
  inicializa_tabela_de_resultado_de_operacoes();
  yyparse();
}

