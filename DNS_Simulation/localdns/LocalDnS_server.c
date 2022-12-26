#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define CLIENT 8385
#define ROOT 8381
#define TLD 8382
#define AUTH 8384
#define SIZE 256

void main() {
	int server_socket, root_socket, tld_socket, auth_socket, recvbytes, sentbytes;
    socklen_t length = sizeof(struct sockaddr_in);
    char buf[SIZE], root[SIZE], tld[SIZE], auth[SIZE], root_ip[SIZE], tld_ip[SIZE], auth_ip[SIZE];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_socket == -1) {
		printf("Socket Creation failed\n");
		exit(0);
	}
	struct sockaddr_in server_address, root_addr, tld_addr, auth_addr, client_addr;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(CLIENT);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
		printf("Bind Failed\n");
		exit(0);
	}
	else {
		printf("\nBind Successful\n");
	}
	while(1) {
        bzero(buf, sizeof(buf));
        recvbytes = recvfrom(server_socket, buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &length);
        printf("\nURL from Client : %s\n", buf);
        buf[strlen(buf)-1] = '\0';
        if(strcmp(buf, "exit") == 0) {
            printf("Local DNS Server Exit\n");
            break;
        }
        strcpy(auth, buf);
        int i = 0, j = 0, k = 0;
        while(buf[i++] != '.');
        while(buf[i] != '.') {
            tld[j++] = buf[i++];
        }
        tld[j++] = buf[i++];
        root[k++] = '.';
        while(buf[i] != ' ' && buf[i] != '\0') {
            tld[j++] = buf[i];
            root[k++] = buf[i];
            i++;
        }
        tld[j] = '\0';
        root[k] = '\0';

        root_socket = socket(AF_INET, SOCK_DGRAM, 0);
        root_addr.sin_family = AF_INET;
        root_addr.sin_port = htons(ROOT);
        root_addr.sin_addr.s_addr = INADDR_ANY;
        sentbytes = sendto(root_socket, root, strlen(root) + 1, 0, (struct sockaddr*)&root_addr, length);
        recvbytes = recvfrom(root_socket, root_ip, sizeof(root_ip), 0, NULL, NULL);
        close(root_socket);
        printf("Response from Root DNS Server: %s", root_ip);

        tld_socket = socket(AF_INET, SOCK_DGRAM, 0);
        tld_addr.sin_family = AF_INET;
        tld_addr.sin_port = htons(TLD);
        tld_addr.sin_addr.s_addr = INADDR_ANY;
        sentbytes = sendto(tld_socket, tld, strlen(tld) + 1, 0, (struct sockaddr*)&tld_addr, length);
        recvbytes = recvfrom(tld_socket, tld_ip, sizeof(tld_ip), 0, NULL, NULL);
        close(tld_socket);
        printf("Response from Top Level DNS Server: %s", tld_ip);

        auth_socket = socket(AF_INET, SOCK_DGRAM, 0);
        auth_addr.sin_family = AF_INET;
        auth_addr.sin_port = htons(AUTH);
        auth_addr.sin_addr.s_addr = INADDR_ANY;
        sentbytes = sendto(auth_socket, auth, strlen(auth) + 1, 0, (struct sockaddr*)&auth_addr, length);
        recvbytes = recvfrom(auth_socket, auth_ip, sizeof(auth_ip), 0, NULL, NULL);
        close(auth_socket);
        printf("Response from Authoritative Name Server: %s", auth_ip);

        sentbytes = sendto(server_socket, auth_ip, strlen(auth_ip) + 1, 0, (struct sockaddr*)&client_addr, length);
    }
	close(server_socket);
}
