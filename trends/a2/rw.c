#include "dlxlib.h"

int main(int argc, char *argv[])
{
    char buffer[256];
    int sockfd = createTcpClientSocket(3000, argv[1]);
    if(sockfd == -1){
        error("bad client socket");
    }
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    int n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}