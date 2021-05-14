#include "Routes.h"

//===========================================================================//
//                                 ROUTE                                     //
//===========================================================================//
Route::Route()
    : router_id_(DEFAULT_NODE), neighbor_id_(DEFAULT_NODE), dest_id_(DEFAULT_NODE),
      link_cost_(DEFAULT_COST), cost_(DEFAULT_COST), has_failed_link_(false),
      classname_("Route")
      {
          formatRouteId();
          prependPath(router_id_);          
          Link failed_link_;
          failed_link_.router_id_ = DEFAULT_NODE;
          failed_link_.neighbor_id_ = DEFAULT_NODE;
          failed_link_.version_ = DEFAULT_VERSION;

      }
//---------------------------------------------------------------------------//
// Direct Route to Neighbor
Route::Route(short router_id, short neighbor_id, int link_cost) 
    : router_id_(router_id), neighbor_id_(neighbor_id_), dest_id_(neighbor_id),
      link_cost_(link_cost), cost_(link_cost), has_failed_link_(false),
      classname_("Route")
      {
        formatRouteId();
        prependPath(router_id);        
      }
//---------------------------------------------------------------------------//
// Testing
Route::Route(short router_id, short neighbor_id, short dest_id, int link_cost, 
              int cost, std::vector<short> path, std::vector<Link> links) 
    : router_id_(router_id), neighbor_id_(neighbor_id), dest_id_(dest_id),
      link_cost_(link_cost), cost_(cost), path_(path), links_(links),
      has_failed_link_(false), classname_("Route")
      {
        formatRouteId();
        prependPath(router_id);
      }        
//---------------------------------------------------------------------------//
Route::~Route() {}
//---------------------------------------------------------------------------//
//                             INITIALIZE                                    //      
//---------------------------------------------------------------------------//
void Route::initialize() {
    iam(classname_, __FUNCTION__);
    
    router_id_ = INIT_NODE;
    neighbor_id_ = INIT_NODE;
    dest_id_ = INIT_NODE;
    link_cost_ = INIT_COST;
    cost_ = INIT_COST;
    path_.clear();
    links_.clear();
    has_failed_link_ = false;
    failed_link_.router_id_ = INIT_NODE;
    failed_link_.neighbor_id_ = INIT_NODE;
    failed_link_.version_ = 0;

    leaving(classname_, __FUNCTION__);                      
}
//---------------------------------------------------------------------------//
//                           FORMAT ROUTE ID                                 //      
//---------------------------------------------------------------------------//
void Route::formatRouteId() {
    iam(classname_, __FUNCTION__);
    route_id_ = intToString(router_id_) + "-" + \
          intToString(neighbor_id_) + "-" + intToString(dest_id_);    
    leaving(classname_, __FUNCTION__);                  
}
//---------------------------------------------------------------------------//
//                      APPEND LINK (DESERIALIZATION)                        //      
//---------------------------------------------------------------------------//
void Route::appendLink(short router_id, short neighbor_id, short version) {
    iam(classname_, __FUNCTION__);
    Link link;
    link.router_id_ = router_id;
    link.neighbor_id_ = neighbor_id;
    link.version_ = version;

    links_.push_back(link);
    leaving(classname_, __FUNCTION__);
}
//---------------------------------------------------------------------------//
//                              PREPEND LINK                                 //      
//---------------------------------------------------------------------------//
void Route::prependLink(Link link) {
    iam(classname_, __FUNCTION__);    

    if (links_.size() > 0) {
        FindLink findLink = FindLink(link);
        std::vector<Link>::iterator it = 
            std::find_if(links_.begin(), links_.end(), findLink);

        if (it==links_.end()) {
            links_.insert(links_.begin(),link);
        }        
    } else {
        links_.push_back(link);
    }
    leaving(classname_, __FUNCTION__);
}       

