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

double logt(double f){
    return (log2(f));
}

double nlogRes(long long c, long long mid){
    return -(mid*logt(mid)-c);
}

double factRes(long long c, long long mid){
    return ((double)c/(double)fact(mid))-1;
}

int main(int argc, char **argv)
{
    if(argc <= 1){
        printf("ALGO\nTo run this program use ALGO X\neg. ./ALGO 24\n");
        return 0;
    }
    //double c = strtof(argv[1], NULL);
    //long long c = 24;
    char * e;
    long long c = strtoll(argv[1], &e, 0);
    //printf("%lld\n", c);
    // double low = 0;
    // double high = 32000;
    // double mid = 0;
    // double res = 0;
    
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

    long long high =  9223372036854775807;
    long long low = 0;
    long long mid = 0;
    long long ans = 0;
    double res = 0;
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
    //printf("n1 = %lld %lld %lld %f\n", ans, high, low, ans*logt(ans)-c);
    printf("n1 = %lld\n", ans);

    high =  20;
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

    //printf("n2 = %lld %lld %lld %f\n", ans, high, low, ((double)c/(double)fact(ans))-1);
    printf("n2 = %lld\n", ans);
    //printf("%d\n", mid);


    return 0;
}

