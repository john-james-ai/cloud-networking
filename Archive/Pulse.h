#ifndef PULSE_H
#define PULSE_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "Neighborhood.h"
#include "Messenger.h"
#include "PacketFactory.h"
#include "Parameters.h"
#include "Routes.h"

class Messenger;
class Neighborhood;
class Neighbor;
class Route;
class Packet;
struct Parameters;
struct Link;

class Pulse {
    private:
        short router_id_;
        std::string classname_, address_;
		short heart_rate_;
		Neighborhood* neighborhood_;        
        Messenger* messenger_;  
        std::vector<Packet*> packets_;
        
        void createPackets();      
    public:
        Pulse();
        Pulse(Parameters* parameters, Neighborhood* neighborhood, Messenger* messenger);
        void start();
		std::string name();
        bool hey();     
};
#endif