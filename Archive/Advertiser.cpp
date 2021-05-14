#include "Advertiser.h"

Advertiser::Advertiser() 
    : classname_("Advertiser")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;
}
Advertiser::Advertiser(Parameters* parameters, EventManager* manager)
    : ad_rate_(parameters->heart_rate_ * parameters->ad_factor_), 
      manager_(manager),
      router_id_(parameters->router_id_),
      classname_("Advertiser")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;        
    }

void Advertiser::start() {
    iam(classname_, __FUNCTION__);

    Packet* advertise = new Packet();
    advertise->header.packet_type_ = ADVERTISE_PACKET;
 
     // Establish now and sleep. Heartbeat is in milliseconds.
    struct timespec sleep;
    
    if (ad_rate_ >= 1000) {
        sleep.tv_sec = ad_rate_ / 1000;
        sleep.tv_nsec = (ad_rate_ % 1000) * 1000 * 1000;         
    } else {
        sleep.tv_sec = 0;
        sleep.tv_nsec = ad_rate_ * 1000 * 1000;                 
    }

    while(1) {

        manager_->notify(advertise);        
        nanosleep(&sleep,0);
    }
};
bool Advertiser::hey() {
	holla(classname_, address_);
	return true;
};