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
#include <pthread.h>
using namespace std;

static vector<client_info*> server_info;
static vector<room_location*> room_list;
static map<int, client_info*> unspec_request;   // key is the file handler, value is the info
static client_info* login_server;

/* checked */
void register_login_server(int fd) {
        /* Get the server information */
    char host_name[256];
    memset(host_name, 0, 256);
    int nbytes = recv(fd, host_name, 256 ,0);

    if (nbytes <= 0) {
        cerr << "FATAL ERROR: can not log the login server" << endl;
        return;
    }
    unsigned short port = 0;
    nbytes = recv(fd, &port, sizeof(unsigned short), 0);
    port = ntohs(port);
    if (nbytes <=0) {
        cerr << "FATAL ERROR: can not log the login server" << endl;
        return;
    }

    map<int, client_info*>::iterator it = unspec_request.find(fd);
    if (it == unspec_request.end()) {
        cerr << "FATAL ERROR: no cerrsponding file handler" << endl;
    }
    login_server = new client_info(host_name, fd, port);
    login_server->ID = 1;
    delete (it->second);
    unspec_request.erase(it);

    cout << "the log in server is " << host_name << " " << port << endl;
}

/* Ask login server for checking the user's account */
/* checked */
int check_info (int fd) {
    if (login_server == NULL) {
        map<int, client_info*>::iterator it = unspec_request.find(fd);
        if (it == unspec_request.end()) {
            cerr << "FATAL ERROR: no cerrsponding file handler" << endl;
        }
        delete (it->second);
        unspec_request.erase(it);
        return 3;
    }
    int size_username;
    int nbytes = recv(fd, &size_username, sizeof(int),0);    
    size_username = ntohl(size_username);
    char user_name[size_username];
    memset(user_name, 0, size_username);
    nbytes = recv(fd, user_name, size_username ,0);
    if (nbytes <= 0) {
        cout << "FATAL ERROR: can not log a user" << endl;
        return 1;
    }
    int size_password;    
    nbytes = recv(fd, &size_password, sizeof(int) ,0);
    size_password = ntohl(size_password);

    char password[size_password];
    memset(password, 0, size_password);
        cout << "i am here" << endl;

    nbytes = recv(fd, password, size_password, 0);
    if (nbytes <=0) {
        cout << "FATAL ERROR: can not log a user" << endl;
        return 1;
    }
    int size_username_n = htonl(size_username);
    int size_password_n = htonl(size_password);
    char check_type[7] = {'S','I','G','N','I','N','\0'};
    send(login_server->fd, check_type, 7, 0);
    send(login_server->fd, &size_username_n, sizeof(int), 0);
    send(login_server->fd, user_name, size_username, 0);
    send(login_server->fd, &size_password_n, sizeof(int), 0);
    send(login_server->fd, password, size_password, 0);
    char check_result[8];
    memset(check_result, 0, 8);
    nbytes = recv(login_server->fd, check_result, 8, 0);
    cout<<"             check_result:   "<<check_result<<endl;
    if (nbytes <=0) {
        cerr << "FATAL ERROR: can not log a user" << endl;
        return 1;
    }
    if (check_result[0] == 's') {
        return 0;
    }
    else {
        return 1;
    }
}

