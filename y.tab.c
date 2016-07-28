/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     _ID = 258,
     _PROGRAM = 259,
     _BEGIN = 260,
     _END = 261,
     _WRITELN = 262,
     _WRITE = 263,
     _VAR = 264,
     _IF = 265,
     _THEN = 266,
     _ELSE = 267,
     _READ = 268,
     _READLN = 269,
     _FOR = 270,
     _TO = 271,
     _DO = 272,
     _ATRIB = 273,
     _FUNCTION = 274,
     _WHILE = 275,
     _MOD = 276,
     _INTEGER = 277,
     _STRING = 278,
     _REAL = 279,
     _BOOLEAN = 280,
     _DOUBLE = 281,
     _CTE_STRING = 282,
     _CTE_INTEGER = 283
   };
#endif
/* Tokens.  */
#define _ID 258
#define _PROGRAM 259
#define _BEGIN 260
#define _END 261
#define _WRITELN 262
#define _WRITE 263
#define _VAR 264
#define _IF 265
#define _THEN 266
#define _ELSE 267
#define _READ 268
#define _READLN 269
#define _FOR 270
#define _TO 271
#define _DO 272
#define _ATRIB 273
#define _FUNCTION 274
#define _WHILE 275
#define _MOD 276
#define _INTEGER 277
#define _STRING 278
#define _REAL 279
#define _BOOLEAN 280
#define _DOUBLE 281
#define _CTE_STRING 282
#define _CTE_INTEGER 283




/* Copy the first part of user declarations.  */
#line 1 "trabalho.y"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
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
 

Tipo Integer = { "int", "int", "d" };
Tipo Real =    { "real", "float", "f" };
Tipo Double =  { "double", "double", "lf" };
Tipo Bool = { "bool", "int", "d" };
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


vector < map<string,Tipo> > ts;
map<string,Tipo> tf;

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

void empilha_nova_tabela_de_simbolos() {
  ts.push_back( map< string, Tipo >() );
}

void desempilha_tabela_de_simbolos() {
  ts.pop_back();
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
                       vector<string> lst, 
                       Tipo tipo ) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() ) 
      erro( "Variável já declarada: " + lst[i] );
    else {
      ts[ts.size()-1][ lst[i] ] = tipo; 
      ss.c += tipo.decl + " " + lst[i] 
              + trata_dimensoes_decl_var( tipo ) + ";\n";
    }  
  }
}

void declara_variavel_func( Atributo& ss, 
                       vector<string> lst, 
                       Tipo tipo ) {
  ss.c = "";
  for( int i = 0; i < lst.size(); i++ ) {
    if( ts[ts.size()-1].find( lst[i] ) != ts[ts.size()-1].end() ) 
      erro( "Variável já declarada: " + lst[i] );
    else {
        
      ts[ts.size()-1][ lst[i] ] = tipo; 
      ss.c += tipo.decl + " " + lst[i] 
              + trata_dimensoes_decl_var( tipo ) + " ,"; 
    }  
  }
}

void declara_variavel( Atributo& ss, string nome, Tipo tipo ) {
 // cout << "Nome: "<< nome<< " Fim" << endl;
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel( ss, lst, tipo );
}

void declara_variavel_func( Atributo& ss, string nome, Tipo tipo ) {
  vector<string> lst;
  lst.push_back( nome );
  declara_variavel_func( ss, lst, tipo );
}


void busca_tipo_da_variavel( Atributo& ss, const Atributo& s1 ) {
  if( ts[ts.size()-1].find( s1.v ) == ts[ts.size()-1].end() )
        erro( "Variável não declarada: " + s1.v );
  else {
    ss.t = ts[ts.size()-1][ s1.v ];
    ss.v = s1.v;
  }
}

// Precisa ajeitar, para double -> integer, float -> integer
void gera_codigo_atribuicao( Atributo& ss, 
                             const Atributo& s1, 
                             const Atributo& s3 ) {
  
  if( s1.t.nome == s3.t.nome && s3.t.nome != "string" )
  {
        ss.c = s1.c + s3.c + "  " + s1.v + " = " + s3.v + ";\n";
  }
  else if( s1.t.nome == s3.t.nome &&  s1.t.nome == "string" ) {
   /*
   ss.c = s1.c + s3.c + "  " 
           + "strcpy( " + s1.v + ", " + s3.v + ", " + 
           toString( s1.t.dim[0].fim ) + " );\n";
  */
  ss.c = s1.c + s3.c + "  " 
           + "strcpy( " + s1.v + ", " + s3.v  + " );\n";
  }
}

