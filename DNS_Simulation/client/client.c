#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define CLIENT 8385
#define SIZE 256

int main() {
    int client_socket;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CLIENT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    char buf[SIZE];
    int n, sentbytes, recvbytes;

    while(1) {
        bzero(buf, sizeof(buf));
        n = 0;
        printf("\nEnter URL: ");
        while((buf[n++] = getchar()) != '\n');
        sentbytes = sendto(client_socket, buf, strlen(buf) + 1, 0, (struct sockaddr*)&server_address, sizeof(server_address));
        buf[n-1] = '\0';
        if(strcmp(buf, "bye") == 0) {
            printf("\nClient closed\n");
            break;
        }
        bzero(buf, sizeof(buf));
        recvbytes = recvfrom(client_socket, buf, sizeof(buf), 0, NULL, NULL);
        printf("\nServer Response:\n\n%s\n", buf);
    }
    close(client_socket);
    return 0;
}
