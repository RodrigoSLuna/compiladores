%{
    int yyrowno = 0;
    void trata_folha( );
    void trata_aspas_simples();
%}

WS                              [\t ]
DIGITO                          [0-9]
LETRA                           [A-Za-z_]
ID                              {LETRA}({LETRA}|{DIGITO})*

Alg                             Alg
BEGIN                           [Bb][Ee][Gg][Ii][Nn]
END			        [Ee][Nn][Dd]
WRITELN		                [Ww][Rr][Ii][Tt][Ee][Ll][Nn]
WRITE		                [Ww][Rr][Ii][Tt][Ee]
SemCasaDecimal	                SemCasaDecimal
CharPointerMelhorado		CharPointerMelhorado
Legenda			        Legenda
IF			        [Ii][Ff]
THEN		                [Tt][Hh][Ee][Nn]
ELSE		                [Ee][Ll][Ss][Ee]
FOR			        [Ff][Oo][Rr]
TO			        [Tt][Oo]
DO			        [Dd][Oo]
FUNCTION	                [Ff][Uu][Nn][Cc][Tt][Ii][Oo][Nn]
BebaEnquanto                     BebaEnquanto
CTE_STRING	                "'"([^'\n]|"''")*"'"
CTE_INTEGER                     {DIGITO}+

%%
"\n"            {yylineno++; yyrowno = 1;}
{WS} {}

{Alg} 	         { trata_folha(); return _PROGRAM; }

{END} 		{ trata_folha(); return _END; }
{WRITELN} 	{ trata_folha(); return _WRITELN; }
{WRITE} 	{ trata_folha(); return _WRITE; }
{CharPointerMelhorado} 	{ trata_folha(); return _STRING; }
{SemCasaDecimal} 	{ trata_folha(); return _INTEGER; }
{Legenda} 		{ trata_folha(); return _VAR; }
{IF} 		{ trata_folha(); return _IF; }
{THEN} 		{ trata_folha(); return _THEN; }
{ELSE} 		{ trata_folha(); return _ELSE; }
{FOR} 		{ trata_folha(); return _FOR; }
{BebaEnquanto} 	{ trata_folha(); return _WHILE; }
{TO} 		{ trata_folha(); return _TO; }
{DO} 		{ trata_folha(); return _DO; }
{FUNCTION}      { trata_folha(); return _FUNCTION; }

{CTE_STRING} 	{ trata_aspas_simples(); return _CTE_STRING; }
{CTE_INTEGER} 	{ trata_folha(); return _CTE_INTEGER; }

":)"            { trata_folha(); return _BEGIN;}
":("            { trata_folha(); return _END;}
"<-"		{ trata_folha(); return _ATRIB; }
"&&"            { trata_folha(); return _ATRIB; } // nao eh atributo eh condicao
"||"            { trata_folha(); return _ATRIB; } // nao eh atributo eh condicao
"%"             { trata_folha(); return _ATRIB; }
"+"		{ trata_folha(); return _ATRIB; }
"-"		{ trata_folha(); return _ATRIB; }
"/"		{ trata_folha(); return _ATRIB; }
"*"		{ trata_folha(); return _ATRIB; }
"^"		{ trata_folha(); return _ATRIB; }

{ID}  { trata_folha(); return _ID; }

.     { trata_folha(); return yytext[0]; }

%%

void trata_folha(){
    yylval.v = yytext;
    yylval.t = "";
    yylval.c = "";
    yyrowno += strlen( yytext );
}
void trata_aspas_simples(){
    trata_folha();
    yylval.v = "\"" + yylval.v.substr( 1, yylval.v.length()-2 ) + "\"";
}


