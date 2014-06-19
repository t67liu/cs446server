#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <cstdlib>

using namespace std;
pthread_mutex_t *mutex;
template <typename T> string to_string (const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

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


void * handle_in(void *arg){
                cout << "udwahiduwhauidh" << endl;

    // string tmp;
                char tmp[256];
    char buffer[256];
    while(true){
                        /* 222222 */
                pthread_mutex_lock(mutex);
        if (servStruct.terminate) {
            pthread_mutex_unlock(mutex);
            return 0;
        }
        
        pthread_mutex_unlock(mutex);
        cout << 5466464654656 << endl;
        while(cin.getline(tmp,256)){
            cout << "udwahiduwhauidh" << endl;
            if(tmp[0] == 'a' || tmp[0] == 'A'){
                int my_id = 0;
                if(tmp[4] == 'M') my_id = 1;
                int my_net_id = htonl(my_id);
                int j = 7;
                while(tmp[j] != '\n'){
                    buffer[j-7] = tmp[j];
                    j++;
                }
                buffer[j-7] = '\0';
                /* send this server's port and address */
                send(servStruct.socketfd_binder, buffer,sizeof(buffer), 0); 
                send(servStruct.socketfd_binder, (const char*)&my_net_id, 1, 0);

            }
            else if(tmp[0] == 'S' || tmp[0] == 's'){
                char shu = 'S'; 
                send(servStruct.socketfd_binder,&shu,sizeof(shu),0);
            }
            else if(tmp[0] == 'T' || tmp[0] == 't'){
                char ter = 'T';

                /* 222222 */
                pthread_mutex_lock(mutex);
                servStruct.terminate = true;
                pthread_mutex_unlock(mutex);

                send(servStruct.socketfd_binder,&ter,sizeof(ter),0);
                break;
            }
            else{
                cerr<<"server input: add/shutdonw/terminate"<<endl;
            }
        }

    }

}


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){

    /*  222222222 */
    mutex = new pthread_mutex_t();
    pthread_mutex_init (mutex , NULL);


    servStruct.terminate = false;
    if(argc != 3){
		cerr<<"Num of args should be 3-- Usage: ./server BINDER_ADDRESS BINDER_PORT"<<endl;
        return 0;
	}


    pthread_t input_thread;

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
   // cout<<portno<<endl;
    bzero((char *) &client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&client_addr.sin_addr.s_addr,
         server->h_length);
    client_addr.sin_port = htons(portno);

    if (connect(socketfd_binder,(struct sockaddr *) &client_addr,sizeof(client_addr)) < 0) 
        error("ERROR connecting");
    servStruct.socketfd_binder = socketfd_binder;


    // pthread for taking input
    cout << "1132131231312312" << endl;
    if(pthread_create(&input_thread,NULL,&handle_in,NULL)){
        cerr<<"error creating pthread"<<endl;
        return 0;
    }
    cout << "1132131231312312" << endl;


    int my_id = 1313;
    cout << "aaaaaa" << my_id << endl;
 	int my_net_id = htons(my_id);
 	/* send a 0 to binder, indicating this is a server */
 	send(socketfd_binder, &my_net_id, 4, 0);


 	/* send this server's port and address */
 	send(socketfd_binder, servStruct.serv_NAME, 256, 0); 

 	my_id = servStruct.serv_PORT;
    cout<<"server port    "<< servStruct.serv_PORT<<endl;
 	my_net_id	= htons(my_id);
 	send(socketfd_binder, &my_net_id, 4, 0);

 	/*now constant listen to clients*/

 	fd_set master, fdlist;
    socklen_t cli_len;
    int lastfd, newsockfd;
    struct sockaddr_in cli_addr;

    FD_SET(servStruct.socketfd_client, &master);
    FD_SET(servStruct.socketfd_binder, &master);
    lastfd = servStruct.socketfd_binder;

    char message[256];
    while (true){

        pthread_mutex_lock(mutex);
        if (servStruct.terminate) {
            pthread_mutex_unlock(mutex);
            return 0;
        }
        
        pthread_mutex_unlock(mutex);
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
                    recv(i, message, 5000, 0);
                    if(message[0] == 'T'){

                        /* 222222 */
                        pthread_mutex_lock(mutex);
                        servStruct.terminate = true;
                        pthread_mutex_unlock(mutex);

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
                    recv(i, message, 5000, 0);
                    for(int j = 0; j <= lastfd;j++){
                            send(j,message,strlen(message),0);
                    }            
                }
            }
        }
    }
    L:
    close(newsockfd);
    close(servStruct.socketfd_binder);
    close(servStruct.socketfd_client);
    return 0;
  }