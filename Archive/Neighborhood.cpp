 #include "Neighborhood.h"
//===========================================================================//
//                                NEIGHBOR                                   //
//===========================================================================//
Neighbor::Neighbor() {};
Neighbor::Neighbor(short router_id, short neighbor_id, int link_cost, short ttl_ms)
    : router_id_(router_id), neighbor_id_(neighbor_id), link_cost_(link_cost),  
      version_(0), 
      active_(false), connected_(false),
      classname_("Neighbor")
    {        
        monitor.ttl_ms_ = ttl_ms;        
        setLastHeartbeat();
    }
//---------------------------------------------------------------------------//
//                               INITIALIZE                                  //
//---------------------------------------------------------------------------//    
void Neighbor::initialize() {
    iam(classname_, __FUNCTION__);
    router_id_ = INIT_NODE;
    neighbor_id_ = INIT_NODE;
    link_cost_ = INIT_COST;
    monitor.ttl_ms_ = 0;
    version_ = 0;
}
//---------------------------------------------------------------------------//
//                             GET ROUTER ID                                 //
//---------------------------------------------------------------------------//
short Neighbor::getRouterId() {
    iam(classname_, __FUNCTION__);
    return router_id_;
}        
//---------------------------------------------------------------------------//
//                             GET NEIGHBOR ID                               //
//---------------------------------------------------------------------------//
short Neighbor::getNeighborId() {
    return neighbor_id_;
}    
//---------------------------------------------------------------------------//
//                                 GET COST                                  //
//---------------------------------------------------------------------------//
int Neighbor::getCost() {
    iam(classname_, __FUNCTION__);
    leaving(classname_, __FUNCTION__);
    return link_cost_;
}
//---------------------------------------------------------------------------//
//                        SET SOCKET ADDR                                    //
//---------------------------------------------------------------------------//
void Neighbor::setCost(int link_cost) {
    iam(classname_, __FUNCTION__);
    link_cost_ = link_cost;
}
//---------------------------------------------------------------------------//
//                               GET VERSION                                 //
//---------------------------------------------------------------------------//
short Neighbor::getVersion() {
    iam(classname_, __FUNCTION__);
    return version_;
}
//---------------------------------------------------------------------------//
//                           BUMP VERSION NUMBER                             //
//---------------------------------------------------------------------------//
void Neighbor::bumpVersion() {
    iam(classname_, __FUNCTION__);
    version_ += 1;
}

//---------------------------------------------------------------------------//
//                        SET LAST HEARTBEAT                                 //
//---------------------------------------------------------------------------//
void Neighbor::setLastHeartbeat() {    
    //iam(classname_, __FUNCTION__);
    gettimeofday(&monitor.last_heartbeat_,0);
}

//---------------------------------------------------------------------------//
//                                ACTIVATE                                   //
//---------------------------------------------------------------------------//
void Neighbor::activate() {
    iam(classname_, __FUNCTION__);
    active_ = true;
}

//---------------------------------------------------------------------------//
//                               DEACTIVATE                                  //
//---------------------------------------------------------------------------//
void Neighbor::deactivate() {
    iam(classname_, __FUNCTION__);
    active_ = false;
}
//---------------------------------------------------------------------------//
//                                CONNECT                                    //
//---------------------------------------------------------------------------//
void Neighbor::connect() {
    iam(classname_, __FUNCTION__);
    connected_ = true;
}
//---------------------------------------------------------------------------//
//                               IS ACTIVE                                   //
//---------------------------------------------------------------------------//
bool Neighbor::isActive() {
    iam(classname_, __FUNCTION__);
    struct timeval now;    
    double interval;
    gettimeofday(&now, 0);

    active_ = (difftime_ms(now, monitor.last_heartbeat_) < monitor.ttl_ms_);        
    return active_;
}
//---------------------------------------------------------------------------//
//                               IS CONNECTED                                //
//---------------------------------------------------------------------------//
bool Neighbor::isConnected() {
    return connected_;
}
//---------------------------------------------------------------------------//
//                                 PRINT                                     //
//---------------------------------------------------------------------------//
void Neighbor::print() {
    std::cout << "Router: " << router_id_ \
              << " Neighbor: " << neighbor_id_ \
              << " Version: " << version_ \
              << " Cost: " << link_cost_ \
              << " Active: " << active_ \
              << " Connected: " << connected_ << std::endl;
}

