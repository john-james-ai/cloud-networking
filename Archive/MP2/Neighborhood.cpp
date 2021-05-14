 #include "Neighborhood.h"
//===========================================================================//
//                                NEIGHBOR                                   //
//===========================================================================//
Neighbor::Neighbor()
    : current_state_(false), prior_state_(false),
      discovered_(false), connected_(false), classname_("Neighbor")
    {
        iam(classname_, __FUNCTION__);
		std::string* address_ = (std::string*)this;    

    };
Neighbor::Neighbor(short router_id, short neighbor_id, int link_cost, int ttl_ms)
    : router_id_(router_id), neighbor_id_(neighbor_id), link_cost_(link_cost),        
      current_state_(false), prior_state_(false), connected_(false),
      discovered_(false),
      classname_("Neighbor")
    {        
        monitor.ttl_ms_ = ttl_ms;        
        monitor.last_heartbeat_ = (struct timeval){0};
		iam(classname_, __FUNCTION__);
		std::string* address_ = (std::string*)this;            
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
//                        SET LAST HEARTBEAT                                 //
//---------------------------------------------------------------------------//
void Neighbor::setLastHeartbeat() {    
    //iam(classname_, __FUNCTION__);
    gettimeofday(&monitor.last_heartbeat_,0);
    connect();
    activate();
}
//---------------------------------------------------------------------------//
//                                CONNECT                                    //
//---------------------------------------------------------------------------//
void Neighbor::connect() {
    iam(classname_, __FUNCTION__);
    connected_ = true;
}
//---------------------------------------------------------------------------//
//                                ACTIVATE                                   //
//---------------------------------------------------------------------------//
void Neighbor::activate() {
    iam(classname_, __FUNCTION__);
    current_state_ = true;
}
//---------------------------------------------------------------------------//
//                               DEACTIVATE                                  //
//---------------------------------------------------------------------------//
void Neighbor::deactivate() {
    iam(classname_, __FUNCTION__);
    current_state_ = false;
}
//---------------------------------------------------------------------------//
//                               IS ACTIVE                                   //
//---------------------------------------------------------------------------//
bool Neighbor::currentState() {
    //iam(classname_, __FUNCTION__);
    struct timeval now;    
    double interval;
    gettimeofday(&now, 0);

    current_state_ = (difftime_ms(now, monitor.last_heartbeat_) < monitor.ttl_ms_);   
    if (current_state_ == false) discovered_ = false;         
    return current_state_;
}
//---------------------------------------------------------------------------//
//                               IS CONNECTED                                //
//---------------------------------------------------------------------------//
bool Neighbor::isConnected() {
    // True if prior state was active and current state is inactive.
    //iam(classname_, __FUNCTION__);
    
    return connected_;
}
//---------------------------------------------------------------------------//
//                               IS ACTIVE                                   //
//---------------------------------------------------------------------------//
bool Neighbor::isActive() {
    // True if prior state was active and current state is inactive.
    //iam(classname_, __FUNCTION__);
    
    return current_state_;
}
//---------------------------------------------------------------------------//
//                               IS FAILING                                  //
//---------------------------------------------------------------------------//
bool Neighbor::isFailing() {
    // True if prior state was active and current state is inactive.
    //iam(classname_, __FUNCTION__);
    
    return (prior_state_ == true && currentState() == false);    
}
//---------------------------------------------------------------------------//
//                             IS RECOVERING                                 //
//---------------------------------------------------------------------------//
bool Neighbor::isRecovering() {
    // True if prior state was active and current state is inactive.
    //iam(classname_, __FUNCTION__);
    return (prior_state_ == false && currentState() == true);
}

//---------------------------------------------------------------------------//
//                             IS DISCOVERED                                 //
//---------------------------------------------------------------------------//
bool Neighbor::isDiscovered() {
    return discovered_;
}
//---------------------------------------------------------------------------//
void Neighbor::discover() {
    discovered_ = true;
}
//---------------------------------------------------------------------------//
//                                 PRINT                                     //
//---------------------------------------------------------------------------//
void Neighbor::print() {
    std::cout << "       Router: " << router_id_ \
              << "     Neighbor: " << neighbor_id_ \
              << "         Cost: " << link_cost_ \
              << "  Prior State: " << prior_state_ \
              << "Current State: " << current_state_ \
              << "   Discovered: " << discovered_ << std::endl;
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
FindConnectedNeighbors::FindConnectedNeighbors() {};

bool FindConnectedNeighbors::operator()(Neighbor* neighbor) {
    return neighbor->isConnected();
}
//---------------------------------------------------------------------------//
FindActiveNeighbors::FindActiveNeighbors() {};

bool FindActiveNeighbors::operator()(Neighbor* neighbor) {
    return neighbor->currentState();
}

//===========================================================================//
//                              NEIGHBORHOOD                                 //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              CONSTRUCTORS                                 //
//---------------------------------------------------------------------------//
Neighborhood::Neighborhood() 
    : classname_("Neighborhood")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;        
}
Neighborhood::Neighborhood(short router_id) 
    : router_id_(router_id), classname_("Neighborhood")
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;        
    neighbors_.reserve(N_NODES);
    createNeighborhood();  
}
Neighborhood::Neighborhood(Parameters* parameters) 
    : router_id_(parameters->router_id_), 
      cost_filename_(parameters->cost_filename_),
      ttl_ms_(parameters->ttl_factor_ * parameters->heart_rate_),      
      classname_("Neighborhood")
    {
        iam(classname_, __FUNCTION__);
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

    int node;
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
    for (int i=0; i< N_NODES; ++i) {
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
//                        GET CONNECTED NEIGHBORS                            //
//---------------------------------------------------------------------------//
std::vector<Neighbor*> Neighborhood::getConnectedNeighbors() {
    iam(classname_, __FUNCTION__);
    
    std::vector<Neighbor*> neighbors;
    FindConnectedNeighbors connected = FindConnectedNeighbors();
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), connected);        
    
    while(it!=neighbors_.end()) {
        if ((*it)->getNeighborId() != router_id_) neighbors.push_back(*it);
        it = std::find_if (++it, neighbors_.end(), connected);  
    } 
    return neighbors;
}
//---------------------------------------------------------------------------//
//                        GET CONNECTED NEIGHBOR IDS                         //
//---------------------------------------------------------------------------//
std::vector<short> Neighborhood::getConnectedNeighborIds() {
    iam(classname_, __FUNCTION__);

    std::vector<short> ids;
    std::vector<Neighbor*>::iterator it;
    std::vector<Neighbor*> connected_neighbors = getConnectedNeighbors();

    for (it=connected_neighbors.begin(); it!= connected_neighbors.end(); ++it) {
        ids.push_back((*it)->getNeighborId());
    }
    return ids;    
}
//---------------------------------------------------------------------------//
//                        GET ACTIVE NEIGHBORS                               //
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
//                      GET ACTIVE NEIGHBOR IDS                              //
//---------------------------------------------------------------------------//
std::vector<short> Neighborhood::getActiveNeighborIds() {
    iam(classname_, __FUNCTION__);
    
    std::vector<short> neighbors;
    FindActiveNeighbors active = FindActiveNeighbors();
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), active);        
    
    while(it!=neighbors_.end()) {
        if ((*it)->getNeighborId() != router_id_) {
            neighbors.push_back((*it)->getNeighborId());
        }
        it = std::find_if (++it, neighbors_.end(), active);  
    } 
    return neighbors;
}
//---------------------------------------------------------------------------//
//                                 GET RECIPIENTS                            //
//---------------------------------------------------------------------------//
std::vector<Neighbor*> Neighborhood::getRecipients(std::vector<short> path) {
    iam(classname_, __FUNCTION__);
    
    int id;
    std::vector<Neighbor*> neighbors;
    std::vector<short>::iterator sit;   
    FindConnectedNeighbors connected = FindConnectedNeighbors();
    std::vector<Neighbor*>::iterator it = \
        std::find_if(neighbors_.begin(), neighbors_.end(), connected);        
    
    while(it!=neighbors_.end()) {
        id = (*it)->getNeighborId();
        if (id != router_id_) {
            sit = std::find(path.begin(), path.end(), id);
            if(sit == path.end()) neighbors.push_back(*it);
        }
        it = std::find_if (++it, neighbors_.end(), connected);  
    } 
    return neighbors;
}
//---------------------------------------------------------------------------//
//                                 GET RECIPIENT IDS                         //
//---------------------------------------------------------------------------//
std::vector<short> Neighborhood::getRecipientIds(std::vector<short> path) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    std::vector<Neighbor*> recipients = getRecipients(path);
    std::vector<short> ids;

    for (it=recipients.begin();it!=recipients.end();++it) {
        ids.push_back((*it)->getNeighborId());
    }

    return ids;
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
std::vector<short> Neighborhood::getDownNeighborIds() {
    iam(classname_, __FUNCTION__);   

    std::vector<short> f;
    std::vector<Neighbor*>::iterator it;
    std::vector<Neighbor*> connected_neighbors = getConnectedNeighbors();    
    f.reserve(connected_neighbors.size());
    

    for (it = connected_neighbors.begin();it!=connected_neighbors.end();++it) {
        if((*it)->isFailing()) {            
            f.push_back((*it)->getNeighborId());
        }
        (*it)->prior_state_ = (*it)->current_state_;
        
    }
    return f;
} 
//---------------------------------------------------------------------------//
//                              IS DISCOVERED                                //
//---------------------------------------------------------------------------//
bool Neighborhood::isDiscovered(short neighbor_id) {
    // returns true if neighbor has been discovered
    iam(classname_, __FUNCTION__);  


    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) +" does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    } else {        
        return (*it)->isDiscovered();
    }      
}
//---------------------------------------------------------------------------//
//                                DISCOVER                                   //
//---------------------------------------------------------------------------//
void Neighborhood::discover(short neighbor_id) {
    // returns true if neighbor has been discovered
    iam(classname_, __FUNCTION__);  


    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) +" does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    } else {        
        (*it)->discover();
    }      
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
//                           HEAR FROM NEIGHBOR                              //
//---------------------------------------------------------------------------//
void Neighborhood::hearFromNeighbor(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it==neighbors_.end()) {
        std::string msg = "Neighbor " + intToString(neighbor_id) + " does not exist.";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
    }
    (*it)->setLastHeartbeat();
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
//                           NEIGHBOR ACTIVE                                 //
//---------------------------------------------------------------------------//
bool Neighborhood::neighborActive(short neighbor_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Neighbor*>::iterator it;
    FindNeighbor theNeighbor = FindNeighbor(neighbor_id);

    it = std::find_if(neighbors_.begin(), neighbors_.end(), theNeighbor);
    if (it == neighbors_.end()) {
        return false;
    } else {        
        return (*it)->currentState();
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
