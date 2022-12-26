#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define SIZE 1024

void write_file(int sockfd)
{

    char from[SIZE], to[SIZE], msg[1024];

    read(sockfd, from, sizeof(from));
    read(sockfd, to, sizeof(to));
    read(sockfd, msg, sizeof(msg));

    FILE *fp;
    strcat(to, ".txt");

    char *filename = to;
    char buffer[SIZE];
    printf("\nE-mail from the client : \n");
    fp = fopen(filename, "a");

    fprintf(fp, "\n");
    fprintf(fp, "%s", from);
    fprintf(fp, " ");

    fprintf(fp, "%s", msg);

    bzero(from, SIZE);
    bzero(to, SIZE);
    bzero(msg, 1024);

    return;
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("\n[-]Socket Creation Failed!");
        exit(1);
    }
    printf("\n[+]Socket creation successful!\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0)
    {
        perror("\n[-]Bind Unsuccessful!");
        exit(1);
    }
    printf("\n[+]Bind successfull!\n");

    if (listen(sockfd, 10) == 0)
    {
        printf("\n[+]Listening!\n");
    }
    else
    {
        perror("\n[-]Listen failed!");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    write_file(new_sock);
    printf("\n[+]E-mail written successfully!\n");

    return 0;
}
