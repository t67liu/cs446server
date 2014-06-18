#include <string>
typedef enum {MC, OTHER} Building;

typedef struct room_location {
<<<<<<< HEAD
    int room_ID;
    int charing_server_ID;
    string room_number;
    Building building;
    unsigned int room_in_charge;
    room_location(char* room_num, Building recv_building) {
    	string temp(room_num);
=======
    std::string room_number;
    Building building;
    room_location(char* room_num) {
        std::string temp(room_num);
>>>>>>> 01220ba64ad518d7fab6f400275ca02416471c95
    	this->room_number = temp;
        this->building = recv_building;
    }
} room_location;

typedef struct client_info {
<<<<<<< HEAD
    int ID;
    string host_name;
=======
    std::string host_name;
>>>>>>> 01220ba64ad518d7fab6f400275ca02416471c95
    unsigned short port;
    unsigned int num_room;
    client_info(char* IP) {
        std::string temp(IP);
    	this->host_name = temp;
        ID = 0;
    }
} client_info;

