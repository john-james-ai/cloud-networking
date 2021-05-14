#ifndef EVENT_HANDLERS_H
#define EVENT_HANDLERS_H

#include "assert.h"
#include <string>
#include <vector>

#include "Constants.h"
#include "RouteInfoBase.h"
#include "IO.h"
#include "Messenger.h"
#include "PacketFactory.h"
#include "Parameters.h"
#include "utils.h"

class RouteInfoBase;
class CPLogger;
class Packet;
struct Parameters;
class Messenger;

//===========================================================================//
//                          EVENT HANDLER BASE                                    //
//===========================================================================//
class  EventHandler {
    protected:
        short router_id_;
        std::string classname_, address_, message_;        

        RouteInfoBase* rib_;
        Packet* announcement_;
        Messenger* messenger_;
        CPLogger* log_;
        
    public:
        EventHandler();
        EventHandler(Parameters* parameters, RouteInfoBase* rib);        
        virtual void process(Packet* packet);                                
        bool hey();
        
};
//---------------------------------------------------------------------------//
//                               ROUTE                                       //
//---------------------------------------------------------------------------//
class RouteHandler : public  EventHandler {
    public:
        RouteHandler();
        RouteHandler(Parameters* parameters, RouteInfoBase* rib);
        void process(Packet* packet);
        
};
//---------------------------------------------------------------------------//
//                             WITHDRAW                                      //
//---------------------------------------------------------------------------//
class WithdrawHandler : public  EventHandler {
    public:        
        WithdrawHandler();
        WithdrawHandler(Parameters* parameters, RouteInfoBase* rib);
        void process(Packet* packet);
};
//---------------------------------------------------------------------------//
//                              FAILURE                                      //
//---------------------------------------------------------------------------//
class FailureHandler : public  EventHandler {
    public:        
        FailureHandler();
        FailureHandler(Parameters* parameters, RouteInfoBase* rib);
        void process(Packet* packet);
};
//---------------------------------------------------------------------------//
//                              ADVERTISE                                    //
//---------------------------------------------------------------------------//
class AdvertiseHandler : public  EventHandler {
    public:        
        AdvertiseHandler();
        AdvertiseHandler(Parameters* parameters, RouteInfoBase* rib);
        void process(Packet* packet);
};
//===========================================================================//
//                          EVENT MANAGER                                    //
//===========================================================================//
class   EventManager {
    private:
        short router_id_;
        std::vector< EventHandler*> handlers_;
        std::string classname_, address_;

    public:
          EventManager();
          EventManager(Parameters* parameters);
        void attach(EventHandler* handler);
        void notify(Packet* packet);
        bool hey();
};
#endif