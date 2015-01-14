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
int rIt = 5;
int wIt = 5;
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
    //usleep((rand() % 1000)+1);
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
                pthread_mutex_lock(&printLock);
                print("Writer: %d has written", wID);
                pthread_mutex_unlock(&printLock);

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
    randSleep();
    int rID = (int)*((int*)arg);
    //pthread_t id = pthread_self();
    //sleep(1);
    sem_wait(&readerSem);{
        char * str = fileToString(fileName);
        pthread_mutex_lock(&printLock);
        print("Reader: %d read: %s", rID, str);
        pthread_mutex_unlock(&printLock);
        free(str);    
    }sem_post(&readerSem);
       
    
    return NULL;
}


int main()
{
    readers = 5;
    int writers = 5;
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
    print("%s", str);
    free(str);
    return 0;
}