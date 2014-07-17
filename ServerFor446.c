static char sqla_program_id[292] = 
{
 172,0,65,69,65,80,65,73,86,65,119,114,87,78,72,101,48,49,49,49,
 49,32,50,32,32,32,32,32,32,32,32,32,8,0,72,53,56,87,85,32,
 32,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,8,0,83,69,82,86,69,82,70,79,0,0,0,0,0,0,0,0,
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


#line 1 "ServerFor446.sqc"
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


#line 21 "ServerFor446.sqc"


// sql declare section

/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 24 "ServerFor446.sqc"

    char user_name[10];     //user name
    char room_sql[10];      //user room history
    char user_email[30];    //email_address also used as a primary key
    char password[20];      //user password
    char email[30];
    char real_pass[20];
    char db[6] = "cs348";

/*
EXEC SQL END DECLARE SECTION;
*/

#line 32 "ServerFor446.sqc"

//end of declare section



void send_msg(int sock, string s){
    char buffer[s.length()+1];
    strcpy(buffer,s.c_str());
    send(sock,buffer,4,0);
}






pthread_mutex_t *mutex;

/* Server information */
struct server_struct
{
	int 	socketfd_binder;
    int     socketfd_client;
    int     socketfd_log;
	fd_set	total_fd_list;
	char 	serv_NAME[256];
	int 	serv_PORT;
	bool    terminate;
};

/* Room information */
class socket_room_info{
public:    
    int id;
    char name[256];
    char room[256];
    bool dummy;
};

void dup_data(){
    //this function is supposed to duplicate user's information to this server
    // but we do not have much information stored for user
    //so we just simple insert currroom and user_name into db
    memset(&user_name,0,sizeof(user_name));
    for(int i = 0 ; email[i] != '\n' && email[i]!= '\0'&& email[i] != ' ';i++){
        user_name[i] = email[i];
    }

    
/*
EXEC SQL SELECT email INTO :user_email
    FROM user
    WHERE email = :user_name;
*/

{
#line 82 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 82 "ServerFor446.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 82 "ServerFor446.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 10;
#line 82 "ServerFor446.sqc"
      sql_setdlist[0].sqldata = (void*)user_name;
#line 82 "ServerFor446.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 82 "ServerFor446.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 82 "ServerFor446.sqc"
  sqlaaloc(3,1,2,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 82 "ServerFor446.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 30;
#line 82 "ServerFor446.sqc"
      sql_setdlist[0].sqldata = (void*)user_email;
#line 82 "ServerFor446.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 82 "ServerFor446.sqc"
      sqlasetdata(3,0,1,sql_setdlist,0L,0L);
    }
#line 82 "ServerFor446.sqc"
  sqlacall((unsigned short)24,1,2,3,0L);
#line 82 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 82 "ServerFor446.sqc"


    if(SQLCODE != 0){
        
/*
EXEC SQL INSERT INTO user(email) VALUES ("hao");
*/

/*
SQL0206N  "hao" is not valid in the context where it is 
used.  SQLSTATE=42703

*/

{
#line 85 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 85 "ServerFor446.sqc"
  sqlacall((unsigned short)24,2,0,0,0L);
#line 85 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 85 "ServerFor446.sqc"

        
/*
EXEC SQL COMMIT;
*/

{
#line 86 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 86 "ServerFor446.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 86 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 86 "ServerFor446.sqc"

    }
    
/*
EXEC SQL UPDATE user
    SET curroom = :room_sql
    WHERE email = :user_name;
*/

{
#line 90 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 90 "ServerFor446.sqc"
  sqlaaloc(2,2,3,0L);
    {
      struct sqla_setdata_list sql_setdlist[2];
#line 90 "ServerFor446.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 10;
#line 90 "ServerFor446.sqc"
      sql_setdlist[0].sqldata = (void*)room_sql;
#line 90 "ServerFor446.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 90 "ServerFor446.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 10;
#line 90 "ServerFor446.sqc"
      sql_setdlist[1].sqldata = (void*)user_name;
#line 90 "ServerFor446.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 90 "ServerFor446.sqc"
      sqlasetdata(2,0,2,sql_setdlist,0L,0L);
    }
#line 90 "ServerFor446.sqc"
  sqlacall((unsigned short)24,3,2,0,0L);
#line 90 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 90 "ServerFor446.sqc"

    
/*
EXEC SQL COMMIT;
*/

{
#line 91 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 91 "ServerFor446.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 91 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 91 "ServerFor446.sqc"


}



static struct server_struct servStruct;
static vector<socket_room_info> info_vect;
int glo_size = 0;



/* Handle msg from the binder */
void * handle_in(void *arg){
    // string tmp;
    pthread_mutex_lock(mutex);
    string tmp;
    char buffer[256];
    while(true){
        if (servStruct.terminate) {
            break;
        }


            printf("type ur input here:\n");
            cin >> tmp;
            if(tmp == "add"){
                char add ='A';
                send(servStruct.socketfd_binder,&add,1,0);
                cin>>tmp;
                int my_id = 2;
                if(tmp == "MC") my_id = 1;
                int my_net_id = htonl(my_id);
                cin >> tmp;
                /* send this server's port and address */
                send_msg(servStruct.socketfd_binder,tmp);
                send(servStruct.socketfd_binder, &my_net_id, 4, 0);


            }
            else if(tmp == "shutdown"){
                cerr<<"input : s"<<endl;
                servStruct.terminate = true;
                char shu = 'S'; 
                send(servStruct.socketfd_binder,&shu,sizeof(shu),0);
            }
            else if(tmp == "terminate"){
                char ter = 'T';

                servStruct.terminate = true;

                send(servStruct.socketfd_binder,&ter,sizeof(ter),0);
                break;
            }
            else{
                cerr<<"server input: add/shutdonw/terminate"<<endl;
            }
        }
    pthread_mutex_unlock(mutex);

}

void *handle_connect(void *arg){
    fd_set master, fdlist;
    socklen_t cli_len;
    int lastfd, newsockfd;
    struct sockaddr_in cli_addr;

    FD_SET(servStruct.socketfd_client, &master);
    FD_SET(servStruct.socketfd_binder, &master);
    lastfd = servStruct.socketfd_binder;

    char message[256];
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
                    cerr<<"incoming socket first try"<<endl;
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
                    recv(i, message, 1, 0);
                    if(strlen(message) == 0){
                        servStruct.terminate = true;
                        break;
                    }
                    if(message[0] == 'T'){
                        servStruct.terminate = true;
                        goto L;
                    }
                    else{
                        char room[256];
                        int building;
                        recv(i,room , 4, 0);
                        recv(i,&building,4,0);
                    }
                    memset(&message,0,sizeof(message));
                }
                else {
                    if(info_vect.size() <= i){
                        socket_room_info tmp;

                        /*recieve client info*/
                        tmp.id = i;
                        recv(i,tmp.room,7,0);
                        recv(i,tmp.name,10,0);
                        strcpy(email,tmp.name);
                        strcpy(room_sql,tmp.room);
                        send(servStruct.socketfd_log,tmp.name,10,0);
                        send(servStruct.socketfd_log,tmp.room,6,0);
                        char room_tmp[256] = "";
                        recv(servStruct.socketfd_log,room_tmp,6,0);
                        cout<<"client:   "<<tmp.name<<" is now entrying room : "<<tmp.room<<endl;   
                        tmp.dummy = false;
                        info_vect.push_back(tmp);
                        dup_data();
                        memset(&email,0,sizeof(email));
                        memset(&room_sql,0,sizeof(email));
                    }
                    else{

                        recv(i, message, 256, 0);   

                        
                        if(strlen(message) == 0){
                            info_vect[i].dummy = true;
                            FD_CLR(i,&master);
                        }
                        message[strlen(message)] = '\n';
                        bool room_equal = true;
                        for(int j = 6; j <= lastfd;j++){
                            for(int k = 0; k < 6;k++){
                                if(info_vect[j].room[k] != info_vect[i].room[k]) room_equal = false;
                            }
                            if(j != i && room_equal&&!info_vect[j].dummy){ 
                                //send(j,info_vect[i].name,sizeof(info_vect[i].name),0);
                                send(j,message,sizeof(message),0);
                            }
                            room_equal = true;
                        }
                    }

                    memset(&message,0,sizeof(message));            
                }
            }
        }
    }
    L:
    close(newsockfd);
    close(servStruct.socketfd_binder);
    close(servStruct.socketfd_client);
    close(servStruct.socketfd_log);
}


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
#line 273 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 273 "ServerFor446.sqc"
  sqlaaloc(2,1,4,0L);
    {
      struct sqla_setdata_list sql_setdlist[1];
#line 273 "ServerFor446.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 6;
#line 273 "ServerFor446.sqc"
      sql_setdlist[0].sqldata = (void*)db;
#line 273 "ServerFor446.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 273 "ServerFor446.sqc"
      sqlasetdata(2,0,1,sql_setdlist,0L,0L);
    }
#line 273 "ServerFor446.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 273 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 273 "ServerFor446.sqc"


    socket_room_info dummy;
    dummy.id = -1;
    info_vect.resize(6,dummy);
    glo_size = 6;

    mutex = new pthread_mutex_t();
    pthread_mutex_init (mutex , NULL);


    servStruct.terminate = false;
    if(argc != 3){
		cerr<<"Num of args should be 3-- Usage: ./server BINDER_ADDRESS BINDER_PORT"<<endl;
        return 0;
	}


    pthread_t input_thread;
    pthread_t accept_thread;

	/* socketfd and portno is for client to connect
		socketfd_binder is the socket connect to binder */
	int socketfd, portno,socketfd_binder,socket_log;
	struct sockaddr_in serv_addr,serverInfo,client_addr,client_addr_2;
	struct hostent *server;
    struct hostent *server_2;

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


    int my_id = 0;
 	int my_net_id = htonl(my_id);
 	/* send a 0 to binder, indicating this is a server */
 	send(socketfd_binder, &my_net_id, 4, 0);


 	/* send this server's port and address */
 	send(socketfd_binder, servStruct.serv_NAME, 256, 0); 
 	unsigned short portR = servStruct.serv_PORT;
 	portR = htons(portR);
 	send(socketfd_binder, &portR, sizeof(unsigned short), 0);

    char tmp_log[256] = "";
    recv(socketfd_binder,tmp_log,256,0);
    recv(socketfd_binder,&my_id,sizeof(unsigned short),0);
    my_id =(int) ntohs(my_id);


    //set u the connection with log server.
    socket_log = socket(AF_INET, SOCK_STREAM, 0);
    portno = my_id;
    server_2 = gethostbyname(tmp_log);
    if (server_2 == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &client_addr_2, sizeof(client_addr_2));
    client_addr_2.sin_family = AF_INET;
    bcopy((char *)server_2->h_addr, 
         (char *)&client_addr_2.sin_addr.s_addr,
         server_2->h_length);
    client_addr_2.sin_port = htons(portno);

    if (connect(socket_log,(struct sockaddr *) &client_addr_2,sizeof(client_addr_2)) < 0) 
        error("ERROR connecting");
    servStruct.socketfd_log = socket_log;



 	/*now constant listen to clients*/

    if(pthread_create(&input_thread,NULL,&handle_in,NULL)){
        cerr<<"error creating pthread"<<endl;
        return 0;
    }


    if(pthread_create(&accept_thread,NULL,&handle_connect,NULL)){
        cerr<<"error creating pthread"<<endl;
    }



    while(!servStruct.terminate){
        sleep(1);
    }

    
/*
EXEC SQL COMMIT;
*/

{
#line 399 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 399 "ServerFor446.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 399 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 399 "ServerFor446.sqc"

    
/*
EXEC SQL CONNECT RESET;
*/

{
#line 400 "ServerFor446.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 400 "ServerFor446.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 400 "ServerFor446.sqc"
  sqlastop(0L);
}

#line 400 "ServerFor446.sqc"

    return 0;
 	
  }
