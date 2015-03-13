/********
dlxlib.h -- header file for dlxlib.c
summary: commonly used functions for c programming
author: Trevor Baron
date: Nov 11 2013
********/
#ifndef DLXLIB_H
#define DLXLIB_H 1
#define DLXLIB_VERSION 0.9f

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "math.h"

//time
#include <sys/time.h>
#include <time.h>

//threading
#include <pthread.h>
#include <semaphore.h>

//sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define h_addr h_addr_list[0] /* for backward compatibility */

typedef struct vector {
   void * ar;
   int size;
} vector;

typedef struct binaryTree {
    struct binaryTree * left;
    struct binaryTree * right;
    void * data;
} binaryTree;

typedef struct linkedNode {
  	struct linkedNode * next;
  	struct linkedNode * prev;
    char * key;
  	void * data;
} linkedNode;

typedef struct linkedMT {
   linkedNode * head;
   linkedNode * tail;
   int size;
   pthread_mutex_t lock;
} linkedMT;

typedef struct timer {
   double start;
} timer;

double get_wall_time();
timer createTimer();
double getTime(timer t);

double dlxlibVersion();

vector * createVector();

binaryTree * createBinaryTree();

linkedMT * createLinkedMT();
void * findLinkedMT(linkedMT * q, char * key);
void * tailLinkedMT(linkedMT * q);
void * removeLinkedMT(linkedMT * q, char * key);
void pushLinkedMT();
void * popLinkedMT();

char * fileToString(char * filename);

char * dupStr(char * str);
char * strAdd(char * a,char * b);
int strToInt(char * str);
char * intToStr(int x);
vector * split(char * str, char * delim);

int print(const char * format, ...) __attribute__((format(printf, 1, 2)));
void error(const char *msg);

int dataAvailible(int fd);
int createTcpServerSocket(int port);
int createTcpClientSocket(int port, char*hostname);

#endif