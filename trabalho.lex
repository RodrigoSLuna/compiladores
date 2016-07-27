%{
    int yyrowno = 0;
    void trata_folha( );
    void trata_aspas_simples();
    char *strupr( char* st );

%}

WS                              [\t ]
DIGITO                          [0-9]
LETRA                           [A-Za-z_]
ID                              {LETRA}({LETRA}|{DIGITO})*

HappyHour                       HappyHour
BEGIN                           [Bb][Ee][Gg][Ii][Nn]
END			        [Ee][Nn][Dd]
ColocaEFechaAConta	        ColocaEFechaAConta
ColocaNaConta		        ColocaNaConta
QuantoDevo		        Quanto
Quanto		                QuantoDevo
HalfHalf                        HalfHalf
MeiaGarrafa                     MeiaGarrafa
CopoInteiro	                CopoInteiro
VoceBebeu                       VoceBebeu
CharPointerMelhorado		CharPointerMelhorado
Amigos			        Amigos
Sera			        Sera
Que		                Que
ELSE		                [Ee][Ll][Ss][Ee]
MaisUma			        MaisUma
Para			        Para
Beber			        Beber
Rodada	                        Rodada
BebaEnquanto                    BebaEnquanto
CTE_STRING	                "'"([^'\n]|"''")*"'"
CTE_INTEGER                     {DIGITO}+

%%
"\n" {yylineno++; yyrowno = 1;}
{WS} { yyrowno += 1;}

{HappyHour} 	        { trata_folha(); return _PROGRAM; }


{END} 		        { trata_folha(); return _END; }
{ColocaEFechaAConta} 	{ trata_folha(); return _WRITELN; }
{ColocaNaConta} 	{ trata_folha(); return _WRITE; } 
{QuantoDevo} 	        { trata_folha(); return _READLN; }
{Quanto} 		{ trata_folha(); return _READ; }
{VoceBebeu}             { trata_folha(); return _BOOLEAN; }
{HalfHalf}              { trata_folha(); return _DOUBLE; }
{MeiaGarrafa}           { trata_folha(); return _REAL; }
{CharPointerMelhorado} 	{ trata_folha(); return _STRING; }
{CopoInteiro} 	        { trata_folha(); return _INTEGER; }
{Amigos} 		{ trata_folha(); return _VAR; }
{Sera} 		        { trata_folha(); return _IF; }
{Que} 		        { trata_folha(); return _THEN; }
{ELSE} 		        { trata_folha(); return _ELSE; }
{MaisUma} 		{ trata_folha(); return _FOR; }
{BebaEnquanto} 	        { trata_folha(); return _WHILE; }
{Para} 		        { trata_folha(); return _TO; }
{Beber} 		{ trata_folha(); return _DO; }
{Rodada}                { trata_folha(); return _FUNCTION; }

{CTE_STRING} 	        { trata_aspas_simples(); return _CTE_STRING; }
{CTE_INTEGER} 	        { trata_folha(); return _CTE_INTEGER; }

":)"                    { trata_folha(); return _BEGIN;}
":("                    { trata_folha(); return _END;}

"<-"			{ trata_folha(); return _ATRIB; }
"equals" 	        { trata_folha(); yylval.v = "=="; return '='; }

"&&"                    { trata_folha(); return _ATRIB; } // nao eh atributo eh condicao
"||"                    { trata_folha(); return _ATRIB; } // nao eh atributo eh condicao
"%"                     { trata_folha(); return _MOD; }

{ID}                    { trata_folha(); return _ID; }

.                       { trata_folha(); return yytext[0]; }

%%

void trata_folha() {
  yylval.v = yytext;
  yylval.t.nome = "";
  yylval.t.decl = "";
  yylval.t.fmt = "";
  yylval.c = "";
  yylval.lst.clear();
  
  yyrowno += strlen( yytext ); 
}

void trata_aspas_simples() {
  trata_folha(); 
  yylval.v = "\"" + yylval.v.substr( 1, yylval.v.length()-2 ) + "\""; 
}

char* strupr( char* st ) {
  char * aux = st;
  
  while( *st ) { 
    *st = toupper( *st ); 
    ++st;
  }
  return aux;
}

