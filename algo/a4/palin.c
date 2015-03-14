#include "dlxlib.h"

int checkPosEq(char * s, int a, int b){
    int len = strlen(s);
    if(a >= 0 && b >= 0 && a < len && b < len){
        return s[a] == s[b];
    }
    return 0;
}

int main(int argc, char **argv)
{
    if(argc < 2){
        error("not enuf args");
    }
    char * seq = fileToString(argv[1]);
    int len = strlen(seq);
    int max = 0;
    for(int i = 0;i < len; i++){
        for(int j = 0;j < 2;j++){
            int a = i;
            int b = i;
            int len = 0;
            if(j==1){
                a++;
            }
            while(checkPosEq(seq, a, b)){
                len+=(a == b)?1:2;
                a++;
                b--;
            }
            if(len > max){
                print("%d %d", i, len);
                max = len;
            }
        }
    }
    print("%d", max);
    print("%s", seq);
    return 0; 
}