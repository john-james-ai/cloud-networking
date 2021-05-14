#include "RouteInfoBase.h"


//===========================================================================//
//                         ROUTE INFORMATION BASE                            //
//===========================================================================//
//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
RouteInfoBase::RouteInfoBase() 
    : classname_("RouteInfoBase")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;        
    }
    
RouteInfoBase::RouteInfoBase(Parameters* parameters, RIBPack* rib_pack, 
                             Messenger* messenger, 
                             CPLogger* logger) 
    :   router_id_(parameters->router_id_),       
        neighborhood_(rib_pack->neighborhood_),      
        ftable_(rib_pack->ftable_),        
        routes_(rib_pack->routes_),    
        log_(logger),    
        classname_("RouteInfoBase")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;
    }

//===========================================================================//
//                             EVENT MANAGEMENT                              //
//===========================================================================//
//---------------------------------------------------------------------------//
//                           REGISTER DIRECT ROUTE                           //
//---------------------------------------------------------------------------// 
void RouteInfoBase::registerDirectRoute(Packet* packet) {
    iam(classname_, __FUNCTION__);

    if (!packet->isDirectRoute()) {
        Packet* direct = new Packet();
        direct->announceDirectRoute(packet->route_);        
        processRoute(direct);
        delete direct;
    }    
}
//---------------------------------------------------------------------------//
//                              LOCALIZE ROUTE                               //
//---------------------------------------------------------------------------// 
void RouteInfoBase::localizeRoute(Packet* packet) {
    // Updates route id, prepends path and adds costs.
    iam(classname_, __FUNCTION__);

    short neighbor_id = packet->getSender();
    neighbor_ = neighborhood_->getNeighbor(neighbor_id);
    packet->checkIn(neighbor_);
}

//---------------------------------------------------------------------------//
//                                 CHECK IN                                  //
//---------------------------------------------------------------------------//  
void RouteInfoBase::checkIn(Packet* packet) {
    // Localizes and registers the direct route
    iam(classname_, __FUNCTION__);

    localizeRoute(packet);
    registerDirectRoute(packet);    
}

//---------------------------------------------------------------------------//
//                               PROCESS ROUTE                               //
//---------------------------------------------------------------------------// 
void RouteInfoBase::processRoute(Packet* packet) {   
    iam(classname_, __FUNCTION__);
    processRoute(packet->route_);
}
//---------------------------------------------------------------------------// 
void RouteInfoBase::processRoute(Route* route) {
    iam(classname_, __FUNCTION__);

    existing_best_route_ = routes_->getBestRoute(route->dest_id_);

    updateRouting(route);

    new_best_route_ = routes_->getBestRoute(route->dest_id_);

    updateForwarding(new_best_route_);
}
//---------------------------------------------------------------------------//
//                        PROCESS WITHDRAW ROUTE                             //
//---------------------------------------------------------------------------//   
void RouteInfoBase::processWithdraw(Packet* packet) {
    iam(classname_, __FUNCTION__);
    processWithdraw(packet->route_);
}
//---------------------------------------------------------------------------//   
void RouteInfoBase::processWithdraw(Route* route) {
    iam(classname_, __FUNCTION__);

    existing_best_route_ = routes_->getBestRoute(route->dest_id_);

    routes_->deleteRoute(route);

    new_best_route_ = routes_->getBestRoute(route->dest_id_);

    updateForwarding(new_best_route_);    
}
//---------------------------------------------------------------------------//
//                           PROCESS FAILURE                                 //
//---------------------------------------------------------------------------//
void RouteInfoBase::processFailure(Packet* packet) {
    iam(classname_, __FUNCTION__);
    processFailure(packet->route_);
}
//---------------------------------------------------------------------------//   
void RouteInfoBase::processFailure(Route* route) {
    iam(classname_, __FUNCTION__);

    std::vector<Route*>::iterator it;
    std::vector<Route*> invalid_routes_ = routes_->getInvalidRoutes(route->failed_link_);

    for (it=invalid_routes_.begin();it!=invalid_routes_.end();++it) {
        processWithdraw(*it);
    }
} 


