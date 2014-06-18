#Include- <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include "Binderfor446.h"
#include <iostream>

using namespace std;

static vector<client_info*> server_info;
static vector<room_location*> room_list;
static map<int, client_info*> unspec_request;   // key is the file handler, value is the info

/* The first time we receive a message from a server */
void log_server(int fd) {
    static int server_id = 1;
    map<int, client_info*>::iterator it = unspec_request.find(fd);
    if (it == unspec_request.end()) {
        cerr << "FATAL ERROR: no cerrsponding file handler" << endl;
    }
    client_info* temp = it->second;
    temp->ID = server_id;
    server_id++;
    unspec_request.erase(fd);
    server_info.push_back(temp);
}

int search_room(string room_num, Building check_building) {
    for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); it++) {
        if ((*it)->building == check_building && !((*it)->room_number.compare(room_num))) return (*it)->charing_server_ID;
    }
    return 0;
}

client_info* search_server(int server_id) {
    for (vector<client_info*>::iterator it = server_info.begin(); it != server_info.end(); it++) {
        if ((*it)->ID == server_id)  {
            client_info* temp = *it;
            return temp;
        }
    }
    return NULL;
}
/* Allocate a server to the client */
int forward_request(int fd) {

    /* Get the room information */
    char room_num[256];
    int nbytes = recv(fd, room_num, 256 ,0);
    if (nbytes <= 0) {
        return -1;
    }
    int building = 0;
    nbytes = recv(fd, &building, sizeof(int), 0);
    if (nbytes <=0) {
        return -1;
    }
    
    string temp_room_num(room_num);
    int charing_server = search_room(temp_room_num, (Building) building);
    if (charing_server = 0) {
        cerr << "The room does not exist" << endl;
        return -1;
    }

    /* Allocation of a server */
    client_info* server = search_server(charing_server);
    if (server = NULL) {
        cerr << "The room does not exist" << endl;
        return -1;
    }
    send(fd, server->host_name.c_str(), server->host_name.length()+1, 0);
    send(fd, &(server->port), sizeof(unsigned short), 0);
    return 0;
}

/* Receive a command from a server */
int handle_msg() {
    return 0;
}

/* Store connection information */
void connection_info(struct sockaddr_in &client, int fd) {
    char* IP = inet_ntoa(client.sin_addr);
    client_info* temp = new client_info(IP);
    temp->port = ntohs(client.sin_port);

    // temp->fd = fd;
    temp->num_room = 0;

    cout << "TEST: We are get the info of connection" << endl;
    cout << "IP is " << temp->host_name << " Port number is " << temp->port << endl;
    map<int, client_info*>::iterator it = unspec_request.find(fd);
    if (it != unspec_request.end()) {
        cerr << "FATAL ERROR: file handler duplicated" << endl;
    }
    else {
        unspec_request[fd] = temp;
    }
}