string par( Tipo a, Tipo b ) {
  return a.nome + "," + b.nome;  
}

void gera_codigo_operador( Atributo& ss, 
                           const Atributo& s1, 
                           const Atributo& s2, 
                           const Atributo& s3 ) {
  //cout << s1.t.nome << "  " << s3.t.nome << endl;
  string aux;
  if( tro.find( s2.v ) != tro.end() ) {
    if( tro[s2.v].find( par( s1.t, s3.t ) ) != tro[s2.v].end() ) {
      ss.t = tro[s2.v][par( s1.t, s3.t )];
      ss.v = gera_nome_var( ss.t );
      if(s1.t.nome == "string" && s3.t.nome == "string"){
        ss.c = "  strcat(" + s1.v + ", " + s3.v + ")"
             +  s1.c + s3.c  +";\n";
        ss.c += "  strcpy("+ ss.v + ", " + s1.v +");\n";
      }
      else{
        ss.c = s1.c + s3.c + "  " + ss.v + " = " + s1.v + s2.v + s3.v 
                + ";\n";
      }
    }
    else
      erro( "O operador '" + s2.v + "' não está definido para os tipos " + s1.t.nome + " e " + s3.t.nome + "." );
  }
  else
    erro( "Operador '" + s2.v + "' não definido." );
}

string declara_nvar_temp( Tipo t, int qtde ) {
  string aux = "";
  string aux1; 
  for( int i = 1; i <= qtde; i++ )
    aux += t.decl + " t_" + t.nome + "_" + toString( i ) + ";\n";
  if(t.nome == "string"){
     aux.erase(aux.begin()+aux.size()-1);
     aux.erase(aux.begin()+aux.size()-1);
     aux += "[10000];\n";
  }
  return aux;  
}

string declara_var_temp( map< string, int >& temp ) {
  string decls = "// variáveis temporárias \n" + 
    declara_nvar_temp( Integer, temp[Integer.nome] ) +
    declara_nvar_temp( Real, temp[Real.nome] ) +
    declara_nvar_temp( Double, temp[Double.nome] ) +
    declara_nvar_temp( String, temp[String.nome] ) +
    declara_nvar_temp( Char, temp[Char.nome] ) +
    declara_nvar_temp( Bool, temp[Bool.nome] ) +
    "\n";
  
  temp.clear();
  
  return decls;
}

void gera_cmd_if( Atributo& ss, 
                  const Atributo& exp, 
                  const Atributo& cmd_then, 
                  string cmd_else ) { 
  string lbl_then = gera_nome_label( "then" );
  string lbl_end_if = gera_nome_label( "end_if" );
  
  if( exp.t.nome != Bool.nome )
    erro( "A expressão do IF deve ser booleana!" );
    
  ss.c = exp.c + 
         "\nif( " + exp.v + " ) goto " + lbl_then + ";\n" +
         cmd_else + "  goto " + lbl_end_if + ";\n\n" +
         lbl_then + ":;\n" + 
         cmd_then.c + "\n" +
         lbl_end_if + ":;\n"; 
}
void gera_cmd_while( Atributo& ss, 
                     const Atributo& exp,
                     const Atributo& cmd ) {
        
     gera_cmd_if( ss, exp, cmd, ""  );
     ss.c = "while(true){\n" + ss.c +"break;\n}" ;
}
void gera_cmd_for( Atributo&ss,
                   const Atributo& atrib,
                   const Atributo& exp,
                    Atributo& cmd1,
                    Atributo& cmd2){
  cmd1.c =   cmd2.c + cmd1.c + "continue;\n";
  gera_cmd_if( ss, exp ,cmd1 , "" );
    ss.c = atrib.c + "\nfor( true ; true ; true  ){\n" + ss.c +"break;\n" + "\t}\n"; 
}
/*  
// apenas um if, sem o else
void gera_cmd_if( Atributo& ss, 
                  const Atributo& exp, 
                  const Atributo& cmd_then) { 
  string lbl_then = gera_nome_label( "then" );
  string lbl_end_if = gera_nome_label( "end_if" );
  
  if( exp.t.nome != Boolean.nome )
    erro( "A expressão do IF deve ser booleana!" );
    
  ss.c = exp.c + 
         "\nif( " + exp.v + " ) goto " + lbl_then + ";\n" +
         lbl_end_if + ":;\n"; 
}
*/
void gera_codigo_funcao( Atributo& ss, 
                         const Atributo& retorno, 
                         string nome, 
                         string params,
                         string bloco ) {

  ss.c = retorno.t.nome + " " + nome + "( " + params + " )" + 
         "{\n" +
         retorno.c +
         declara_var_temp( temp_local ) + 
         bloco +
         "return Result;\n}\n";
}         

