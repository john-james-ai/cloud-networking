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
#include "PacketFactory.h"
#include "Network.h"
#include "Forwarders.h"
#include "utils.h"

class Socket;
class Forwarders;
class EventManager;
class Network;
class Packet;
struct Connection;
//===========================================================================//
//                               LISTENER                                    //
//===========================================================================//
class Listener {
    private:
        short router_id_;        
        std::string classname_, address_;
        Socket* socket_;
        Forwarders* forwarders_;
        EventManager* manager_;
                    
        
        void listen();
    public:
        Listener();
        Listener(Parameters* parameters, Socket* socket);
        void setForwarders(Forwarders* forwarders);
        void setEventManager(EventManager* event_manager);            
        void start();
        bool hey();
        std::string name();
};
#endif
