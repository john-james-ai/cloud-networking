#ifndef BUILDER_H
#define BUILDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// Parameters
#include "Parameters.h"
// Packets
#include "Packet.h"

// Foundation objects
#include "Constants.h"
#include "IO.h"
#include "utils.h"

// Data Plane
#include "Network.h"
#include "Socket.h"
#include "Messenger.h"
#include "Listener.h"
#include "DataPlane.h"

// Control Plane
#include "ControlPlane.h"

// Network Management
#include "NOC.h"
#include "Pulse.h"

// Route Information Base
#include "Neighborhood.h"
#include "Routes.h"
#include "FTable.h"

// Event Handlers
#include "EventHandlers.h"

#include "Router.h"

struct Parameters;
class Packet;
class PacketFactory;
class Logger;

// Data Plane
class Network;
class Socket;
class Messenger;
class Listener;
class DataPlane;

// Network Management
class NOC;
class Pulse;

// Control Plane
class ControlPlane;
class Neighbor;
class Neighborhood;
struct Route;
class Routes;
class FTable;

// Event Handlers

class EventHandler;
class DataPlaneHandler;
class RouteHandler;
class WithdrawHandler;
class FailureHandler;
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
        ControlPlane* control_plane_;
        DataPlane* data_plane_;
    public:
        Builder();
        Builder(Parameters* parameters);
  
        void resetRouter();
        void buildRouter();
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