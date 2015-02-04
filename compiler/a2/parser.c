//clear; flex scanner.l; gcc lex.yy.c -o scanner -lfl;
#include "dlxlib.h"
#include "scanner.h"
#include "tokens.h"

typedef struct grammer {
	int id;
	linkedMT * orList;
} grammer;

typedef struct grammerList {
	linkedMT * orList;
} grammerList;

int main(int argc, char **argv)
{
	int x = yylex();
	while(x != TOKEN_FILE_END){
		print("%d", x);
		if(x == TOKEN_PLUS){
			print("plus hit");
		}
		x = yylex();
	}
}