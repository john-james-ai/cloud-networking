#ifndef ROUTER_H
#define ROUTER_H

#include "assert.h"
#include <string>
#include <vector>

#include "ControlPlane.h"
#include "DataPlane.h"

#include "Parameters.h"
#include "Pulse.h"
#include "NOC.h"

//---------------------------------------------------------------------------//
//                                   ROUTER                                  //
//---------------------------------------------------------------------------//
class Router {
    private:
        std::string classname_, message_, address_;
        short router_id_;
        Parameters* parameters_;
        ControlPlane* control_plane_;
        NOC* noc_;
        DataPlane* data_plane_;        
    public:
        Router();
        Router(Parameters* parameters, ControlPlane* control_plane, 
                NOC* noc, DataPlane* data_plane);
        short getRouterId();
        ControlPlane* getControlPlane();
        DataPlane* getDataPlane();
        void start();
        void startNOC();
        void startPulse();
        bool hey();
};

#endif