/* checked */
int register_info(int fd) {
    if (login_server == NULL) {
        map<int, client_info*>::iterator it = unspec_request.find(fd);
        if (it == unspec_request.end()) {
            cerr << "FATAL ERROR: no cerrsponding file handler" << endl;
        }
        delete (it->second);
        unspec_request.erase(it);
        return 3;
    }
    int size_username;
    int nbytes = recv(fd, &size_username, sizeof(int),0);    
    size_username = ntohl(size_username);
    char user_name[size_username];
    memset(user_name, 0, size_username);
    nbytes = recv(fd, user_name, size_username ,0);
    if (nbytes <= 0) {
        cout << "FATAL ERROR: can not log a user" << endl;
        return 1;
    }
    int size_password;    
    nbytes = recv(fd, &size_password, sizeof(int) ,0);
    size_password = ntohl(size_password);

    char password[size_password];
    memset(password, 0, size_password);
        cout << "i am here" << endl;

    nbytes = recv(fd, password, size_password, 0);
    if (nbytes <=0) {
        cout << "FATAL ERROR: can not log a user" << endl;
        return 1;
    }
    int size_username_n = htonl(size_username);
    int size_password_n = htonl(size_password);
    char check_type[7] = {'S','I','G','N','U','P','\0'};
    send(login_server->fd, check_type, 7, 0);
    send(login_server->fd, &size_username_n, sizeof(int), 0);
    send(login_server->fd, user_name, size_username, 0);
    send(login_server->fd, &size_password_n, sizeof(int), 0);
    send(login_server->fd, password, size_password, 0);
    char check_result[8];
    memset(check_result, 0, 8);
    nbytes = recv(login_server->fd, check_result, 8, 0);
    cout<<"             check_result:   "<<check_result<<endl;
    if (nbytes <=0) {
        cerr << "FATAL ERROR: can not log a user" << endl;
        return 1;
    }
    if (check_result[0] == 's') {
        return 0;
    }
    else {
        return 1;
    }
}

/* The first time we receive a message from a server */
/* checked */
void log_server(int fd) {
    if (login_server == NULL) {
        close(fd);
        map<int, client_info*>::iterator it = unspec_request.find(fd);
        if (it == unspec_request.end()) {
            cerr << "FATAL ERROR: no cerrsponding file handler" << endl;
            return;
        }
        delete (it->second);
        unspec_request.erase(it);
        return;
    }
    /* Get the server information */
    char host_name[256];
    memset(host_name, 0, 256);
    int nbytes = recv(fd, host_name, 256 ,0);
    string str(host_name);
        cout << "the log in server is " << host_name << endl;

    if (nbytes <= 0) {
        cerr << "FATAL ERROR: can not log a server" << endl;
        return;
    }
    unsigned short port = 0;
    nbytes = recv(fd, &port, sizeof(unsigned short), 0);
    port = ntohs(port);
    if (nbytes <=0) {
        cerr << "FATAL ERROR: can not log a server" << endl;
        return;
    }

    send(fd, login_server->host_name.c_str(), login_server->host_name.length(), 0);
    unsigned short tmp_port = htons(login_server->port);
    send(fd, &(tmp_port), sizeof(unsigned short), 0);

    static int server_id = 2;
    map<int, client_info*>::iterator it = unspec_request.find(fd);
    if (it == unspec_request.end()) {
        cerr << "FATAL ERROR: no cerrsponding file handler" << endl;
    }
    client_info* temp = new client_info(host_name, fd, port);
    temp->ID = server_id;
    server_id++;
    delete (it->second);
    unspec_request.erase(it);
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
void* forward_request(void* new_fd) {
    int* fd_addr = (int*) new_fd;
    int fd = *fd_addr;
    /* Send out all room's info */
    for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); it++) {
        char room_num[5];
        memcpy(room_num, (*it)->room_number,5);
        room_num[4] = '\n';
        char command[2] = {'0', '\n'};
        send(fd, command, 2, 0);
        char build_buffer[3];
        itoa((*it)->building, build_buffer, 2);
        build_buffer[2] = '\n';
        //int building_network = htonl((*it)->building);
        //char buffer = itoa(building_network,buffer, 4);
        send(fd, build_buffer, 3, 0);
        //send(fd, room_num, 5, 0);
        char room1[2] = {room_num[0], '\n'};
        send(fd, room1, 2, 0);
        room1[0] = room_num[1];
        send(fd, room1, 2, 0);
        room1[0] = room_num[2];
        send(fd, room1, 2, 0);
        room1[0] = room_num[3];
        send(fd, room1, 2, 0);
        cout << "Send room " << (*it)->room_number << " " << building_network<< endl;
    }
    char command[2] = {'1', '\n'};
    send(fd, command, 2, 0);
    cout << "we are here" << endl;
    char room_num[4];
    int nbytes = recv(fd, room_num, 4,0);
    if (nbytes <= 0) {
        return NULL;
    }
    int building = 0;
    nbytes = recv(fd, &building, sizeof(int), 0);
    if (nbytes <=0) {
        close(fd);
        return NULL;
    }
    
    string temp_room_num(room_num);
    int charing_server = search_room(temp_room_num, (Building) ntohl(building));
    if (charing_server == 0) {
        cerr << "The room does not exist" << endl;
        close(fd);
        return NULL;
    }

    /* Allocation of a server */
    client_info* server = search_server(charing_server);
    if (server == NULL) {
        cerr << "The room does not exist" << endl;
        close(fd);
        return NULL;
    }
    send(fd, server->host_name.c_str(), server->host_name.length(), 0);
    unsigned short tmp_port = htons(server->port);
    send(fd, &(tmp_port), sizeof(unsigned short), 0);
    close(fd);
    return NULL;
}

