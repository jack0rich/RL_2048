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
void errif(bool condition, const char *errmsg);
std::string square4ToString(board::square4 data);

class Server {
public:
    int sockfd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;
    bool isShakeWithPy = false;
    Server();

};


#endif //RL_2048_SERVER_H