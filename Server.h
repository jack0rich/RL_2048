//
// Created by Jack Rich on 2023/4/6.
//

#ifndef RL_2048_SERVER_H
#define RL_2048_SERVER_H
#include <iostream>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include "Board.h"

typedef int (*Functype)();

class Server {
private:
    int sockfd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;

public:
    bool isShakeWithPy = false;
    Server();
    void start(Functype right, Functype left, Functype up, Functype down, board::Board *core);
    void action(Functype motion, int clnt_sockfd, board::Board *core);


};


#endif //RL_2048_SERVER_H
