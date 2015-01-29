#include "dlxlib.h"

void error(const char *msg)
{
    print("%s", msg);
    exit(1);
}

int createTcpServerSocket(int port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
        
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }

    listen(sockfd,5);
    return sockfd;
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

    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0){
        error("ERROR writing to socket");
    }

    close(newsockfd);
    
    return NULL;
}
#define MAX_CONN 1000
int main(int argc, char **argv)
{    
    
    pthread_t tid[MAX_CONN];
    int connSocket[MAX_CONN];

    int threadCount = 0;
    int sockfd = createTcpServerSocket(3000);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    while(1){
        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0){
            error("ERROR on accept");
        }

        //spawn thread
        connSocket[threadCount] = newsockfd;
        int err = pthread_create(&(tid[threadCount]), NULL, &connectionHandler, &connSocket[threadCount]);
        threadCount++;
        if (err != 0){
             printf("\ncan't create thread :[%s]", strerror(err));
        }
        //NEED TO JOIN
        //pthread_join(tid[threadCount-1], NULL);
    }
    close(sockfd);
    return 0; 
}