#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    clock_t begin, end;
    double totalTime;
    begin = clock();

    //Init counting array to 0
    int x[100];
    for(int i = 0;i<100;i++){
        x[i]=0;
    }
    //COUNTING SORT O(n)
    char * line = malloc(10*sizeof(char));
    int first = 1;
    int numberOfNums = 0;
    while(fgets(line, 10, stdin) != NULL){
        int num = atoi(line);
        if(first){
            numberOfNums = num;
        }else{
            //printf("%d\n", num, line);
            x[num-1]++;
        }
        first = 0;
    }
    free(line);

    //Bottom half of sorted array is small sum O(n)
    int half = numberOfNums/2;
    double smallTotal = 0;
    double largeTotal = 0;
    for(int i = 0;i<100;i++){
        if(half > 0){
            if(half < x[i]){
                smallTotal += (i+1)*half;
                half = 0;
            }else{
                smallTotal += (i+1)*x[i];
                half = half - x[i];
            }
        }else{
            //end of small sublist
            largeTotal += (i+1)*x[i];
        }
    }

    end = clock();
    totalTime = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("%f  %f\n",largeTotal, smallTotal);
    printf("Sum Dif = %f\nRun Time = %f seconds\n", largeTotal - smallTotal, totalTime);
    return 0;
}

