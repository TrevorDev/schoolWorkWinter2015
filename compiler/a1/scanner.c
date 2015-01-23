#include "dlxlib.h"

#define STRING "STRING"
#define SPACE " "
#define LEFT_PARENTH "("

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
	n->elseNode=0;
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
	int nodeCount = 3;
	node nodes[nodeCount];

	for(int i = 0;i<nodeCount;i++){
		switch( i ){
			    case 0:
			    	//malloc more than needed
			        setNode(&nodes[i], STRING, 10);
					addMatcherToNode(&nodes[i], "3", 1);
			        break;
			    default :
        			setNode(&nodes[i], STRING, 10);
					addMatcherToNode(&nodes[i], "", 1);
        			break;
		}
	}

	int curNode = 0;
	int done = 0;
	for(int c = getchar();c!=EOF;c = getchar()){
        //print("%c", c);
        if(c == ' '){
        	print("%s\n", nodes[curNode].token);
        	curNode = 0;
        	continue;
        }
        int found = 0;
        for(int i=0;i<nodes[curNode].curMatcher;i++){
        	if (strchr(nodes[curNode].m[i].chars, c)){
	        	printf("%c is in \"%s\"\n", c, nodes[curNode].m[i].chars);
	        	curNode = nodes[curNode].m[i].toNode;
	        	break;
	        }
        }
        
    }


	print("%s", nodes[0].token);


	for(int i = 0;i<nodeCount;i++){
		freeNode(&nodes[i]);
	}
	
	
    
    return 0;
}