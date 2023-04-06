//
// Created by Jack Rich on 2023/4/6.
//

#ifndef RL_2048_SERVER_H
#define RL_2048_SERVER_H
#include <iostream>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>

typedef void (*Functype)();

class Server {
private:
    int sockfd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;

public:
    Server();
    void start(Functype right, Functype left, Functype up, Functype down);
    void action(Functype motion);


};


#endif //RL_2048_SERVER_H
