#include "NOC.h"
//===========================================================================//
//                                 NOC                                       //
//===========================================================================//
NOC::NOC() 
    : classname_("NOC")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;
}
NOC::NOC(Parameters* parameters, ControlPlane* control_plane, PacketFactory* packet_factory) 
    : router_id_(parameters->router_id_), 
      ttl_ms_(parameters->ttl_factor_ * parameters->heart_rate_),   
      control_plane_(control_plane), packet_factory_(packet_factory),
      classname_("NOC")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;             
    }
//---------------------------------------------------------------------------//
//                               SET SLEEP                                   //
//---------------------------------------------------------------------------//
void NOC::setSleep(timespec &sleep){
    iam(classname_, __FUNCTION__);

    if (ttl_ms_ >= 1000) {
        sleep.tv_sec = ttl_ms_ / 1000;
        sleep.tv_nsec = (ttl_ms_ % 1000) * 1000 * 1000;         
    } else {
        sleep.tv_sec = 0;
        sleep.tv_nsec = ttl_ms_ * 1000 * 1000;                 
    }
}
//---------------------------------------------------------------------------//
//                                 START                                     //
//---------------------------------------------------------------------------//
void NOC::start() {
    iam(classname_, __FUNCTION__);
    
    struct timespec sleep;
    setSleep(sleep);

    while(1) {
        control_plane_->checkNetwork();
        nanosleep(&sleep,0);
    };
}
bool NOC::hey() {
    holla(classname_, address_);
    return true;
}