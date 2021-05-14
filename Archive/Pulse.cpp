#include "Pulse.h"

Pulse::Pulse() 
    : classname_("Pulse")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;
}
Pulse::Pulse(Parameters* parameters, Neighborhood* neighborhood, Messenger* messenger)
    : heart_rate_(parameters->heart_rate_), 
      neighborhood_(neighborhood), 
      messenger_(messenger),      
      router_id_(parameters->router_id_),
      classname_("Pulse")
    {
        
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;    
        packets_.reserve(256);
        packets_.clear();    
    }

void Pulse::createPackets() {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*> recipients  = neighborhood_->getAllNeighbors();
    std::vector<Neighbor*>::iterator it;        
    
    Packet* packet = new Packet();   
    Route* route = new Route();      

    // Create announcement packet.
    route->router_id_ = router_id_;
    route->neighbor_id_ = router_id_;
    route->dest_id_ = router_id_;
    route->link_cost_ = 0;
    route->cost_ = 0;
    route->path_.push_back(router_id_);    

    for (it=recipients.begin();it!=recipients.end();++it) {
        packet->announceRoute(route);
        packet->checkOut(*it);
        packets_.push_back(packet);
        packet->print();
    }
}
void Pulse::start() {
    iam(classname_, __FUNCTION__);

    createPackets();
    std::vector<Packet*>::iterator it;
     
    // Establish now and sleep. Heartbeat is in milliseconds.
    struct timespec sleep;
    
    if (heart_rate_ >= 1000) {
        sleep.tv_sec = heart_rate_ / 1000;
        sleep.tv_nsec = (heart_rate_ % 1000) * 1000 * 1000;         
    } else {
        sleep.tv_sec = 0;
        sleep.tv_nsec = heart_rate_ * 1000 * 1000;                 
    }

    while(1) {                
        
        for (it=packets_.begin();it!=packets_.end();++it) {         
            (*it)->print();   
            messenger_->send(*it);
        }
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