/* Get the server with the minimum workload */
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
    char room_num[5];
    memset(room_num, 0, 4);
    int nbytes = recv(fd, room_num, 4 ,0);
    room_num[4] = '\0';
    if (nbytes <= 0) {
        return -1;
    }
    int building = 0;
    nbytes = recv(fd, &building, sizeof(int), 0);
    if (nbytes <=0) {
        return -1;
    }
    room_location* new_room = new room_location(room_num, (Building) ntohl(building));
        cout << ntohl(building) << endl;

    new_room->room_ID = room_id;
    room_id++;
    /* Find the minimum workload server */
    client_info* min_server=min_load_server();

    if (min_server!=NULL) {
        min_server->room_in_charge++;
        new_room->charing_server_ID = min_server->ID;
        int min_server_fd = min_server->fd;
        char command = 'A';
        send(min_server_fd, &command, 1, 0);
        send(min_server_fd, room_num, 4, 0);
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
            close(fd);
            break;
        }
    }
    if (server_id==0) {
        cerr << "Server does not exist, so cannot delete it" << endl;
        return;
    }
    for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); it++) {
        if ((*it)->charing_server_ID == server_id) {
            client_info* min_server=min_load_server();
            char room_num[4];
            memcpy(room_num, (*it)->room_number.c_str(), 4);
            if (min_server!=NULL) {
                min_server->room_in_charge++;
                (*it)->charing_server_ID = min_server->ID;
                int min_server_fd = min_server->fd;
                char command = 'A';
                send(min_server_fd, &command, 1, 0);
                send(min_server_fd, room_num, 4, 0);
                int building = htonl((*it)->building);
                send(min_server_fd, &((*it)->building), sizeof(int), 0);
            }
            else {
                /* Erase all the rooms' information */
                for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); ) {
                    room_location* temp = *it;
                    room_list.erase(it);
                    delete temp;
                }
            }
        }
    }
}

/* Shut down the system */
void shut_down() {

    /* Erase all the elements that are waiting for being processed */
    for (map<int, client_info*>::iterator it = unspec_request.begin(); it != unspec_request.end(); ) {
        client_info* temp = it->second;
        unspec_request.erase(it);
        int fd = temp->fd;
        close(fd);
        delete temp;
    }

    /* Erase all the servers' information */
    for (vector<client_info*>::iterator it = server_info.begin(); it != server_info.end(); ) {
        client_info* temp = *it;
        int fd = temp->fd;
        char command = 'T';
        send(fd, &command, 1, 0);
        close(fd);
        delete temp;
        server_info.erase(server_info.begin());
    }

    /* Erase all the rooms' information */
    for (vector<room_location*>::iterator it = room_list.begin(); it != room_list.end(); ) {
        room_location* temp = *it;
        room_list.erase(it);
        delete temp;
    }
}

