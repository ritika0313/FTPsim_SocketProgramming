#include "client_func.h"

/*FUNCTION TO READ THE USERNAME FROM THE USER & SEND IT TO THE SERVER*/ 
void user() 
{ 
      char usr[Nc]; 

      memset(user_name,'\0',25); 
      printf("\n %sName(username) ",prom); 
      gets(user_name); 

      memset(usr,'\0',Nc); 
      strcat(usr,"USER "); 
      strcat(usr,user_name); 
      strcat(usr,"\n"); 

      /*Username sent to the server*/ 
      if( send(sock, usr, strlen(usr)+1, 0) < 0 ) //send USER cmd 
      { 
         perror("send"); 
         exit(1); 
      } 
      /*Acknowledgement received from the server upon verification of 
      username*/ 
      memset(buff, '\0', Nc); 
      if(recv(sock, buff, Nc, 0))   
      printf(" %s \n\n",buff); 
} 
 
/*FUNCTION TO RECEIVE LOGIN PASSWORD FROM THE USER & SEND IT TO THE SERVER*/ 
void passwrd() 
{ 
char *c; 
char pw[25],pass[Nc]; 
int i; 

for(i=0;i<3;i++) 
{ 
      /*Function to read the password from the user*/ 
      memset(pass,'\0',Nc); 
      c=getpass(" password "); 
      sprintf(pw,"%s",c); 
      strcat(pass,"PASS "); 
      strcat(pass,pw); 
      strcat(pass,"\n"); 

      /*User Password sent to the server*/ 
      if(send(sock, pass, (strlen(pass)+1), 0) < 0) 
      { 
      perror("sending:"); 
      exit(1); 
      } 

      /*receive acknowledgement from the server upon successful verification of the 
      user password*/ 
         memset(buff,'\0',Nc); 
      recv(sock,buff,Nc,0); 
      printf("%s\n\n",buff); 

      if(!strncmp(buff,"230",3)) 
      { 
         printf("\n user %s logged in \n",user_name);  
         break; 
      }   
   } 
   
   /*In case the user fails to enter the corrrect password(which is asked for 
   thrice) the connection is closed*/ 
    if(i==3) 
    { 
      close(sock); 
      exit(1);  
    }    
} 
 