int main(void)
{
    /* we are listening on welcome_fd*/
    int sockfd;

    /* new connection is on new_fd */
    int new_fd;

    int yes=1;
    
    char s[INET6_ADDRSTRLEN];
    
    /* Internet address information is stored here, defined in <netinet/in.h> */
    struct sockaddr_in Server_addr;

    /* create a new socket
     * AF_INET is for the domain address, we choose Internet rather than Unix domain
     * SOCK_STREAM is for the type of socket
     * 0 is for the protocal, we let the system to choose the most appropriate one
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Binder Socket: Creation failed" << endl;
        return -1;
    }
    
    /* Initialize the address information */
    Server_addr.sin_family = AF_INET;   // Internet address domain
    Server_addr.sin_port = htons(0);    // Dynamically binding a port, it is necessary to convert this to network byte order
    Server_addr.sin_addr.s_addr = INADDR_ANY;   // IP address of the host
    
    // memset(Server_addr.sin_zero, '\0', sizeof Server_addr);
    
    /* Bind the socket to an address */
    if (bind(sockfd, (struct sockaddr *) &Server_addr, sizeof (struct sockaddr_in)) < 0) {
        cerr << "Binder Socket: Binding failed" << endl;
        return -1;
    }
    
    // rtn = getsockname(sockfd, (struct sockaddr*) &Server_addr, &ServerAddrSize);
    // if(rtn == -1){
    //     std::cout<<"getsockname failed: " << strerror(errno) <<std::endl;
    //     return BINDER_GET_SOCKFD_FAIL;
    // }
    
    /* Listen on the socket with the maximum number of pending requests of 20 */
    if (listen(sockfd, 20) == -1) {
        std::cout<< "Binder Socket: Listening failed"<<std::endl;
        return -1;
    }
    
    /* Print out server information */
    //cerr<< "BINDER_ADDRESS " << hostname <<endl;
    cerr<< "BINDER_PORT " << ntohs(Server_addr.sin_port) <<endl;
    

    /* The structure to store all the handles */
    fd_set server_fds;
    FD_ZERO(&server_fds);
    FD_SET(sockfd,&server_fds);
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);
    int max_fd = sockfd;
    int min_fd = sockfd;

    /* Storing clients' information */
    socklen_t sin_size;
    struct sockaddr_in Client_addr;

    /* Start to handling connections */
    while(true) {

        /* Deep copy it */
        fd_set newfds = fds;

        /* To see how many socket handles that are ready and contained in the fd_set 
         * If there is no handle, just keep looping
         */
        if (select(max_fd+1, &newfds, NULL, NULL, NULL) < 0) {
            continue;               // no handle has I/O request
        }

        /* Otherwise, go through handles to listen their request */
        for (int i=min_fd; i <= max_fd; i++) {

            /* If this is a relavent handler */
            if (FD_ISSET(i,&newfds)) {

                /* If this the welcome handler */
                if (i == sockfd) {
                    sin_size = sizeof(Client_addr);
                    int newfd = accept(sockfd, (struct sockaddr*) &Client_addr, &sin_size);
                    if (newfd == -1) {
<<<<<<< HEAD
                        cerr << "Cannot allocate a new file handler" << endl;
=======
>>>>>>> 01220ba64ad518d7fab6f400275ca02416471c95
                        continue;
                    }

                    /* Add to unspecified list */
                    connection_info(Client_addr, newfd);

                    // inet_ntop(Client_addr.ss_family,
                    //           get_in_addr((struct sockaddr *)&Client_addr),
                    //           s, sizeof s);
                    if (newfd > max_fd) {
                        max_fd = newfd;
                    }
                    if (newfd < min_fd) {
                        min_fd = newfd;
                    }
                    FD_SET(newfd,&fds);
                }

                /* Or this is a already connected socket */
                else {

                    /* Shake hand */
                    unsigned int iden;
                    int nbytes = recv(i,&iden,sizeof(unsigned int),0);

                    /* Get nothing */
                    if (nbytes == 0) {
                        continue;
                    }

                    else if (nbytes < 0) {
                        close(i);
                        FD_CLR(i, &fds);
                    }
                    else {
                        if (FD_ISSET(i, &server_fds)) {
                                handle_msg();
                        }
                        else {
                            /* If this is a server */
                            if (iden == 0) {

                                /* Add this handler to the server_fds */
                                log_server(i);
                                FD_SET(i,&server_fds);
                            }

                            /* If this is a client */
                            else if (iden == 1) {

                                /* Allocate the corresponding server to the client */
                                if (forward_request(i) == -1) {
                                    cerr << "SOME ERROR when allocate a server" << endl;
                                }
                                close(i);
                                FD_CLR(i, &fds);
                            }

                            /* Other type of messages */
                            else {

                                /* Someone might try to mimic servers  */
                                cerr << "Wrong type of message from clients" << endl;

                            }
                        }
                    }
                }
            }
        }
    }     
    return 0;
}
