#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "dlxlib.h"

pthread_t tid[1000];
int readers;
int writers;
int rIt;
int wIt;
sem_t readerSem;
pthread_mutex_t wLock;
pthread_mutex_t printLock;

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

void randSleep(){
    nanosleep((struct timespec[]){{0, rand()%900000000}}, NULL);
}

void writerWait(){
    pthread_mutex_lock(&wLock);
    for(int i = 0;i<readers;i++){
        sem_wait(&readerSem);
    }
}

void writerPost(){
    for(int i = 0;i<readers;i++){
        sem_post(&readerSem);
    }
    pthread_mutex_unlock(&wLock);
}

void* writer(void *arg)
{
    for(int i = 0;i<wIt;i++){
        randSleep();
        int wID = (int)*((int*)arg);
        //pthread_t id = pthread_self();
        writerWait();{
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
                // pthread_mutex_lock(&printLock);
                // print("Writer: %d has written", wID+1);
                // pthread_mutex_unlock(&printLock);

                //free vector
                for(int i = 0;i < v->size;i++){
                    free(ar[i]);
                }
                free(v->ar);
                free(v);        
        }writerPost();
    }
    
       
    
    return NULL;
}

void* reader(void *arg)
{
    for(int i = 0;i<rIt;i++){
        randSleep();
        int rID = (int)*((int*)arg);
        //pthread_t id = pthread_self();
        //sleep(1);
        sem_wait(&readerSem);{
            char * str = fileToString(fileName);
            pthread_mutex_lock(&printLock);
            print("Reader: %d read: %s", rID+1, str);
            fflush(stdout);
            pthread_mutex_unlock(&printLock);
            free(str);    
        }sem_post(&readerSem);
    }
    
    return NULL;
}

//./rw 20 5 3 >> out 2>&1;./rw 20 10 5 >> out 2>&1;./rw 20 4 7 >> out 2>&1
int main(int argc, char **argv)
{
    if(argc <= 1){
        printf("To run this program use arguments eg. ./rw numIter numReaders numWriters\n");
        return 0;
    }

    rIt = (int)atof(argv[1]);
    // rIt = 5;
    // wIt = 20;
    //rIt = 20;
    wIt = rIt;
    readers = (int)atof(argv[2]);
    writers = (int)atof(argv[3]);

    printf("Starting %d iterations with %d readers and %d writers\n", rIt, readers, writers);

    sem_init(&readerSem, 0, readers);
    int wIDs[writers];
    int rIDs[readers];
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
    
    int threadCount = 0;
    for(int i = 0;i<writers;i++)
    {
        wIDs[i] = i;
        int err = pthread_create(&(tid[threadCount++]), NULL, &writer, &wIDs[i]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        //else
            //printf("\n Thread created successfully\n");
    }

    for(int i = 0;i<readers;i++)
    {
        rIDs[i] = i;
        int err = pthread_create(&(tid[threadCount++]), NULL, &reader, &rIDs[i]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        //else
            //printf("\n Thread created successfully\n");
    }

    for(int i = 0;i<threadCount;i++)
    {
        pthread_join(tid[i], NULL);
    }

    //sleep(3);

    char * str = fileToString(fileName);
    print("All have finished resulting file:\n%s\n", str);
    free(str);
    return 0;
}