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

using namespace std;


pthread_mutex_t *mutex;



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

/* Room information */
class socket_room_info{
public:    
    int id;
    char name[256];
    char room[256];
    bool dummy;
};



static struct server_struct servStruct;
static vector<socket_room_info> info_vect;
int glo_size = 0;



/* Handle msg from the binder */
void * handle_in(void *arg){
    // string tmp;
    pthread_mutex_lock(mutex);
    cerr<<"input thread"<<endl;
    string tmp;
    char buffer[256];
    while(true){
        if (servStruct.terminate) {
            break;
        }


            printf("type ur input here:\n");
            cin >> tmp;
            if(tmp == "add"){
                cerr<<"adding"<<endl;
                char add ='A';
                send(servStruct.socketfd_binder,&add,1,0);
                cin>>tmp;
                int my_id = 2;
                if(tmp == "MC") my_id = 1;
                int my_net_id = htonl(my_id);
                cin >> tmp;
                memcpy(&buffer,&tmp,sizeof(tmp));
                /* send this server's port and address */
                send(servStruct.socketfd_binder, buffer,sizeof(buffer), 0); 
                send(servStruct.socketfd_binder, &my_net_id, 1, 0);

            }
            else if(tmp == "shutdown"){
                cerr<<"input : s"<<endl;
                servStruct.terminate = true;
                char shu = 'S'; 
                send(servStruct.socketfd_binder,&shu,sizeof(shu),0);
            }
            else if(tmp == "terminate"){
                char ter = 'T';
                cerr<<"input : t"<<endl;
                /* 222222 */
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
    printf("accepting client\n");
    while (true){
        cerr<<"main is running"<<endl;
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
                cerr<<"incoming is in fd list"<<endl;
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
                    cerr<<"recv from binder"<<endl;   
                    recv(i, message, 256, 0);
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
                        recv(i,room , 256, 0);
                        recv(i,&building,4,0);
                    }
                }
                else {
                    cerr<<"recv from client :   "<<i<<endl;
                    if(info_vect.size() <= i){
                        /*recieve client info*/
                        socket_room_info tmp;
                        tmp.id = i;
                        recv(i,tmp.room,6,0);
                        recv(i,tmp.name,6,0);
                        tmp.name[strlen(tmp.name)] = '\n';
                        tmp.room[strlen(tmp.room)] = '\0';
                        tmp.dummy = false;
                        info_vect.push_back(tmp);
                    }
                    else{

                        recv(i, message, 256, 0);   

                        
                        if(strlen(message) == 0){
                            info_vect[i].dummy = true;
                            FD_CLR(i,&master);
                        }
                        message[strlen(message)] = '\n';
                        bool room_equal = true;
                        for(int j = 5; j <= lastfd;j++){
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
}


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){


    socket_room_info dummy;
    dummy.id = -1;
    info_vect.resize(5,dummy);
    glo_size = 5;
    /*  222222222 */
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


    int my_id = 0;
 	int my_net_id = htons(my_id);
 	/* send a 0 to binder, indicating this is a server */
 	send(socketfd_binder, &my_net_id, 4, 0);


 	/* send this server's port and address */
 	send(socketfd_binder, servStruct.serv_NAME, 256, 0); 
    cerr<<" my port    : "<<servStruct.serv_PORT<<endl;
 	my_id = servStruct.serv_PORT;
 	my_net_id	= htons(my_id);
 	send(socketfd_binder, &my_net_id, 4, 0);

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

    return 0;
 	
  }
