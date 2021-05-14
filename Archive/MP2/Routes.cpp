#include "Routes.h"

//===========================================================================//
//                                 ROUTE                                     //
//===========================================================================//
Route::Route()
    : router_id_(DEFAULT_NODE), neighbor_id_(DEFAULT_NODE), dest_id_(DEFAULT_NODE),
      link_cost_(DEFAULT_COST), cost_(DEFAULT_COST), active_(true), state_(SERVICABLE),
      classname_("Route")
      {
          formatRouteId();
      }
//---------------------------------------------------------------------------//
// Direct Route to Neighbor
Route::Route(short router_id, short neighbor_id, int link_cost) 
    : router_id_(router_id), neighbor_id_(neighbor_id_), dest_id_(neighbor_id),
      link_cost_(link_cost), cost_(link_cost), active_(true), state_(SERVICABLE),
      classname_("Route")
      {
        formatRouteId();        
      }
//---------------------------------------------------------------------------//
// Standard route constructor
Route::Route(short router_id, short neighbor_id, short dest_id, int link_cost, 
              int cost, std::vector<short> path) 
    : router_id_(router_id), neighbor_id_(neighbor_id), dest_id_(dest_id),
      link_cost_(link_cost), cost_(cost), path_(path), active_(true), 
      state_(SERVICABLE), classname_("Route")
      {
        formatRouteId();
      }        
//---------------------------------------------------------------------------//
//Route::~Route() {}

//---------------------------------------------------------------------------//
//                           FORMAT ROUTE ID                                 //      
//---------------------------------------------------------------------------//
void Route::formatRouteId() {
    iam(classname_, __FUNCTION__);
    route_id_ = shortToString(router_id_) + "-" + \
          shortToString(neighbor_id_) + "-" + shortToString(dest_id_);    
    leaving(classname_, __FUNCTION__);                  
}            
//---------------------------------------------------------------------------//
//                               SET STATE                                   //      
//---------------------------------------------------------------------------//
void Route::activate() {
    iam(classname_, __FUNCTION__);  
    state_ = ACTIVE;
}
//---------------------------------------------------------------------------//
void Route::service() {
    iam(classname_, __FUNCTION__);  
    state_ = SERVICABLE;
}
//---------------------------------------------------------------------------//
void Route::fail() {
    iam(classname_, __FUNCTION__);  
    state_ = FAILED;
}
//---------------------------------------------------------------------------//
void Route::withdraw() {
    iam(classname_, __FUNCTION__);  
    state_ = WITHDRAWN;
}
//---------------------------------------------------------------------------//
//                              CHECK STATE                                  //      
//---------------------------------------------------------------------------//
bool Route::isActive() {
    iam(classname_, __FUNCTION__);  
    return state_ == ACTIVE;
}
//---------------------------------------------------------------------------//
bool Route::isServicable() {
    iam(classname_, __FUNCTION__);  
    return state_ == SERVICABLE;
}
//---------------------------------------------------------------------------//
bool Route::hasFailed() {
    iam(classname_, __FUNCTION__);  
    return state_ == FAILED;
}
//---------------------------------------------------------------------------//
bool Route::isWithdrawn() {
    iam(classname_, __FUNCTION__);  
    return state_ == WITHDRAWN;
}
//---------------------------------------------------------------------------//
//                              PREPEND PATH                                 //      
//---------------------------------------------------------------------------//
void Route::prependPath(short router_id) {
    iam(classname_, __FUNCTION__);  

    if (path_.size() > 0) {
        SearchPath search = SearchPath(router_id);
        std::vector<short>::iterator it = 
            std::find_if(path_.begin(), path_.end(), search);         

        if (it==path_.end()) {
            path_.insert(path_.begin(),router_id);               
        }
    } else {
        path_.push_back(router_id);
    }
    leaving(classname_, __FUNCTION__);  
}
//---------------------------------------------------------------------------//
//                                ON PATH                                    //      
//---------------------------------------------------------------------------//
bool Route::onPath(short router_id) {
    iam(classname_, __FUNCTION__);  

    SearchPath search = SearchPath(router_id);
    std::vector<short>::iterator it = 
        std::find_if(path_.begin(), path_.end(), search);         

    if (it!=path_.end()) {
        return true;
    } else {
        return false;
    }
}
//---------------------------------------------------------------------------//
bool Route::notOnPath(short router_id) {
    iam(classname_, __FUNCTION__);  

    SearchPath search = SearchPath(router_id);
    std::vector<short>::iterator it = 
        std::find_if(path_.begin(), path_.end(), search);         

    if (it==path_.end()) {
        return true;
    } else {
        return false;
    }
}

