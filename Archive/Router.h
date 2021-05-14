#ifndef ROUTER_H
#define ROUTER_H

#include "assert.h"
#include <string>
#include <vector>

#include "ControlPlane.h"
#include "DataPlane.h"

#include "Parameters.h"
#include "Pulse.h"
#include "EKG.h"

//---------------------------------------------------------------------------//
//                                   ROUTER                                  //
//---------------------------------------------------------------------------//
class Router {
    private:
        std::string classname_, message_, address_;
        short router_id_;
        Parameters* parameters_;
        ControlPlane* control_plane_;
        DataPlane* data_plane_;        
    public:
        Router();
        Router(Parameters* parameters, ControlPlane* control_plane, 
                DataPlane* data_plane);
        short getRouterId();
        ControlPlane* getControlPlane();
        DataPlane* getDataPlane();
        void startDataPlane();
        void startPulse();
        void startEKG();
        void startAdvertiser();
        
        bool hey();
};

#endif