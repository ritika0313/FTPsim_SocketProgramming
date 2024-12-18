/*******************************************************************************  
header.h  
HEADER FILE TO INCLUDE THE HEADER FILES AND DATA STRUCTURES COMMON TO THE 
SERVER AND CLIENT CODE  
*******************************************************************************/ 
 
#ifndef HEADER_H 
#define HEADER_H 
 
#include<stdio.h> 
#include<stdlib.h> 
#include<sys/socket.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<errno.h> 
#include<string.h> 
#include<unistd.h> 
#include<arpa/inet.h> 
#include<sys/dir.h> 
 
#define ERROR -1 
#define N 16192 
#define ND 1024 
#define Nc 64 
 
char buff[Nc]; /*buffer to send and receive commands and FTP replies*/ 
char data[N]; /*buffer to receive the contents of the files*/ 
 
int connected=0; /*variable to indicate the status of control connection 
i.e. if established, connected is set to 1*/ 
 
#endif 