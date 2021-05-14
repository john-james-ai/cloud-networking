#include "ControlPlane.h"
//===========================================================================//
//                               CONTROL PLANE                               //
//===========================================================================//
//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
ControlPlane::ControlPlane() 
	: classname_("ControlPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
//---------------------------------------------------------------------------//
ControlPlane::ControlPlane(Parameters* parameters, Routes* routes, 
                     	   Neighborhood* neighborhood, FTable* ftable,
							Logger* logger, 
							PacketFactory* packet_factory)
	: router_id_(parameters->router_id_), 
      routes_(routes), neighborhood_(neighborhood), ftable_(ftable),
	  logger_(logger), packet_factory_(packet_factory),
	  classname_("ControlPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
//===========================================================================//
//                         SEND EVENT PROCESSING                             //
//===========================================================================//
//---------------------------------------------------------------------------//
//                            GET NEXT HOP                                   //
//---------------------------------------------------------------------------//
short ControlPlane::getNextHop(Packet* packet) {
    // Returns next hop for a destination, ie. the neighbor 
    iam(classname_, __FUNCTION__);

    return ftable_->getNextHop(packet->dest_id_);	
}
//---------------------------------------------------------------------------//
//                            NEIGHBOR CHECK IN                              //
//---------------------------------------------------------------------------//
void ControlPlane::neighborCheckIn(short neighbor_id) {
	// Sets last heart beat and activates neighbor.
	iam(classname_, __FUNCTION__);	

	neighborhood_->hearFromNeighbor(neighbor_id); 
}
//---------------------------------------------------------------------------//
//                            NEIGHBOR CHECK OUT                             //
//---------------------------------------------------------------------------//
void ControlPlane::neighborCheckOut(short neighbor_id) {
	// Advertise routes to discovered (initial and recovery) neighbors. 
	iam(classname_, __FUNCTION__);		

	if (!neighborhood_->isDiscovered(neighbor_id)) {
		logger_->discoverNeighbor(neighbor_id, __FUNCTION__);
		advertise(neighbor_id);
		neighborhood_->discover(neighbor_id);
	}

}
//===========================================================================//
//                                CONFIG                                     //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              CONFIG ROUTE                                 //
//---------------------------------------------------------------------------//
void ControlPlane::configRoute(Packet* packet) {
    // Processes a route announcement from a neighbor
    iam(classname_, __FUNCTION__);	

	std::string message;	

	neighborCheckIn(packet->header->sender_);

	if (packet->route_->notOnPath(router_id_)) {

		// Build the route from route information in the packet.
		Route* proposed_route = buildRoute(packet);	

		// update routing table
		processRoute(proposed_route);
	}

	neighborCheckOut(packet->header->sender_);	
};
//---------------------------------------------------------------------------//
//                               BUILD ROUTE                                 //
//---------------------------------------------------------------------------//
Route* ControlPlane::buildRoute(Packet* packet) {
	// Builds route based upon route embedded in packet object.
	iam(classname_, __FUNCTION__);
	
	int cost, link_cost;

	link_cost = neighborhood_->getCost(packet->header->sender_);	
	cost = link_cost + packet->route_->cost_;
	Route* proposed_route = routes_->createRoute(packet->header->sender_,
												 packet->route_->dest_id_,
												 cost, link_cost, 
												 packet->route_->path_);
	return proposed_route;
}
//---------------------------------------------------------------------------//
//                              CONFIG NEIGHBOR                              //
//---------------------------------------------------------------------------//
void ControlPlane::configNeighbor(Packet* packet) {
    // Processes a new or existing neighbor that has sent a NEIGHBOR_PACKET.
    iam(classname_, __FUNCTION__);

	std::string message;

	// Acknowledge that the neighbor was heard from
	neighborCheckIn(packet->header->sender_);	

	// Build the route from route information in the packet.
	Route* proposed_route = buildNeighborRoute(packet);	

	// update routing table
	processRoute(proposed_route);

	neighborCheckOut(packet->header->sender_);
};
//---------------------------------------------------------------------------//
//                         BUILD NEIGHBOR ROUTE                              //
//---------------------------------------------------------------------------//
Route* ControlPlane::buildNeighborRoute(Packet* packet) {
	// Builds direct route to neighbor based upon route embedded in packet object.
	iam(classname_, __FUNCTION__);

	int cost = neighborhood_->getCost(packet->header->sender_);
	std::vector<short> path;
	path.push_back(packet->header->sender_);
	Route* proposed_route = routes_->createRoute(packet->header->sender_,
												 packet->header->sender_,
												 cost, cost, path);	
	return proposed_route;
}
//---------------------------------------------------------------------------//
//                             PROCESS ROUTE                                 //
//---------------------------------------------------------------------------//
void ControlPlane::processRoute(Route* route) {
	// Updates the routing table 
	iam(classname_, __FUNCTION__);

	// This checks for existence of the exact same route in active state.
	if (routes_->activeRouteExists(route)) {
		// delete proposed route. Nothing to do.
		delete route;
	
	} else {
		
		if (routes_->routeExists(route)) {
			// route exists, but is in active. Activate it.
			routes_->activateRoute(route);
			logger_->activatedRoute(route, __FUNCTION__);				

		} else {

			routes_->postRoute(route);
			logger_->addRoute(route, __FUNCTION__);				
		
		}
		if (routes_->isBestRoute(route)) {

			ftable_->postHop(route);

			announceRoute(route);			
		}
		// Delete this
		routes_->print();
		ftable_->print();
	}	

}
//===========================================================================//
//                                WITHDRAW                                   //
//===========================================================================//
void ControlPlane::configWithdraw(Packet* packet) {
	iam(classname_, __FUNCTION__);	
	
	std::string message;
	short neighbor_id = packet->header->sender_;
	short dest_id = packet->route_->dest_id_;
	Route* route_withdraw;

	// Acknowledge that the neighbor was heard from
	neighborCheckIn(neighbor_id);

	// delete this 
	logger_->withdraw(packet, __FUNCTION__);

	route_withdraw = buildRoute(packet);

	if (routes_->activeRouteExists(route_withdraw)) {

		routes_->deactivateRoute(route_withdraw);
		logger_->deactivatedRoute(route_withdraw, __FUNCTION__);

		announceWithdraw(route_withdraw);	

		if (ftable_->hopExists(route_withdraw)) {
			
			ftable_->removeHop(dest_id);

			new_best_route_ = routes_->getBestRoute(dest_id);

			if (new_best_route_->dest_id_ != DESTINATION_NOT_FOUND) {

				ftable_->postHop(new_best_route_);
				announceRoute(new_best_route_);
			}
		}
		// delete this
		routes_->print();
		ftable_->print();					
	}
}

//---------------------------------------------------------------------------//
//                               ANNOUNCE ROUTE                              //
//---------------------------------------------------------------------------//
void ControlPlane::announceRoute(Route* route) {
	// Announce proposed route to neighbors
    iam(classname_, __FUNCTION__);

	std::string message;

	recipient_ids_ = neighborhood_->getRecipientIds(route->path_);
	if (recipient_ids_.size() > 0) {

		Packet* packet = packet_factory_->createRoutePacket(route, recipient_ids_);	
		logger_->announceRoute(packet, __FUNCTION__);
		event_manager_->notify(packet);	
		delete packet;
	}
}
//---------------------------------------------------------------------------//
//                              ANNOUNCE WITHDRAW                            //
//---------------------------------------------------------------------------//
void ControlPlane::announceWithdraw(Route* route) {
	// Announce withdraw as a consequence of failed link.
    iam(classname_, __FUNCTION__);

	recipient_ids_ = neighborhood_->getRecipientIds(route->path_);
	

	if (recipient_ids_.size() > 0) {		
		Packet* packet = packet_factory_->createWithdrawPacket(route, recipient_ids_);
		logger_->announceWithdraw(packet, __FUNCTION__);
		event_manager_->notify(packet);	
		delete packet;	
	}
}
//---------------------------------------------------------------------------//
//                                ADVERTISE                                  //
//---------------------------------------------------------------------------//
void ControlPlane::advertise(short neighbor_id) {
	// Advertises forwarding table to newly discovered neighbors.
    iam(classname_, __FUNCTION__);		
	
	// The routes do not have the neighbor id on the path. 
	std::vector<Route*>::iterator it;
	std::vector<Route*> routes = routes_->getRoutesToAdvertise(neighbor_id); 	
	std::vector<short> recipients;
	recipients.push_back(neighbor_id);

	for (it=routes.begin(); it!=routes.end(); ++it) {
		
		Packet* packet = packet_factory_->createRoutePacket(*it, recipients);
		logger_->advertiseRoute(packet, __FUNCTION__);
		event_manager_->notify(packet);	
		delete packet;

	}
}

//---------------------------------------------------------------------------//
//                             MONITOR NETWORK                               //
//---------------------------------------------------------------------------//
void ControlPlane::checkNetwork() {
	// Gets downed neighbors and disables routes and forwarding table entries.
	iam(classname_, __FUNCTION__);	

	std::set<short> destinations;
	std::set<short>::iterator dit;
	std::vector<short> down_neighbors;
	std::vector<short>::iterator nit;	
	Route* route;
	std::vector<Route*> routes;
	std::vector<Route*>::iterator rit;

	// Neighbors that connected by unreachable.
	down_neighbors = neighborhood_->getDownNeighborIds();		

	// Process routes for each down neighbor.
	for (nit=down_neighbors.begin();nit!=down_neighbors.end();++nit) {

		routes = routes_->getRoutes(*nit);

		for (rit=routes.begin();rit!=routes.end();++rit) {

			routes_->deactivateRoute(*rit);
			logger_->deactivatedRoute(*rit, __FUNCTION__);

			ftable_->removeHop(*rit);

			announceWithdraw(*rit);

			// Now find an alternative and insert into the set of routes to announce.
			route = routes_->getBestRoute((*rit)->dest_id_);
			
			if (route->dest_id_ != DESTINATION_NOT_FOUND) {

				ftable_->postHop(route);
				announceRoute(route);
			}
		}
	}

}
//===========================================================================//
//                            SET EVENT MANAGER                              //
//===========================================================================//
void ControlPlane::setEventManager(EventManager* event_manager) {
	iam(classname_, __FUNCTION__);
	event_manager_ = event_manager;
}
//---------------------------------------------------------------------------//
//                            ROUTER BUILD TESTS                             //
//---------------------------------------------------------------------------//
bool ControlPlane::hey() {
	holla(classname_, address_);

	assert(routes_->hey());
	assert(neighborhood_->hey());

    return true;
};