/*******************************************************************************  
client_func.h  
HEADER FILE FOR THE VARIOUS FUNCTIONS OF THE CLIENT CODE  
*******************************************************************************/ 
 
#ifndef CLIENT_FUNC_H 
#define CLIENT_FUNC_H 
 
/*FTP commands simulated*/ 
#define USER 0 
#define CD 1 
#define PWD 2 
#define CDUP 3 
#define MKD 4 
#define RMD 5 
#define DEL 6   
#define DIR 7 
#define GET 8 
#define PUT 9 
#define MGET 10 
#define MPUT 11 
#define QUIT 12 
#define HELP 13 
 
/*Number of Commands*/ 
#define NB_CMDS 14 
 
int sock; 
int portno=1025; 
char user_name[25]; 
 
char *commands[NB_CMDS]={ "user","cd","pwd","cdup","mkd", 
      "rmd","del","dir","get","put", 
      "mget","mput","quit","help"}; 
char *prom="ftp> "; 
 
/*Function Prototype Declaration*/ 
void conn_cli(char* []); 
void initial_cmd(); 
void user(); 
void cd(char*); 
void input_cmd(); 
void cdparent(); 
void getfile(char *); 
void pwdir(); 
void makdir(); 
void rmvdir(); 
void passwrd(); 
void delet(char *); 
void quit(); 
void help(); 
void dir(); 
void put(char *); 
void mgt(char *); 
void mpt(char *); 
 
#endif