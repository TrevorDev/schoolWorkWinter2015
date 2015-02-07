//clear; flex scanner.l; gcc lex.yy.c -o scanner -lfl;
#include "dlxlib.h"
#include "scanner.h"
#include "tokens.h"

void stmts();
void stmtend();
void stmtendend();
void stmt();
void assign();
void consume(int x);
void addexpr();
void addexpra();
void iaddexpr();
void iaddexpra();
void item();
void itemend();
void items();
void var();
void listexpr();
void printToken();
int curTok = 0;

int main(int argc, char **argv)
{
	curTok = yylex();
	stmts();
	print("Input matches grammer!");
	// int x = yylex();
	// while(x != TOKEN_FILE_END){
	// 	printToken(x);
	// 	x = yylex();
	// }

}

void stmts(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_SET:
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			stmt();
			stmtend();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void stmtend(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_SET:
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			stmt();
			stmtendend();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void stmtendend(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_FILE_END:
			break;
		case TOKEN_SET:
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			stmtend();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void stmt(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_SET:
			assign();
			break;
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			addexpr();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void assign(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_SET:
			consume(TOKEN_SET);
			var();
			addexpr();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void addexpr(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			listexpr();
			addexpra();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void addexpra(){
	//printf("ENTERING %s\n", __func__);
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
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void iaddexpr(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_CHAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
			listexpr();
			iaddexpra();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void iaddexpra(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_CHAR:
		case TOKEN_STR:
		case TOKEN_INT:
		case TOKEN_REAL:
		case TOKEN_CAR:
		case TOKEN_CDR:
		case TOKEN_LPAREN:
		case TOKEN_RPAREN:
			itemend();
			break;
		case TOKEN_PLUS:
			consume(TOKEN_PLUS);
			listexpr();
			iaddexpra();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void var(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_CHAR:
			consume(TOKEN_CHAR);
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void listexpr(){
	//printf("ENTERING %s\n", __func__);
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
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void items(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_RPAREN:
			consume(TOKEN_RPAREN);
			break;
		case TOKEN_LPAREN:
		case TOKEN_CDR:
		case TOKEN_CHAR:
			iaddexpr();
			break;
		case TOKEN_STR:
		case TOKEN_INT:
		case TOKEN_REAL:
		case TOKEN_CAR:
			item();
			itemend();
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void itemend(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
		case TOKEN_CDR:
		case TOKEN_CHAR:
		case TOKEN_LPAREN:
			iaddexpr();
			break;
		case TOKEN_STR:
		case TOKEN_INT:
		case TOKEN_REAL:
		case TOKEN_CAR:
			item();
			itemend();
			break;
		case TOKEN_RPAREN:
			consume(TOKEN_RPAREN);
			break;
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void item(){
	//printf("ENTERING %s\n", __func__);
	switch(curTok){
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
		default:
			printf("Unexpected token ");
			printToken(curTok);
			printf(" in production %s on line %d\n", __func__, yylineno);
			exit(0);
	}
}

void consume(int x){
	//printToken(x);
	if(curTok == x){
		curTok = yylex();
		// if(curTok == TOKEN_FILE_END){
		// 	error("DONE");
		// }
	}else{
		printf("Expected ");
		printToken(x);
		printf("but found ");
		printToken(curTok);
		printf(" on line %d\n", yylineno);
		error(0);
	}
}

void printToken(int t){
	switch(t){
		case TOKEN_SET:
	    printf("TOKEN_SET");
	    break;
		case TOKEN_CHAR:
	    printf("TOKEN_CHAR");
	    break;
		case TOKEN_INT:
	    printf("TOKEN_INT");
	    break;
		case TOKEN_REAL:
	    printf("TOKEN_REAL");
	    break;
		case TOKEN_STR:
	    printf("TOKEN_STR");
	    break;
		case TOKEN_CAR:
	    printf("TOKEN_CAR");
	    break;
		case TOKEN_CDR:
	    printf("TOKEN_CDR");
	    break;
		case TOKEN_LPAREN:
	    printf("TOKEN_LPAREN");
	    break;
		case TOKEN_RPAREN:
	    printf("TOKEN_RPAREN");
	    break;
		case TOKEN_PLUS:
	    printf("TOKEN_PLUS");
	    break;
		case TOKEN_SEMICOLIN:
	    printf("TOKEN_SEMICOLIN");
	    break;
		case TOKEN_FILE_END:
	    printf("TOKEN_FILE_END");
	    break;
		default:
			error("unknownToken");
	}
	
}
