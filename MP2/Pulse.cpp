#include "Pulse.h"

Pulse::Pulse() 
    : classname_("Pulse")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;
}
Pulse::Pulse(Parameters* parameters, Neighborhood* neighborhood, 
             Messenger* messenger, PacketFactory* packet_factory)
    : heart_rate_(parameters->heart_rate_), 
      neighborhood_(neighborhood), 
      messenger_(messenger),      
      packet_factory_(packet_factory),
      router_id_(parameters->router_id_),
      classname_("Pulse")
    {        
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;            
    }

Route* Pulse::createRoute() {
    iam(classname_, __FUNCTION__);

    Route* route = new Route();      
    route->router_id_ = router_id_;
    route->neighbor_id_ = router_id_;
    route->dest_id_ = router_id_;
    route->link_cost_ = 0;
    route->cost_ = 0;
    route->path_.clear();
    route->path_.push_back(router_id_);   
    route->active_ = true;    
    return route;     
}
void Pulse::start() {
    iam(classname_, __FUNCTION__);    

    // Designate recipients. This only needs to be done once.
    recipients_ = neighborhood_->getNeighborIds();

    // Create packet, this only needs to be done once. It is deleted when the 
    // process is killed.
    
     
    // Establish now and sleep. Heartbeat is in milliseconds.
    struct timespec sleep;
    
    if (heart_rate_ >= 1000) {
        sleep.tv_sec = heart_rate_ / 1000;
        sleep.tv_nsec = (heart_rate_ % 1000) * 1000 * 1000;         
    } else {
        sleep.tv_sec = 0;
        sleep.tv_nsec = heart_rate_ * 1000 * 1000;                 
    }   

    Route* route = createRoute();          
    Packet* packet = packet_factory_->createNeighborPacket(route, recipients_);
    while(1) {        
        
        messenger_->send(packet);  
        nanosleep(&sleep,0);        
        
    }
};
bool Pulse::hey() {
    
	holla(classname_, address_);
	return true;
};
std::string Pulse::name() {
    return classname_;
}