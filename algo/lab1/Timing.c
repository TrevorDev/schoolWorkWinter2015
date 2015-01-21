//
//  main.c
//  C_Program
//
//  Created by Charlie on 1/5/14.
//  Copyright (c) 2014 Charlie Obimbo. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <math.h>
#define PHI 1.618033988749895
double  elapsed;


int fib(int n) {
   if (n<=1)
      return n;
   else
      return ( fib(n-1) + fib(n-2) );
}

long long int fib1(int n) {
    int fnow = 0, fnext = 1, tempf;
    while(--n>0){
        tempf = fnow + fnext;
        fnow = fnext;
        fnext = tempf;
        }
        return fnext;
}



long long unsigned fib2(unsigned n) {
    return floor( (pow(PHI, n) - pow(1 - PHI, n))/sqrt(5) );
}



int main()   {
    clock_t start, finish;
     start = clock();

    int sum = 0,i;
    for (i = 0; i < 100000; i++) {
        sum+=i;
    }
    printf("sum = %d \n\n", sum);

     finish = clock();
     elapsed = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("time = %f8.5 \n\n", elapsed);

}
