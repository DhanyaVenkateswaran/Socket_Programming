#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define AUTH 8384
#define SIZE 256

int main() {
    int auth_socket, sentbytes, recvbytes;
    socklen_t length = sizeof(struct sockaddr_in);
    char buffer[SIZE];
    int i, j, f = 0;
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    fp = fopen("authdns.txt","r+");
    auth_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(auth_socket == -1) {
        printf("could not create socket\n");
        exit(0);
    }
    struct sockaddr_in server_address, client_addr;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(AUTH);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(auth_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        printf("error in binding\n");
        exit(0);
    }
    else {
        printf("\nSuccessfully Binded\n");
    }
    while(1) {
        recvbytes = recvfrom(auth_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &length);
        printf("\nLocal DNS Server: %s\n", buffer);
        rewind(fp);
        while ((getline(&line, &len, fp)) != -1) {
            for(i=0; i<strlen(buffer) && buffer[i] == line[i] && buffer[i] != ' '; i++);
            if(line[i] == ' ') {
                f = 1;
                i++;
                j = 0;
                bzero(buffer, sizeof(buffer));
                while(i < len) {
                    buffer[j++] = line[i++];
                }
                break;
            }
        }
        if(f == 0) {
            strcpy(buffer, "URL Not Found\n");
        }
        printf("Response: %s\n", buffer);
        sentbytes = sendto(auth_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&client_addr, length);
        bzero(buffer, sizeof(buffer));
    }
    close(auth_socket);
    return 0;
}