//---------------------------------------------------------------------------//
//                              PREPEND LINK                                 //      
//---------------------------------------------------------------------------//
void Route::prependLink(short router_id, short neighbor_id, short version) {
    iam(classname_, __FUNCTION__);

    Link link;
    link.router_id_ = router_id;
    link.neighbor_id_ = neighbor_id;
    link.version_ = version;
    prependLink(link);
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
//                                IS DIRECT                                  //      
//---------------------------------------------------------------------------//
bool Route::isDirect() {
    iam(classname_, __FUNCTION__);  
    return (neighbor_id_ == dest_id_);
}
//---------------------------------------------------------------------------//
//                               GET LAST LINK                               //      
//---------------------------------------------------------------------------//
Link Route::getLastLink() {
    iam(classname_, __FUNCTION__);  
    return links_.back();
}

//---------------------------------------------------------------------------//
//                              GET FAILED LINK                              //      
//---------------------------------------------------------------------------//
Link Route::getFailedLink() {
    iam(classname_, __FUNCTION__);  
    return failed_link_;
}

//---------------------------------------------------------------------------//
//                              SET FAILED LINK                              //      
//---------------------------------------------------------------------------//
void Route::setFailedLink(short router_id, short neighbor_id, short version) {
    iam(classname_, __FUNCTION__);  
    Link failed_link_;
    failed_link_.router_id_ = router_id;
    failed_link_.neighbor_id_ = neighbor_id_;
    failed_link_.version_ = version;    
    has_failed_link_ = true;
    leaving(classname_, __FUNCTION__);  

}
//---------------------------------------------------------------------------//
//                            LINKS  EQUAL                                   //      
//---------------------------------------------------------------------------//
bool Route::linksEqual(Link a, Link b) {
    iam(classname_, __FUNCTION__);  
    
    return (a.router_id_ == b.router_id_ &&
            a.neighbor_id_ == b.neighbor_id_ &&
            a.version_ == b.version_);

}
//---------------------------------------------------------------------------//
//                         LINKS VECTORS EQUAL                               //      
//---------------------------------------------------------------------------//
bool Route::linkVectorsEqual(std::vector<Link> a, std::vector<Link> b) {
    iam(classname_, __FUNCTION__);  

    if (a.size() == 0 && b.size() == 0) return true;
    if (a.size() != b.size()) return false;
    
    std::vector<Link>::iterator it1, it2;
    it1 = a.begin();
    it2 = b.begin();
    
    while(it1 != a.end() && it2 != b.end()) {
        if (!linksEqual((*it1),(*it2))) return false;
        it1++;
        it2++;
    }
    return true;
}
//---------------------------------------------------------------------------//
//                                SERIALIZE                                  //      
//---------------------------------------------------------------------------//
void Route::serialize(char* buffer, size_t& offset) {
    iam(classname_, __FUNCTION__); 

    short temp_short;
    unsigned int vec_length, temp_unsigned;
    int temp_int;

    //  Router id
    std::string message = "Serializing router id: " + intToString(router_id_) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);    
    temp_short = htons(router_id_);
    memcpy(buffer+offset,&temp_short, sizeof(short));
    offset += sizeof(short);    

    // Neighbor id
    message = "Serializing neighbor id: " + intToString(neighbor_id_) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);    
    temp_short = htons(neighbor_id_);
    memcpy(buffer+offset,&temp_short, sizeof(short));
    offset += sizeof(short);    

    // Dest id
    message = "Serializing dest id: " + intToString(dest_id_) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);
    temp_short = htons(dest_id_);
    memcpy(buffer+offset,&temp_short, sizeof(short));
    offset += sizeof(short);   

    // Link Cost
    message = "Serializing link cost: " + intToString(link_cost_) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);
    temp_int = htonl(link_cost_);
    memcpy(buffer+offset,&temp_int, sizeof(int));
    offset += sizeof(int);

    // Cost
    message = "Serializing cost: " + intToString(cost_) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);
    temp_int = htonl(cost_);
    memcpy(buffer+offset,&temp_int, sizeof(int));
    offset += sizeof(int);    

    // Path length
    vec_length = path_.size();
    message = "Serializing path length: " + intToString(vec_length) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);    
    temp_unsigned = htonl(vec_length);
    memcpy(buffer+offset,&temp_unsigned, sizeof(unsigned));
    offset += sizeof(unsigned);  

    // Path
    message = "Serializing path to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);
    std::vector<short>::iterator it;
    for (it=path_.begin();it != path_.end();++it) {

        temp_short = htons(*it);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short); 
    }

    // Links length
    vec_length = links_.size();
    message = "Serializing links size: " + intToString(vec_length) + " to offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);    
    temp_unsigned = htonl(vec_length);
    memcpy(buffer+offset,&temp_unsigned, sizeof(unsigned));
    offset += sizeof(unsigned); 

    std::vector<Link>::iterator lit;    
    for (lit=links_.begin();lit!=links_.end();++lit) {        
        
        temp_short = htons((*lit).router_id_);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short); 

        temp_short = htons((*lit).neighbor_id_);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short);         

        temp_short = htons((*lit).version_);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short);         

    }

    // Failed link indicator
    message = "Serializing failed link indicator: " + intToString(has_failed_link_) + " to offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);    
    if (has_failed_link_ == true) {
        temp_int = htonl(1);
        memcpy(buffer+offset,&temp_int, sizeof(int));
        offset += sizeof(int);                         
    

        message = "Serializing failed link:  to offset = " + intToString(offset);
        note(classname_,__FUNCTION__, message);    
        temp_short = htons(failed_link_.router_id_);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short);     

        temp_short = htons(failed_link_.neighbor_id_);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short);             

        temp_short = htons(failed_link_.version_);
        memcpy(buffer+offset,&temp_short, sizeof(short));
        offset += sizeof(short);                
    } else {
        temp_int = htonl(0);
        memcpy(buffer+offset,&temp_int, sizeof(int));
        offset += sizeof(int);                                 
    }

    message = "Serialized " + intToString(offset) + " bytes. Serialization complete.";
    note(classname_,__FUNCTION__, message);    
    leaving(classname_, __FUNCTION__);    

}
//---------------------------------------------------------------------------//
//                               DESERIALIZE                                 //      
//---------------------------------------------------------------------------//
void Route::deserialize(char* buffer, size_t& offset) {
    iam(classname_, __FUNCTION__); 

    short temp_short, node_id, router_id, neighbor_id, version;
    int temp_int;    
    unsigned int vec_length, temp_unsigned;

    std::string message = "Inside deserialize + at offset = " + intToString(offset);
    note(classname_,__FUNCTION__, message);    

    //  Router id
    memcpy(&temp_short, buffer + offset, sizeof(short));
    router_id_ = ntohs(temp_short);
    message = "Deserialized router_id: " + intToString(router_id_) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);    
    offset += sizeof(short);

    // Neighbor id
    memcpy(&temp_short, buffer + offset, sizeof(short));
    neighbor_id_ = ntohs(temp_short);
    message = "Deserialized neighbor_id: " + intToString(neighbor_id_) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);    
    offset += sizeof(short);    

    // Dest id
    memcpy(&temp_short, buffer + offset, sizeof(short));
    dest_id_ = ntohs(temp_short);
    message = "Deserialized dest_id: " + intToString(dest_id_) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);    
    offset += sizeof(short);    

    formatRouteId();
    message = "Deserialized router_id: " + route_id_;
    note(classname_,__FUNCTION__, message);

    // Link Cost
    memcpy(&temp_int, buffer + offset, sizeof(int));
    link_cost_ = ntohl(temp_int);
    message = "Deserialized Link cost: " + intToString(link_cost_) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);    
    offset += sizeof(int);    

    // Cost
    memcpy(&temp_int, buffer + offset, sizeof(int));
    cost_ = ntohl(temp_int);
    message = "Deserialized cost: " + intToString(cost_) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);        
    offset += sizeof(int);    

    // Path length
    memcpy(&temp_unsigned, buffer + offset, sizeof(unsigned));
    vec_length = ntohl(temp_unsigned);
    message = "Deserialized path length: " + intToString(vec_length) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);           
    offset += sizeof(unsigned);  

    // Path
    for (int i = 0;i<vec_length;++i) {

        memcpy(&temp_short, buffer + offset, sizeof(short));
        node_id = ntohs(temp_short);
        offset += sizeof(short);            
        path_.push_back(node_id);
    }

    // Links length
    memcpy(&temp_unsigned, buffer + offset, sizeof(unsigned));
    vec_length = ntohl(temp_unsigned);
    message = "Deserialized links length: " + intToString(vec_length) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);           
    offset += sizeof(unsigned);  

    // Links    
    for (int i=0;i < vec_length;++i) {        

        memcpy(&temp_short, buffer + offset, sizeof(short));
        router_id = ntohs(temp_short);
        offset += sizeof(short);            

        memcpy(&temp_short, buffer + offset, sizeof(short));
        neighbor_id = ntohs(temp_short);
        offset += sizeof(short);                    

        memcpy(&temp_short, buffer + offset, sizeof(short));
        version = ntohs(temp_short);
        offset += sizeof(short);                    

        appendLink(router_id, neighbor_id, version);
    }
    message = "Deserialized links ";
    note(classname_,__FUNCTION__, message);        

    // failed link indicator
    memcpy(&temp_int, buffer + offset, sizeof(int));
    has_failed_link_ = ntohl(temp_int);
    message = "Deserialized failed_link indicator: " + intToString(has_failed_link_) + " from offset = " + intToString(offset);    
    note(classname_,__FUNCTION__, message);           
    offset += sizeof(int);            

    if (has_failed_link_ == true) {

        // failed link
        memcpy(&temp_short, buffer + offset, sizeof(short));
        router_id = ntohs(temp_short);
        message = "Deserialized failed_link.router_id: " + intToString(router_id) + " from offset = " + intToString(offset);    
        note(classname_,__FUNCTION__, message);                 
        offset += sizeof(short);            

        memcpy(&temp_short, buffer + offset, sizeof(short));
        neighbor_id = ntohs(temp_short);
        message = "Deserialized failed_link.neighbor_id: " + intToString(neighbor_id) + " from offset = " + intToString(offset);    
        note(classname_,__FUNCTION__, message);                
        offset += sizeof(short);
                    
        memcpy(&temp_short, buffer + offset, sizeof(short));        
        version = ntohs(temp_short);
        message = "Deserialized failed_link.version: " + intToString(version) + " from offset = " + intToString(offset);    
        note(classname_,__FUNCTION__, message);                
        offset += sizeof(short);                    


        setFailedLink(router_id, neighbor_id, version);

    }
    message = "Deserialized " + intToString(offset) + " bytes. Deserialization complete.";
    note(classname_,__FUNCTION__, message);    

    leaving(classname_, __FUNCTION__);    
}
//---------------------------------------------------------------------------//
//                                 PRINT                                     //      
//---------------------------------------------------------------------------//
void Route::print() {
    std::vector<Link>::iterator it;
        std::cout << "\n==============================================================" << std::endl;
        std::cout << "                Router Id: " << router_id_ << std::endl;                                        
        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << "              Neighbor Id: " << intToString(neighbor_id_) << std::endl;
        std::cout << "           Destination Id: " << intToString(dest_id_) << std::endl;
        std::cout << "                Link Cost: " << intToString(link_cost_) << std::endl;
        std::cout << "                     Cost: " << intToString(cost_) << std::endl;
        std::cout << "                     Path: ";
        print_vec(path_); 
        std::cout << "                    Links: " << std::endl;
        for (it=links_.begin();it!=links_.end();++it) {
            std::cout << "          Router Id: " << (*it).router_id_ << "    Neighbor Id: " <<\
                (*it).neighbor_id_ << "    Version: " << (*it).version_ << std::endl;
        }
        std::cout << "==============================================================" << std::endl;
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
// Used to generate an iterator to a specific path to a destination. 
FindExactRoute::FindExactRoute(Route* route) 
    : route_(route)
    {}

bool FindExactRoute::operator()(Route* route) {    
    return (route_->neighbor_id_ == route->neighbor_id_ &&
            route_->dest_id_ == route->dest_id_ &&
            route_->link_cost_ == route->link_cost_ &&
            route_->cost_ == route->cost_ &&
            route_->path_ == route->path_);
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to an iterator for any path from a neighbor
// to a destination.
FindRoute::FindRoute(Route* route) 
    : route_(route)
    {}

bool FindRoute::operator()(Route* route) {
    return (route->neighbor_id_ == route_->neighbor_id_ && 
            route->dest_id_ == route_->dest_id_);
}
//---------------------------------------------------------------------------//
// Failed Link Search
FindLink::FindLink(Link link) 
    : link_(link)
    {}

bool FindLink::operator()(Link link) {
    return (link.router_id_ == link_.router_id_ &&
            link.neighbor_id_ == link_.neighbor_id_ &&
            (link.version_ == link_.version_ || 
             link.version_ < link_.version_));
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
FindNeighborDestination::FindNeighborDestination(short neighbor_id, short dest_id) 
    : neighbor_id_(neighbor_id), dest_id_(dest_id)
    {}

bool FindNeighborDestination::operator()(Route* route) {
    return (route->neighbor_id_ == neighbor_id_ && 
            route->dest_id_ == dest_id_);
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
FindRoutesDestination::FindRoutesDestination(short dest_id) 
    : dest_id_(dest_id)
    {}

bool FindRoutesDestination::operator()(Route* route) {
    return route->dest_id_ == dest_id_;
}
//---------------------------------------------------------------------------//
// Used to generate an iterator to all Route objects for a specified destination.
SearchPath::SearchPath(short node_id) 
    : node_id_(node_id)
    {}

bool SearchPath::operator()(short node_id) {
    return node_id_ == node_id;
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
    return rnf;
}

//===========================================================================//
//                            PUBLIC INTERFACE                               //
//===========================================================================//
//---------------------------------------------------------------------------//
//                                ADD ROUTE                                  //
//---------------------------------------------------------------------------//
void Routes::addRoute(Route* route) {
    iam(classname_, __FUNCTION__);            
                    
    FindRoute theRoute = FindRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 

    if (it != routes_.end()) {
        std::string msg = "Unable to add route. Route to " + intToString(route->dest_id_) + \
        " thru " + intToString(route->neighbor_id_) + " already exists.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    } else {
        routes_.push_back(route);
    }    

}
//---------------------------------------------------------------------------//
//                              BUILD ROUTE                                  //
//---------------------------------------------------------------------------//
Route* Routes::nullRoute() {
    iam(classname_, __FUNCTION__);
    Route* route = new Route(0,0,0);
    route->initialize();
    return route;
}
//---------------------------------------------------------------------------//
//                              BUILD ROUTE                                  //
//---------------------------------------------------------------------------//
Route* Routes::buildRoute() {
    iam(classname_, __FUNCTION__);
    Route* route = new Route(0,0,0);
    return route;
}
//---------------------------------------------------------------------------//
//                              CREATE ROUTE                                 //
//---------------------------------------------------------------------------//
void Routes::createRoute(short neighbor_id, int link_cost) {
    iam(classname_, __FUNCTION__);
    Route* route = new Route(router_id_, neighbor_id, link_cost);
    routes_.push_back(route);
}
//---------------------------------------------------------------------------//
//                               DELETE ROUTE                                //
//---------------------------------------------------------------------------//
void Routes::deleteRoute(Route* route) {
    iam(classname_, __FUNCTION__);

    FindRoute theRoutes = FindRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoutes); 

    for (it = routes_.begin(); it != routes_.end(); ++ it) {
        delete *it;
        it = routes_.erase(it);
    }
}

//---------------------------------------------------------------------------//
void Routes::deleteRoute(short neighbor_id, short dest_id) {
    iam(classname_, __FUNCTION__);

    Route* route;

    FindNeighborDestination theRoute = FindNeighborDestination(neighbor_id, dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 

    for (it = routes_.begin(); it != routes_.end(); ++ it) {
        delete *it;
        it = routes_.erase(it);
    }
}
//---------------------------------------------------------------------------//
//                            GET INVALID ROUTES                             //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getInvalidRoutes(Link failed_link) {
    iam(classname_, __FUNCTION__);

    FindLink findlink = FindLink(failed_link);
    std::vector<Route*> invalid;
    std::vector<Route*>::iterator it;
    std::vector<Link>::iterator lit;
    
    for (it=routes_.begin();it!=routes_.end();++it) {
        lit = std::find_if((*it)->links_.begin(),(*it)->links_.end(),findlink); 
        
        for (lit=(*it)->links_.begin();lit!=(*it)->links_.end(); ++lit) {
            if (lit!=(*it)->links_.end()) {
                invalid.push_back(*it);
            }
        }
    }
    return invalid;
}

//---------------------------------------------------------------------------//
//                              UPDATE COSTS                                 //
//---------------------------------------------------------------------------//
void Routes::updateCosts(short neighbor_id, int link_cost, int cost_differential) {
    iam(classname_, __FUNCTION__);

    FindRoutesNeighbor thruNeighbor = FindRoutesNeighbor(neighbor_id);    
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), thruNeighbor); 

    for (it = routes_.begin(); it != routes_.end(); ++ it) {
        (*it)->link_cost_ = link_cost;
        (*it)->cost_ += cost_differential;
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
    if (a->links_.size() != b->links_.size()) return false;
    if (a->links_.size()>0) {
        return (routesEquivalent(a,b) && a->path_ == b->path_ && \
        a->link_cost_ == b->link_cost_ && a->cost_ == b->cost_ &&
        a->links_.size() == b->links_.size() &&
        a->linkVectorsEqual(a->links_,b->links_));  
    } else {
        return (routesEquivalent(a,b) && a->path_ == b->path_ && \
        a->link_cost_ == b->link_cost_ && a->cost_ == b->cost_ &&
        a->links_.size() == b->links_.size());          
    }



}
//---------------------------------------------------------------------------//
//                              A ROUTE EXISTS                               //
//---------------------------------------------------------------------------//
bool Routes::aRouteExists(short dest_id) {

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
bool Routes::aRouteExists(short neighbor_id, short dest_id) {

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
//                               ROUTE EXISTS                                //
//---------------------------------------------------------------------------//
bool Routes::altRouteExists(Route* route) {

    FindNeighborDestination theRoute = \
        FindNeighborDestination(route->neighbor_id_, route->dest_id_);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), theRoute); 
    
    if (it == routes_.end()) {
        return false;        
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
//                               PATH EXISTS                                 //
//---------------------------------------------------------------------------//
bool Routes::equalRouteExists(Route* route) {
    iam(classname_, __FUNCTION__); 

    FindExactRoute thePath = FindExactRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), thePath); 

    if (it == routes_.end()) {
        return false;
    } else {
        return true;
    }    
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
//                             GET BEST ROUTE                                //
//---------------------------------------------------------------------------//
Route* Routes::getBestRoute(short dest_id) {
    iam(classname_, __FUNCTION__);                           

    std::sort(routes_.begin(), routes_.end(), SortRoutes());

    FindRoutesDestination routes = FindRoutesDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), routes); 

    if (it != routes_.end()) {
        return *it;
    } else {
        return routeNotFound();
    }
} 
//---------------------------------------------------------------------------//
//                           GET NEXT HOP                                    //
//---------------------------------------------------------------------------//
short Routes::getNextHop(short dest_id) {
    iam(classname_, __FUNCTION__); 
    return getBestRoute(dest_id)->neighbor_id_;
}
//---------------------------------------------------------------------------//
//                          GET BEST ROUTES                                  //
//---------------------------------------------------------------------------//
std::vector<Route*> Routes::getBestRoutes() {    
    iam(classname_, __FUNCTION__); 

    Route* route;
    std::vector<Route*> best;
    std::set<short> destinations = getDestinations();
    std::set<short>::iterator it;
    best.reserve(destinations.size());

    for (it=destinations.begin();it!=destinations.end();++it) {
        route = getBestRoute(*it);
        best.push_back(route);
    }
    return best;
}
//---------------------------------------------------------------------------//
//                          GET DESTINATIONS                                 //
//---------------------------------------------------------------------------//
std::set<short> Routes::getDestinations() {
    iam(classname_, __FUNCTION__); 

    std::set<short> destinations;
    std::vector<Route*>::iterator it;
    for (it=routes_.begin();it!=routes_.end();++it) {
        destinations.insert((*it)->dest_id_);
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

    std::vector<Route*>::iterator it;
    for (it=routes_.begin(); it!=routes_.end(); ++it) {
        ((*it)->print());
    }
}

