#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define ROOT 8381
#define SIZE 256

int main() {
    int root_socket, sentbytes, recvbytes;
    socklen_t length = sizeof(struct sockaddr_in);
    char buf[SIZE];
    int i, j, f = 0;
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    fp = fopen("rootdns.txt","r+");
    root_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(root_socket == -1) {
        printf("Socket Creation failed\n");
        exit(0);
    }
    struct sockaddr_in server_address, client_addr;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(ROOT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(root_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        printf("Bind Failed\n");
        exit(0);
    }
    else {
        printf("\nBind Successful\n");
    }
    while(1) {
        recvbytes = recvfrom(root_socket, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &length);
        printf("\nMessage from Local DNS Server: %s\n", buf);
        rewind(fp);
        while ((getline(&line, &len, fp)) != -1) {
            for(i=0; i<strlen(buf) && buf[i] == line[i] && buf[i] != ' '; i++);
            if(line[i] == ' ') {
                f = 1;
                i++;
                j = 0;
                bzero(buf, sizeof(buf));
                while(i < len) {
                    buf[j++] = line[i++];
                }
                break;
            }
        }
        if(f == 0) {
            strcpy(buf, "404 - URL Not Found\n");
        }
        printf("Response from Root DNS : %s\n", buf);
        sentbytes = sendto(root_socket, buf, strlen(buf) + 1, 0, (struct sockaddr*)&client_addr, length);
        bzero(buf, sizeof(buf));
    }
    close(root_socket);
    return 0;
}
