#ifndef CONTROL_PLANE_H
#define CONTROL_PLANE_H

#include "assert.h"

#include "Advertiser.h"
#include "EKG.h"
#include "Pulse.h"
#include "RouteInfoBase.h"
#include "EventHandlers.h"
#include "Messenger.h"
#include "utils.h"

class Parameters;
class EventHandler;
struct NPack;
//===========================================================================//
//                               DATAPLANE                                   //
//===========================================================================//
struct NetworkManager {
    EKG* ekg_;
    Pulse* pulse_;
    Advertiser* advertiser_;
};

class ControlPlane {
    private:  
        std::string classname_, address_;
        RouteInfoBase* rib_;
        EKG* ekg_;
        Pulse* pulse_;
        Advertiser* advertiser_;
    
        

    public:
        ControlPlane();
        ControlPlane(RouteInfoBase* rib, NetworkManager* manager);         
        void startPulse();
        void startEKG();
        void startAdvertiser();
        bool hey();

};        
#endif