void conserta(string &nome){
    cout << nome << endl;
    string k = ";";
    for(int i = 0;i<nome.size();i++){
       if( nome[i] == ';' )
        nome[i] = ',';
    }
    nome[nome.size()-2] = ' ';
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 480 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   224

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  150

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,     2,
      41,    42,    35,    33,    32,    34,    46,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    40,
      30,    31,    29,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    44,     2,    45,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     7,    11,    14,    15,    17,    19,    20,
      21,    34,    35,    36,    46,    50,    52,    55,    59,    62,
      66,    68,    70,    72,    74,    77,    81,    82,    86,    88,
      93,    97,    98,   100,   102,   104,   106,   108,   110,   112,
     117,   121,   123,   130,   134,   138,   140,   146,   156,   160,
     165,   172,   177,   182,   187,   192,   196,   200,   204,   208,
     212,   216,   220,   224,   226,   228,   230,   232,   236
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    49,    50,    64,    -1,     4,     3,    40,
      -1,    51,    50,    -1,    -1,    58,    -1,    52,    -1,    -1,
      -1,    19,     3,    53,    41,    57,    42,    43,    61,    40,
      54,    73,    40,    -1,    -1,    -1,    19,     3,    55,    43,
      61,    40,    56,    73,    40,    -1,    60,    40,    57,    -1,
      60,    -1,     9,    59,    -1,    60,    40,    59,    -1,    60,
      40,    -1,    63,    43,    61,    -1,    22,    -1,    24,    -1,
      26,    -1,    25,    -1,    23,    62,    -1,    44,    28,    45,
      -1,    -1,    63,    32,     3,    -1,     3,    -1,     5,    65,
       6,    46,    -1,    66,    40,    65,    -1,    -1,    75,    -1,
      76,    -1,    74,    -1,    72,    -1,    71,    -1,    73,    -1,
      67,    -1,    68,    69,    18,    77,    -1,    68,    18,    77,
      -1,     3,    -1,    44,    70,    45,    44,    70,    45,    -1,
      44,    70,    45,    -1,    77,    32,    70,    -1,    77,    -1,
      20,    41,    77,    42,    66,    -1,    15,    41,    77,    16,
      77,    17,    67,    42,    66,    -1,     5,    65,     6,    -1,
      10,    77,    11,    66,    -1,    10,    77,    11,    66,    12,
      66,    -1,     8,    41,    77,    42,    -1,     7,    41,    77,
      42,    -1,    14,    41,    68,    42,    -1,    13,    41,    68,
      42,    -1,    77,    33,    77,    -1,    77,    34,    77,    -1,
      77,    35,    77,    -1,    77,    36,    77,    -1,    77,    29,
      77,    -1,    77,    30,    77,    -1,    77,    31,    77,    -1,
      77,    21,    77,    -1,    78,    -1,    27,    -1,    28,    -1,
       3,    -1,    41,    77,    42,    -1,     3,    41,    70,    42,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   335,   335,   340,   347,   350,   353,   354,   357,   360,
     357,   367,   370,   367,   378,   379,   383,   386,   387,   390,
     393,   395,   396,   397,   398,   401,   403,   405,   406,   409,
     413,   414,   417,   418,   419,   420,   421,   422,   423,   427,
     428,   433,   436,   437,   440,   441,   444,   447,   450,   453,
     455,   459,   461,   465,   467,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   482,   483,   484,   485,   486
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ID", "_PROGRAM", "_BEGIN", "_END",
  "_WRITELN", "_WRITE", "_VAR", "_IF", "_THEN", "_ELSE", "_READ",
  "_READLN", "_FOR", "_TO", "_DO", "_ATRIB", "_FUNCTION", "_WHILE", "_MOD",
  "_INTEGER", "_STRING", "_REAL", "_BOOLEAN", "_DOUBLE", "_CTE_STRING",
  "_CTE_INTEGER", "'>'", "'<'", "'='", "','", "'+'", "'-'", "'*'", "'/'",
  "'^'", "'&'", "'|'", "';'", "'('", "')'", "':'", "'['", "']'", "'.'",
  "$accept", "S", "NOME", "MIOLOS", "MIOLO", "FUNCTION", "@1", "@2", "@3",
  "@4", "PARAMETROS", "VARS", "DECLS", "DECL", "TIPO", "TAM_STRING", "IDS",
  "PRINCIPAL", "CMDS", "CMD", "CMD_ATRIB", "LVALUE", "INDICE", "EXPS",
  "CMD_WHILE", "CMD_FOR", "BLOCO", "CMD_IF", "SAIDA", "ENTRADA", "E", "F", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    62,
      60,    61,    44,    43,    45,    42,    47,    94,    38,   124,
      59,    40,    41,    58,    91,    93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    50,    50,    51,    51,    53,    54,
      52,    55,    56,    52,    57,    57,    58,    59,    59,    60,
      61,    61,    61,    61,    61,    62,    62,    63,    63,    64,
      65,    65,    66,    66,    66,    66,    66,    66,    66,    67,
      67,    68,    69,    69,    70,    70,    71,    72,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    78,    78,    78,    78,    78
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     2,     0,     1,     1,     0,     0,
      12,     0,     0,     9,     3,     1,     2,     3,     2,     3,
       1,     1,     1,     1,     2,     3,     0,     3,     1,     4,
       3,     0,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     1,     6,     3,     3,     1,     5,     9,     3,     4,
       6,     4,     4,     4,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     5,     0,     1,     0,     0,     0,     5,
       7,     6,     3,    28,    16,     0,     0,     8,    31,     2,
       4,    18,     0,     0,     0,     0,    41,    31,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    38,     0,    36,
      35,    37,    34,    32,    33,    17,    27,    20,    26,    21,
      23,    22,    19,     0,     0,     0,     0,     0,    66,    64,
      65,     0,     0,    63,     0,     0,     0,     0,     0,    31,
       0,     0,     0,     0,    24,     0,    15,     0,    48,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    29,    30,    40,     0,
      45,     0,     0,     0,     0,    12,    52,    51,     0,    67,
      49,    62,    59,    60,    61,    55,    56,    57,    58,    54,
      53,     0,     0,    43,     0,    39,    25,     0,    14,     0,
      68,     0,     0,    46,     0,    44,     0,     0,    50,     0,
       0,     9,    13,     0,    42,     0,     0,     0,    47,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     8,     9,    10,    24,   145,    25,   129,
      75,    11,    14,    15,    52,    74,    16,    19,    35,    36,
      37,    38,    72,    99,    39,    40,    41,    42,    43,    44,
     100,    63
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -116
static const yytype_int16 yypact[] =
{
       4,     8,    13,    -2,   -19,  -116,    31,    35,    34,    -2,
    -116,  -116,  -116,  -116,  -116,     9,     3,     0,   177,  -116,
    -116,    31,    41,    76,    10,    21,  -116,   177,    25,    26,
       1,    27,    32,    44,    45,    47,    36,  -116,   -12,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,    28,  -116,
    -116,  -116,  -116,    31,    76,    69,     1,     1,    46,  -116,
    -116,     1,    59,  -116,    94,    94,     1,     1,    57,   177,
       1,     1,    89,    85,  -116,    72,    78,    79,  -116,    75,
      91,     1,   107,   177,     1,     1,     1,     1,     1,     1,
       1,     1,    73,    74,   -11,   123,  -116,  -116,    48,    84,
     172,     1,    86,    80,    31,  -116,  -116,  -116,    88,  -116,
     120,  -116,   188,   188,   188,    12,    12,  -116,  -116,  -116,
    -116,     1,   177,    90,     1,    48,  -116,    76,  -116,   130,
    -116,   177,   143,  -116,     1,  -116,    99,   105,  -116,    94,
     101,  -116,  -116,   106,  -116,   130,   177,   110,  -116,  -116
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -116,  -116,  -116,   138,  -116,  -116,  -116,  -116,  -116,  -116,
      51,  -116,   140,   -41,   -53,  -116,  -116,  -116,   -24,   -81,
      23,   -49,  -116,   -72,  -116,  -116,  -115,  -116,  -116,  -116,
     -30,  -116
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -12
static const yytype_int16 yytable[] =
{
      62,    77,   110,    55,    58,   121,    70,     6,     1,   108,
      84,     4,    76,     5,   137,    92,    93,     7,    85,    86,
      87,    12,    88,    89,    90,    91,    79,    80,    59,    60,
     147,    82,    71,    84,    13,    22,    94,    95,    17,    18,
      98,   133,    61,   -11,    46,    97,    23,    90,    91,    21,
     138,    53,   135,    68,   111,   112,   113,   114,   115,   116,
     117,   118,   140,    76,    54,   148,    56,    57,    64,    84,
      83,   125,    73,    65,   136,    78,    69,    85,    86,    87,
      84,    88,    89,    90,    91,    66,    67,    81,    85,    86,
      87,   132,    88,    89,    90,    91,    84,    26,    47,    48,
      49,    50,    51,    96,    85,    86,    87,   101,    88,    89,
      90,    91,    84,   102,   103,   119,   120,   106,   104,   105,
      85,    86,    87,   127,    88,    89,    90,    91,    84,   123,
     130,   126,   131,   107,   134,    27,    85,    86,    87,   141,
      88,    89,    90,    91,    84,   142,   144,    20,   146,   109,
     149,     0,    85,    86,    87,   128,    88,    89,    90,    91,
     139,    45,   143,     0,    84,   122,     0,     0,     0,     0,
       0,     0,    85,    86,    87,     0,    88,    89,    90,    91,
      26,     0,    27,     0,    28,    29,     0,    30,     0,     0,
      31,    32,    33,    84,     0,     0,     0,    34,     0,     0,
       0,    85,    86,    87,   124,    88,    89,    90,    91,    84,
       0,     0,     0,     0,     0,     0,     0,   -12,   -12,   -12,
       0,    88,    89,    90,    91
};

static const yytype_int16 yycheck[] =
{
      30,    54,    83,    27,     3,    16,    18,     9,     4,    81,
      21,     3,    53,     0,   129,    64,    65,    19,    29,    30,
      31,    40,    33,    34,    35,    36,    56,    57,    27,    28,
     145,    61,    44,    21,     3,    32,    66,    67,     3,     5,
      70,   122,    41,    43,     3,    69,    43,    35,    36,    40,
     131,    41,   124,     6,    84,    85,    86,    87,    88,    89,
      90,    91,   134,   104,    43,   146,    41,    41,    41,    21,
      11,   101,    44,    41,   127,     6,    40,    29,    30,    31,
      21,    33,    34,    35,    36,    41,    41,    41,    29,    30,
      31,   121,    33,    34,    35,    36,    21,     3,    22,    23,
      24,    25,    26,    46,    29,    30,    31,    18,    33,    34,
      35,    36,    21,    28,    42,    42,    42,    42,    40,    40,
      29,    30,    31,    43,    33,    34,    35,    36,    21,    45,
      42,    45,    12,    42,    44,     5,    29,    30,    31,    40,
      33,    34,    35,    36,    21,    40,    45,     9,    42,    42,
      40,    -1,    29,    30,    31,   104,    33,    34,    35,    36,
      17,    21,   139,    -1,    21,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    -1,    33,    34,    35,    36,
       3,    -1,     5,    -1,     7,     8,    -1,    10,    -1,    -1,
      13,    14,    15,    21,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    33,    34,    35,    36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    48,    49,     3,     0,     9,    19,    50,    51,
      52,    58,    40,     3,    59,    60,    63,     3,     5,    64,
      50,    40,    32,    43,    53,    55,     3,     5,     7,     8,
      10,    13,    14,    15,    20,    65,    66,    67,    68,    71,
      72,    73,    74,    75,    76,    59,     3,    22,    23,    24,
      25,    26,    61,    41,    43,    65,    41,    41,     3,    27,
      28,    41,    77,    78,    41,    41,    41,    41,     6,    40,
      18,    44,    69,    44,    62,    57,    60,    61,     6,    77,
      77,    41,    77,    11,    21,    29,    30,    31,    33,    34,
      35,    36,    68,    68,    77,    77,    46,    65,    77,    70,
      77,    18,    28,    42,    40,    40,    42,    42,    70,    42,
      66,    77,    77,    77,    77,    77,    77,    77,    77,    42,
      42,    16,    42,    45,    32,    77,    45,    43,    57,    56,
      42,    12,    77,    66,    44,    70,    61,    73,    66,    17,
      70,    40,    40,    67,    45,    54,    42,    73,    66,    40
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 336 "trabalho.y"
    { cout << (yyvsp[(1) - (3)]).c << declara_var_temp( temp_global )
           << (yyvsp[(2) - (3)]).c <<  (yyvsp[(3) - (3)]).c << endl;}
    break;

  case 3:
#line 341 "trabalho.y"
    { (yyval).c = "#include <stdlib.h>\n"
                "#include <string.h>\n" 
                "#include <stdio.h>\n\n";
        }
    break;

  case 4:
#line 348 "trabalho.y"
    { (yyval).c = (yyvsp[(1) - (2)]).c + (yyvsp[(2) - (2)]).c ; }
    break;

  case 5:
#line 350 "trabalho.y"
    {(yyval).c = "" ;}
    break;

  case 8:
#line 357 "trabalho.y"
    { escopo_local = true; 
                           empilha_nova_tabela_de_simbolos(); }
    break;

  case 9:
#line 360 "trabalho.y"
    { declara_variavel( (yyvsp[(8) - (9)]), "Result", (yyvsp[(8) - (9)]).t );
             tf[(yyvsp[(2) - (9)]).v] = (yyvsp[(8) - (9)]).t; }
    break;

  case 10:
#line 363 "trabalho.y"
    { conserta((yyvsp[(5) - (12)]).c) ;gera_codigo_funcao( (yyval), (yyvsp[(8) - (12)]), (yyvsp[(2) - (12)]).v, (yyvsp[(5) - (12)]).c, (yyvsp[(11) - (12)]).c ); 
             escopo_local = false;
             desempilha_tabela_de_simbolos(); }
    break;

  case 11:
#line 367 "trabalho.y"
    { escopo_local = true; 
                           empilha_nova_tabela_de_simbolos(); }
    break;

  case 12:
#line 370 "trabalho.y"
    { declara_variavel( (yyvsp[(5) - (6)]), "Result", (yyvsp[(5) - (6)]).t ); 
             tf[(yyvsp[(2) - (6)]).v] = (yyvsp[(5) - (6)]).t; }
    break;

  case 13:
#line 373 "trabalho.y"
    { gera_codigo_funcao( (yyval), (yyvsp[(5) - (9)]), (yyvsp[(2) - (9)]).v, "", (yyvsp[(8) - (9)]).c ); 
             escopo_local = false;
             desempilha_tabela_de_simbolos(); }
    break;

  case 16:
#line 383 "trabalho.y"
    {(yyval).c = (yyvsp[(2) - (2)]).c;}
    break;

  case 17:
#line 386 "trabalho.y"
    { (yyval).c =  (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c;}
    break;

  case 19:
#line 390 "trabalho.y"
    { declara_variavel( (yyval), (yyvsp[(1) - (3)]).lst, (yyvsp[(3) - (3)]).t);}
    break;

  case 20:
#line 393 "trabalho.y"
    { (yyval).t = Integer;}
    break;

  case 21:
#line 395 "trabalho.y"
    { (yyval).t = Real; }
    break;

  case 22:
#line 396 "trabalho.y"
    { (yyval).t = Double; }
    break;

  case 23:
#line 397 "trabalho.y"
    { (yyval).t = Bool;}
    break;

  case 24:
#line 398 "trabalho.y"
    { (yyval).t = (yyvsp[(2) - (2)]).t ; }
    break;

  case 25:
#line 402 "trabalho.y"
    { (yyval).t = String; (yyval).t.dim[0].fim = toInt( (yyvsp[(2) - (3)]).v ); }
    break;

  case 26:
#line 403 "trabalho.y"
    { (yyval).t = String; }
    break;

  case 27:
#line 405 "trabalho.y"
    { (yyval).lst = (yyvsp[(1) - (3)]).lst; (yyval).lst.push_back( (yyvsp[(3) - (3)]).v ); }
    break;

  case 28:
#line 406 "trabalho.y"
    { (yyval).lst.push_back( (yyvsp[(1) - (1)]).v ); }
    break;

  case 29:
#line 410 "trabalho.y"
    { (yyval).c = "int main() {\n" + (yyvsp[(2) - (4)]).c + "}\n";}
    break;

  case 30:
#line 413 "trabalho.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c;  }
    break;

  case 31:
#line 414 "trabalho.y"
    {(yyval).c = "";}
    break;

  case 40:
#line 429 "trabalho.y"
    { gera_codigo_atribuicao( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 41:
#line 433 "trabalho.y"
    { busca_tipo_da_variavel( (yyval), (yyvsp[(1) - (1)]) ); }
    break;

  case 44:
#line 440 "trabalho.y"
    { (yyval).c = (yyvsp[(1) - (3)]).c + (yyvsp[(3) - (3)]).c; (yyval).v = (yyvsp[(1) - (3)]).v + " , " + (yyvsp[(3) - (3)]).v; }
    break;

  case 46:
#line 444 "trabalho.y"
    { gera_cmd_while( (yyval), (yyvsp[(3) - (5)]), (yyvsp[(5) - (5)])  );  }
    break;

  case 47:
#line 447 "trabalho.y"
    { gera_cmd_for( (yyval), (yyvsp[(3) - (9)]), (yyvsp[(5) - (9)]), (yyvsp[(7) - (9)]), (yyvsp[(9) - (9)]) );  }
    break;

  case 48:
#line 450 "trabalho.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 49:
#line 454 "trabalho.y"
    { gera_cmd_if( (yyval), (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)]), ""); }
    break;

  case 50:
#line 456 "trabalho.y"
    { gera_cmd_if ((yyval) , (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)]).c); }
    break;

  case 51:
#line 460 "trabalho.y"
    { (yyval).c = "  printf( \"%"+ (yyvsp[(3) - (4)]).t.fmt + "\", " + (yyvsp[(3) - (4)]).v + " );\n";}
    break;

  case 52:
#line 462 "trabalho.y"
    { (yyval).c = "  printf( \"%"+ (yyvsp[(3) - (4)]).t.fmt + "\\n\", " + (yyvsp[(3) - (4)]).v + " );\n";}
    break;

  case 53:
#line 466 "trabalho.y"
    { (yyval).c = "  scanf( \"%"+ (yyvsp[(3) - (4)]).t.fmt + "\", &"+ (yyvsp[(3) - (4)]).v + " );\n"; }
    break;

  case 54:
#line 468 "trabalho.y"
    { (yyval).c = "  scanf( \"%"+ (yyvsp[(3) - (4)]).t.fmt + "\", &"+ (yyvsp[(3) - (4)]).v + " );\n"; }
    break;

  case 55:
#line 471 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 56:
#line 472 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 57:
#line 473 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 58:
#line 474 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 59:
#line 475 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 60:
#line 476 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 61:
#line 477 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 62:
#line 478 "trabalho.y"
    { gera_codigo_operador( (yyval), (yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)]) ); }
    break;

  case 64:
