#ifndef ADVERTISER_H
#define ADVERTISER_H

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "EventHandlers.h"
#include "PacketFactory.h"
#include "Parameters.h"

class Packet;
struct Parameters;
class EventManager;

class Advertiser {
    private:
        short router_id_;
        std::string classname_, address_;
		short ad_rate_;        
		EventManager* manager_;
        
    public:
        Advertiser();
        Advertiser(Parameters* parameters, EventManager* manager);
        void start();
        bool hey();     
};
#endif