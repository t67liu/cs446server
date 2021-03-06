static char sqla_program_id[292] = 
{
 172,0,65,69,65,80,65,73,100,65,110,114,87,78,72,101,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,72,53,56,87,85,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,76,79,71,83,69,82,86,69,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0
};

#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};


static const short sqlIsLiteral   = SQL_IS_LITERAL;
static const short sqlIsInputHvar = SQL_IS_INPUT_HVAR;


#line 1 "LogServer.sqc"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "util.h"


using namespace std;


/*
EXEC SQL INCLUDE SQLCA;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 21 "LogServer.sqc"


// sql declare section

/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 24 "LogServer.sqc"

    char user_name[16] ="NULL";     //user name
    char room_sql[16] = "";
    char room_his[16] = "";
    char user_email[16]= "";    //email_address also used as a primary key
    char password[16]= "";      //user password
    char email[16]= "";
    char real_pass[16]= "";
    char db[6] = "cs348";
    char tmp1[16] = "123";
    char tmp2[16] = "456";
    char tmp3[16] = "789";

/*
EXEC SQL END DECLARE SECTION;
*/

#line 36 "LogServer.sqc"

//end of declare section



void send_msg(int sock, string s){
    char buffer[s.length()+1];
    strcpy(buffer,s.c_str());
    send(sock,buffer,sizeof(buffer),0);
}



//insert for signup() return true for success, return false for duplicate email

bool sign_up(){
    cerr<<"sign up for user : "<<user_email<<endl;

    
/*
EXEC SQL SELECT email INTO :user_email
    FROM user
    WHERE email = :email;
*/

{
#line 56 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 56 "LogServer.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 56 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 56 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)email;
#line 56 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 56 "LogServer.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 56 "LogServer.sqc"
  sqlaaloc(3,1,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 56 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 56 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)user_email;
#line 56 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 56 "LogServer.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 56 "LogServer.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 56 "LogServer.sqc"
  sqlastop(0L);
}

#line 56 "LogServer.sqc"

    if(SQLCODE == 0){           //user exist
        return false;
    }
    else{
        
/*
EXEC SQL INSERT INTO user(email,password) VALUES (:email,:password);
*/

{
#line 61 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 61 "LogServer.sqc"
  sqlaaloc(2,2,3,0L);
    {
      struct sqla_setdata_list sql_setdlist[2];
#line 61 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 61 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)email;
#line 61 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 61 "LogServer.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 16;
#line 61 "LogServer.sqc"
      sql_setdlist[1].sqldata = (void*)password;
#line 61 "LogServer.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 61 "LogServer.sqc"
      sqlasetdata(2,0,2,sql_setdlist,0L,0L);
    }
#line 61 "LogServer.sqc"
  sqlacall((unsigned short)24,2,2,0,0L);
#line 61 "LogServer.sqc"
  sqlastop(0L);
}

#line 61 "LogServer.sqc"

        cerr <<" client sign up successfully::"<<"    "<<email<< "   "<<user_name<<"     "<<password<<"         sqlcode"<<SQLCODE<<endl;
        
/*
EXEC SQL COMMIT;
*/

{
#line 63 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 63 "LogServer.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 63 "LogServer.sqc"
  sqlastop(0L);
}

#line 63 "LogServer.sqc"

    }


    return true;
}

//log_in function return true if password is correct and user email exist, otherwise false
bool log_in(int pass_length){
    cerr<<"log in for user : "<<email<<endl;

    
/*
EXEC SQL SELECT email INTO :user_email
    FROM user
    WHERE email = :email;
*/

{
#line 76 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 76 "LogServer.sqc"
  sqlaaloc(2,1,4,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 76 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 76 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)email;
#line 76 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 76 "LogServer.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 76 "LogServer.sqc"
  sqlaaloc(3,1,5,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 76 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 76 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)user_email;
#line 76 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 76 "LogServer.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 76 "LogServer.sqc"
  sqlacall((unsigned short)24,3,2,3,0L);
#line 76 "LogServer.sqc"
  sqlastop(0L);
}

#line 76 "LogServer.sqc"

    if(SQLCODE == 0){           //user exist
        
/*
EXEC SQL SELECT password INTO :real_pass
        FROM user
        WHERE email = :email;
*/

{
#line 80 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 80 "LogServer.sqc"
  sqlaaloc(2,1,6,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 80 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 80 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)email;
#line 80 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 80 "LogServer.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 80 "LogServer.sqc"
  sqlaaloc(3,1,7,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 80 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 80 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)real_pass;
#line 80 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 80 "LogServer.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 80 "LogServer.sqc"
  sqlacall((unsigned short)24,4,2,3,0L);
#line 80 "LogServer.sqc"
  sqlastop(0L);
}

#line 80 "LogServer.sqc"

        for(int i = 0; i < pass_length;i++){
            if(real_pass[i] != password[i]) return false;
        }
        return true;
    }
    return false;
}

