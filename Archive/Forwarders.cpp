#include "Forwarders.h"
//===========================================================================//
//                                 FORWARDER                                 //
//===========================================================================//
Forwarder::Forwarder()
    : classname_("Forwarder")
      {
          iam(classname_, __FUNCTION__);
          std::string* address_ = (std::string*)this;
      }
Forwarder::Forwarder(Parameters* parameters, FTable* ftable, 
                     Messenger* messenger, DPLogger* logger)
    : router_id_(parameters->router_id_),
      ftable_(ftable), 
      messenger_(messenger),
      log_(logger),
      classname_("Forwarder")
      {
          iam(classname_, __FUNCTION__);
          std::string* address_ = (std::string*)this;
      } 

void Forwarder::process(Packet* packet) {
    iam(classname_, __FUNCTION__);

    if (packet->header.packet_type_ == SEND_PACKET_TYPE || 
        packet->header.packet_type_ == FORWARD_PACKET_TYPE) {     

        if (packet->route_->dest_id_ == router_id_) {
            log_->receive(packet);
        } else {
            short next_hop = ftable_->getRoute(packet->route_->dest_id_)->neighbor_id_;               
            if (next_hop < 0) {
                log_->drop(packet);
            } else {
                
                if (packet->header.packet_type_ == SEND_PACKET_TYPE) {
                    log_->send(packet);
                } else {
                    log_->forward(packet);
                }
                
                packet->header.packet_type_ = FORWARD_PACKET_TYPE;
                packet->header.recipients_.clear();     
                packet->header.recipients_.push_back(next_hop);
                messenger_->send(packet);                
            }
        }
    }
};
bool Forwarder::hey() {
	holla(classname_, address_);
	return true;
};    
std::string Forwarder::name() {
    return classname_;
};


//===========================================================================//
//                                 FORWARDERS                                //
//===========================================================================//

Forwarders::Forwarders() 
    : classname_("Forwarders")
    {}

void Forwarders::attach(Forwarder* forwarder) {
    iam(classname_, __FUNCTION__);
    
    forwarders_.push_back(forwarder);
};

void Forwarders::notify(Packet* packet) {
    std::vector<Forwarder*>::iterator it;
    
    for (it=forwarders_.begin();it != forwarders_.end(); ++it) {
        (*it)->process(packet);
    }        
};

bool Forwarders::hey() {

    holla(classname_, address_);

    std::vector<Forwarder*>::iterator it;
    for (it = forwarders_.begin();it != forwarders_.end();++it) {
        assert((*it)->hey());
    }
    return true;
}

std::string Forwarders::name() {
    return classname_;
};


