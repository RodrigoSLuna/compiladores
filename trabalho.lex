%{
int yyrowno = 1;
void trata_folha();
void trata_aspas_simples();
%}
WS      [\t ]
DIGITO  [0-9]
LETRA   [A-Za-z_]
ID      {LETRA}({LETRA}|{DIGITO})*

Algoritmo 	Algoritmo
WRITELN		[Ww][Rr][Ii][Tt][Ee][Ll][Nn]
WRITE		[Ww][Rr][Ii][Tt][Ee]
SemCasaDecimal	SemCasaDecimal
STRING		[Ss][Tt][Rr][Ii][Nn][Gg]
VAR		[Vv][Aa][Rr]
SERA_QUE	SERA_QUE
ELSE		[Ee][Ll][Ss][Ee]
FOR			[Ff][Oo][Rr]
TO			[Tt][Oo]
DO			[Dd][Oo]
PARA_NAO_ESCREVER_TUDO_DE_NOVO	PARA_NAO_ESCREVER_TUDO_DE_NOVO

CTE_STRING	"'"([^'\n]|"''")*"'"
CTE_INTEGER {DIGITO}+

%%
"\n" { yylineno++; yyrowno = 1; }
{WS} { yyrowno += 1; }

{Algoritmo} 	{ trata_folha(); return _PROGRAM; }

{WRITELN} 	{ trata_folha(); return _WRITELN; }
{WRITE} 	{ trata_folha(); return _WRITE; }
{STRING} 	{ trata_folha(); return _STRING; }
{SemCasaDecimal} { trata_folha(); return _INTEGER; }
{VAR} 		{ trata_folha(); return _VAR; }
{SERA_QUE} 	{ trata_folha(); return _IF; }
{ELSE} 		{ trata_folha(); return _ELSE; }
{FOR} 		{ trata_folha(); return _FOR; }
{TO} 		{ trata_folha(); return _TO; }
{DO} 		{ trata_folha(); return _DO; }
{PARA_NAO_ESCREVER_TUDO_DE_NOVO}  { trata_folha(); return _FUNCTION; }

{CTE_STRING} 	{ trata_aspas_simples(); return _CTE_STRING; }
{CTE_INTEGER} 	{ trata_folha(); return _CTE_INTEGER; }

":)" 		{trata_folha(); return _BEGIN;}
":("		{trata_folha(); return _END;}
"<-"			{ trata_folha(); return _ATRIB; }
"?" 		{ trata_folha(); return _THEN; }

{ID}  { trata_folha(); return _ID; }

.     { trata_folha(); return yytext[0]; }

%%

void trata_folha() {
  yylval.v = yytext;
  yylval.t = "";
  yylval.c = "";
  
  yyrowno += strlen( yytext ); 
}

void trata_aspas_simples() {
  trata_folha(); 
  yylval.v = "\"" + yylval.v.substr( 1, yylval.v.length()-2 ) + "\""; 
}

