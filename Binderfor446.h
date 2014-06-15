#include <string>
typedef enum {MC, OTHER} Building;

typedef struct room_location {
    std::string room_number;
    Building building;
    room_location(char* room_num) {
        std::string temp(room_num);
    	this->room_number = temp;
    }
} room_location;

typedef struct client_info {
    std::string host_name;
    unsigned short port;
    unsigned int num_room;
    client_info(char* IP) {
        std::string temp(IP);
    	this->host_name = temp;
    }
} client_info;