// change room history

void roomhiss(int sock){
    memset(&user_name,0,sizeof(user_name));
    for(int i = 0 ; email[i] != '\n' && email[i]!= '\0'&& email[i] != ' ';i++){
        user_name[i] = email[i];
    }
    
/*
EXEC SQL SELECT curroom INTO :room_his
    FROM user
    WHERE email = :user_name;
*/

{
#line 98 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 98 "LogServer.sqc"
  sqlaaloc(2,1,8,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 98 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 98 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)user_name;
#line 98 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 98 "LogServer.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 98 "LogServer.sqc"
  sqlaaloc(3,1,9,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 98 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 98 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)room_his;
#line 98 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 98 "LogServer.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 98 "LogServer.sqc"
  sqlacall((unsigned short)24,5,2,3,0L);
#line 98 "LogServer.sqc"
  sqlastop(0L);
}

#line 98 "LogServer.sqc"

    if(SQLCODE == 0){               // there is a history
        send(sock,room_his,16,0);
    }
    else{
        send_msg( sock,"NO-HIS");
    }
    
/*
EXEC SQL UPDATE user
    SET roomhis = :room_his,
        curroom = :room_sql
    WHERE email = :user_name;
*/

{
#line 108 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 108 "LogServer.sqc"
  sqlaaloc(2,3,10,0L);
    {
      struct sqla_setdata_list sql_setdlist[3];
#line 108 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 16;
#line 108 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)room_his;
#line 108 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 108 "LogServer.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 16;
#line 108 "LogServer.sqc"
      sql_setdlist[1].sqldata = (void*)room_sql;
#line 108 "LogServer.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 108 "LogServer.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 16;
#line 108 "LogServer.sqc"
      sql_setdlist[2].sqldata = (void*)user_name;
#line 108 "LogServer.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 108 "LogServer.sqc"
      sqlasetdata(2,0,3,sql_setdlist,0L,0L);
    }
#line 108 "LogServer.sqc"
  sqlacall((unsigned short)24,6,2,0,0L);
#line 108 "LogServer.sqc"
  sqlastop(0L);
}

#line 108 "LogServer.sqc"

    
/*
EXEC SQL COMMIT;
*/

{
#line 109 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 109 "LogServer.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 109 "LogServer.sqc"
  sqlastop(0L);
}

#line 109 "LogServer.sqc"


}


/* Server information */
struct server_struct
{
	int 	socketfd_binder;
    int     socketfd_client;
	fd_set	total_fd_list;
	char 	serv_NAME[256];
	int 	serv_PORT;
	bool    terminate;
};





static struct server_struct servStruct;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    
/*
EXEC SQL CONNECT TO :db;
*/

