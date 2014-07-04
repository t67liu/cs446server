 #include <stdio.h>
 #include <string.h>    //strlen
 #include <sys/socket.h>
 #include <sys/ioctl.h>
 #include <arpa/inet.h>    
 #include <unistd.h>
 #include <iostream>
 #include <errno.h>
 using namespace std;

 //This function is to be used once we have confirmed that an image is to be sent
 //It should read and output an image file
 int receive_image(int socket){

int buffersize = 0, recv_size = 0,size = 0, read_size, write_size;
char imagearray[10241],verify = '1';
FILE *image;

//Find the size of the image
read(socket, &size, sizeof(int));



//Send our verification signal
write(socket, &verify, sizeof(char));
//Make sure that the size is bigger than 0
if(size <= 0 ){
    printf("Error has occurred. Size less than or equal to 0\n");
    return -1;
}

image = fopen("2.jpg", "w");

if( image == NULL) {
    printf("Error has occurred. Image file could not be opened\n");
    return -1;
}

//Loop while we have not received the entire file yet
while(recv_size < size) {
    ioctl(socket, FIONREAD, &buffersize); 

    //We check to see if there is data to be read from the socket    
    if(buffersize > 0 ) {

        if(read_size = read(socket,imagearray, buffersize) < 0){
            printf("%s", strerror(errno));
        }

        //Write the currently read data into our image file
        write_size = fwrite(imagearray,1,(buffersize), image);

        if(write_size != buffersize) {
          printf("write and buffersizes wrong");
        }

        if(read_size !=write_size) {
            printf("error in read write");
        }

        //Increment the total number of bytes read
        recv_size += read_size;

                    //Send our handshake verification info
        write(socket, &verify, sizeof(char));

    }
 }

fclose(image);
printf("Image successfully Received!\n");
return 1;
}

int main(int argc , char *argv[])
{

int socket_desc;
struct sockaddr_in server;
char *parray;


//Create socket
socket_desc = socket(AF_INET , SOCK_STREAM , 0);

if (socket_desc == -1) {
    printf("Could not create socket");
}

memset(&server,0,sizeof(server));
server.sin_addr.s_addr = inet_addr("10.42.0.1");
server.sin_family = AF_INET;
server.sin_port = htons( 8889 );

//Connect to remote server
if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
    cout<<strerror(errno);
    close(socket_desc);
    puts("Connect Error");
    return 1;
}

puts("Connected\n");

receive_image(socket_desc);

close(socket_desc);

return 0;
}