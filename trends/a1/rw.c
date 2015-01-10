#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "dlxlib.h"

pthread_t tid[1000];
pthread_mutex_t rLock;
pthread_mutex_t wLock;
char * fileName = "file.txt";

FILE* getFile(char*str){
    return fopen(fileName, str);
}

vector * readFileToVector(){
    char * str = fileToString(fileName);
    vector * v = split(str, " ");
    free(str);
    return v;
}

void* writer(void *arg)
{
    int wID = (int)*((int*)arg);
    //pthread_t id = pthread_self();
    pthread_mutex_lock(&wLock);{
            vector * v = readFileToVector();
            char**ar = (char**)v->ar;
            
            int myCount = atoi(ar[wID]);
            myCount++;
            
            char myCStr[15];
            sprintf(myCStr, "%d", myCount);

            //write increased count
            FILE *fptr = getFile("w+");{
                for(int i = 0;i<v->size;i++){
                    char*token = ar[i];
                    if(i==wID){
                        token=myCStr;
                    }
                    if(i == v->size-1){
                        fprintf(fptr, "%s", token);
                    }else{
                        fprintf(fptr, "%s ", token);
                    }
                }
            }fclose(fptr);
            
            //free vector
            for(int i = 0;i < v->size;i++){
                free(ar[i]);
            }
            free(v->ar);
            free(v);
                
    }pthread_mutex_unlock(&wLock);
       
    
    return NULL;
}


int main()
{
    //int readers = 5;
    int writers = 5;
    int wIDs[writers];
    FILE *fptr = getFile("w+");
    if(fptr == NULL)
    {
            printf("unable to open or create file\n");
        return 1;
    }
    for(int i = 0;i<writers;i++){
        if(i == writers-1){
            fprintf(fptr, "%d", 0);
        }else{
            fprintf(fptr, "%d ", 0);
        }
    }
    fclose(fptr);
    // char * str = fileToString(fileName);
    // print("%s", str);
    // free(str);
    int threadCount = 0;
    for(int i = 0;i<writers;i++)
    {
        wIDs[i] = i;
        int err = pthread_create(&(tid[threadCount++]), NULL, &writer, &wIDs[i]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread created successfully\n");
    }

    for(int i = 0;i<threadCount;i++)
    {
        pthread_join(tid[i], NULL);
    }

    sleep(3);
    return 0;
}