/*FUNCTION TO INTERPRET THE CHANGE DIRECTORY COMMAND*/ 
void cd(char *dir) 
{ 
      char cwd[Nc]; 

      memset(cwd,'\0',Nc); 

      strcat(cwd,"CWD "); 
      strcat(cwd,dir); 
      strcat(cwd,"\n"); 

      /*The command interpreted & sent to the server for execution*/ 
      if(send(sock, cwd, (strlen(cwd)+1), 0) < 0)                  
      { 
         perror("send"); 
         exit(1); 
      } 
      memset(buff, '\0', Nc); 
      /*recieve Acknowledgement from the server*/ 
      if( recv(sock, buff, Nc, 0 ) < 0)           
      { 
         perror("recv"); 
         exit(1); 
      } 
      printf(" %s \n\n",buff);          
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO DETERMINE THE PARENT DIRECTORY OF THE 
CURRENT WORKING DIRECTORY*/ 
void cdparent() 
{ 
   char cdpr[Nc]="CDUP\n";   
   /*The command interpreted & sent to the server for execution*/ 
   send(sock,cdpr,strlen(cdpr)+1,0);   
   memset(buff,'\0',Nc);   
   /*recieve Acknowledgement from the server*/ 
   recv(sock,buff,Nc,0);   
   printf("%s \n\n",buff);    
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO DELETE A FILE ON THE SERVER*/ 
void delet(char *file) 
{ 
   char del[Nc]="DELE ";   
   strcat(del,file); 
   strcat(del,"\n"); 

   /*The command interpreted & sent to the server for execution*/ 
   send(sock,del,strlen(del)+1,0); 
   /*recieve Acknowledgement from the server*/ 
   recv(sock,buff,Nc,0); 
   printf(" %s\n\n",buff); 

} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO DETERMINE THE PRESENT WORKING 
DIRECTORY*/ 
void pwdir() 
{ 
   char pwd[Nc]="PWD\n";  
   /*The command interpreted & sent to the server for execution*/ 
   send(sock,pwd,strlen(pwd)+1,0);  
   /*recieve the name of the present working directory from the server*/ 
   memset(buff,'\0',Nc); 
   recv(sock,buff,Nc,0);   
   printf(" %s \n\n",buff); 
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO CREATE A NEW DIRECTORY ON THE SERVER*/ 
void makdir(char *dname) 
{ 
   char mkd[Nc];    
   memset(mkd,'\0',Nc); 
   strcat(mkd, "MKD "); 
   strcat(mkd, dname); 
   strcat(mkd, "\n");    
   /*The command interpreted & sent to the server for execution*/ 
   send(sock, mkd, strlen(mkd)+1, 0);    

   /*recieve Acknowledgement from the server*/ 
   memset(buff,'\0',Nc); 
   recv(sock, buff, Nc, 0);    
   printf(" %s\n\n",buff,dname);    
} 
 
void rmvdir(char *dname) 
{ 
   char rmd[Nc];   
   memset(rmd,'\0',Nc); 
   strcat(rmd, "RMD ");  
   strcat(rmd,dname); 
   strcat(rmd, "\n"); 
   
   /*The command interpreted & sent to the server for execution*/ 
   send(sock, rmd, strlen(rmd)+1, 0); 
   
   /*recieve Acknowledgement from the server*/ 
   memset(buff,'\0',Nc); 
   recv(sock, buff, Nc, 0); 
   printf(" %s \n\n", buff); 
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO DETERMINE THE CONTENTS OF THE 
SPECIFIED DIRECTORY*/ 
void dir() 
{ 
   char dir[Nc]= "LIST\n"; 
   struct sockaddr_in serv; 
   struct sockaddr_in cli; 

   int sd_s,sd_c; 
   char port[Nc]; 
   char pn[Nc]; 
   int sockaddr_len = sizeof(struct sockaddr_in); 

   /*client creates a socket*/ 
   if((sd_c = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
   { 
      perror("server socket: "); 
      exit(1); 
   } 

   /*now client searches for a port available*/ 
   cli.sin_family = AF_INET; 
   cli.sin_addr.s_addr = INADDR_ANY; 
   cli.sin_port=htons(portno); 
   if((bind(sd_c, (struct sockaddr *)&cli, sockaddr_len)) != ERROR) 
   { 
      sprintf( pn,"%d",cli.sin_port); 
   } 
   portno++; 
   bzero(&cli.sin_zero, 8); 
      
   /*port number of client sent*/ 
   memset(port,'\0',Nc); 
   strcat(port,"PORT "); 
   strcat(port,pn); 
   strcat(port,"\n"); 
   send(sock,port,strlen(port)+1,0); 

   memset(buff,'\0',Nc); 
   recv(sock, buff, Nc, 0); 
   printf("\n%s",buff);  

   /*The command interpreted & sent to the server for execution*/ 
   if((send(sock,dir,strlen(dir)+1,0)) < 0)  
   { 
      perror("dir send: "); 
      return; 
   } 

   memset(buff,'\0',Nc); 
   recv(sock, buff, Nc, 0); 
   printf("\n%s",buff); 
   if(!strncmp(buff,"500",3)) 
   { 
      close(sd_c); 
      return; 
   } 
   if((listen(sd_c, 5)) == ERROR) 
   { 
      perror("listen"); 
      return; 
   } 

   /*client recieves the connect call from the server*/ 
   if((sd_s = accept(sd_c, (struct sockaddr*)&serv, &sockaddr_len)) == ERROR) 
   { 
      perror("accept"); 
      return; 
   } 

   printf("\nIn data connection!!!\n"); 
   /*To receive the contents of the specified directory*/   
   memset(buff,'\0',Nc); 
   while(recv(sd_s, buff,Nc, 0)) 
   { 
      printf("\n%s", buff); 
      memset(buff,'\0',Nc); 
   } 
   close(sd_s); 
   close(sd_c); 

   memset(buff,'\0',Nc); 
   if((recv(sock, buff, Nc, 0)) < 0) 
   { 
      perror("dir recv: "); 
      exit(1); 
   } 
   printf(" %s \n",buff);    
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TI RETRIEVE A FILE FROM THE SERVER*/ 
void getfile(char *fname) 
{  
   char retr[Nc];  
   char *param1; 
   FILE *ft; 

   struct sockaddr_in serv; 
   struct sockaddr_in cli; 

   int sd_s,sd_c; 
   char port[Nc]; 
   char pn[Nc]; 
   int sockaddr_len = sizeof(struct sockaddr_in); 

   //client creates a socket 
   if((sd_c = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
   { 
      perror("server socket: "); 
      return; 
   } 
   //now client searches for a port available 
   cli.sin_family = AF_INET; 
   cli.sin_addr.s_addr = INADDR_ANY; 
   cli.sin_port=htons(portno); 

   if((bind(sd_c, (struct sockaddr *)&cli, sockaddr_len)) != ERROR) 
   { 
      sprintf( pn,"%d",cli.sin_port);  
      portno++;   
   } 
   else 
   { 
      close(sd_c); 
      return; 
   } 

   bzero(&cli.sin_zero, 8); 

   //port number of client sent 
   memset(port,'\0',Nc); 
   strcat(port,"PORT "); 
   strcat(port,pn); 
   strcat(port,"\n"); 

   send(sock,port,strlen(port)+1,0); 
   memset(buff,'\0',Nc); 
   recv(sock,buff,Nc,0);    
   printf("%s\n",buff); 

   param1 = strchr(fname,' '); 
   if(param1==NULL) 
   { 
      param1 = fname; 
   } 
   else 
   { 
      *param1='\0'; 
      param1++; 
   } 
   memset(retr,'\0',Nc); 
   strcat(retr,"RETR ");  
   strcat(retr,fname); 
   strcat(retr,"\n");  
   /*The command interpreted & sent to the server for execution*/ 
   send(sock,retr,strlen(retr)+1,0); 

   memset(buff,'\0',Nc); 
   recv(sock,buff,Nc,0); 
   printf("%s\n",buff); 

   if(!strncmp(buff,"450",3)) 
   { 
      close(sd_c); 
      return; 
   } 
   if((listen(sd_c, 5)) == ERROR) 
   { 
      perror("listen"); 
      return; 
   } 
   //client recieves the connect call from the server 
   if((sd_s = accept(sd_c, (struct sockaddr*)&serv, &sockaddr_len)) == 
   ERROR) 
   { 
      perror("accept"); 
      return; 
   } 
   printf("\nIn data connection!!!\n"); 
   if((ft=fopen(param1,"w"))==NULL) 
   { 
   printf("\n 450 file action not taken \n"); 
   return; 
   } 
   printf("\nCopying file %s...",fname); 
   printf("\nNew file name is: %s\n",param1); 

   memset(data,'\0',N); 
   while(recv(sd_s,data, N, 0))  
   {  
   fputs(data,ft);  
   memset(data,'\0',N); 
   } 
   fclose(ft); 
   close(sd_s); 
   close(sd_c); 

   memset(buff,'\0',Nc); 
   recv(sock,buff,Nc,0); 
   printf("%s\n",buff); 
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO TRANSFER A FILE FROM THE SERVER TO THE 
CLIENT*/ 
void put(char *fname) 
{ 
   char put[Nc]; 
   char *param1; 
   FILE *fs; 

   struct sockaddr_in serv; 
   struct sockaddr_in cli; 

   int sockaddr_len = sizeof(struct sockaddr_in); 
   int sd_s,sd_c; 

   char port[Nc]; 
   char pn[Nc]; 

   param1 = strchr(fname,' '); 

   if(param1==NULL) 
   { 
   param1=fname; 
   } 

   else 
   { 
   *param1='\0'; 
   param1++; 
   }  

   if((fs=fopen(fname,"r"))==NULL) 
   { 
   printf("\n File action not taken (File does not exist) \n"); 
   return; 
   } 

   //client creates a socket 
   if((sd_c = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
   { 
   perror("server socket: "); 
   return; 
   } 

   //now client searches for a port available 
   cli.sin_family = AF_INET; 
   cli.sin_addr.s_addr = INADDR_ANY; 
   cli.sin_port=htons(portno); 

   if((bind(sd_c, (struct sockaddr *)&cli, sockaddr_len)) != ERROR) 


   { 
   sprintf( pn,"%d",cli.sin_port); 
   portno++;    
   } 

   bzero(&cli.sin_zero, 8); 
      
   memset(port,'\0',Nc); 
   strcat(port,"PORT "); 

   strcat(port,pn); 
   strcat(port,"\n"); 

   //send port number of client 
   send(sock,port,strlen(port)+1,0); 

   memset(buff,'\0',Nc); 
   recv(sock,buff,Nc,0);    
   printf("%s\n",buff); 

   memset(put,'\0',Nc); 
   strcat(put,"STOR "); 
   strcat(put,param1); 
   strcat(put,"\n"); 

   printf("In data connection!!!\n"); 

   /*The command interpreted & sent to the server for execution*/ 
   if(send(sock,put,strlen(put)+1,0) < 0) 
   { 
   perror("put send: "); 
   return; 
   } 

   if((listen(sd_c, 5)) == ERROR) 
   { 
   perror("listen"); 
   return; 
   } 

   //client recieves the connect call from the server 
   if((sd_s = accept(sd_c, (struct sockaddr*)&serv, &sockaddr_len)) == 
   ERROR) 
   { 
   perror("accept"); 
   return; 
   } 

   printf("\nCopying file %s...",fname); 
   printf("\nNew file name is: %s\n",param1); 

   while(fgets(data , N, fs)!=NULL) 
   {      
   send(sd_s,data,strlen(data),0); 
   memset(data,'\0', N);    
   } 
   fclose(fs); 
   close(sd_s); 
   close(sd_c); 

   memset(buff,'\0', Nc);    
   recv(sock,buff,Nc,0); 
   printf(" %s\n",buff); 
} 
 
void mgt(char *fname) 
{ 
   char mgt[Nc]; 
   char buff1[5]; 
   char *param1; 
   FILE *ft; 
   struct sockaddr_in serv; 
   struct sockaddr_in cli; 

   int sd_s,sd_c; 
   char port[Nc]; 
   char pn[Nc],path[Nc]; 
   int sockaddr_len = sizeof(struct sockaddr_in); 

   while(1) 
   { 
      param1 = strchr(fname,' '); 
      if(param1==NULL) 
         break;            
      *param1='\0'; 
      param1++; 

      memset(buff1,'\0',5); 
      printf("\nDo you want to copy %s ? (yes / no): ",fname); 
      gets(buff1); 
         
      if((!strcmp(buff1,"yes"))||(!strcmp(buff1,"YES"))) 
      { 
         //client creates a socket 
         if((sd_c = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
         { 
            perror("server socket: "); 
            return; 
         }  
         //now client searches for a port available 
         cli.sin_family = AF_INET; 
         cli.sin_addr.s_addr = INADDR_ANY;  
         cli.sin_port=htons(portno); 
            
         if((bind(sd_c,(struct sockaddr *)&cli,sockaddr_len)) != 
         ERROR) 
         { 
            sprintf( pn,"%d",cli.sin_port);  
            portno++;  
         } 
         else 
         { 
            close(sd_c); 
            break; 
         } 
         bzero(&cli.sin_zero, 8); 

         //send port number of client 
         memset(port,'\0',Nc); 
         strcat(port,"PORT "); 
         strcat(port,pn); 
         strcat(port,"\n"); 
         send(sock,port,strlen(port)+1,0); 
            
         memset(buff,'\0',Nc); 
         recv(sock,buff,Nc,0);    
         printf("%s\n",buff); 

         memset(buff1,'\0',5); 
            
         memset(mgt,'\0', Nc); 
         strcat(mgt,"RETR "); 
         strcat(mgt,fname); 
         strcat(mgt,"\n");          
         send(sock, mgt, strlen(mgt)+1, 0); 
            
         memset(buff,'\0',Nc); 
         recv(sock,buff,Nc,0); 
         printf("%s\n",buff); 

         if(!strncmp(buff,"450",3)) 
         { 
            close(sd_c); 
            strcpy(fname,param1); 
            continue; 
         } 
         if((listen(sd_c, 5)) == ERROR) 
         { 
            perror("listen"); 
            return; 
         } 
         //client recieves the connect call from the server 
         if((sd_s=accept(sd_c,(struct 
         sockaddr*)&serv,&sockaddr_len))==ERROR) 
         { 
            perror("accept"); 
            return; 
         } 
         printf("\nIn data connection!!!\n");          
         if((ft=fopen(fname,"w"))==NULL) 
         { 
            printf("\n File action not taken \n"); 
            close(sd_s); 
            close(sd_c); 
            
            strcpy(fname,param1); 

            continue; 
         } 
         memset(data,'\0',N); 
         while(recv(sd_s, data, N, 0)) 
         {     
            fputs(data,ft); 
            memset(data,'\0',N); 
         } 
         fclose(ft); 
         close(sd_s); 
         close(sd_c); 
            
         memset(buff,'\0',Nc); 
         recv(sock,buff,Nc,0); 
         printf("%s\n",buff);       
      }    
      strcpy(fname,param1); 
   } 
} 
 
void mpt(char *fname) 
{ 
   char mpt[Nc]; 
   char buff1[5]; 
   char data1[ND]; 
   char path[Nc]; 
   char *param1; 
   FILE *fs; 
   struct sockaddr_in serv; 
   struct sockaddr_in cli; 

   int sd_s,sd_c; 
   char port[Nc]; 
   char pn[Nc]; 
   int sockaddr_len = sizeof(struct sockaddr_in); 

   while(1) 
   { 
      param1 = strchr(fname,' ');  
      if(param1==NULL) 
      break; 

      *param1='\0'; 
      param1++; 

      memset(buff1,'\0',5); 
      printf("\nDo you want to copy %s ? (yes / no): ",fname); 
      gets(buff1);   

      if((!strcmp(buff1,"yes"))||(!strcmp(buff1,"YES"))) 
      { 
         if((fs=fopen(fname,"r"))==NULL) 
         { 
         printf("\n File action not taken (File does not      
         exist)\n"); 
            strcpy(fname,param1); 
            continue; 
         } 

         //client creates a socket 
         if((sd_c = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
         { 
            perror("server socket: "); 
            return; 
         } 

         //now client searches for a port available 
         cli.sin_family = AF_INET; 
         cli.sin_addr.s_addr = INADDR_ANY; 
         cli.sin_port=htons(portno); 

         if((bind(sd_c, (struct sockaddr *)&cli, 
         sockaddr_len))!=ERROR) 
         { 
            sprintf( pn,"%d",cli.sin_port); 
            portno++;    
         } 

         bzero(&cli.sin_zero, 8); 
            
         //port number of client sent 
         memset(port,'\0',Nc); 
         strcat(port,"PORT "); 
         strcat(port,pn); 
         strcat(port,"\n"); 

         send(sock,port,strlen(port)+1,0); 

         memset(buff,'\0',Nc); 
         recv(sock,buff,Nc,0);    
         printf("%s\n",buff); 

         memset(mpt, '\0', Nc); 
         strcat(mpt,"STOR "); 
         strcat(mpt,fname); 
         strcat(mpt,"\n");  

         /*The command interpreted & sent to the server for 
         execution*/ 
         if(send(sock,mpt,strlen(mpt)+1,0) < 0) 
         { 
            perror("put send: "); 
            return; 
         } 

         printf("\nCopying file %s...",fname); 

         if((listen(sd_c, 5)) == ERROR) 
         { 
            perror("listen"); 
            return; 
         } 

         //client recieves the connect call from the server 
         if((sd_s=accept(sd_c,(struct sockaddr*)&serv, 
         &sockaddr_len)) == ERROR) 
         { 
            perror("accept"); 
            return; 
         } 
         printf("In data connection!!!\n"); 

         while(fgets(data, N, fs)!=NULL) 
         {      
            send(sd_s,data,strlen(data),0); 
            memset(data,'\0', N);    
         } 
            
         fclose(fs); 
         close(sd_s); 
         close(sd_c); 
         memset(buff,'\0', Nc);    
         recv(sock,buff,Nc,0); 
         printf(" %s\n",buff); 
      } //end of if 
      strcpy(fname,param1);       
   } 
} 
 
/*FUNCTION TO INTERPRET THE COMMAND TO CLOSE THE CONNECTION BETWEEN CLIENT & SERVER*/  
void quit() 
{  
   char quit[N]="ABOR\n"; 

   /*The command interpreted & sent to the server for execution*/ 
   send(sock,quit,strlen(quit)+1,0); 

   memset(buff,'\0',Nc); 
   recv(sock,buff,Nc,0); 
   printf("%s\n",buff); 

   /*connection closed*/ 
   close(sock); 
   connected=0; 
} 
 
/*FUNCTION TO DETERMINE THE WORKING OF THE SIMULATED FTP COMMANDS*/ 
void help() 
{ 
   printf("\n\nCommand \tDescription \t\t\t\tSyntax"); 

   printf("\n\nCD \t Changes the current working directory \t\t cd <new 
   directory name>"); 

   printf("\n\nPWD \t Prints the current working directory \t\t pwd"); 

   printf("\n\nCDUP \t Switch to the parent directory \t\t cdup"); 

   printf("\n\nMKD \t Makes a new directory \t\t\t\t mkd <directory 
   name>"); 

   printf("\n\nRMD \t Removes an existing directory\t\t\t rmd <directory 
   name>"); 

   printf("\n\nDIR \t Lists all the directories and sub-directories \t 
   dir"); 

   printf("\n\nDEL \t Deletes a file \t\t\t\t del <file name>"); 

   printf("\n\nGET \t Downloads a file from the remote host \t get <file 
   name> *new filename"); 

   printf("\n\nPUT \t Transfers a file to remote host \t\t put <filename> 
   *new filename"); 

   printf("\n\nMGET \t Transfers multiple files from remote host \t mget 
   <filename1> *filename2 *..."); 

   printf("\n\nMPUT \t Transfers multiple files to remote host \t mput 
   <filename1> *filename2 *..."); 

   printf("\n\nQUIT \t Closes the session \t\t\t\t quit"); 

   printf("\n\nHELP \t Gives details of all commands \t\t\t help"); 

   printf("\n\n\n < >: mandatory parameters"); 

   printf("\n   *: optional parameters \n"); 

} 

 
 
 