/* Receive a command from a server */
int handle_msg(int fd, fd_set* server_fd, fd_set* fds) {
    char command;
    int nbytes = recv(fd, &command, 1, 0);
    cout << command << endl;
    if (nbytes <= 0) return 0;
    if (command == 'A') {
        if (add_room(fd) == -1) {
            cerr << "Failed to add a room" << endl;
        }
    }
    else if (command == 'T') {
        terminate(fd);
        FD_CLR(fd, server_fd);
        FD_CLR(fd, fds);
    }
    else if (command == 'S') {
        shut_down();
        return 1;
    }
    else {
        cerr << "Unrecognized type of command received from server  " << command << endl;
    }
    return 0;
}

/* Store connection information */
void connection_info(struct sockaddr_in &client, int fd) {
    char* IP = inet_ntoa(client.sin_addr);
    client_info* temp = new client_info(IP);
    temp->port = htons(client.sin_port);
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

    socklen_t len = sizeof(Server_addr);
    getsockname(sockfd, (struct sockaddr *)&Server_addr, &len);	
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
                /* If this is a server */
                else if (FD_ISSET(i, &server_fds)) {
                    
                    if (handle_msg(i, &server_fds, &fds)) return 0;
                }

                /* Or this is a already connected socket */
                else {

                    /* Shake hand */
                    int iden;
                    int nbytes = recv(i,&iden,4,0);
                    iden = ntohl(iden);
                    cout << "what i receive is " << iden << endl;
                    /* Get nothing */
                    if (nbytes == 0) {
                        continue;
                    }

                    else if (nbytes < 0) {
                        close(i);
                        FD_CLR(i, &fds);
                    }
                    else {
                        /* If this is a server */
                        if (iden == 0) {
                            cout << "Get request from server" << endl;

                            /* Add this handler to the server_fds */
                            log_server(i);
                            FD_SET(i,&server_fds);
                        }

                        /* If this is a client */
                        else if (iden == 1) {
                            cout << "Get request from client" << endl;
                            char login_type[9];
                            memset(login_type,0,9);
                            nbytes = recv(i, &login_type,8,0);
                            cout << nbytes << "nbytes" << endl;
                            login_type[8] = '\0';
                            string type(login_type);
                            if (type.compare("FACEBOOK") == 0) {
                                cout << "FaceBook Login" << endl;
                            }
                            else if (type.compare("SIGN_INN") == 0) {
                                cout << "Database Login" << endl;
                                int check_result = check_info(i);
                                if (check_result == 1) {
                                    char msg[2] = {'F','\n'};
                                    send(i, msg, 2, 0);
                                    continue;
                                }
                                if (check_result == 3) {
                                    char msg[2] = {'N','\n'};
                                    send(i, &msg, 2, 0);
                                    close(i);
                                    FD_CLR(i, &fds);
                                    continue;
                                }
                                char msg[2] = {'S','\n'};
                                send(i, msg, 2, 0);
                            }
                            else if (type.compare("SIGN_UPP") == 0) {
                                cout << "Database Sign up" << endl;
                                int check_result = register_info(i);
                                if (check_result == 1) {
                                    char msg[2] = {'F','\n'};
                                    send(i, msg, sizeof(msg), 0);
                                    continue;
                                }
                                if (check_result == 3) {
                                    char msg = 'N';
                                    send(i, &msg, 1, 0);
                                    close(i);
                                    FD_CLR(i, &fds);
                                    continue;
                                }
                                char msg[2] = {'S','\n'};
                                send(i, msg, sizeof(msg), 0);
                            }
                            else {
                                cout << "ERROR type of command received from the Client " << type << endl;
                                continue;
                            }

                            /* Allocate the corresponding server to the client */
                            pthread_t t;
                            int copy_fd = i;
                            pthread_create(&t, NULL, &forward_request, &copy_fd);
                            map<int, client_info*>::iterator it = unspec_request.find(i);
                            client_info* temp = it->second;
                            unspec_request.erase(it);
                            delete temp;
                            FD_CLR(i, &fds);
                        }
                        else if (iden == 2) {
                            register_login_server(i);
                        }
                        /* Other type of messages */
                        else {
                            cout << iden << endl;
                            /* Someone might try to mimic servers  */
                            cerr << "Wrong type of message from clients" << endl;

                        }
                    }
                }
            }
        }
    }     
    return 0;
}
