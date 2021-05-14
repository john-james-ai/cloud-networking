#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"
//===========================================================================//
//                                  SOCKET                                   //
//===========================================================================//
class Socket {
    private:        
        std::string classname_, address_;
        short router_id_;
        short global_UDP_socket_;

        char my_ip_addr_[100];
        struct sockaddr_in socket_addr_;

    public:
        Socket();
        Socket(short router_id);  
        void openSocket();
        void closeSocket();   
        short getSocket();     
        void print();
        bool hey();
        std::string name();
};
#endif
