#include <stdio.h>
#include <math.h>
#include <stdlib.h>

long long fact(long long x){
    if(x==0||x==1){
        return x;
    }else{
        return x*fact(x-1);
    }
}

float logt(float f){
    return (logf(f)/logf(2));
}

float nlogRes(long long c, long long mid){
    return -(mid*logt(mid)-c);
}

float factRes(long long c, long long mid){
    return ((float)c/(float)fact(mid))-1;
}

int main(int argc, char **argv)
{
    if(argc <= 1){
        printf("ALGO\nTo run this program use ALGO X\neg. ./ALGO 24\n");
        return 0;
    }
    //float c = strtof(argv[1], NULL);
    //long long c = 24;
    long long c = atoi(argv[1]);
    // float low = 0;
    // float high = 32000;
    // float mid = 0;
    // float res = 0;
    
    // while(high - low > 0.0001){
    //     mid = (low+high)/2;
    //     res = mid*logt(mid);
    //     if(res < c){
    //         low = mid;
    //     }else{
    //         high = mid;
    //     }
    // }
    // printf("%f\n%f\n%f\n", mid, mid*logt(mid), c);

    long long high =  2147483647;
    long long low = 0;
    long long mid = 0;
    long long ans = 0;
    float res = 0;
    while((high - low) > 1){
        mid = (high+low)/2;
        res = nlogRes(c, mid);
        //printf("%d %d %d %f\n", high, low, fact(mid), res);
        if(res > 0){
            low = mid;
        }else{
            high = mid;
        }
    }
    if(abs(nlogRes(c, low)) > abs(nlogRes(c, high))){
        ans = high;
    }else{
        ans = low;
    }
    printf("n1 = %lld %lld %lld %f\n", ans, high, low, ans*logt(ans)-c);

    high =  10;
    low = 0;
    mid = 0;
    ans = 0;
    res = 0;
    while((high - low) > 1){
        mid = (high+low)/2;
        res = factRes(c, mid);
        //printf("%lld %lld %lld %f\n", high, low, fact(mid), res);
        if(res > 0){
            low = mid;
        }else{
            high = mid;
        }
    }
    if(abs(factRes(c, low)) > abs(factRes(c, high))){
        ans = high;
    }else{
        ans = low;
    }

    printf("n2 = %lld %lld %lld %f\n", ans, high, low, ((float)c/(float)fact(ans))-1);
    //printf("%d\n", mid);


    return 0;
}

