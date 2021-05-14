#ifndef BUILDER_H
#define BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Parameters
#include "Parameters.h"
// Bundles

// Foundation objects
#include "Constants.h"
#include "IO.h"
#include "utils.h"
#include "PacketFactory.h"

// Data Plane
#include "Network.h"
#include "Socket.h"
#include "Messenger.h"
#include "Listener.h"
#include "Forwarders.h"
#include "DataPlane.h"

// Control Plane
#include "ControlPlane.h"

// Network Management
#include "EKG.h"
#include "Pulse.h"
#include "Advertiser.h"

// Route Information Base
#include "Neighborhood.h"
#include "Routes.h"
#include "FTable.h"
#include "RouteInfoBase.h"

// Event Handlers
#include "EventHandlers.h"

#include "Router.h"

struct Parameters;
class Packet;
class CPLogger;
class DPLogger;

// Data Plane
class Network;
class Socket;
class Messenger;
class Listener;
class Forwarders;
class DataPlane;

// Control Plane
class ControlPlane;
// Network Management
class EKG;
class Pulse;
class Advertise;

// Route Information Base
class Cost;
class Costs;
struct Link;
class Neighbor;
class Neighborhood;
struct  Route;
class Routes;
class FTable;
class RouteInfoBase;
// Route Information Base Event Handlers
class AddRoute;
class DropRoute;
class DiscoverNeighbor;
class DeactivateNeighbor;
class EventHandler;
class EventManager;

class Router;

//---------------------------------------------------------------------------//
//                                   BUILDER                                 //
//---------------------------------------------------------------------------//
class Builder {

    private:                
        std::string classname_, address_;
        Parameters* parameters_;
        Router* router_;
    public:
        Builder();
        Builder(Parameters* parameters);
  
        void resetRouter();
        void buildRouter();
        void loadRouter();
        Router* getRouter();
};
//---------------------------------------------------------------------------//
//                             DIRECTOR                                      //
//---------------------------------------------------------------------------//
class Director {
    private:        
        Builder builder_;        
        std::string classname_, address_;
    public:
        Director();
        Director(Builder& builder);
        Router* buildRouter();
};
#endif