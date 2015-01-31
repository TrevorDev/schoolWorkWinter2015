#include "dlxlib.h"
#define MAX_CONN 1000

typedef struct fileGroup {
    linkedMT * queue;
    int reading;
    int writing;
} fileGroup;

typedef struct queuedReq {
    int id;
    char type;
    int sock;
} queuedReq;

queuedReq * createQueuedReq(int id, char type, int sock){
    queuedReq * ret = (queuedReq *)calloc(1, sizeof(queuedReq));
    ret->id = id;
    ret->type = type;
    ret->sock = sock;
    return ret;
}

fileGroup * createfileGroup(){
    fileGroup * ret = (fileGroup *)calloc(1, sizeof(fileGroup));
    ret->queue = createLinkedMT();
    ret->reading = 0;
    ret->writing = 0;
    return ret;
}

void* connectionHandler(void *arg)
{
    int newsockfd = ((int*)arg)[0];
    char buffer[256];
    bzero(buffer,256);
    int n = read(newsockfd,buffer,255);
    if (n < 0){
        error("ERROR reading from socket");
    }

    print("Here is the message: %s",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0){
        error("ERROR writing to socket");
    }

    close(newsockfd);
    pthread_exit(0);
    return NULL;
}

int sendAck(int cliSock){
    int n = write(cliSock,"I got your message",18);
    if (n < 0){
        error("ERROR writing to socket");
    }
    return n;
}

void* runServer(void *arg)
{
    int stopfd = *((int*)arg);
    //pthread_t tid[MAX_CONN];
    //int connSocket[MAX_CONN];

    //int threadCount = 0;
    int sockfd = createTcpServerSocket(3000);
    if(sockfd == -1){
        error("bad server socket");
    }
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    linkedMT * clients = createLinkedMT();
    linkedMT * groups = createLinkedMT();
    print("server started");
    while(1){
        //weird it breaks if this stuff is outside the while????
        fd_set read_fd_set;
        FD_ZERO (&read_fd_set);
        FD_SET (sockfd, &read_fd_set);
        FD_SET (stopfd, &read_fd_set);
        int maxfd = stopfd > sockfd ? stopfd : sockfd;

        //NOT THREAD SAFE BUT IM NOT USING MULTIPLE THREADS
        {
            linkedNode * it = clients->head;
            while(it!=NULL){
                int cliSock = *((int*)(it->data));
                if(cliSock>maxfd){
                    maxfd = cliSock;
                }
                FD_SET (cliSock, &read_fd_set);
                it = it->next;
            }
        }
        

        //print("selecting");
        if(select(maxfd+1, &read_fd_set, NULL, NULL, NULL) == -1){
            error("bad select");
        }
        if(dataAvailible(stopfd)){
            char buf[5];
            read(stopfd, buf, 5);
            //print("%s", buf);
            close(sockfd);
            //TODO FREE CLIENT ELEMENTS
            free(clients);

            {
                linkedNode * it = groups->head;
                while(it!=NULL){
                    linkedNode * toFree = it;
                    it = it->next;
                    free(toFree->key);
                    fileGroup * fg = (fileGroup *)toFree->data;

                     {
                        linkedNode * it = fg->queue->head;
                        while(it!=NULL){
                            linkedNode * toFree = it;
                            it = it->next;
                            free(toFree->key);
                            free(toFree->data);
                            free(toFree);
                        }
                    }

                    free(fg->queue);
                    free(toFree->data);
                    free(toFree);
                }
            }

            free(groups);
            pthread_exit(0);
            return NULL;
        }

        if(dataAvailible(sockfd)){
            int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0){
                error("ERROR on accept");
            }
            int * data = malloc(sizeof(int));
            (*data) = newsockfd;
            char* key = intToStr(newsockfd);
            pushLinkedMT(clients, data, key);
            free(key);
            continue;
        }

        {
            linkedNode * it = clients->head;
            while(it!=NULL){
                int cliSock = *((int*)(it->data));
                if(dataAvailible(cliSock)){
                    char buffer[256];
                    bzero(buffer,256);
                    int n = read(cliSock,buffer,255);
                    if (n < 0){
                        error("ERROR reading from socket");
                    }
                    if(n==0){
                        char* key = intToStr(cliSock);
                        int* data = (int*)removeLinkedMT(clients, key);
                        free(data);
                        free(key);
                        close(cliSock);
                        break;
                    }
                    print("Here is the message: %s",buffer);
                    vector * v = split(buffer, " ");
                    char ** args = (char **)(v->ar);
                    int * tmp = strToInt(args[0]);
                    int id = *tmp;
                    free(tmp);
                    char type = args[1][0];
                    char * filename = args[2];
                    int req = (args[3][2] == 'q' ? 1 : 0);

                    fileGroup * g = (fileGroup *)findLinkedMT(groups, filename);
                    if(req){                        
                        if(g == NULL){
                            g = createfileGroup();
                            pushLinkedMT(groups, g, filename);
                        }
                        char* key = intToStr(cliSock);
                        pushLinkedMT(g->queue, createQueuedReq(id, type, cliSock), key);
                        free(key);
                        //print("request");
                    }else{
                        //RELEASE TICKET ON RELEASE
                        if(type == 'W'){
                            //print("release");
                            g->writing--;
                        }else{
                            g->reading--;
                        }
                        sendAck(cliSock);
                    }

                    //check current group queue for readers or writers who can act
                    queuedReq * nextInQueue = tailLinkedMT(g->queue);
                    if(nextInQueue != NULL){
                        if(nextInQueue->type == 'W'){
                            if(g->reading == 0 && g->writing == 0){
                                popLinkedMT(g->queue);
                                g->writing++;
                                sendAck(nextInQueue->sock);
                                free(nextInQueue);
                            }
                        }else{
                            if(g->writing == 0){
                                popLinkedMT(g->queue);
                                g->reading++;
                                sendAck(nextInQueue->sock);
                                free(nextInQueue);
                            }
                        }
                    }

                    for(int i=0;i<v->size;i++){
                        free(args[i]);
                    }
                    free(v->ar);
                    free(v);
                    break;
                }
                it = it->next;
            }

        }

        // //spawn thread
        // connSocket[threadCount] = newsockfd;
        // int err = pthread_create(&(tid[threadCount]), NULL, &connectionHandler, &connSocket[threadCount]);
        // threadCount++;
        // if (err != 0){
        //      error("can't create thread");
        // }
        // //NEED TO JOIN
        // pthread_join(tid[threadCount-1], NULL);
    }
    
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t serverThread;
    int fd[2];
    pipe(fd);
    int err = pthread_create(&serverThread, NULL, &runServer, &fd[0]);
    if (err != 0){
         printf("\ncan't create thread :[%s]", strerror(err));
    }
    
    int c = 0;
    while(c!='q'){
        c=getchar();
    }
    if (write(fd[1], "stop\0", 5) != 5) {
        error("unable to write");
    }
    pthread_join(serverThread, NULL);
    return 0; 
}