#line 482 "trabalho.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).t = String; }
    break;

  case 65:
#line 483 "trabalho.y"
    { (yyval) = (yyvsp[(1) - (1)]); (yyval).t = Integer; }
    break;

  case 66:
#line 484 "trabalho.y"
    { busca_tipo_da_variavel( (yyval), (yyvsp[(1) - (1)]) ); }
    break;

  case 67:
#line 485 "trabalho.y"
    {(yyval) = (yyvsp[(2) - (3)]);}
    break;

  case 68:
#line 486 "trabalho.y"
    { (yyval).v = gera_nome_var( tf[(yyvsp[(1) - (4)]).v] );
                    (yyval).c = (yyvsp[(3) - (4)]).c +
                    " " + (yyval).v + " = " + (yyvsp[(1) - (4)]).v + "( " + (yyvsp[(3) - (4)]).v + " );\n";
                    (yyval).t = tf[ (yyvsp[(1) - (4)]).v ] ; }
    break;


/* Line 1267 of yacc.c.  */
#line 2079 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 492 "trabalho.y"


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
  r[par(Integer, Integer)] = Bool; 
  r[par(Real, Real)] = Bool;    
  r[par(Real, Double)] = Bool;    
  r[par(Double, Real)] = Bool;    
  r[par(Double, Double)] = Bool;    
  r[par(Char, Char)] = Bool;      
  r[par(String, Char)] = Bool;      
  r[par(Char, String)] = Bool;    
  r[par(String, String)] = Bool;    
  r[par(Bool, Bool)] = Bool;    
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
  empilha_nova_tabela_de_simbolos(); 
  
  yyparse();
}


