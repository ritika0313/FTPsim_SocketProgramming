/*******************************************************************************
ftp_client.c 

CLIENT CODE 
Enter the IP address & port number as command line arguments while executing 
the program  
*******************************************************************************/ 
 
#include "header.h" 
#include "client_func.h" 
 
/*MAIN FUNCTION*/
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("\n Invalid command line arguments");
        printf("\n Usage: %s <server IP address> <server port no.>\n",argv[0]); 
        exit(1);
    }
    conn_cli(argv); /*Function call to establish control connection*/
    initial_cmd();  /*Function call to authenticate user*/
    input_cmd();    /*Function call to read a command from the User*/
}

/*FUNCTION TO ESTABLISH CONTROL CONNECTION*/ 
void conn_cli(char *argv[]) 
{ 
    struct sockaddr_in remote_server;     /*Structure variable of the type 
    sockaddr_in to denote the server socket*/ 

    /*creation of a socket*/ 
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
    { 
        perror("socket"); 
        exit(1); 
    } 

    /*Initialization of the socket sructure*/ 
    remote_server.sin_family = AF_INET; 
    remote_server.sin_port = htons(atoi(argv[2])); 
    remote_server.sin_addr.s_addr = inet_addr(argv[1]); 
    bzero(&remote_server.sin_zero, 8); 

    /*Connect() function call by the client to establish connection with 
    the server*/ 
    if((connect(sock, (struct sockaddr *) &remote_server, sizeof(struct  
    sockaddr_in))) == ERROR) 
    { 
        perror("connect"); 
        exit(1); 
    } 
    printf("\n %s  Connecting to %s  ...",prom,argv[1]); 
    memset(buff, '\0', Nc);   
    recv(sock, buff, Nc, 0); 
    printf("\n%s\n",buff); 
} 
 
/*FUNCTION TO AUTHENTICATE USER*/ 
void initial_cmd() 
{ 
    char root[Nc];  
    user();  /*Function call to interpret the username of the user*/ 
    passwrd();  /*Function call to interpret the password of the user*/ 

    /*to read the working directory from the user*/ 
    memset(root,'\0',Nc); 
    printf("\n %s Choose a home directory on remote host :\n ",prom); 
    gets(root); 
    cd(root); 
    connected=1; 
} 
 
/*FUNCTION TO READ FTP COMMAND FROM THE USER*/ 
void input_cmd() 
{ 
    int j; 
    char *param; 
    char buff1[Nc]; 

    while(connected!=0) 
    { 
    memset(buff, '\0', Nc); 

    printf("\n %s ",prom); 
    gets(buff); 

    memset(buff1, '\0', Nc); 
    strcpy(buff1,buff); 
    param = strchr(buff1,' ');  

    if(param) 
    { 
        *param='\0'; 
        param++;   
    } 
    for(j=0; j<NB_CMDS; j++) 
    { 
        if( strcmp(buff1, commands[j])==0 ) 
        break;  
    }  
    switch(j) 
    { 
        case (CD):  if(!param) 
            { 
                printf(" ERROR:no parameters!! \n"); 
                    break; 
            } 
            cd(param); 
            break;  

        case (CDUP): cdparent(); 
            break; 

        case (GET):  if(!param) 
            { 
                printf(" ERROR: no  parameters!! \n"); 
                break; 
            } 
            getfile(param); 
            break; 

        case (DEL):  if(!param) 
            { 
                printf(" ERROR: no parameters!!\n"); 
                break; 
            } 
            delet(param); 
            break; 

        case (PWD):  pwdir(); 
            break; 

        case (MKD):  if(!param) 
            { 
                printf(" ERROR: no parameters!! \n"); 
                break; 
            } 
            makdir(param); 
            break; 

        case (RMD):  if(!param) 
            { 
                printf(" ERROR: no parameters!! \n"); 
                break; 
            } 
            rmvdir(param); 
            break; 
            
        case (QUIT): quit(); 
            break; 

        case (HELP):  help(); 
            break; 
            
        case (DIR):  dir(param); 
            break; 
            
        case (PUT):  if(!param) 
            { 
                printf(" ERROR: no parameters!!\n"); 
                break; 
            } 
            put(param); 
            break; 
            
        case (MGET): if(!param) 
            { 
                printf(" ERROR: no parameters!! \n"); 
                break; 
            } 
            strcat(param," "); 
            mgt(param); 
            break; 
            
        case (MPUT): if(!param) 
            { 
                printf(" ERROR: no parameters!!\n"); 
                break; 
            } 
            strcat(param," "); 
            mpt(param); 
            break; 

        default:  printf(" Invalid command!! \n"); 
            break; 
        } 
    } 
} 