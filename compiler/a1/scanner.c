#include "dlxlib.h"

#define STRING "STR"
#define CHAR "CHAR"
#define CAR "CAR"
#define CDR "CDR"
#define INT "INT"
#define REAL "REAL"
#define HEX "HEX"
#define OCT "OCT"
#define SPACE " "
#define LEFT_PAREN "LPAREN"
#define RIGHT_PAREN "RPAREN"



#define INTDIG "0123456789"
#define INTDIG_NO_ZERO "123456789"
#define HEXDIG "0123456789abcdefABCDEF"
#define OCTDIG "01234567"

typedef struct {
   char * chars;
   int toNode;
} matcher;

typedef struct {
   char * token;
   matcher * m;
   int matcherC;
   int curMatcher;
   int elseNode;
} node;

void setNode(node * n, char * token, int matcherC){
	n->token = token;
	n->m = malloc (sizeof (matcher)*matcherC);
	n->matcherC = matcherC;
	n->curMatcher=0;
	n->elseNode=19;
}

void addMatcherToNode(node * n, char * chars, int toNode){
	n->m[n->curMatcher].chars = dupStr(chars);
	n->m[n->curMatcher].toNode = toNode;
	n->curMatcher++;
}

void freeNode(node * n){
	for(int i =0; i < n->curMatcher; i++){
		free(n->m[i].chars);
	}
	free(n->m);
}

int main(int argc, char **argv)
{
	int nodeCount = 20;
	node nodes[nodeCount];

	for(int i = 0;i<nodeCount;i++){
		switch( i ){
					//malloc more than needed
			    case 0:
		        setNode(&nodes[i], "", 10);
						addMatcherToNode(&nodes[i], "0", 2);
						addMatcherToNode(&nodes[i], "+-", 3);
						addMatcherToNode(&nodes[i], INTDIG_NO_ZERO, 4);
						addMatcherToNode(&nodes[i], "(", 5);
						addMatcherToNode(&nodes[i], ")", 6);
						addMatcherToNode(&nodes[i], "c", 7);
						nodes[i].elseNode = 1;
		        break;
		      case 1:
		        setNode(&nodes[i], CHAR, 10);
		        break;
		      case 2:
		        setNode(&nodes[i], INT, 10);
						addMatcherToNode(&nodes[i], "x", 8);
						addMatcherToNode(&nodes[i], ".", 9);
						addMatcherToNode(&nodes[i], OCTDIG, 10);
		        break;
		      case 3:
		        setNode(&nodes[i], CHAR, 10);
						addMatcherToNode(&nodes[i], INTDIG_NO_ZERO, 4);
		        break;
		      case 4:
		        setNode(&nodes[i], INT, 10);
						addMatcherToNode(&nodes[i], INTDIG, 4);
						addMatcherToNode(&nodes[i], ".", 9);
		        break;
		      case 5:
		        setNode(&nodes[i], LEFT_PAREN, 10);
		        break;
		      case 6:
		        setNode(&nodes[i], RIGHT_PAREN, 10);
		        break;
		      case 7:
		        setNode(&nodes[i], CHAR, 10);
						addMatcherToNode(&nodes[i], "a", 11);
						addMatcherToNode(&nodes[i], "d", 12);
		        break;
		      case 8:
		        setNode(&nodes[i], STRING, 10);
						addMatcherToNode(&nodes[i], HEXDIG, 15);
		        break;
		      case 9:
		        setNode(&nodes[i], STRING, 10);
						addMatcherToNode(&nodes[i], INTDIG, 16);
		        break;
		      case 10:
		        setNode(&nodes[i], OCT, 10);
						addMatcherToNode(&nodes[i], OCTDIG, 10);
		        break;
		      case 11:
		        setNode(&nodes[i], CHAR, 10);
						addMatcherToNode(&nodes[i], "r", 13);
		        break;
		      case 12:
		        setNode(&nodes[i], CHAR, 10);
						addMatcherToNode(&nodes[i], "r", 14);
		        break;
		      case 13:
		        setNode(&nodes[i], CAR, 10);
		        break;
		      case 14:
		        setNode(&nodes[i], CDR, 10);
		        break;
		      case 15:
		        setNode(&nodes[i], HEX, 10);
						addMatcherToNode(&nodes[i], HEXDIG, 15);
		        break;
		      case 16:
		        setNode(&nodes[i], REAL, 10);
						addMatcherToNode(&nodes[i], INTDIG, 16);
						addMatcherToNode(&nodes[i], "eE", 17);
		        break;
		      case 17:
		        setNode(&nodes[i], STRING, 10);
						addMatcherToNode(&nodes[i], INTDIG, 18);
		        break;
		      case 18:
		        setNode(&nodes[i], "", 10);
						addMatcherToNode(&nodes[i], INTDIG, 18);
		        break;
		      case 19:
		        setNode(&nodes[i], STRING, 10);
						addMatcherToNode(&nodes[i], "", 1);
		        break;
			    default :
			    	print("ERROR DEFAULT HAS BEEN HIT");
      			setNode(&nodes[i], STRING, 10);
						addMatcherToNode(&nodes[i], "", 1);
      			break;
		}
	}

	int curNode = 0;
	for(int c = getchar();c!=EOF;c = getchar()){
      //print("%c", c);
      if(c == ' ' || c == '\n'){
      	printf("%s ", nodes[curNode].token);
      	curNode = 0;
      	continue;
      }
      int found = 0;
      for(int i=0;i<nodes[curNode].curMatcher;i++){
      	if (strchr(nodes[curNode].m[i].chars, c)){
        	//printf("%c is in \"%s\"\n", c, nodes[curNode].m[i].chars);
        	curNode = nodes[curNode].m[i].toNode;
        	found = 1;
        	break;
        }
      }
      if(!found){
      	curNode = nodes[curNode].elseNode;
      }
      
  }
  print("");

	//print("%s", nodes[0].token);


	for(int i = 0;i<nodeCount;i++){
		freeNode(&nodes[i]);
	}
	
	
    
    return 0;
}