//---------------------------------------------------------------------------//
//                            PROCESS ADVERTISE                              //
//---------------------------------------------------------------------------//   
void RouteInfoBase::processAdvertise(Packet* packet) {
    iam(classname_, __FUNCTION__);

    std::vector<Route*> routes = ftable_->getRoutes();
    std::vector<Route*>::iterator it;

    if (routes.size() > 0) {
        for (it=routes.begin();it!=routes.end();++it) {
            recipients_ = neighborhood_->getAllNeighbors();
            announcement_->announceRoute(*it);
            announce();
        }
    }
}

//===========================================================================//
//                             DATA MANAGEMENT                               //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              UPDATE NEIGHBOR                              //
//---------------------------------------------------------------------------//  
void RouteInfoBase::updateNeighbor(Route* route) {
    iam(classname_, __FUNCTION__);    

    int link_cost;

    if (neighborhood_->neighborExists(route->neighbor_id_)) {

        neighbor_ = neighborhood_->getNeighbor(route->neighbor_id_);
        
        if (!neighbor_->isActive()) {
            neighborhood_->bumpVersion(route->neighbor_id_);
        }
    } else {
        neighborhood_->createNeighbor(route->neighbor_id_, 
                                      route->link_cost_);           
    }    
    neighborhood_->activateNeighbor(route->neighbor_id_);
    neighborhood_->setLastHeartbeat(route->neighbor_id_);
    neighborhood_->connectNeighbor(route->neighbor_id_);    
}
//---------------------------------------------------------------------------//
//                              UPDATE ROUTING                               //
//---------------------------------------------------------------------------//  
void RouteInfoBase::updateRouting(Route* route) {
    iam(classname_, __FUNCTION__);    

    if (routes_->equalRouteExists(route)) {
        ; // do nothing
    } else if (routes_->altRouteExists(route)) {
        routes_->deleteRoute(route);
        routes_->addRoute(route);
        log_->processDeleteRoute(route);        
    } else {
        routes_->addRoute(route);
        log_->processDeleteRoute(route);                
    }
}
//---------------------------------------------------------------------------//
//                         UPDATE FORWARDING TABLE                           //
//---------------------------------------------------------------------------// 
void RouteInfoBase::updateForwarding(Route* route) {
    iam(classname_, __FUNCTION__);

    if (ftable_->equalRouteExists(route)) {
        ; // do nothing
    } else if (ftable_->altRouteExists(route)) {
        Route* alt_route = ftable_->getRoute(route->dest_id_);
        ftable_->deleteRoute(route->dest_id_);
        announceWithdraw(alt_route);

        ftable_->addRoute(route);
        announceRoute(route);
        log_->processForwarding(route);

    } else {
        ftable_->addRoute(route);
        announceRoute(route);        
        log_->processForwarding(route);
    }
}
//===========================================================================//
//                              ANNOUNCEMENTS                                //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              ANNOUNCEMENT                                 //
//---------------------------------------------------------------------------//   
void RouteInfoBase::announce() {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    for (it=recipients_.begin();it!=recipients_.end();++it) {
        announcement_->checkOut(*it);
        messenger_->send(announcement_);
    }    
}

//---------------------------------------------------------------------------//
//                              ANNOUNCE ROUTE                               //
//---------------------------------------------------------------------------//   
void RouteInfoBase::announceRoute(Route* route) {
    iam(classname_, __FUNCTION__);
    
    recipients_ = neighborhood_->getRecipients(route->path_);
    announcement_->announceRoute(route);
    announce();
                                         
} 
//---------------------------------------------------------------------------//
//                         ANNOUNCE WITHDRAW ROUTE                           //
//---------------------------------------------------------------------------//   
void RouteInfoBase::announceWithdraw(Route* route) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*> recipients = neighborhood_->getAllNeighbors();
    announcement_->announceWithdraw(route);
    announce();
} 

//---------------------------------------------------------------------------//
//                                   HEY                                     //
//---------------------------------------------------------------------------//
bool RouteInfoBase::hey() {
    holla(classname_, address_);
    
    assert((*neighborhood_).hey());
    assert((*routes_).hey());
    
    return true;
}