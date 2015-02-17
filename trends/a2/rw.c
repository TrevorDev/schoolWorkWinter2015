#include "dlxlib.h"

//clear; valgrind ./rw 1 W cool.txt 192.168.1.107
//clear; valgrind --leak-check=full ./server


void sendReceiveMsg(int sockfd, char*msg){
     //send
    int n = write(sockfd, msg,strlen(msg));
    if (n < 0){
        error("ERROR writing to socket");
    }
         
    //receive
    char buffer[256];
    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    //printf("%s\n",buffer);
}

void randSleep(){
    nanosleep((struct timespec[]){{0, rand()%900000000}}, NULL);
}

int main(int argc, char *argv[])
{
    //INIT MSGES
    if(argc < 5){
        error("Not enuf args");
    }
    char*id = argv[1];
    char*type = argv[2];
    char*filename = argv[3];
    char*host = argv[4];
    char * msgStart = malloc(sizeof(char)*(strlen(id)+strlen(type)+strlen(filename)+4));
    sprintf (msgStart, "%s %s %s ", id, type, filename);
    char * msgReq = strAdd(msgStart,"req");
    char * msgRel = strAdd(msgStart,"rel");
    free(msgStart);

    //CREATE TCP SERVER
    int sockfd = createTcpClientSocket(3000, host);
    if(sockfd == -1){
        error("bad client socket");
    }

    while(1){
        randSleep();
        sendReceiveMsg(sockfd, msgReq);
        if(type[0] == 'W'){
            FILE *fptr = fopen(filename, "a+");{
                fprintf(fptr, "%s", id);
            }fclose(fptr);
        }else{
            char * str = fileToString(filename);
            print("%s", str);
        }
        sendReceiveMsg(sockfd, msgRel);
    }
    
   
    

    free(msgReq);
    free(msgRel);
    close(sockfd);
    return 0;
}