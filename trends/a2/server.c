#include "dlxlib.h"
#define MAX_CONN 1000

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

void* runServer(void *arg)
{
    int stopfd = *((int*)arg);
    pthread_t tid[MAX_CONN];
    int connSocket[MAX_CONN];

    int threadCount = 0;
    int sockfd = createTcpServerSocket(3000);
    if(sockfd == -1){
        error("bad server socket");
    }
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    
    print("server started");
    while(1){
        //weird it breaks if this stuff is outside the while????
        fd_set read_fd_set;
        FD_ZERO (&read_fd_set);
        FD_SET (sockfd, &read_fd_set);
        FD_SET (stopfd, &read_fd_set);
        int maxfd = stopfd > sockfd ? stopfd : sockfd;

        //print("selecting");
        if(select(maxfd+1, &read_fd_set, NULL, NULL, NULL) == -1){
            error("bad select");
        }
        if(dataAvailible(stopfd)){
            char buf[5];
            read(stopfd, buf, 5);
            //print("%s", buf);
            close(sockfd);
            pthread_exit(0);
            return NULL;
        }
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0){
            error("ERROR on accept");
        }

        //spawn thread
        connSocket[threadCount] = newsockfd;
        int err = pthread_create(&(tid[threadCount]), NULL, &connectionHandler, &connSocket[threadCount]);
        threadCount++;
        if (err != 0){
             error("can't create thread");
        }
        //NEED TO JOIN
        pthread_join(tid[threadCount-1], NULL);
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