//---------------------------------------------------------------------------//
//                                SERIALIZE                                  //
//---------------------------------------------------------------------------//
void Neighbor::serialize(char* buffer, size_t& offset) {
    iam(classname_, __FUNCTION__);
    serialize(buffer, offset);
}
//---------------------------------------------------------------------------//
//                                DESERIALIZE                                //
//---------------------------------------------------------------------------//
void Neighbor::deserialize(char* buffer, size_t& offset) {
    iam(classname_, __FUNCTION__);        
    deserialize(buffer, offset);
}
//---------------------------------------------------------------------------//
//                                  HEY                                      //
//---------------------------------------------------------------------------//
bool Neighbor::hey() {
    iam(classname_, __FUNCTION__);
	holla(classname_, address_);    
    return true;
}
//===========================================================================//
//                               UNARY FUNCTIONS                             //
//===========================================================================//
// Used to generate an iterator to a Route object specified by a route_id. 
FindNeighbor::FindNeighbor() {}
FindNeighbor::FindNeighbor(short neighbor_id) 
    : neighbor_id_(neighbor_id)
    {}

bool FindNeighbor::operator()(Neighbor* neighbor) {    
    return neighbor->getNeighborId() == neighbor_id_;
}
//---------------------------------------------------------------------------//
FindActiveNeighbors::FindActiveNeighbors() {}
bool FindActiveNeighbors::operator()(Neighbor* neighbor) {
    return neighbor->isActive();
}
//===========================================================================//
//                              NEIGHBORHOOD                                 //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              CONSTRUCTORS                                 //
//---------------------------------------------------------------------------//
Neighborhood::Neighborhood() 
    : classname_("Neighborhood")
{}
Neighborhood::Neighborhood(short router_id) 
    : router_id_(router_id), classname_("Neighborhood")
{
    neighbors_.reserve(N_NODES);
    createNeighborhood();  
}
Neighborhood::Neighborhood(Parameters* parameters) 
    : router_id_(parameters->router_id_), 
      cost_filename_(parameters->cost_filename_),
      ttl_ms_(parameters->ttl_factor_ * parameters->heart_rate_),      
      classname_("Neighborhood")
    {
        std::string* address_ = (std::string*)this;
        neighbors_.reserve(N_NODES);
        createNeighborhood();  
        loadCosts();                 
        
    }
//---------------------------------------------------------------------------//
//                               LOAD COSTS                                  //
//---------------------------------------------------------------------------//
void Neighborhood::loadCosts() {
    iam(classname_, __FUNCTION__);

    short node;
    int cost;
    std::string n_string, line;
    std::ifstream costfile (cost_filename_.c_str());

    if (costfile.is_open()) {        

        while(getline(costfile, line)) {            

            n_string = line.substr(0, line.find(' '));
            std::istringstream(n_string) >> node;
            
            n_string = line.substr(line.find(' ')+1, line.find('\n'));
            std::istringstream(n_string) >> cost;

            setCost(node, cost);
        }
    } else {
        std::string message = "Could not open " + cost_filename_;
        error(classname_, __FUNCTION__, message);
        exit(1);
    }    
    costfile.close();    
}    
//---------------------------------------------------------------------------//
//                              CREATE NEIGHBOR                              //
//---------------------------------------------------------------------------//
void Neighborhood::createNeighbor(short neighbor_id, int link_cost) {
    //iam(classname_, __FUNCTION__);

    if (neighborExists(neighbor_id)) {
        std::string message = "Neighbor " + intToString(neighbor_id) + " already exists.";
        error(classname_, __FUNCTION__, message);
        exit(1);        
    }
    Neighbor* neighbor = new Neighbor(router_id_, neighbor_id, link_cost, ttl_ms_);    
    neighbors_.push_back(neighbor);
}

