#ifndef EVENT_HANDLERS_H
#define EVENT_HANDLERS_H

#include "assert.h"
#include <string>
#include <vector>

#include "Constants.h"
#include "ControlPlane.h"
#include "DataPlane.h"
#include "Packet.h"
#include "Parameters.h"
#include "utils.h"

class DataPlane;
class ControlPlane;
class Packet;
struct Parameters;

//===========================================================================//
//                          EVENT HANDLER BASE                                    //
//===========================================================================//
class EventHandler {
    protected:
        std::string classname_, address_, message_;        

        DataPlane* data_plane_;
        ControlPlane* control_plane_;
        Packet* announcement_;
        
    public:
        short router_id_;
        EventHandler();
        EventHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane);        
        virtual void process(Packet* packet);                                
        bool hey();
        
};
//---------------------------------------------------------------------------//
//                                 SEND                                      //
//---------------------------------------------------------------------------//
class DataPlaneHandler : public  EventHandler {
    public:
        DataPlaneHandler();
        DataPlaneHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane);
        void process(Packet* packet);
        
};
//---------------------------------------------------------------------------//
//                             ROUTE PACKET                                  //
//---------------------------------------------------------------------------//
class RouteHandler : public  EventHandler {
    public:
        RouteHandler();
        RouteHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane);
        void process(Packet* packet);
        
};
//---------------------------------------------------------------------------//
//                         NEIGHBOR HANDLER                                  //
//---------------------------------------------------------------------------//
class NeighborHandler : public  EventHandler {
    public:
        NeighborHandler();
        NeighborHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane);
        void process(Packet* packet);
        
};
//---------------------------------------------------------------------------//
//                           WITHDRAW HANDLER                                //
//---------------------------------------------------------------------------//
class WithdrawHandler : public  EventHandler {
    public:        
        WithdrawHandler();
        WithdrawHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane);
        void process(Packet* packet);
};
//---------------------------------------------------------------------------//
//                       ANNOUNCE ROUTE HANDLER                              //
//---------------------------------------------------------------------------//
class AnnouncementHandler : public  EventHandler {
    public:
        AnnouncementHandler();
        AnnouncementHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane);
        void process(Packet* packet);
        
};
//===========================================================================//
//                          EVENT MANAGER                                    //
//===========================================================================//
class EventManager {
    private:
        short router_id_;
        std::vector<EventHandler*> handlers_;
        std::string classname_, address_;

    public:
          EventManager();
          EventManager(Parameters* parameters);
        void attach(EventHandler* handler);
        void notify(Packet* packet);
        bool hey();
};
#endif