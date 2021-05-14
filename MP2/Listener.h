#ifndef LISTENER_H
#define LISTENER_H

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>

#include "EventHandlers.h"
#include "Socket.h"
#include "Packet.h"
#include "Network.h"
#include "utils.h"

class Socket;
class EventManager;
class Network;
class Packet;
class PacketFactory;
struct Connection;
//===========================================================================//
//                               LISTENER                                    //
//===========================================================================//
class Listener {
    private:
        short router_id_;        
        std::string classname_, address_;
        Socket* socket_;
        EventManager* event_manager_;
        PacketFactory* packet_factory_;                    
        
        void listen();
    public:
        Listener();
        Listener(Parameters* parameters, Socket* socket, PacketFactory* packet_factory);        
        void setEventManager(EventManager* event_manager);          
        void start();
        bool hey();
        std::string name();
};
#endif