//---------------------------------------------------------------------------//
//                              CREATE NEIGHBORHOOD                          //
//---------------------------------------------------------------------------//
void Neighborhood::createNeighborhood() {
    iam(classname_, __FUNCTION__);

    // initializes costs for each node to 1;
    for (short i=0; i< N_NODES; ++i) {
        if (i != router_id_) {
            createNeighbor(i,DEFAULT_COST);            
        }
    }
}
//---------------------------------------------------------------------------//
//                                 GET NEIGHBOR                              //
//---------------------------------------------------------------------------//
Neighbor* Neighborhood::getNeighbor(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        return neighborNotFound();
    } else {        
        return (*it);
    }
}
//---------------------------------------------------------------------------//
//                          GET ALL NEIGHBORS                                //
//---------------------------------------------------------------------------//
std::vector<Neighbor*> Neighborhood::getAllNeighbors() {
    iam(classname_, __FUNCTION__);        
    return neighbors_;
}
//---------------------------------------------------------------------------//
//                             GET NEIGHBORS                                 //
//---------------------------------------------------------------------------//
std::vector<Neighbor*> Neighborhood::getActiveNeighbors() {
    iam(classname_, __FUNCTION__);
    
    std::vector<Neighbor*> neighbors;
    FindActiveNeighbors active = FindActiveNeighbors();
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), active);        
    
    while(it!=neighbors_.end()) {
        if ((*it)->getNeighborId() != router_id_) neighbors.push_back(*it);
        it = std::find_if (++it, neighbors_.end(), active);  
    } 
    return neighbors;
}
//---------------------------------------------------------------------------//
//                                 GET RECIPIENTS                            //
//---------------------------------------------------------------------------//
std::vector<Neighbor*> Neighborhood::getRecipients(std::vector<short> path) {
    iam(classname_, __FUNCTION__);
    
    short id;
    std::vector<Neighbor*> neighbors;
    std::vector<short>::iterator sit;   
    FindActiveNeighbors active = FindActiveNeighbors();
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), active);        
    
    while(it!=neighbors_.end()) {
        id = (*it)->getNeighborId();
        if (id != router_id_) {
            sit = std::find(path.begin(), path.end(), id);
            if(sit == path.end()) neighbors.push_back(*it);
        }
        it = std::find_if (++it, neighbors_.end(), active);  
    } 
    return neighbors;
}
//---------------------------------------------------------------------------//
//                                 GET RECIPIENT IDS                         //
//---------------------------------------------------------------------------//
std::vector<short> Neighborhood::getRecipientIds(std::vector<short> path) {
    iam(classname_, __FUNCTION__);
    
    short id;
    std::vector<short> neighbors;
    std::vector<short>::iterator sit;   
    FindActiveNeighbors active = FindActiveNeighbors();
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), active);        
    
    while(it!=neighbors_.end()) {
        id = (*it)->getNeighborId();
        if (id != router_id_) {
            sit = std::find(path.begin(), path.end(), id);
            if(sit == path.end()) neighbors.push_back(id);
        }
        it = std::find_if (++it, neighbors_.end(), active);  
    } 
    return neighbors;
}
//---------------------------------------------------------------------------//
//                            GET NEIGHBOR IDS                               //
//---------------------------------------------------------------------------//
std::vector<short> Neighborhood::getNeighborIds() {
    iam(classname_, __FUNCTION__);

    std::vector<short> ids;
    size_t n_neighbors = neighbors_.size();
    std::vector<Neighbor*>::iterator it;
    ids.reserve(n_neighbors);

    for (it=neighbors_.begin();it!=neighbors_.end(); ++it) {
        if ((*it)->getNeighborId() != router_id_) {
            ids.push_back((*it)->getNeighborId());
        }
    }
    return ids;
}
//---------------------------------------------------------------------------//
//                               GET ROUTER ID                               //
//---------------------------------------------------------------------------//
short Neighborhood::getRouterId() {
    return router_id_;    
}
//---------------------------------------------------------------------------//
//                             NEIGHBOR NOT FOUND                            //
//---------------------------------------------------------------------------//
Neighbor* Neighborhood::neighborNotFound() {
    iam(classname_, __FUNCTION__);

    Neighbor* neighbor = new Neighbor(NODE_NOT_FOUND, NEIGHBOR_NOT_FOUND, COST_NOT_FOUND, 0);
    return neighbor;
}


