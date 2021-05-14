#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

struct Command {
    char* command;
    short node_id;
    short node_id;
    short dest_id;
    int cost;
    char* message;
};

class Manager {
    private:
        struct sockaddr_in source_addr_;
        short socket_;
        std::vector<Command> commands_;
        void connect();
        void disconnect();        
        void send(short node_id, short dest_id, char* message);
        void cost(short node_id, short node_id, int cost);
    public:
        Manager();
        Manager(std::vector<Command> commands);
        void run();
};
#endif
