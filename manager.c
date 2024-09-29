#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <strings.h>

#define PORT_MANAGER "8080" 
#define IP           "127.0.0.1"

void check_error(const int code, const char* func_name){
  if(code == -1){ 
    perror(func_name);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char** argv){
  int num, receive;
  struct addrinfo hints, *res;
  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int err = getaddrinfo(NULL, PORT_MANAGER, &hints, &res);

  if(err != 0){
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "socket()");
  
  int opt = 1;
  err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  check_error(err, "settings");

  err = bind(sockfd, res->ai_addr, res->ai_addrlen);
  check_error(err, "bind()");

  err = listen(sockfd, 1);
  check_error(err, "listen()");

  int clientfd = accept(sockfd, NULL, NULL);
  check_error(clientfd, "accept()");

  ssize_t bytes;

  bytes = recv(clientfd, &num, sizeof(num), 0);
  check_error((int)bytes, "recv()");
  
  printf("The result number in manager: %d\n", num);

  send(clientfd, &num, sizeof(num), 0);

  close(sockfd);
  close(clientfd);
  freeaddrinfo(res);
}