//---------------------------------------------------------------------------//
//                           GET DOWN NEIGHBORS                              //
//---------------------------------------------------------------------------//
std::vector<Neighbor*> Neighborhood::getDownNeighbors() {
    iam(classname_, __FUNCTION__);    

    std::vector<Neighbor*> f;
    f.reserve(N_NODES);
    std::vector<Neighbor*>::iterator it;
    for (it = neighbors_.begin();it!=neighbors_.end();++it) {
        if((*it)->isConnected() && ! (*it)->isActive()) {            
            f.push_back(*it);   
        }
    }
    return f;
} 

//---------------------------------------------------------------------------//
//                                GET COST                                   //
//---------------------------------------------------------------------------//
int Neighborhood::getCost(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) +" does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    return (*it)->getCost();
}
//---------------------------------------------------------------------------//
//                                SET COST                                   //
//---------------------------------------------------------------------------//
int Neighborhood::setCost(short neighbor_id, int link_cost) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) +" does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    (*it)->setCost(link_cost);
    leaving(classname_, __FUNCTION__);    
}
//---------------------------------------------------------------------------//
//                             BUMP LINK VERSION                             //
//---------------------------------------------------------------------------//
void Neighborhood::bumpVersion(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it==neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) + " does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    (*it)->bumpVersion();
}
//---------------------------------------------------------------------------//
//                           SET LAST HEARTBEAT                              //
//---------------------------------------------------------------------------//
void Neighborhood::setLastHeartbeat(short neighbor_id) {
    //iam(classname_, __FUNCTION__);

    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
        
    if (it == neighbors_.end()) {        
        std::string message = "Node " + intToString(neighbor_id) + " not found.";
        error(classname_, __FUNCTION__, message);
        exit(1);
    } else {
        (*it)->setLastHeartbeat();
    }
}
//---------------------------------------------------------------------------//
//                             ACTIVATE NEIGHBOR                             //
//---------------------------------------------------------------------------//
void Neighborhood::activateNeighbor(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it==neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) + " does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    (*it)->activate();
}
//---------------------------------------------------------------------------//
//                            DEACTIVATE NEIGHBOR                            //
//---------------------------------------------------------------------------//
void Neighborhood::deactivateNeighbor(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it==neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) + " does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    (*it)->deactivate();
}
//---------------------------------------------------------------------------//
//                             CONNECT NEIGHBOR                              //
//---------------------------------------------------------------------------//
void Neighborhood::connectNeighbor(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it==neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) + " does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    (*it)->connect();
}

//---------------------------------------------------------------------------//
//                              NEIGHBOR EXISTS                              //
//---------------------------------------------------------------------------//
bool Neighborhood::neighborExists(short neighbor_id) {
    //iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it != neighbors_.end()) {
        return true;        
    } else {        
        return false;
    }
}
//---------------------------------------------------------------------------//
//                              NEIGHBOR ACTIVE                              //
//---------------------------------------------------------------------------//
bool Neighborhood::neighborActive(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        return false;
    } else {        
        return (*it)->isActive();
    }
}
//---------------------------------------------------------------------------//
//                              NEIGHBOR CONNECTED                           //
//---------------------------------------------------------------------------//
bool Neighborhood::neighborConnected(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        return false;
    } else {        
        return (*it)->isConnected();
    }
}

//---------------------------------------------------------------------------//
//                                  HEY                                      //
//---------------------------------------------------------------------------//
bool Neighborhood::hey() {
    return true;
}
//---------------------------------------------------------------------------//
//                                  PRINT                                    //
//---------------------------------------------------------------------------//
void Neighborhood::print() {    
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "                      Neighborhood" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::vector<Neighbor*>::iterator it;
    for (it=neighbors_.begin();it != neighbors_.end(); ++it) {
        (*it)->print();
    }
}