{
#line 138 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 138 "LogServer.sqc"
  sqlaaloc(2,1,11,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 138 "LogServer.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 138 "LogServer.sqc"
      sql_setdlist[0].sqldata = (void*)db;
#line 138 "LogServer.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 138 "LogServer.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 138 "LogServer.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 138 "LogServer.sqc"
  sqlastop(0L);
}

#line 138 "LogServer.sqc"



    servStruct.terminate = false;

	/* socketfd and portno is for client to connect  in this server , other servers are clients.
		socketfd_binder is the socket connect to binder */
	int socketfd, portno,socketfd_binder;
	struct sockaddr_in serv_addr,serverInfo,client_addr;
	struct hostent *server;

	/* create the socket for client to connect*/
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(0);
    if (bind(socketfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    listen(socketfd,20);
    socklen_t len = sizeof(serverInfo);
    getsockname(socketfd, (struct sockaddr *)&serverInfo, &len);
    gethostname(servStruct.serv_NAME,sizeof servStruct.serv_NAME);
    servStruct.serv_PORT = ntohs(serverInfo.sin_port);
    servStruct.socketfd_client = socketfd;
    FD_SET(servStruct.socketfd_client, &(servStruct.total_fd_list));


	/*set up connection to binder*/
	socketfd_binder = socket(AF_INET, SOCK_STREAM, 0);
    portno = atoi(argv[2]);
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
   // cerr<<portno<<endl;
    bzero((char *) &client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&client_addr.sin_addr.s_addr,
         server->h_length);
    client_addr.sin_port = htons(portno);

    if (connect(socketfd_binder,(struct sockaddr *) &client_addr,sizeof(client_addr)) < 0) 
        error("ERROR connecting");
    servStruct.socketfd_binder = socketfd_binder;


    int my_id = 2;
 	int my_net_id = htonl(my_id);
 	/* send a 0 to binder, indicating this is a server */
 	send(socketfd_binder, &my_net_id, 4, 0);


 	/* send this server's port and address */
 	send(socketfd_binder, servStruct.serv_NAME, 256, 0); 
 	my_id = servStruct.serv_PORT;
 	my_net_id	= htons(my_id);
 	send(socketfd_binder, &my_net_id, 2, 0);

 	/*now constant listen to clients*/

    fd_set master, fdlist;
    socklen_t cli_len;
    int lastfd, newsockfd;
    struct sockaddr_in cli_addr;

    FD_SET(servStruct.socketfd_client, &master);
    FD_SET(servStruct.socketfd_binder, &master);
    lastfd = servStruct.socketfd_binder;

    char message[256]= "";
    while (true){
        if (servStruct.terminate) {
            break;
        }
        
        fdlist = master;
        if (select(lastfd + 1, &fdlist, NULL, NULL, NULL) < 0) { // get list of read sockets
            perror("Server: select");
            exit(-1);
        }

        //loop through all sockets
        for (int i = 0; i <= lastfd; i++) {
            if (FD_ISSET (i, &fdlist)){ // new connection
                if (i == servStruct.socketfd_client) {
                    cli_len = sizeof(cli_addr);
                    newsockfd = accept(servStruct.socketfd_client, (struct sockaddr *) &cli_addr, &cli_len);
                    if (newsockfd < 0) {
                        perror("Server: accept");
                        break;
                    }
                    FD_SET(newsockfd, &master);
                    if (newsockfd > lastfd) {
                        lastfd = newsockfd;
                    }
                }
                else if (i == servStruct.socketfd_binder) { /* message from binder to terminate all servers */
                    int tmp_size = 0;  
                    recv(i, message, 7, 0);
                    if(strlen(message) == 0){
                        servStruct.terminate = true;
                        break;
                    }
                    if(message[0] == 'T'){
                        servStruct.terminate = true;
                        goto L;
                    }
                    else if(message[4] == 'U'){
                        recv(i,&tmp_size,4,0);
                        tmp_size = ntohl(tmp_size); 
                        recv(i, email, tmp_size, 0);
                        recv(i,&tmp_size,4,0);
                        tmp_size = ntohl(tmp_size); 
                        recv(i, password, tmp_size, 0);
                        if(sign_up()){
                            send_msg(i,"success");
                        }
                        else{
                            send_msg(i,"failure");
                        }
                        memset(&email,0,sizeof(email));
                        memset(&password,0,sizeof(password));   
                    }
                    else if(message[4] == 'I'){
                        recv(i,&tmp_size,4,0);
                        tmp_size = ntohl(tmp_size); 
                        recv(i, email, tmp_size, 0);
                        recv(i,&tmp_size,4,0);
                        tmp_size = ntohl(tmp_size); 
                        recv(i, password, tmp_size, 0);
                        if(log_in(tmp_size - 1)){
                            cout<<"     sign in succ"<<endl;
                            send_msg(i,"success");  
                        }
                        else{
                            send_msg(i,"failure");
                        }
                        memset(&email,0,sizeof(email));
                        memset(&password,0,sizeof(password)); 
                        memset(&user_name,0,sizeof(user_name));  
                    }
                }
                else {
                        recv(i,email,10,0);
                        recv(i,room_sql,6,0);
                        roomhiss(i);
                        memset(&email,0,sizeof(email));
                        memset(&room_sql,0,sizeof(room_sql));  
                        memset(&room_his,0,sizeof(room_his));     
                }
            }
        }
    }
    L:
    close(newsockfd);
    close(servStruct.socketfd_binder);
    close(servStruct.socketfd_client);



    
/*
EXEC SQL COMMIT;
*/

{
#line 304 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 304 "LogServer.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 304 "LogServer.sqc"
  sqlastop(0L);
}

#line 304 "LogServer.sqc"

    
/*
EXEC SQL CONNECT RESET;
*/

{
#line 305 "LogServer.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 305 "LogServer.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 305 "LogServer.sqc"
  sqlastop(0L);
}

#line 305 "LogServer.sqc"

    return 0;
 	
  }
