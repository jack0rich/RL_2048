//
// Created by Jack Rich on 2023/4/6.
//

#include "Server.h"
#include "Board.h"
#include <iostream>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <strings.h>
#include <cstdio>
#include <cstdlib>


void errif(bool condition, const char *errmsg) {
    if (condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

std::string square4ToString(board::square4 data) {
    std::string str;
    for (const auto& row : data) {
        for (const auto& elem : row) {
            str += std::to_string(elem) + ",";
        }
        str += "|"; // 每一行结束后，添加一个换行符
    }
    return str;
}


Server::Server() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket creat error");

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8080);

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");
    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
}
