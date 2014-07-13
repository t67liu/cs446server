#include <string>
using namespace std;


typedef enum {MC=1, DC, OTHER} Building;

typedef struct room_location {
    int room_ID;
    int charing_server_ID;
    string room_number;
    Building building;
    room_location(char* room_num, Building recv_building) {
    	string temp(room_num);
    	this->room_number = temp;
        this->building = recv_building;
    }
} room_location;

typedef struct client_info {
    int ID;
    int fd;
    string host_name;
    unsigned short port;
    unsigned int num_room;
    unsigned int room_in_charge;
    client_info(char* IP, int temp_fd, unsigned short temp_port) {
    	string temp(IP);
    	this->host_name = temp;
        ID = 0;
        fd = temp_fd;
        port = temp_port;
    }
    client_info(char* IP) {
        string temp(IP);
        this->host_name = temp;
        ID = 0;
    }

} client_info;

