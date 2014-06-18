#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <string.h>
#include "Binderfor446.h"
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

void inform_server() {

}

client_info* min_load_server() {
    client_info* min_server=NULL;
    for (vector<client_info*>::iterator it = server_info.begin(); it != server_info.end(); it++) {
        if (min_server==NULL) {
            min_server=*it;
            continue;
        }
        if (min_server->room_in_charge > (*it)->room_in_charge) min_server=*it;
    }
    return min_server;
}
/* Add a room */
int add_room(int fd) {
    static int room_id = 1;
    char room_num[256];
    memset(room_num, 0, 256);
    int nbytes = recv(fd, room_num, 256 ,0);
    if (nbytes <= 0) {
        return -1;
    }
    int building = 0;
    nbytes = recv(fd, &building, sizeof(int), 0);
    if (nbytes <=0) {
        return -1;
    }
    room_location* new_room = new room_location(room_num, (Building) building);
    new_room->room_ID = room_id;
    room_id++;
    /* Find the minimum workload server */
    client_info* min_server=min_load_server();

    if (min_server!=NULL) {
        min_server->room_in_charge++;
        new_room->charing_server_ID = min_server->ID;
        int min_server_fd = min_server->fd;
        char command[] = "ADD_ROOM";
        send(min_server_fd, command, 9, 0);
        send(min_server_fd, room_num, 256, 0);
        send(min_server_fd, &building, sizeof(int), 0);
    }
    room_list.push_back(new_room);
    return 0;
}

/* Terminate this server */
void terminate(int fd) {
    int server_id=0;
    for (vector<client_info*>::iterator it = server_info.begin(); it != server_info.end(); it++) {
        if ((*it)->fd == fd) {
            client_info* temp = *it;
            server_id = (*it)->ID;
            server_info.erase(it);
            delete temp;
            break;
        }
    }
    if (server_id==0) {
        cerr << "Server does not exist, so cannot delete it" << endl;
        return;
    }
    for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); it++) {
        if ((*it)->room_ID == server_id) {
            client_info* min_server=min_load_server();
            char* room_num = new char[(*it)->room_number.size()+1];
            room_num[(*it)->room_number.size()] = 0;
            memcpy(room_num, (*it)->room_number.c_str(), (*it)->room_number.size()+1);
            if (min_server!=NULL) {
                min_server->room_in_charge++;
                (*it)->charing_server_ID = min_server->ID;
                int min_server_fd = min_server->fd;
                char command[] = "ADD_ROOM";
                send(min_server_fd, command, 9, 0);
                send(min_server_fd, room_num, (*it)->room_number.size()+1, 0);
                send(min_server_fd, &((*it)->building), sizeof(int), 0);
            }
        }
    }
}

/* Shut down the system */
void shut_down() {
    for (map<int, client_info*>::iterator it = unspec_request.begin(); it != unspec_request.end(); it++) {
        client_info* temp = it->second;
        unspec_request.erase(it);
        delete temp;
    }
    for (vector<client_info*>::iterator it = server_info.begin(); it != server_info.end(); it++) {
        client_info* temp = *it;
        server_info.erase(it);
        delete temp;
    }
    for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); it++) {
        room_location* temp = *it;
        room_list.erase(it);
        delete temp;
    }
}

/* Receive a command from a server */
int handle_msg(int fd) {
    char command[10];
    recv(fd, command, 10, 0);
    string str_command(command);
    if (!str_command.compare("ADD_ROOM")) {
        if (add_room(fd) == -1) {
            cerr << "Failed to add a room" << endl;
        }
    }
    else if (!str_command.compare("TERMINATE")) {
        terminate(fd);
    }
    else if (!str_command.compare("SHUT_DOWN")) {
        shut_down();
        return -2;
    }
    else {
        cerr << "Unrecognized type of command received from server" << endl;
    }
    return 0;
}

/* Store connection information */
void connection_info(struct sockaddr_in &client, int fd) {
    char* IP = inet_ntoa(client.sin_addr);
    client_info* temp = new client_info(IP);
    temp->port = ntohs(client.sin_port);
    temp->fd = fd;
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
    char hostname[256];
    gethostname(hostname,256);
    cerr<< "BINDER_ADDRESS " << hostname <<endl;
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
                        cerr << "Cannot allocate a new file handler" << endl;
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
                    iden = ntohs(iden);

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
                            if (handle_msg(i)) return 0;
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
                                forward_request(i);
                                map<int, client_info*>::iterator it = unspec_request.find(i);
                                client_info* temp = it->second;
                                unspec_request.erase(it);
                                delete temp;
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
