#include "EKG.h"

//===========================================================================//
//                                 EKG                                       //
//===========================================================================//
EKG::EKG() 
    : classname_("EKG")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;
}
EKG::EKG(Parameters* parameters, Neighborhood* neighborhood, Routes* routes,
            EventManager* event_manager) 
    : router_id_(parameters->router_id_), 
      ttl_ms_(parameters->ttl_factor_ * parameters->heart_rate_),   
      neighborhood_(neighborhood), 
      routes_(routes),
      manager_(event_manager),
      classname_("EKG")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;    
        failed_neighbors_.reserve(N_NODES);  
        recipients_.reserve(N_NODES);  
    }

void EKG::setSleep(timespec &sleep){
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
void EKG::start() {
    iam(classname_, __FUNCTION__);
    
    int link_cost;
    short neighbor_id;
    std::vector<Neighbor*>::iterator it;
    std::vector<Neighbor*>::iterator rit;
    Packet* notification = new Packet();   
    Route* route = new Route();
    Link link = Link();
    struct timespec sleep;

    setSleep(sleep);

    while(1) {
        failed_neighbors_ = neighborhood_->getDownNeighbors();     

        if (failed_neighbors_.size()>0) {
            for (it=failed_neighbors_.begin();it!=failed_neighbors_.end();++it) {   

                route = routes_->getRoute((*it)->getNeighborId(),(*it)->getNeighborId());
                route->setFailedLink(route->links_.front());
                notification->announceFailure(route);
                manager_->notify(notification);

            };
            failed_neighbors_.clear(); 
            notification->initialize();            
        }
        nanosleep(&sleep,0);
    }
    delete notification;
    delete route;
    delete link;
}
bool EKG::hey() {
    holla(classname_, address_);
    return true;
}