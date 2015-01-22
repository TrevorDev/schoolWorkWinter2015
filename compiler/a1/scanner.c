#include "dlxlib.h"


int main(int argc, char **argv)
{
    for(int c = getchar();c!=EOF;c = getchar()){
        print("%c", c);
    }
    
    return 0;
}