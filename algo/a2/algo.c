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
    int numbersCounted = 0;
    double smallTotal = 0;
    double largeTotal = 0;
    for(int i = 0;i<100;i++){
        if(numbersCounted < half){
            if(numbersCounted + x[i] > half){
                int sC = half - numbersCounted;
                int lC = numbersCounted + x[i] -half;
                smallTotal += (i+1)*sC;
                largeTotal += (i+1)*lC;
                numbersCounted += smallTotal;
            }else{
                smallTotal += (i+1)*x[i];
                numbersCounted += x[i];
            }
        }else{
            //end of small sublist
            largeTotal += (i+1)*x[i];
        }
    }

    end = clock();
    totalTime = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("%f  %f %d\n",largeTotal, smallTotal, half);
    printf("Sum Dif = %f\nRun Time = %f seconds\n", largeTotal - smallTotal, totalTime);
    printf("Freq 15 = %d\n", x[14]);
    printf("Freq 27 = %d\n", x[26]);
    printf("Freq 54 = %d\n", x[53]);
    printf("Freq 89 = %d\n", x[88]);
    return 0;
}