//---------------------------------------------------------------------------//
//                                 PRINT                                     //      
//---------------------------------------------------------------------------//
void Route::print(std::string note) {
    if (VERBOSE) {
        std::string message = "Route from " + shortToString(router_id_) + " to " + shortToString(dest_id_)  + " through " + shortToString(neighbor_id_) + " " + note;    
        std::cout << message << std::endl;
    }
}
//---------------------------------------------------------------------------//
void Route::print(std::string note, std::vector<short> recipients) {
    if (VERBOSE) {
        std::string message = "Route from " + shortToString(router_id_) + " to " + \
            shortToString(dest_id_)  + " through " + shortToString(neighbor_id_) + " " + note + \
            " Recipients: " + vecToString(recipients);    
        std::cout << message << std::endl;
    }
}
//---------------------------------------------------------------------------//
void Route::print() {
    if (VERBOSE) {
        std::cout << "\n==============================================================" << std::endl;
        std::cout << "                Router Id: " << router_id_ << std::endl;                                        
        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << "              Neighbor Id: " << shortToString(neighbor_id_) << std::endl;
        std::cout << "           Destination Id: " << shortToString(dest_id_) << std::endl;
        std::cout << "                Link Cost: " << intToString(link_cost_) << std::endl;
        std::cout << "                     Cost: " << intToString(cost_) << std::endl;
        std::cout << "                     Path: ";
        print_vec(path_); 
        std::cout << "                   Active: "  << active_ << std::endl;
        std::cout << "==============================================================" << std::endl;
    }
}

//===========================================================================//
//                                 ROUTES                                    //
//===========================================================================//

//---------------------------------------------------------------------------//
//                         UNARY FUNCTIONS: ROUTES                           //
//---------------------------------------------------------------------------//
// Used to sort Routes by Destination and Total Cost
bool SortRoutes::operator()(Route* lhs, Route* rhs) const {
    if (lhs->dest_id_ == rhs->dest_id_) {
        if (lhs->cost_ == rhs->cost_) {
            return (lhs->neighbor_id_ < rhs->neighbor_id_);
        } else {
            return (lhs->cost_ < rhs->cost_);
        }
    } else {
        return (lhs->dest_id_ < rhs->dest_id_);
    }    
};
//---------------------------------------------------------------------------//
FindRoute::FindRoute(Route* route) 
    : route_(route)
    {}

bool FindRoute::operator()(Route* route) {
    return (route_->neighbor_id_ == route->neighbor_id_ &&
            route_->dest_id_ == route->dest_id_ &&
            route_->link_cost_ == route->link_cost_ &&
            route_->cost_ == route->cost_ &&
            route_->path_ == route->path_);
}
//---------------------------------------------------------------------------//
FindActiveRoute::FindActiveRoute(Route* route) 
    : route_(route)
    {}

bool FindActiveRoute::operator()(Route* route) {
    return (route_->neighbor_id_ == route->neighbor_id_ &&
            route_->dest_id_ == route->dest_id_ &&
            route_->link_cost_ == route->link_cost_ &&
            route_->cost_ == route->cost_ &&
            route_->path_ == route->path_ &&
            route->active_ == true);
}
//---------------------------------------------------------------------------//
FindNeighborDestination::FindNeighborDestination(short neighbor_id, short dest_id) 
    : neighbor_id_(neighbor_id), dest_id_(dest_id)
    {}

