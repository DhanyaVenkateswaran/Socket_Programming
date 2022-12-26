#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    printf("%s\n",data);
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("\n[-]Error in sending the e-mail!");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("\n[-]Socket Creation Unsuccessful!");
    exit(1);
  }
  printf("[+]Socket Creation Successful!\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("\n[-]Socket Connect Failed!");
    exit(1);
  }
 printf("\n[+]Socket Connect Successful!\n");

  char from[SIZE],to[SIZE],msg[SIZE];


  bzero(from , 1024);
  bzero(to , 1024);
  bzero(msg , 1024);

  printf("Enter from e-mail address : ");
  gets(from);
  printf("Enter to e-mail address : ");
  gets(to);
  printf("Enter the body of the mail : ");
  gets(msg);
write(sockfd , &from , sizeof(from));

write(sockfd , &to , sizeof(to));

write(sockfd , &msg , sizeof(msg));

  printf("\n[+]E-mail sent successfully!\n");

  printf("\n[+]Closing the connection!\n");
  close(sockfd);

  return 0;
}
