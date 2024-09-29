#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <netdb.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>

#define PORT_MANAGER "8080" 
#define PORT_WORKER0 "8081"
#define PORT_WORKER1 "8082"
#define PORT_WORKER2 "8083"
#define PORT_WORKER3 "8084"
#define PORT_WORKER4 "8085"
#define PORT_WORKER5 "8086"
#define PORT_WORKER6 "8087"
#define PORT_WORKER7 "8088"
#define IP           "127.0.0.1"

typedef struct payload{
  int id;
  int num;
} payload_t;


typedef struct worker{
  payload_t payload;
  bool ready;
  int curr_stage;
  int is_receiver;
} worker_t;


void check_error(const int code, const char* func_name){
  if(code == -1){
    perror(func_name);
    exit(EXIT_FAILURE);
  }
}

char* fetch_ports(int id){
  switch(id){
    case 0:
      return PORT_WORKER0;
    case 1:
      return PORT_WORKER1;
    case 2:
      return PORT_WORKER2;
    case 3:
      return PORT_WORKER3;
    case 4:
      return PORT_WORKER4;
    case 5:
      return PORT_WORKER5;
    case 6:
      return PORT_WORKER6;
    case 7:
      return PORT_WORKER7;
    default:
        fprintf(stderr, "Invalid number (must be within 0 and 7).\n");
        exit(EXIT_FAILURE);
  }
}


void sender(const char* port, worker_t* worker){
  ssize_t bytes;
  struct addrinfo address, *res;
  memset(&address, 0, sizeof(address));
  address.ai_family = AF_UNSPEC;
  address.ai_socktype = SOCK_STREAM;

  int err = getaddrinfo(IP, port, &address, &res);

  if(err != 0){
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "Erro em socket()");

   do
  {
      err = connect(sockfd, res->ai_addr, res->ai_addrlen);
      if (err == -1)
      {
        perror("Error in connection\n");
        sleep(1);
      }
      else{
        break;
      }
  } while(err != -1);

  bytes = send(sockfd, worker, sizeof(worker_t), 0);
  check_error((int)bytes, "send()");
  ssize_t aux;
  aux = recv(sockfd, worker, sizeof(worker_t), 0);
  check_error(aux, "recv()");

  if(worker->ready == false){
    close(sockfd);
    freeaddrinfo(res);
  }else{
    printf("Number after reduce: %d\n", worker->payload.num);
    close(sockfd);
    freeaddrinfo(res);
  }
}

void receiver(const char* port, worker_t* receiver){
  int opt = 1;
  ssize_t bytes;
  worker_t sender;
  struct addrinfo address, *res;
  memset(&address, 0, sizeof(address));
  address.ai_family = AF_UNSPEC;
  address.ai_socktype = SOCK_STREAM;
  address.ai_flags = AI_PASSIVE;
  
  int err = getaddrinfo(NULL, port, &address, &res);

  if(err != 0){
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "socket()");

  err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  check_error(err, "settings");

  err = bind(sockfd, res->ai_addr, res->ai_addrlen);
  check_error(err, "bind()");

  err = listen(sockfd, 8);
  check_error(err, "listen()");

  int clientfd = accept(sockfd, NULL, NULL);
  check_error(clientfd, "accept()");

  bytes = recv(clientfd, &sender, sizeof(worker_t), 0);
  check_error(bytes, "recv()");


  if(receiver->curr_stage == sender.curr_stage){
    receiver->ready = true;
    receiver->is_receiver = 1;
    receiver->payload.num += sender.payload.num;
    printf("Sum's result in stage %d: %d\n", receiver->curr_stage, receiver->payload.num);
    send(clientfd, receiver, sizeof(worker_t), 0);
  }
  else{
    printf("Payload not accepted!! Wrong Stage\n");
    receiver->is_receiver = 0;
    sender.ready = false;
    send(clientfd, &sender, sizeof(worker_t), 0);
  }

  close(clientfd);
  close(sockfd);
  freeaddrinfo(res);
}



void send_results(int result){
  struct addrinfo address, *res;
  memset(&address, 0, sizeof(address));
  address.ai_family = AF_UNSPEC;
  address.ai_socktype = SOCK_STREAM;
  address.ai_protocol = IPPROTO_TCP;
  
  int err = getaddrinfo(IP, PORT_MANAGER, &address, &res);

  if(err != 0){
    fprintf(stderr, "getaddrinfo: %s", gai_strerror(err));
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  check_error(sockfd, "socket()");

  do
  {
      err = connect(sockfd, res->ai_addr, res->ai_addrlen);
      if (err != -1)
      {
        perror("Error in connection\n");
        sleep(1);
      }
      else{
        break;
      }
  } while(err != -1);

  ssize_t num = send(sockfd, &result, sizeof(result), 0);
  if(num < 0){
    perror("error at sending result!!\n");
    exit(EXIT_FAILURE);
  }

  ssize_t result_num = recv(sockfd, &result, sizeof(result), 0);
  check_error(result_num, "recv()");
  close(sockfd);
  freeaddrinfo(res);
}



int main(int argc, char** argv){
  worker_t worker;
  worker.payload.id = atoi(argv[1]);
  char* port_worker = strdup(fetch_ports(worker.payload.id));
  int count = 1;
  worker.curr_stage = 0;
  worker.ready = 0;

  srand(time(NULL));

  if (argc != 2){
    fprintf(stderr, "Must specify worker id %s", argv[0]);
    exit(EXIT_FAILURE);
  }

  worker.payload.num = rand() % 100;
  printf("Worker %d has the number %d\n", worker.payload.id, worker.payload.num);

  for(int i = 0; i < 3; i++){
    if((worker.payload.id / count) % 2 == 0){
      worker.is_receiver = 1;
      receiver(port_worker, &worker);
      printf("Receiver has sent number %d\n", worker.payload.num);
      while(!worker.is_receiver)
        receiver(port_worker, &worker);
      worker.curr_stage++;
    }
    else{
      printf("Sender has sent number %d\n", worker.payload.num);
      int neighbor = worker.payload.id - count;
      char* port_neighbor;
      port_neighbor = strdup(fetch_ports(neighbor));
      sender(port_neighbor, &worker);
      while(worker.ready == false){
        printf("Client waiting to be ready!!\n");
        sleep(1);
        sender(port_neighbor, &worker);
      }
      break;
    }
    count *= 2;
  }    
  
  if(worker.payload.id == 0 && worker.curr_stage == 2){
    send_results(worker.payload.num);
  }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
}