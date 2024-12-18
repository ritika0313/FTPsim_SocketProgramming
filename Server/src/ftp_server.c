/******************************************************************************* 
 
ftp_server.c 
 
SERVER CODE 
Enter the port number as a command line argument when executing the program 
 
*******************************************************************************/ 
 
#include "header.h" 
#include "func_server.h" 
 
/*MAIN FUNCTION*/ 
main(int argc, char *argv[]) 
{ 
   if(argc!=2) 
   { 
      printf("\n Invalid command line arguments"); 
      printf("\n Usage: %s <port no.> \n",argv[0]); 
      exit(1); 
   } 
   conn_serv(argv);  /*function call to establish control connection*/ 
   recv_cmd(); /*function call to receive FTP command from the client*/ 
} 
  
/*FUNCTION TO ESTABLISH CONTROL CONNECTION*/ 
void conn_serv(char *a[]) 
{ 
   struct sockaddr_in server; /*structure variable to define the server socket*/ 
   struct sockaddr_in client; /*structure variable to define the client socket*/    
   int sockaddr_len = sizeof(struct sockaddr_in); 
      
   /*Creation of socket on the server terminal*/ 
   if((scmd_s = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
   { 
      perror("server socket: "); 
      exit(1); 
   }    
   /*Initializing the server socket structure*/ 
   server.sin_family = AF_INET; 
   server.sin_port = htons(atoi(a[1])); 
   server.sin_addr.s_addr = INADDR_ANY; 
   bzero(&server.sin_zero, 8); 
   
   /*bind() called by the server process to attach itself to a specific 
   port & IP address*/   
   if((bind(scmd_s, (struct sockaddr *)&server, sockaddr_len)) == ERROR) 
   { 
      perror("bind : "); 
      close(scmd_s); 
      exit(1); 
   } 
   
   /*Server waits for a connection request from the client using the listen() function*/ 
   if((listen(scmd_s, 5)) == ERROR) 
   { 
      perror("listen"); 
      exit(1); 
   } 
   
   /*accept() is the function called by the server to accept connection 
   requests from client(s)*/ 
   if((scmd_c = accept(scmd_s, (struct sockaddr *) &client, &sockaddr_len)) == ERROR) 
   { 
      perror("accept"); 
      exit(1); 
   }       
   ip.s_addr=client.sin_addr.s_addr;    
   send(scmd_c, "220 service ready",Nc, 0); 
} 
 
 
/*FUNCTION TO RECEIVE FTP COMMANDS FROM THE CLIENT*/ 
void recv_cmd() 
{ 
   char *param; /* pointer to the character array containing command parameter(s)*/ 
   char cmd[Nc]; /*character array to receive the command(along with the paramters if any) from the client*/ 
   char cmd1[Nc]; /*character array to hold the command(minus the parameter(s))received from the client*/    
   int size,j;  
   
   do{ 
      //clear the array of any undefined values using memset() function 
      memset(cmd,'\0', Nc);       
      if((size=recv(scmd_c,cmd,Nc,0)) == ERROR) /*command along with parameter(s) received in cmd[]*/ 
      { 
         perror("recv cmd:"); 
      }       
      cmd[size-2]=0; 
      printf("\n Cmd received: %s\n",cmd); 
      
      /*to separate the command from its parameters*/ 
      memset(cmd1,'\0', Nc);  
      strcpy(cmd1,cmd);    
      param = strchr(cmd1,' ');        
      if(param)  
      { 
         *param='\0'; 
         param++; /*param now points to the command parameters*/ 
      }      
      
      /*to determine which command the User(on the client side) wishes to execute*/ 
      for(j=0; j<N_CMD; j++) 
      {   
         if( strcmp(cmd1,commands[j])==0) 
         break;  
      } 
      
      switch(j) 
      {    
         case (USER) : user(param);  /*Input username*/ 
            break; 
            
         case (PASS): passwrd(param); /*Verify the login password of the user*/ 
            break; 
         
         case (CWD): cwd(param); /*Change the current working directory of the user*/ 
            break; 
         
         case (CDUP): cdup(); /*Switch to the parent directory of the current directory*/ 
            break; 
         
         case (RETR): retr(param);/*Retrieve a file from the server & tranfer it to the 
            client*/ 
            break; 
         
         case (DELE): del(param); /*Delete a file on the remote host*/ 
            break; 
         
         case (PWD):  pwd(); /*Determine the current working directory of the user*/ 
            break; 
         
         case (MKD):  mkd(param); /*Create a new directory on remote host*/ 
            break; 
         
         case (RMD):  rmd(param); /*Remove an existing directory on server*/   
            break; 
      
         case (LIST): list(); /*List the contents of the specified directory*/ 
            break; 
         
         case (STOR): upload(param);/*Upload a file to the server from the client*/ 
            break; 
         
         case (PORT): port(param);/*Receive the port no. from client*/ 
            break; 
            
         case (ABOR): quit(); /*Close the connection between server & client*/ 
            break;        
            
      }       
   }while(connected!=0);   
} 