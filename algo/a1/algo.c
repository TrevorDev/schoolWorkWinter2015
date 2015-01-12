#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int fact(int x){
    if(x==0||x==1){
        return x;
    }else{
        return x*fact(x-1);
    }
}

int main(int argc, char **argv)
{
    //float c = strtof(argv[1], NULL);
    int c = 5;
    // float low = 0;
    // float high = 32000;
    // float mid = 0;
    // float res = 0;
    
    // while(high - low > 0.0001){
    //     mid = (low+high)/2;
    //     res = mid*logf(mid);
    //     if(res < c){
    //         low = mid;
    //     }else{
    //         high = mid;
    //     }
    // }
    // printf("%f\n%f\n%f\n", mid, mid*logf(mid), c);

    int high =  10;
    int low = 0;
    int mid = 0;
    float res = 0;
    while(high - low > 1){
        mid = (high+low)/2;
        res = (float)c/(float)fact(mid);
        printf("%d %d %d %f\n", mid, c, fact(mid), res);
        if(res > 1){
            low = mid;
        }else{
            high = mid;
        }
    }
    printf("%f\n", res);
    printf("%d\n", mid);


    return 0;
}

