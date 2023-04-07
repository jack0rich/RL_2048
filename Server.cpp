//
// Created by Jack Rich on 2023/4/6.
//

#include "Server.h"
#include "Board.h"
#include <iostream>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
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

void Server::start(Functype right, Functype left, Functype up, Functype down, board::Board *core) {
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    std::cout << "new client fd " << clnt_sockfd  << "! IP: " << inet_ntoa(clnt_addr.sin_addr)  <<
    " Port: " << ntohs(clnt_addr.sin_port) << std::endl;
    this->isShakeWithPy = true;

    while (true) {
        char buf[1024];  // 定义缓冲区
        bzero(&buf, sizeof(buf));  // 清空缓冲区
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));  // 从客户端socket读到缓冲区，返回已读数据大小
        if (read_bytes > 0) {
            ::printf("message from client fd %d: %s\n", clnt_sockfd, buf);
//            write(clnt_sockfd, buf, sizeof(buf));  // 将相同的数据写到客户端
        std::string py_cmd(buf);
            if (py_cmd == "right") {
                action(right, clnt_sockfd, core);
            }
            else if (py_cmd == "left") {
                action(left, clnt_sockfd, core);
            }
            else if (py_cmd == "up") {
                action(up, clnt_sockfd, core);
            }
            else if (py_cmd == "down") {
                action(down, clnt_sockfd, core);
            }
        }
        else if (read_bytes == 0) {  // read返回0，表示EOF
            ::printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }
        else if (read_bytes == -1) {  // read返回-1，表示发生错误，按照上文方法进行错误处理
            close(clnt_sockfd);
            errif(true, "socket read error");
            break;
        }
    }
    close(sockfd);
}

void Server::action(Functype motion, int clnt_sockfd, board::Board *core) {
    int score = motion();
    board::square4 data = core->blocks_array;
    std::string str = std::to_string(score);
    std::string array_str = square4ToString(data);
    str = array_str + str;
    char *str_ptr = str.data();
    size_t str_size = str.size();
    write(clnt_sockfd, str_ptr, str_size);
}



