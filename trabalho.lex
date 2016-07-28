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

//Palavras reservadas

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

//Declara o nome do programa
{HappyHour} 	        { trata_folha(); return _PROGRAM; }

{END} 		        { trata_folha(); return _END; }

//Equivalente ao WriteLn do Pascal 
{ColocaEFechaAConta} 	{ trata_folha(); return _WRITELN; }

//Equivalente ao Write do Pascal
{ColocaNaConta} 	{ trata_folha(); return _WRITE; } 

//Equivalente ao ReadLn do Pascal
{QuantoDevo} 	        { trata_folha(); return _READLN; }

//Equivalente ao Read do Pascal
{Quanto} 		{ trata_folha(); return _READ; }

//Variavel do tipo booleana 
{VoceBebeu}             { trata_folha(); return _BOOLEAN; }

//Variavel do tipo double
{HalfHalf}              { trata_folha(); return _DOUBLE; }

//Variavel do tipo real
{MeiaGarrafa}           { trata_folha(); return _REAL; }

//Variavel do tipo String
{CharPointerMelhorado} 	{ trata_folha(); return _STRING; }

//Variavel do tipo inteiro
{CopoInteiro} 	        { trata_folha(); return _INTEGER; }

//Declaracao de variaveis
{Amigos} 		{ trata_folha(); return _VAR; }

//Comando if
{Sera} 		        { trata_folha(); return _IF; }

//Comando then
{Que} 		        { trata_folha(); return _THEN; }

//Comando else
{ELSE} 		        { trata_folha(); return _ELSE; }

//Comando for 
{MaisUma} 		{ trata_folha(); return _FOR; }

//Comando while
{BebaEnquanto} 	        { trata_folha(); return _WHILE; }

//Comando to 
{Para} 		        { trata_folha(); return _TO; }

//Comando do
{Beber} 		{ trata_folha(); return _DO; }

//Declaracao de funcao
{Rodada}                { trata_folha(); return _FUNCTION; }

//Constante string
{CTE_STRING} 	        { trata_aspas_simples(); return _CTE_STRING; }

//Constante inteiro
{CTE_INTEGER} 	        { trata_folha(); return _CTE_INTEGER; }

//Inicio do bloco
":)"                    { trata_folha(); return _BEGIN;}

//Fim do bloco
":("                    { trata_folha(); return _END;}

//Sinal de atribuicao
"<-"			{ trata_folha(); return _ATRIB; }

//Equivalente ao == (operador relacional)
"equals" 	        { trata_folha(); yylval.v = "=="; return '='; }

"&&"                    { trata_folha(); return _ATRIB; } // nao eh atributo eh condicao
"||"                    { trata_folha(); return _ATRIB; } // nao eh atributo eh condicao

//Equivalente ao % (mod)
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

