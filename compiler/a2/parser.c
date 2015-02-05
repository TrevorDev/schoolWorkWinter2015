//clear; flex scanner.l; gcc lex.yy.c -o scanner -lfl;
#include "dlxlib.h"
#include "scanner.h"
#include "tokens.h"

void stmts();
void stmtend();
void stmt();
void assign();
void consume(int x);
void addexpr();
void addexpra();
void item();
void itemend();
void items();
void var();
void listexpr();
int curTok = 0;

int main(int argc, char **argv)
{
	curTok = yylex();
	stmts();
	// int x = yylex();
	// while(x != TOKEN_FILE_END){
	// 	print("%d", x);
	// 	if(x == TOKEN_PLUS){
	// 		print("plus hit");
	// 	}
	// 	x = yylex();
	// }

}

void stmts(){
	switch(curTok){
		case TOKEN_SET:
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			stmt();
			stmtend();
			break;
		default:
			error("PARSE ERROR");
	}
}

void stmtend(){
	switch(curTok){
		case TOKEN_SET:
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			stmt();
			stmtend();
			break;
		default:
			error("PARSE ERROR");
	}
}

void stmt(){
	switch(curTok){
		case TOKEN_SET:
			assign();
			consume(TOKEN_SEMICOLIN);
			break;
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			addexpr();
			break;
		default:
			error("PARSE ERROR");
	}
}

void assign(){
	switch(curTok){
		case TOKEN_SET:
			consume(TOKEN_SET);
			var();
			addexpr();
			break;
		default:
			error("PARSE ERROR");
	}
}

void addexpr(){
	switch(curTok){
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			listexpr();
			addexpra();
			break;
		default:
			error("PARSE ERROR");
	}
}

void addexpra(){
	switch(curTok){
		case TOKEN_SEMICOLIN:
			consume(TOKEN_SEMICOLIN);
			break;
		case TOKEN_PLUS:
			consume(TOKEN_PLUS);
			listexpr();
			addexpra();
			break;
		default:
			error("PARSE ERROR");
	}
}

void var(){
	switch(curTok){
		case TOKEN_CHAR:
			consume(TOKEN_CHAR);
			break;
		default:
			error("PARSE ERROR");
	}
}

void listexpr(){
	switch(curTok){
		case TOKEN_CHAR:
			var();
			break;
		case TOKEN_CDR:
			consume(TOKEN_CDR);
			listexpr();
			break;
		case TOKEN_LPAREN:
			consume(TOKEN_LPAREN);
			items();
			break;
		default:
			error("PARSE ERROR");
	}
}

void items(){
	switch(curTok){
		case TOKEN_CHAR:
		case TOKEN_STR:
		case TOKEN_INT:
		case TOKEN_REAL:
		case TOKEN_CAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			item();
			itemend();
			break;
		default:
			error("PARSE ERROR");
	}
}

void itemend(){
	switch(curTok){
		case TOKEN_CHAR:
		case TOKEN_STR:
		case TOKEN_INT:
		case TOKEN_REAL:
		case TOKEN_CAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			item();
			itemend();
			break;
		case TOKEN_RPAREN:
		consume(TOKEN_RPAREN);
		break;
		default:
			error("PARSE ERROR");
	}
}

void item(){
	switch(curTok){
		case TOKEN_CHAR:
			addexpr();
			break;
		case TOKEN_STR:
			consume(TOKEN_STR);
			break;
		case TOKEN_INT:
			consume(TOKEN_INT);
			break;
		case TOKEN_REAL:
			consume(TOKEN_REAL);
			break;
		case TOKEN_CAR:
			consume(TOKEN_CAR);
			listexpr();
			break;
		case TOKEN_CDR:
			addexpr(); // ERROR?
			break;
		case TOKEN_LPAREN:
			addexpr();
			break;
		default:
			error("PARSE ERROR");
	}
}

void consume(int x){
	if(curTok == x){
		curTok = yylex();
		if(curTok == TOKEN_FILE_END){
			error("DONE");
		}
	}else{
		error("PARSE ERROR");
	}
}