#ifndef PULSE_H
#define PULSE_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "Neighborhood.h"
#include "Messenger.h"
#include "Packet.h"
#include "Parameters.h"
#include "Routes.h"

class Messenger;
class Neighborhood;
class Route;
class Packet;
class PacketFactory;
struct Parameters;

class Pulse {
    private:
        short router_id_;
        std::string classname_, address_;
		int heart_rate_;
		Neighborhood* neighborhood_;        
        Messenger* messenger_;  
        Packet* packet_;
        PacketFactory* packet_factory_;
        std::vector<short> recipients_;
        
        Route* createRoute();      
    public:
        Pulse();
        Pulse(Parameters* parameters, Neighborhood* neighborhood, Messenger* messenger,
              PacketFactory* packet_factory);
        void start();
		std::string name();
        bool hey();     
};
#endif