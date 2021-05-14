#ifndef CONTROL_PLANE_H
#define CONTROL_PLANE_H

#include "assert.h"

#include "EventHandlers.h"
#include "Routes.h"
#include "Neighborhood.h"
#include "FTable.h"
#include "IO.h"
#include "NOC.h"
#include "Packet.h"
#include "utils.h"

class Parameters;
class Neighborhood;
class Routes;
class FTable;
class EventHandler;
class EventManager;
class Packet;
class PacketFactory;
class NOC;
class Logger;
//===========================================================================//
//                            CONTROL PLANE                                  //
//===========================================================================//
class ControlPlane {
    protected:  
        // Id
        std::string classname_, address_, message_; 
      
        // logger
        Logger* logger_;
        
        // Control Plane Objects
        Routes* routes_;
        Neighborhood* neighborhood_;
        
        // EventManager
        EventManager* event_manager_;

        // Packet Factory
        PacketFactory* packet_factory_;

        // Inbound Packet Header Information
        int packet_sender_;
        std::string packet_type_;
        
        // Objects for internal use 
        std::vector<Neighbor*> recipients_;
        std::vector<short> recipient_ids_;
        Neighbor* neighbor_;        
        Packet* announcement_;
        FTable* ftable_;
        
        Route* existing_best_route_;
        Route* new_best_route_;        

        std::vector<Route*> invalid_routes_;

    public:
        short router_id_;
        
        ControlPlane();
        ControlPlane(Parameters* parameters, Routes* routes, 
                     Neighborhood* neighborhood, FTable* ftable,
                     Logger* logger, 
                     PacketFactory* packet_factory); 

        void start();
        
        // Send Event Processing
        short getNextHop(Packet* packet);        

        // Neighbor processing
        void neighborCheckIn(short neighbor_id);
        void neighborCheckOut(short neighbor_id);

        // Route event processing        
        void configRoute(Packet* packet);
        Route* buildRoute(Packet* packet);
        bool isBetterRoute(Route* a, Route* b);
        void processRoute(Route* route);

        // Neighbor Event Processing
        void configNeighbor(Packet* packet);
        Route* buildNeighborRoute(Packet* packet);

        // Withdraw event processing
        void configWithdraw(Packet* packet);         
        void replaceWithdraw(Packet* packet);
        void propogateWithdraw(Packet* packet);

        // Announcements
        void announceRoute(Route* route);        
        void announceRoute(Packet* packet);  
        void announceWithdraw(Route* route); 
        
        // Advertise (Multiple Routes to New Neighbor)
        void advertise(short neighbor_id);        

        // Network Monitoring
        void checkNetwork();
        std::set<short> processFailedRoutes(std::vector<Route*> failed_routes);
        void findBestRoutes(std::set<short> destinations);

        // Install the event handler
        void setEventManager(EventManager* event_manager);

        bool hey();        
};        
#endif
