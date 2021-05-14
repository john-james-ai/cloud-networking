#ifndef EKG_H
#define EKG_H

#include <string>
#include <time.h>
#include <vector>

#include "EventHandlers.h"
#include "Parameters.h"

#include "Parameters.h"
#include "Constants.h"
#include "Neighborhood.h"
#include "PacketFactory.h"
#include "Routes.h"
#include "utils.h"

class EventHandler;
class Neighborhood;
class Packet;
class Parameters;
class Routes;


//===========================================================================//
//                                 EKG                                       //
//===========================================================================//
// Class responsible for monitoring and reporting failed and recovered network.
class EKG  {
    private:
        std::string classname_, address_, message_;
        short router_id_, sleep, ttl_ms_;

        std::vector<Neighbor*> failed_neighbors_;
        std::vector<Neighbor*> recipients_;
        
        Neighborhood* neighborhood_;        
        Routes* routes_;
        EventManager* manager_;
        
        void setSleep(timespec &sleep);
        
    public:
        EKG();
        EKG(Parameters* parameters, Neighborhood* neighborhood, Routes* routes, 
            EventManager* event_manager);                   
        void start();                        
        bool hey();        
};

#endif