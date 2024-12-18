#include "server_func.h"

char*commands[N_CMD]={"USER","PASS","CWD","PWD","CDUP","DELE","MKD","RMD","PORT", "LIST","RETR","STOR","ABOR"}; 
 
void user(char *param) 
{ 
  printf("\n Received username: %s",param );  
  if((send(scmd_c, "user ok",Nc,0)) < 0)   //ack 
  { 
    perror("send user ack:"); 
    exit(1); 
  }     
  connected=1; 
} 
 
/*FUNCTION TO VERIFY THE USER'S LOGIN PASSWORD TO ENABLE FTP*/ 
void passwrd(char *param) 
{  
    static int j=0; 
    char pw[]="guest"; 
      
    if(!strcmp(pw,param)) 
    { 
      printf("\nUser authenticated.Login Ok\n");             
      /*Acknowledgement sent to the client when password is correct*/ 
      if(send(scmd_c,"230 welcomes you",Nc,0) == ERROR) 
      { 
        perror("send user ack:");
      }       
      return;       
    } 
  
    else 
    { 
        j++; 
        if(j==3) /*counter to keep track of no. of password input trials*/ 
        {   
          send(scmd_c,"ABORTING...",Nc,0); 
          close(scmd_c); 
          close(scmd_s); 
          exit(1); 
        }     
        else 
        { 
        /*Acknowledgement sent to the client in case the password is incorrect*/ 
          send(scmd_c,"500 login incorrect",Nc,0); 
        } 
    } 
} 
 
/*FUNCTION TO CHANGE THE WORKING DIRECTORY*/ 
void cwd(char *param) 
{ 
    strcpy(curdir,param);       
    /*Acknowledgement sent to the client upon successful execution of the 
    command*/ 
    if(send(scmd_c,"250 CWD command successful",Nc,0) == ERROR) 
    { 
      perror("send cwd:"); 
    }   
} 
 
/*FUNCTION TO DETERMINE THE CURRENT WORKING DIRECTORY & SEND IT'S NAME TO THE 
CLIENT*/   
void pwd() 
{ 
    memset(buff,'\0',Nc); 
    sprintf(buff,"257 %s is current directory",curdir); 
      
    /*The name of the current working directory sent to the client*/ 
    if(send(scmd_c,buff,strlen(buff)+1,0) == ERROR) 
    { 
      perror("send pwd:"); 
    }  
} 
  
/*FUNCTION TO DETERMINE THE PARENT DIRECTORY OF THE CURRENT WORKING 
DIRECTORY*/ 
void cdup() 
{ 
    char *ptr;     
    ptr=strrchr(curdir,'/'); 
    if(ptr == NULL) 
    { 
      send(scmd_c,"500 ERROR",Nc,0); 
      return; 
    }       
    *ptr='\0';       
    if(send(scmd_c,"250 CWD command successful",Nc,0) == ERROR) 
    { 
      perror("send cwd ack: "); 
    } 
} 
 
/*FUNCTION TO DELETE THE SPECIFIED FILE(if present) ON THE SERVER*/ 
void del(char *param) 
{ 
  /*Removal of the specified file*/ 
  if(remove(param) == ERROR) 
  { 
    perror("remove file:"); 
    send(scmd_c,"500 File doesnt exist",Nc,0); 
    return; 
  } 
  
  /*Acknowledgement sent to the client upon successful deletion of the 
  specified file*/ 
  if(send(scmd_c,"250 delete command successful",Nc,0) == ERROR) 
  { 
    perror("send del ack: "); 
  }   
} 
 
/*FUNCTION TO CREATE A DIRECTORY WITH THE SPECIFIED NAME ON THE SERVER*/ 
void mkd(char *param) 
{  
    char dir[ND]; 
    memset(dir,'\0',ND); 
    strcat(dir, curdir); 
    strcat(dir, "/"); 
    strcat(dir, param); 
    /*Creation of the specified directory on the server*/ 
    if(mkdir(dir, 0777)== ERROR) 
    {  
    perror("\n mkdir: "); 
    send(scmd_c,"500 Directory already exists!",Nc,0); 
    return; 
    }  
    /*Acknowledgement sent to the client upon successful creation of the 
    specified directory on the server*/ 
    if(send(scmd_c, "257 directory created",Nc, 0) == ERROR) 
    { 
    perror("send mkd ack:"); 
    } 
}  
 
/*FUNCTION TO REMOVE THE DIRECTORY SPECIFIED BY THE CLIENT ON THE SERVER*/ 
void rmd(char *param) 
{ 
    char dirc[ND];  
    memset(dirc,'\0',ND); 
    strcat(dirc, curdir); 
    strcat(dirc, "/"); 
    strcat(dirc,param); 

    /*Removal of the specified directory from the server*/ 
    if(rmdir(dirc) == ERROR) 
    { 
    perror("\n rmdir: "); 
    send(scmd_c,"500 Directory does not exist",Nc,0); 
    return;    
    } 

    /*Acknowledgement sent to the client upon successful deletion of the 
    specified directory*/ 
    if(send(scmd_c, "112 Directory Removed!!!", Nc, 0) == ERROR) 
    { 
    perror("send rmdir ack:"); 
    } 
} 
 
