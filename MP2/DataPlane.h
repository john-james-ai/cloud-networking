#ifndef DATA_PLANE_H
#define DATA_PLANE_H

#include "assert.h"
#include "IO.h"
#include "Packet.h"
#include "Parameters.h"
#include "NOC.h"
#include "Listener.h"
#include "Messenger.h"
#include "Pulse.h"
#include "utils.h"

class NOC;
class Logger;
class Listener;
class Messenger;
class Parameters;
class Packet;
class PacketFactory;
class Pulse;

//===========================================================================//
//                               DATAPLANE                                   //
//===========================================================================//
class DataPlane {
    private:  
        std::string classname_, address_;
        Parameters* parameters_;
        PacketFactory* packet_factory_;        
        Pulse* pulse_;
        Listener* listener_;
        Messenger* messenger_;
        Logger* logger_;

    public:
        DataPlane();
        DataPlane(Parameters* parameters, Pulse* pulse,  
                  Listener* listener, 
                  Messenger* messenger, Logger* logger);    
        void setEventManager(EventManager* event_manager);    
        void start();
        void startPulse();
        void send(Packet* packet);        
        void forward(Packet* packet);
        void announce(Packet* packet);
        void receive(Packet* packet);
        void drop(Packet* packet);
        bool hey();

};        
#endif
