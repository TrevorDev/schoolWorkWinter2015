#include <stdio.h>
#include <math.h>
#include <stdlib.h>

long double fact(long double x){
    if(x==0 || x==1){
        return x;
    }else{
        return x*fact(x-1);
    }
}

long double logt(long double f){
    return (log2(f));
}

long double nlogRes(long double c, long double mid){
    return -(mid*logt(mid)-c);
}

long double factRes(long double c, long double mid){
    return ((long double)c/(long double)fact(mid))-1;
}

int main(int argc, char **argv)
{
    if(argc <= 1){
        printf("ALGO\nTo run this program use ALGO X\neg. ./ALGO 24\n");
        return 0;
    }
    long double c = atof(argv[1]);
    
    //PART 1
    long double high =  9223372036854775807;
    long double low = 0;
    long double mid = 0;
    long double ans = 0;
    long double res = 0;
    while((high - low) > 1){
        mid = floorl((high+low)/2);
        res = nlogRes(c, mid);
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
    printf("n1 = %LF\n", ans);


    //PART 2
    high =  50;
    low = 0;
    mid = 0;
    ans = 0;
    res = 0;
    while((high - low) > 1){
        mid = floorl((high+low)/2);
        res = factRes(c, mid);
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
    printf("n2 = %LF\n", ans);
    return 0;
}

