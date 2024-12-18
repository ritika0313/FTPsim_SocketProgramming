/*******************************************************************************  
server_func.h  
HEADER FILE TO INCLUDE THE FUNCTIONS OF THE SERVER CODE  
*******************************************************************************/ 
 
#ifndef SERVER_FUNC_H 
#define SERVER_FUNC_H 

#include "header.h" 

/*The commands defined by integers to enable the use of 'switch case 
construct' later in the program*/ 
 
#define USER 0 
#define PASS 1 
#define CWD 2 
#define PWD 3 
#define CDUP 4 
#define DELE 5 
#define MKD 6 
#define RMD 7 
#define PORT 8 
#define LIST 9 
#define RETR 10 
#define STOR 11 
#define ABOR 12 
 
/*Number of Commands*/ 
#define N_CMD 13   
 
/*Function prototype declaration*/ 
void conn_serv(char* []); 
void recv_cmd(); 
void user(char*); 
void passwrd(char*); 
void cwd(char*); 
void pwd(); 
void cdup(); 
void mkd(char*); 
void rmd(char*); 
void list(); 
void port(char*); 
void del(char*); 
void retr(char*); 
void upload(char*); 
void quit(); 
 
int scmd_c;   /* socket descriptor for client*/ 
int scmd_s;   /* socket descriptor for server*/ 
char curdir[Nc]; /* array to hold the name of the current working directory*/ 
struct in_addr ip;   /* IP-address of the client*/ 
int dataport;  /* port no. to bind to*/ 

#endif // SERVER_FUNC_H