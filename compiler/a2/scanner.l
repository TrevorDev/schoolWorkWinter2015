%{
         #include "tokens.h"
         int num_lines = 0, num_chars = 0;
%}
DIGIT    [0-9]
HEX      [0-9a-fA-F]
OCT      [0-7]
CHAR     [^ \t\n]
%%
[ \t\n]+																																				/* ignore this token */
set          																																		return TOKEN_SET;
0|([-+]?[1-9]{DIGIT}*)          																								return TOKEN_INT;
{DIGIT}+\.{DIGIT}+[eE](0|([-+]?[1-9]{DIGIT}*))?        								  				return TOKEN_REAL;
car       								   																										return TOKEN_CAR;
cdr       									   																									return TOKEN_CDR;
\(         										 																									return TOKEN_LPAREN;
\)        												  																						return TOKEN_RPAREN;
\+          																																		return TOKEN_PLUS;
;          																																			return TOKEN_SEMICOLIN;
{CHAR}{CHAR}+       									   																				return TOKEN_STR;
{CHAR}         											 																						return TOKEN_CHAR;
<<EOF>>																																					return TOKEN_FILE_END;
%%