bool FindNeighborDestination::operator()(Route* route) {
    return (route->neighbor_id_ == neighbor_id_ && 
            route->dest_id_ == dest_id_);
}
//---------------------------------------------------------------------------//
FindActiveNeighborDestination::FindActiveNeighborDestination(short neighbor_id, short dest_id) 
    : neighbor_id_(neighbor_id), dest_id_(dest_id)
    {}

bool FindActiveNeighborDestination::operator()(Route* route) {
    return (route->neighbor_id_ == neighbor_id_ && 
            route->dest_id_ == dest_id_ && 
            route->active_ == true);
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to all Route objects for a specified destination.
FindRoutesNeighbor::FindRoutesNeighbor(short neighbor_id) 
    : neighbor_id_(neighbor_id)
    {}

bool FindRoutesNeighbor::operator()(Route* route) {
    return route->neighbor_id_ == neighbor_id_;
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to all Route objects for a specified destination.
FindActiveRoutesNeighbor::FindActiveRoutesNeighbor(short neighbor_id) 
    : neighbor_id_(neighbor_id)
    {}

bool FindActiveRoutesNeighbor::operator()(Route* route) {
    return (route->neighbor_id_ == neighbor_id_ && route->active_ == true);
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to all Route objects for a specified destination.
FindRoutesDestination::FindRoutesDestination(short dest_id) 
    : dest_id_(dest_id)
    {}

bool FindRoutesDestination::operator()(Route* route) {
    return (route->dest_id_ == dest_id_);
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to all Route objects for a specified destination.
FindActiveRoutesDestination::FindActiveRoutesDestination(short dest_id) 
    : dest_id_(dest_id)
    {}

bool FindActiveRoutesDestination::operator()(Route* route) {
    return (route->dest_id_ == dest_id_ && route->active_ == true);
}
//---------------------------------------------------------------------------//
// Predicate that returns true if a route matches destination, is active, and 
// does not have the neighbor on path. Used to find routes around failures and
// withdraws.
FindActiveRoutesDestinationAroundNeighbor::FindActiveRoutesDestinationAroundNeighbor(short neighbor_id, short dest_id) 
    : neighbor_id_(neighbor_id), dest_id_(dest_id)
    {}

bool FindActiveRoutesDestinationAroundNeighbor::operator()(Route* route) {
    return (route->dest_id_ == dest_id_ && route->notOnPath(neighbor_id_) &&
            route->active_ == true);
}
//---------------------------------------------------------------------------//
// Finds routes to destination that go through neighbor.
FindActiveRoutesDestinationThruNeighbor::FindActiveRoutesDestinationThruNeighbor(short neighbor_id, short dest_id) 
    : neighbor_id_(neighbor_id), dest_id_(dest_id)
    {}

bool FindActiveRoutesDestinationThruNeighbor::operator()(Route* route) {
    return (route->dest_id_ == dest_id_ && route->notOnPath(neighbor_id_) &&
            route->active_ == true);
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to all Route objects for a specified destination.
SearchPath::SearchPath(short node_id) 
    : node_id_(node_id)
    {}

bool SearchPath::operator()(short router_id) {
    return node_id_ == router_id;
}
//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
Routes::Routes() 
    : classname_("Routes")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;
        route_ = new Route();
    }
    
Routes::Routes(Parameters* parameters) 
    :   router_id_(parameters->router_id_),
        classname_("Routes")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;
        route_ = new Route();
        
    }
//===========================================================================//
//                            PRIVATE INTERFACE                              //
//===========================================================================//
//---------------------------------------------------------------------------//
//                             ROUTE NOT FOUND                               //
//---------------------------------------------------------------------------//
Route* Routes::routeNotFound() {
    iam(classname_, __FUNCTION__);     
    Route* rnf = new Route();
    rnf->router_id_ = NODE_NOT_FOUND;
    rnf->neighbor_id_ = NODE_NOT_FOUND;    
    rnf->dest_id_ = DESTINATION_NOT_FOUND;
    rnf->link_cost_ = COST_NOT_FOUND;
    rnf->cost_ = COST_NOT_FOUND;    
    rnf->active_ = false;
    return rnf;
}

//===========================================================================//
//                            PUBLIC INTERFACE                               //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              CREATE ROUTE                                 //
//---------------------------------------------------------------------------//
Route* Routes::createRoute(short neighbor_id, short dest_id, int cost, int link_cost,
                         std::vector<short> path) {
    // Creates standard route through neighbor to destination. 
    iam(classname_, __FUNCTION__);
    Route* route = new Route(router_id_, neighbor_id, dest_id, link_cost, cost, path);        
    route->prependPath(router_id_);
    return route;
}
//---------------------------------------------------------------------------//
//                               UPDATE ROUTE                                //
//---------------------------------------------------------------------------//
void Routes::postRoute(Route* route) {
    // Updates a route, given the neighbor_id, dest_id key.
    iam(classname_, __FUNCTION__);

    FindNeighborDestination theRoute = FindNeighborDestination(route->neighbor_id_, route->dest_id_);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute);     

    if (it == routes_.end()) {
        routes_.push_back(route);    
    
    }  else {    
        (*it)->link_cost_ = route->link_cost_;
        (*it)->cost_ = route->cost_;
        (*it)->path_ = route->path_;
        (*it)->active_ = true;  
    }
}
//---------------------------------------------------------------------------//
//                            ACTIVATE ROUTES                                //
//---------------------------------------------------------------------------//
void Routes::activateRoute(Route* route) {
    iam(classname_, __FUNCTION__);

    FindRoute theRoute = FindRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 

    while (it != routes_.end()) {
        (*it)->active_ = true;
        it = std::find_if(++it, routes_.end(), theRoute);
    }
}
//---------------------------------------------------------------------------//
//                           DEACTIVATE ROUTES                               //
//---------------------------------------------------------------------------//
void Routes::deactivateRoute(Route* route) {
    // Deactivates route from neighbor to destination
    iam(classname_, __FUNCTION__);

    FindRoute theRoute = FindRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 

    while (it != routes_.end()) {
        (*it)->active_ = false;
        it = std::find_if(++it, routes_.end(), theRoute);
    }
}
//---------------------------------------------------------------------------//
void Routes::deactivateRoute(short neighbor_id, short dest_id) {
    // Deactivates route from neighbor to destination
    iam(classname_, __FUNCTION__);

    FindActiveNeighborDestination theRoute = FindActiveNeighborDestination(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 

    while (it != routes_.end()) {
        (*it)->active_ = false;
        it = std::find_if(++it, routes_.end(), theRoute);
    }
}
//---------------------------------------------------------------------------//
void Routes::deactivateRoutesNeighbor(short neighbor_id) {
    // Deactivates all active routes through neighbor
    iam(classname_, __FUNCTION__);

    FindActiveRoutesNeighbor theRoutes = FindActiveRoutesNeighbor(neighbor_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoutes); 

    while (it != routes_.end()) {
        (*it)->active_ = false;
        it = std::find_if(++it, routes_.end(), theRoutes);
    }
}
//---------------------------------------------------------------------------//
void Routes::deactivateRoutesDestination(short dest_id) {
    // Deactivates all active routes through neighbor
    iam(classname_, __FUNCTION__);
    
    FindActiveRoutesDestination theRoutes = FindActiveRoutesDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoutes); 

    while (it != routes_.end()) {
        (*it)->active_ = false;
        it = std::find_if(++it, routes_.end(), theRoutes);
    }
}


//---------------------------------------------------------------------------//
//                                COMPARISON                                 //
//---------------------------------------------------------------------------//
bool Routes::routesEquivalent(Route* a, Route* b) {
    return (a->neighbor_id_ == b->neighbor_id_ &&
            a->dest_id_ == b->dest_id_);    
}
//---------------------------------------------------------------------------//
bool Routes::routesEqual(Route* a, Route* b) {    
    iam(classname_, __FUNCTION__);   
    return (a->neighbor_id_ == b->neighbor_id_ &&
            a->dest_id_ == b->dest_id_ &&
            a->link_cost_ == b->link_cost_ &&
            a->cost_ == b->cost_ &&
            a->path_ == b->path_ &&
            a->active_ == b->active_);      
}
//===========================================================================//
//                             BOOLEAN QUERIES                               //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              ROUTE EXISTS                                 //
//---------------------------------------------------------------------------//
bool Routes::routeExists(short dest_id) {
    // Returns true if any route exists to the destination, active or not.
    iam(classname_, __FUNCTION__);   

    FindRoutesDestination aRoute = FindRoutesDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
bool Routes::routeExists(short neighbor_id, short dest_id) {
    // Returns true if any route exists from neighbor to the destination, active or not.
    iam(classname_, __FUNCTION__);   

    FindNeighborDestination aRoute = FindNeighborDestination(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
bool Routes::routeExists(Route* route) {
    // Returns true if the exact route exists, active or not.
    iam(classname_, __FUNCTION__);   

    FindRoute theRoute = FindRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
//                            ACTIVE ROUTE EXISTS                            //
//---------------------------------------------------------------------------//
bool Routes::activeRouteExists(short dest_id) {

    FindActiveRoutesDestination aRoute = FindActiveRoutesDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
bool Routes::activeRouteExists(short neighbor_id, short dest_id) {

    FindActiveNeighborDestination aRoute = FindActiveNeighborDestination(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
bool Routes::activeRouteExists(Route* route) {

    FindActiveRoute activeRoute = FindActiveRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), activeRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}

//---------------------------------------------------------------------------//
//                             IS BEST ROUTE                                 //
//---------------------------------------------------------------------------//
bool Routes::isBestRoute(Route* route) {
    // Determines 
    iam(classname_, __FUNCTION__); 

    Route* best_route = getBestRoute(route->dest_id_);
    return (routesEqual(route, best_route));
}
//---------------------------------------------------------------------------//
//                             GET ROUTE                                     //
//---------------------------------------------------------------------------//
Route* Routes::getRoute(short neighbor_id, short dest_id) {
    iam(classname_, __FUNCTION__);                           
    
    FindNeighborDestination theRoute = FindNeighborDestination(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 
    
    if (it == routes_.end()) {
        return routeNotFound();        
    } else {
        return *it;
    }
}
//---------------------------------------------------------------------------//
//                             GET ROUTES                                    //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getRoutes(short neighbor_id) {
    iam(classname_, __FUNCTION__);                           
    
    std::vector<Route*> routes;
    FindActiveRoutesNeighbor theRoutes = FindActiveRoutesNeighbor(neighbor_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoutes); 

    while (it != routes_.end()) {
        routes.push_back(*it);
        it = std::find_if(++it, routes_.end(), theRoutes);
    }
        
    return routes;
}
//---------------------------------------------------------------------------//
//                             GET ACTIVE ROUTE                              //
//---------------------------------------------------------------------------//
Route* Routes::getActiveRoute(short neighbor_id, short dest_id) {
    iam(classname_, __FUNCTION__);                           
    
    FindActiveNeighborDestination theRoute = FindActiveNeighborDestination(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 
    
    if (it == routes_.end()) {
        return routeNotFound();        
    } else {
        return *it;
    }
}
//---------------------------------------------------------------------------//
//                             GET BEST ROUTE                                //
//---------------------------------------------------------------------------//
Route* Routes::getBestRoute(short dest_id) {
    // Gets best active route to destination.
    iam(classname_, __FUNCTION__);                           

    std::sort(routes_.begin(), routes_.end(), SortRoutes());

    FindActiveRoutesDestination routes = FindActiveRoutesDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), routes); 

    if (it != routes_.end()) {
        return *it;
    } else {
        return routeNotFound();
    }
} 
//---------------------------------------------------------------------------//
Route* Routes::getBestRoute(short neighbor_id, short dest_id) {
    // Gets best active route to destination that avoids the neighbor.
    iam(classname_, __FUNCTION__);                           

    std::sort(routes_.begin(), routes_.end(), SortRoutes());

    FindActiveRoutesDestinationAroundNeighbor routes = \
        FindActiveRoutesDestinationAroundNeighbor(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), routes); 

    if (it != routes_.end()) {
        return *it;
    } else {

    return routeNotFound();    
    }
} 

//---------------------------------------------------------------------------//
//                          GET BEST ROUTES                                  //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getBestRoutes() {    
    iam(classname_, __FUNCTION__); 

    Route* route;
    std::vector<Route*> best;
    std::set<short> destinations = getActiveDestinations();
    std::set<short>::iterator it;
    best.reserve(destinations.size());

    for (it=destinations.begin();it!=destinations.end();++it) {
        route = getBestRoute(*it);
        if (route->dest_id_ != DESTINATION_NOT_FOUND) best.push_back(route);
    }
    return best;
}
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getBestRoutes(short neighbor_id) {    
    // Gets best routes that go through the designated neighbor
    iam(classname_, __FUNCTION__); 
    
    std::vector<Route*>::iterator it;
    std::vector<Route*> best = getBestRoutes();
    std::vector<Route*> best_thru_neighbor;

    for (it=best.begin(); it!=best.end(); ++it) {
        if ((*it)->neighbor_id_ == neighbor_id) {
            best_thru_neighbor.push_back(*it);
        }
    }
    return best_thru_neighbor;
}
//---------------------------------------------------------------------------//
//                     GET ACTIVE ROUTES FOR NEIGHBOR                        //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getActiveRoutesNeighbor(short neighbor_id) {    
    // Gets routes through the destignated neighbor
    iam(classname_, __FUNCTION__);     

    std::vector<Route*> active_routes_thru_neighbor;
    FindActiveRoutesNeighbor thruNeighbor = FindActiveRoutesNeighbor(neighbor_id);    
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), thruNeighbor); 

    while (it != routes_.end()) {
        active_routes_thru_neighbor.push_back(*it);
        it = std::find_if(++it, routes_.end(), thruNeighbor);
    }
        
    return active_routes_thru_neighbor;
}
//---------------------------------------------------------------------------//
//                     GET ACTIVE ROUTES FOR NEIGHBORS                       //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getActiveRoutesNeighbors(std::vector<short> neighbor_ids) {    
    // Gets routes through the destignated neighbor
    iam(classname_, __FUNCTION__);     

    std::vector<short>::iterator nit;
    std::vector<Route*>::iterator rit;
    std::vector<Route*> active_routes;
    std::vector<Route*> active_routes_thru_neighbor;

    for (nit=neighbor_ids.begin(); nit!=neighbor_ids.end();++nit) {
        active_routes_thru_neighbor = getActiveRoutesNeighbor(*nit);
        for (rit=active_routes_thru_neighbor.begin(); 
             rit!=active_routes_thru_neighbor.end();++rit) {
            
            active_routes.push_back(*rit);
        }    
    }
    return active_routes;
}
//---------------------------------------------------------------------------//
//                    GET ACTIVE ROUTES FOR DESTINATION                      //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getActiveRoutesDestination(short dest_id) {    
    // Gets routes to destination.
    iam(classname_, __FUNCTION__);     

    std::vector<Route*> active_routes_to_destination;
    FindActiveRoutesDestination thruNeighbor = FindActiveRoutesDestination(dest_id);    
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), thruNeighbor); 

    while (it != routes_.end()) {
        active_routes_to_destination.push_back(*it);
        it = std::find_if(++it, routes_.end(), thruNeighbor);
    }
        
    return active_routes_to_destination;
}
//---------------------------------------------------------------------------//
//                       GET ROUTES TO ADVERTISE                             //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getRoutesToAdvertise(short neighbor_id) {    
    // Obtain best routes that don't go through the designated neighbor
    iam(classname_, __FUNCTION__); 

    std::vector<Route*> routes_to_advertise;
    std::vector<Route*> best_routes = getBestRoutes(); 
    std::vector<Route*>::iterator it;

    for (it=best_routes.begin(); it!=best_routes.end(); ++it) {
        if ((*it)->notOnPath(neighbor_id)) {
            routes_to_advertise.push_back(*it);
        }
    }

    return routes_to_advertise;
}
//---------------------------------------------------------------------------//
//                          GET DESTINATIONS                                 //
//---------------------------------------------------------------------------//
std::set<short> Routes::getActiveDestinations() {
    iam(classname_, __FUNCTION__); 

    std::set<short> destinations;
    std::vector<Route*>::iterator it;
    for (it=routes_.begin();it!=routes_.end();++it) {
        destinations.insert((*it)->dest_id_);
    }
    return destinations;
}
//---------------------------------------------------------------------------//
std::set<short> Routes::getActiveDestinations(short neighbor_id) {
    // Returns all destinations through neighbor, active or not.
    iam(classname_, __FUNCTION__); 

    std::set<short> destinations;

    FindActiveRoutesNeighbor thruNeighbor = FindActiveRoutesNeighbor(neighbor_id);    
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), thruNeighbor); 

    while (it != routes_.end()) {
        destinations.insert((*it)->dest_id_);
        it = std::find_if(++it, routes_.end(), thruNeighbor);
    }
    return destinations;
}
//---------------------------------------------------------------------------//
std::set<short> Routes::getDestinations(short neighbor_id) {
    // Returns all destinations going through neighbor_id
    iam(classname_, __FUNCTION__); 

    std::set<short> destinations;

    FindRoutesNeighbor thruNeighbor = FindRoutesNeighbor(neighbor_id);    
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), thruNeighbor); 

    while (it != routes_.end()) {
        destinations.insert((*it)->dest_id_);
        it = std::find_if(++it, routes_.end(), thruNeighbor);
    }
    return destinations;
}
//---------------------------------------------------------------------------//
std::set<short> Routes::getDestinations(std::vector<short> neighbor_ids) {
    // Returns all destinations going through all neighbor_ids
    iam(classname_, __FUNCTION__); 

    std::set<short> destinations;
    std::set<short> neighbor_destinations;
    std::vector<short>::iterator it;
    std::set<short>::iterator sit;

    for (it=neighbor_ids.begin(); it!=neighbor_ids.end(); ++it) {
        neighbor_destinations = getDestinations(*it);
        for (sit=neighbor_destinations.begin(); sit!= neighbor_destinations.end(); ++sit) {
            destinations.insert(*sit);
        }
    }

    return destinations;
}
//---------------------------------------------------------------------------//
//                              HEY AND NAME                                 //
//---------------------------------------------------------------------------//
bool Routes::hey() {
    holla(classname_, address_);
    return true;
}
//---------------------------------------------------------------------------//
//                               PRINT ROUTE                                 //
//---------------------------------------------------------------------------//
void Routes::print() {    
    iam(classname_, __FUNCTION__);   
    
    if (VERBOSE) {

        std::cout << "\n==============================================================" << std::endl;
        std::cout << "                          ROUTING TABLE" << std::endl;
        std::cout << "--------------------------------------------------------------" << std::endl;  

        printf("|%5s|%5s|%5s|%5s|%5s|%5s||%10s|\n", "From ", "  To ", " Thru", "Link ", "Cost ",
                                            "State","   Path   ");

        std::vector<Route*>::iterator it;    
        for (it=routes_.begin(); it!=routes_.end(); ++it) {
            printf("|%5s|%5s|%5s|%5s|%5s|%5s|%10s|\n", shortToString((*it)->router_id_).c_str(),
                                            shortToString((*it)->dest_id_).c_str(),
                                            shortToString((*it)->neighbor_id_).c_str(),
                                            shortToString((*it)->link_cost_).c_str(),
                                            shortToString((*it)->cost_).c_str(),
                                            shortToString((*it)->active_).c_str(),
                                            vecToString((*it)->path_).c_str());        
        }
    }
}

