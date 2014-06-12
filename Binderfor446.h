#define Binder_h
#include <vector>
#include <string>
#include <map>
#define maxclient 5
#define INTSIZE 4
#define mode 1 //1 is test and programming
               //0 is submission version
using namespace std;


typedef enum {MC, OTHER} Building;

typedef struct room_location {
    string room_number;
    Building building;
    room_location(char* room_num) {
    	string temp(room_num);
    	this->room_number = temp;
    }
} room_location;

typedef struct client_info {
    string host_name;
    unsigned short port;
    unsigned int num_room;
    client_info(char* IP) {
    	string temp(IP);
    	this->host_name = temp;
    }
} client_info;

