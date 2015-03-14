#include "dlxlib.h"
int gridSize = 9;
int ** makeGrid(char * str){
    int ** ret = malloc(sizeof(int*)*gridSize);
    for(int i = 0;i<gridSize;i++){
        ret[i] = calloc(sizeof(int),gridSize);
    }
    if(str != NULL){
        vector * s = split(str, "\n");
        for(int i = 0;i<gridSize;i++){
            for(int j = 0;j<gridSize;j++){
                char** a = (char**)(s->ar);
                ret[i][j] = a[i][j] - '0';
            }
        }
        for(int i = 0;i<s->size;i++){
            char ** c = s->ar;
            free(c[i]);
        }
        free(s->ar);
        free(s);
    } 
    return ret;
}

void printGrid(int ** g){
    for(int i = 0;i<gridSize;i++){
        for(int j = 0;j<gridSize;j++){
           printf("%d", g[i][j]);
        }
        printf("\n");
    }
}

void freeGrid(int ** g){
    for(int i = 0;i<gridSize;i++){
       free(g[i]);
    }
    free(g);
}

int isValid(int ** g, int x, int y){
    int * checker = calloc(gridSize, sizeof(int));
    for(int i = 0;i<gridSize;i++){
        if(g[y][i] > 0){
            int index = g[y][i]-1;
            if(checker[index] == 1){
                return 0;
            }
            checker[index] = 1;
        }
    }
    memset(checker, 0, gridSize*sizeof(int));
    for(int i = 0;i<gridSize;i++){
        if(g[i][x] > 0){
            int index = g[i][x]-1;
            if(checker[index] == 1){
                return 0;
            }
            checker[index] = 1;
        }
    }
    memset(checker, 0, gridSize*sizeof(int));
    int innerX = x/3;
    int innerY = y/3;
    for(int i = innerX*3;i<innerX*3+3;i++){
        for(int j = innerY*3;j<innerY*3+3;j++){
            if(g[i][j] > 0){
                int index = g[i][j]-1;
                if(checker[index] == 1){
                    return 0;
                }
                checker[index] = 1;
            }
        }
    }
    free(checker);
    return 1;
}

int ** copyGrid(int ** g){
    int ** ret = makeGrid(NULL);
    for(int i = 0;i<gridSize;i++){
        for(int j = 0;j<gridSize;j++){
            ret[i][j] = g[i][j];
        }
    }
    return ret;
}

int ** solveGrid(int ** g, int x, int y){
    //print("%d %d", x, y);
    if(g[y][x] == 0){
        int ** gg = copyGrid(g);
        for(int k = 1;k<10;k++){
            gg[y][x] = k;
            if(isValid(gg, x, y)){
               // print("%d", k);
                if(x == gridSize-1 && y == gridSize-1){
                    return gg;
                }else if(x == gridSize-1){
                    int ** r =  solveGrid(gg, 0, y+1);
                    if(r != NULL){
                        return r;
                    }
                }else{
                    int ** r = solveGrid(gg, x+1, y);
                    if(r != NULL){
                        return r;
                    }
                }
                
            }
        }
    }else{
        if(x == gridSize-1 && y == gridSize-1){
            return g;
        }else if(x == gridSize-1){
            return solveGrid(g, 0, y+1);
        }else{
            return solveGrid(g, x+1, y);
        }
    }
    // print("\n");
    // printGrid(g);
    return NULL;
}

int main(int argc, char **argv)
{
    if(argc < 2){
        error("not enuf args");
    }
    char * str = fileToString(argv[1]);
    //print("%s\n\n", str);
    int ** g = makeGrid(str);
    free(str);
    printGrid(g);

    for(int i = 0;i<gridSize;i++){
        for(int j = 0;j<gridSize;j++){
            if(!isValid(g, i, j)){
                print("bad %d %d", i, j);
                return 0;
            }
        }
    }

    g = solveGrid(g, 0, 0);
    if(g!=NULL){
        print("\n");
        printGrid(g);
    }else{
        print("bad");
    }
    //freeGrid(g);
    return 0; 
}