/*FUNCTION TO LIST THE CONTENTS OF THE SPECIFIED DIRECTORY*/ 
void list() 
{  
    struct sockaddr_in srv; 
    int sd; 

    char dn[Nc]; 
    struct dirent *dirp; 
    DIR *dirfd;  

    if(!(dirfd = opendir(curdir))) 
    { 
      perror("opendir: "); 
      send(scmd_c,"500 Directory does not exist",Nc,0); 
      return; 
    }
    else 
    { 
      send(scmd_c,"111 Directory found",Nc,0); 
    } 

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
    { 
      perror("socket"); 
      return; 
    } 

    srv.sin_family = AF_INET; 
    srv.sin_port =dataport; 
    srv.sin_addr.s_addr =ip.s_addr; 
    bzero(&srv.sin_zero, 8); 

    if((connect(sd, (struct sockaddr *)&srv, sizeof(struct sockaddr_in))) == ERROR) 
    { 
      perror("connect"); 
      return; 
    } 

    while((dirp = readdir(dirfd)) != NULL) 
    {   
    memset(dn,'\0',Nc); 
    strcat(dn,dirp->d_name); 
    strcat(dn,"\t"); 
    send(sd, dn,strlen(dn), 0); 
    } 

    closedir(dirfd); 
    close(sd); 

    if((send(scmd_c, "226 transfer ok", Nc, 0)) < 0)  
    { 
    perror("error in sending"); 
    return; 
    }   

} 
 
 
/*FUNCTION TO TRANSFER A FILE TO THE CLIENT FROM THE SERVER*/ 
void retr(char *param) 
{ 
    char data1[ND]; 
    FILE *fs; 
    char path[Nc]; 
      
    struct sockaddr_in srv; 
    int sd; 
      
    memset(path,'\0',Nc); 
    strcat(path,curdir); 
    
    
    strcat(path,"/"); 
    strcat(path,param); 
  
    if((fs=fopen(path,"r"))==NULL) 
    {   
      printf(" File does not exist\n"); 
      send(scmd_c,"450 file action not taken(File doesn't exist)",Nc,0); 
      return; 
    } 
    else 
      send(scmd_c,"File found",Nc,0); 
      
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
    { 
      perror("socket"); 
      return; 
    } 
      
    srv.sin_family = AF_INET; 
    srv.sin_port =dataport; 
    srv.sin_addr.s_addr =ip.s_addr; 
    bzero(&srv.sin_zero, 8); 
      
    if((connect(sd, (struct sockaddr *)&srv, sizeof(struct sockaddr_in)))== ERROR) 
    { 
      perror("connect"); 
      return; 
    }        
    memset(data1,'\0',ND);        
    while(fgets(data1,ND,fs)!=NULL) 
    {      
      send(sd,data1,strlen(data1),0); 
      memset(data,'\0',ND);    
    
    }     
    fclose(fs);     
    close(sd);  
    send(scmd_c,"226 transfer ok",Nc,0); 
} 
 
/*FUNCTION TO TRANSFER THE FILE SPECIFIED BY THE CLIENT FROM THE CLIENT TO 
THE SERVER(uploading)*/ 
 
void upload(char *param) 
{ 
      
    FILE *ft; 
    char path[Nc];    
    struct sockaddr_in srv;  
    int sd; 

    memset(path,'\0',Nc); 
    strcat(path,curdir); 
    strcat(path,"/"); 
    strcat(path,param); 

    if((ft=fopen(path,"w"))==NULL) 
    { 
        send(scmd_c,"450 file action not taken",Nc,0); 
        return; 
    } 

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) 
    { 
        perror("socket"); 
        return; 
    } 
    srv.sin_family = AF_INET; 
    srv.sin_port =dataport; 
    srv.sin_addr.s_addr =ip.s_addr; 
    bzero(&srv.sin_zero, 8); 
    if((connect(sd, (struct sockaddr *)&srv, sizeof(struct sockaddr_in))) == ERROR) 
    { 
        perror("connect"); 
        return; 
    } 
    /*To receive the contents of the file specified by the client in the file 
    specified by the fopen() function on the server*/ 
    memset(data,'\0',N); 
    while(recv(sd, data, N, 0)) 
    {  
        fputs(data,ft); 
        memset(data,'\0',N); 
    } 
    close(sd); 
    fclose(ft); 
    send(scmd_c,"226 transfer ok",Nc,0); 
} 
 
void port(char *param) 
{  
    dataport=atoi(param); 
    send(scmd_c,"PORT command successful",Nc,0); 
} 
 
/*FUNCTION TO CLOSE CONNECTION BETWEEN CLIENT & SERVER*/  
void quit() 
{ 
    send(scmd_c,"221 GOODBYE !!",Nc,0); 
    close(scmd_c); /*client socket closed*/ 
    close(scmd_s); /*server socket closed*/ 
      
    connected=0; 
} 