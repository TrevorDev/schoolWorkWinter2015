/********
dlxlib.c
summary: commonly used functions for c programming
author: Trevor Baron
date: Nov 11 2013
********/
#include "dlxlib.h"

//INFO
double dlxlibVersion(){
	return DLXLIB_VERSION;
}

//CLOCK
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

timer createTimer(){
	timer t;
	t.start = get_wall_time();
	return t;
}

double getTime(timer t){
	return get_wall_time()-t.start;
}

//VECOTR
vector * createVector(){
	vector * ret = (vector *)calloc(1, sizeof(vector));
	return ret;
}

//queueMT
queueMT * createQueueMT(){
	queueMT * ret = (queueMT *)calloc(1, sizeof(queueMT));
	ret->head = NULL;
	ret->tail = NULL;
	ret->size = 0;
	return ret;
}

void pushQueueMT(queueMT * q, void * data){
	pthread_mutex_lock(&(q->lock));

	linkedNode * node = (linkedNode *)calloc(1, sizeof(linkedNode));
	node->next = q->head;
	node->prev = NULL;
	node->data = data;

	if(q->head != NULL){
		q->head->prev = node;
	}
	q->head = node;

	if(q->tail == NULL){
		q->tail = node;
	}

	q->size++;

	pthread_mutex_unlock(&(q->lock));
}

void * popQueueMT(queueMT * q){
	pthread_mutex_lock(&(q->lock));
	linkedNode * toFree = q->tail;
	void * ret = toFree->data;

	if(q->tail->prev != NULL){
		q->tail->prev->next = NULL;
	}
	q->tail = q->tail->prev;
	q->size--;
	if(q->size == 0){
		q->head = NULL;
		q->tail = NULL;
	}

	free(toFree);
	pthread_mutex_unlock(&(q->lock));
	return ret;
}

//FILE IO
char * fileToString(char * filename){
	char * ret;
	long input_file_size;
	FILE * input_file = fopen(filename,"r");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);
	ret = malloc((input_file_size+1) * (sizeof(char)));
	fread(ret, sizeof(char), input_file_size, input_file);
	ret[input_file_size] = '\0';
	fclose(input_file);
	return ret;
}

//STRINGS
char * strAdd(char * a,char * b){
	char * ret=(char *)calloc(strlen(a)+strlen(b)+1,sizeof(char));
	ret[0]='\0';
	strcat(ret,a);
	strcat(ret,b);
	return ret;
}

char * dupStr(char * str){
	char * ret = (char *)calloc(strlen(str)+1, sizeof(char));
	strcpy(ret, str);
	return ret;
}

vector * split(char * str, char * delim){
	vector * v = createVector();
	char *token;
	int count = 0;
	token = strtok(str, delim);
	while( token != NULL ) 
	{    
		token = strtok(NULL, delim);
		count++;
	}

	v->size=count;
	v->ar = malloc(sizeof(char*)*count);
	token = str;
    for (int i = 0; i < count;i++) {
    	((char **)v->ar)[i] = dupStr(token);
    		if(i<count-1){
    			token += strlen(token) + 1;  // get the next token by skipping past the '\0'
        	token += strspn(token, delim); //   then skipping any starting delimiters
    		}
    }

   return v;
}

int * strToInt(char * str){
	int * ret = calloc(1,sizeof(int));
	errno = 0;
	int num = (int)strtol(str, NULL, 0);
	if(errno!=0){
		ret = NULL;
	}else{ 
		*ret = num;
	}
	
	return ret;
}

//PRINTING
int print(const char * format, ...) {
	va_list arg;
	int done;
	va_start (arg, format);
	done = vfprintf(stderr, format, arg);
	va_end (arg);
	printf("\n");
	return done;
}

//SOCKETS/NETWORKING

int dataAvailible(int fd){
    fd_set read_fd_set;
    FD_ZERO (&read_fd_set);
    FD_SET (fd, &read_fd_set);
    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;
    if(select(fd+1, &read_fd_set, NULL, NULL, &timeout) == 0){
        return 0;
    }else{
        return 1;
    }
}

int createTcpServerSocket(int port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        //error("ERROR opening socket");
        return -1;
    }
        
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        //error("ERROR on binding");
        return -1;
    }

    listen(sockfd,5);
    return sockfd;
}

int createTcpClientSocket(int port, char*hostname){
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        //error("ERROR opening socket");
        return -1;
    }
    server = gethostbyname(hostname);
    if (server == NULL) {
        // fprintf(stderr,"ERROR, no such host\n");
        // exit(0);
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);


    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        //error("ERROR connecting");
        return -1;
    }
    return sockfd;
}