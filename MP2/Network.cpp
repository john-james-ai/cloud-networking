 #include "Network.h"


//===========================================================================//
//                               UNARY FUNCTIONS                             //
//===========================================================================//
// Used to generate an iterator to a Route object specified by a route_id. 
FindNode::FindNode(short node_id) 
    : node_id_(node_id)
    {}

bool FindNode::operator()(Node node) {    
    return node.getNodeId() == node_id_;
}

//===========================================================================//
//                               NODE                                        //
//===========================================================================//
//---------------------------------------------------------------------------//
//                           CONSTRUCTORS                                    //
//---------------------------------------------------------------------------//
Node::Node() {}
Node::Node(short node_id) 
    : node_id_(node_id),  active_(false),
      classname_("Node")
    {   
        setIPAddr();
        setSocketAddr();        
        active_ = false; 
        struct timeval last_Heartbeat_ = {0,0};    
        std::string* address_ = (std::string*)this;   
    };

//---------------------------------------------------------------------------//
//                           SET IP ADDR                                     //
//---------------------------------------------------------------------------//
void Node::setIPAddr() {
    sprintf(ip_addr_,"10.1.1.%d", node_id_);    
}
//---------------------------------------------------------------------------//
//                        SET SOCKET ADDR                                    //
//---------------------------------------------------------------------------//
void Node::setSocketAddr() {
    memset(&socket_addr_, 0, sizeof(socket_addr_));
    socket_addr_.sin_family = AF_INET;
    socket_addr_.sin_port = htons(7777);
    inet_pton(AF_INET, ip_addr_, &socket_addr_.sin_addr);        
}
//---------------------------------------------------------------------------//
//                           GET NODE ID                                     //
//---------------------------------------------------------------------------//
short Node::getNodeId() {
    return node_id_;
}
//---------------------------------------------------------------------------//
//                           GET IP ADDR                                     //
//---------------------------------------------------------------------------//
std::string Node::getIPAddr() {
    return ip_addr_;
}
//---------------------------------------------------------------------------//
//                        GET SOCKET ADDR                                    //
//---------------------------------------------------------------------------//
void Node::getSocketAddr(sockaddr_in& sock_addr) {
    sock_addr = socket_addr_;
}
//---------------------------------------------------------------------------//
//                                  HEY                                      //
//---------------------------------------------------------------------------//
bool Node::hey() {
    holla(classname_, address_);
    return true;
};
//===========================================================================//
//                                NETWORK                                  //
//===========================================================================//
//---------------------------------------------------------------------------//
//                              CONSTRUCTORS                                 //
//---------------------------------------------------------------------------//
Network::Network() 
    : classname_("Network")
    {
        iam(classname_, __FUNCTION__);
        std::string* address_ = (std::string*)this;
    }
Network::Network(Parameters* params, Socket* socket) 
    : router_id_(params->router_id_), 
      socket_(socket),
      socket_fd_(socket_->getSocket()),
      n_network_(params->n_network_), 
      classname_("Network")
    {
        iam(classname_, __FUNCTION__);        
        std::string* address_ = (std::string*)this;          
        createNetwork();
    }
//---------------------------------------------------------------------------//
//                               CREATE NODE                                 //
//---------------------------------------------------------------------------//
void Network::createNode(short node_id) {
    iam(classname_, __FUNCTION__);

    if (nodeExists(node_id)) {
        std::string message = "Node " + intToString(node_id) + " already exists.";
        error(classname_, __FUNCTION__, message);
        exit(1);        
    }

    Node node = Node(node_id);    
    network_.push_back(node);
}
//---------------------------------------------------------------------------//
//                              CREATE NETWORK                               //
//---------------------------------------------------------------------------//
void Network::createNetwork() {
    iam(classname_, __FUNCTION__);

    for (int i=0; i< n_network_; ++i) {
        if (i != router_id_) {
            createNode(i);
            
        }
    }    
}
//---------------------------------------------------------------------------//
//                               NODE EXISTS                                 //
//---------------------------------------------------------------------------//
bool Network::nodeExists(short node_id) {
    iam(classname_, __FUNCTION__);

    std::vector<Node>::iterator it;
    FindNode theNode = FindNode(node_id);

    it = std::find_if(network_.begin(), network_.end(), theNode);
    if (it != network_.end()) {
        return true;        
    } else {        
        return false;
    }
}
//---------------------------------------------------------------------------//
//                                 GET NODE                                  //
//---------------------------------------------------------------------------//
Node Network::getNode(short node_id) {
    iam(classname_, __FUNCTION__);

    FindNode theNode = FindNode(node_id);

    node_iterator_ = std::find_if(network_.begin(), network_.end(), theNode);
    if (node_iterator_ == network_.end()) {
        std::string message = "Node " + intToString(node_id) + " not found.";
        error(classname_, __FUNCTION__, message);
        exit(1);
    } else {        
        return *node_iterator_;
    }
}
//---------------------------------------------------------------------------//
//                             GET NODE IDS                                  //
//---------------------------------------------------------------------------//
std::vector<short> Network::getNodeIds() {    
    int i;
    std::vector<short> ids;
    ids.reserve(N_NODES);

    for (i=0; i<N_NODES; ++i) {
        if (i != router_id_) {
            ids.push_back(i);
        }
    }
    return ids;
}
//---------------------------------------------------------------------------//
//                             GET NETWORK                                   //
//---------------------------------------------------------------------------//
std::vector<Node> Network::getNetwork() {
    iam(classname_, __FUNCTION__);
    return network_;
}
//---------------------------------------------------------------------------//
//                              GET SOCKET ADDR                              //
//---------------------------------------------------------------------------//
// This is the full sockaddr_in struct
void Network::getSocketAddr(short node_id, sockaddr_in& sock_addr) {    

    long addr;
    FindNode theNode = FindNode(node_id);

    node_iterator_ = std::find_if(network_.begin(), network_.end(), theNode);
    if (node_iterator_ == network_.end()) {
        std::string message = "Node " + intToString(node_id) + " not found.";
        error(classname_, __FUNCTION__, message);
        exit(1);
    } else {
        (*node_iterator_).getSocketAddr(sock_addr);
    }
}


//---------------------------------------------------------------------------//
//                                 N NODES                                   //
//---------------------------------------------------------------------------//
size_t Network::nNodes() {
    iam(classname_, __FUNCTION__);

    return network_.size();
}
//---------------------------------------------------------------------------//
//                               HEY AND NAME                                //
//---------------------------------------------------------------------------//
bool Network::hey() {
    std::vector<Node>::iterator it;
    assert(network_.size()== 255);
    for (it=network_.begin();it != network_.end();++it) {
        assert((*it).hey());
    }
	holla(classname_, address_);    
    return true;
};
std::string Network::name() {
    return classname_;
}
//---------------------------------------------------------------------------//
//                                  PRINT                                    //
//---------------------------------------------------------------------------//
void Network::printNode(short node_id) {
    Node node = getNode(node_id);
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "                     Node Id: " << node.getNodeId() << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "                  IP Address: " << node.getIPAddr() << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;

}

void Network::printNetwork() {    
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "                             Network" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;    
    std::cout << "      Node Id         IP Address        State" << std::endl;
    std::vector<Node>::iterator it;
    for (it=network_.begin();it != network_.end(); ++it) {
        std::cout << "     " << (*it).getNodeId();
        std::cout << "         " << (*it).getIPAddr();
        std::cout << std::endl;
    }
}
