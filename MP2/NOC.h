#ifndef NOC_H
#define NOC_H

#include <string>
#include <time.h>
#include <vector>

#include "Parameters.h"
#include "Packet.h"
#include "Constants.h"
#include "ControlPlane.h"
#include "utils.h"

class ControlPlane;
class Packet;
class PacketFactory;
class Parameters;

//===========================================================================//
//                                 NOC                                       //
//===========================================================================//
// Class responsible for monitoring and reporting failed and recovered network.
class NOC  {
    private:
        std::string classname_, address_, message_;
        short router_id_, sleep, ttl_ms_;

        ControlPlane* control_plane_;        
        PacketFactory* packet_factory_; 
        
        void setSleep(timespec &sleep);
        
    public:
        NOC();
        NOC(Parameters* parameters, ControlPlane* control_plane,
            PacketFactory* packet_factory);               
        void start();                        
        bool hey();        
};

#endif