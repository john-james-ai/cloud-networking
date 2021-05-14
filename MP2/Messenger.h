#ifndef MESSENGER_H
#define MESSENGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

#include "Network.h"
#include "utils.h"

class Packet;
class Network;
class Socket;
//===========================================================================//
//                               MESSENGER                                   //
//===========================================================================//
class Messenger {
    private:        
        std::string classname_, address_;
        short router_id_;
        int global_UDP_socket_;        
        Socket* socket_;
        Network* network_;
        
    public:
        Messenger();
        Messenger(Parameters* parameters, Network* network, Socket* socket);
        void send(Packet* packet);
        bool hey();
        std::string name();
};
#endif
