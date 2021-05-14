#ifndef ROUTE_INFO_BASE_H
#define ROUTE_INFO_BASE_H

#include <string>
#include <vector>

#include "IO.h"
#include "Messenger.h"
#include "Neighborhood.h"
#include "PacketFactory.h"
#include "Routes.h"
#include "FTable.h"

class Costs;
class CPLogger;
class Neighborhood;
class Packet;
class FTable;
//===========================================================================//
//                    ROUTE INFORMATION BASE                                 //
//===========================================================================//
struct RIBPack {
    Neighborhood* neighborhood_;
    Routes* routes_;
    FTable* ftable_;
};

class RouteInfoBase {
    private:
        std::string classname_, address_, message_;
        short router_id_;
        Neighborhood* neighborhood_;        
        Routes* routes_;
        FTable* ftable_;
        Messenger* messenger_;        
        CPLogger* log_;

        std::vector<Neighbor*> recipients_;    
        std::vector<Neighbor*> neighbors_;
        Neighbor* neighbor_;
        Packet* announcement_;
        Route* existing_best_route_;
        Route* new_best_route_;
        std::vector<Route*> invalid_routes_;

    public: 
        RouteInfoBase();
        RouteInfoBase(Parameters* parameters, RIBPack* rib_pack, 
                      Messenger* messenger, 
                      CPLogger* logger); 


        // Registers the sending neighbor 
        void registerDirectRoute(Packet* packet);
        void localizeRoute(Packet* packet);
        void checkIn(Packet* packet);                       

        // Forwarding Table
        short getNextHop(short dest_id);        

        // Core event processing methods
        void processRoute(Route* route);
        void processRoute(Packet* packet);
        void processDirectRoute(Route* route);         
        void processDirectRoute(Packet* packet);         
        void processWithdraw(Route* route);                    
        void processWithdraw(Packet* packet);    
        void processFailure(Route* route);                    
        void processFailure(Packet* packet);                            
        void processAdvertise(Packet* packet);        

        // Event Announcement 
        void announce();
        void announceRoute(Route* route);
        void announceWithdraw(Route* route);

        // Helper methods used by the above process methods        
        void updateNeighbor(Route* route);        
        void updateRouting(Route* route);
        void updateForwarding(Route* route);

        bool hey();


};
#endif