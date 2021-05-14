#ifndef NETWORK_H
#define NETWORK_H

#include <algorithm>
#include <arpa/inet.h>
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

#include "Constants.h"
#include "Packet.h"
#include "Parameters.h"
#include "Socket.h"

class Parameters;
class Packet;
class Messenger;
class Socket;
//---------------------------------------------------------------------------//
//                                    NODE                                   //
//---------------------------------------------------------------------------//
class Node {
    private:        
        short node_id_;
        char ip_addr_[100];        
        sockaddr_in socket_addr_;    
        bool active_;
        std::string classname_, address_;        

        void setIPAddr();
        void setSocketAddr();

    public:
        Node();
        Node(short node_id);

        short getNodeId();
        void getSocketAddr(sockaddr_in& sock_addr);
        std::string getIPAddr();    

        bool hey();

};
//---------------------------------------------------------------------------//
//                               UNARY FUNCTIONS                             //
//---------------------------------------------------------------------------//
class FindNode {
    private:
        short node_id_;
    public:
        FindNode(short node_id);             
        bool operator()(Node node);
};
class FindActiveNetwork {
    public:
        FindActiveNetwork();
        bool operator()(Node node);
};
//---------------------------------------------------------------------------//
//                                NEIGHBORS                                  //
//---------------------------------------------------------------------------//
class Network {
    private:
        short router_id_;                
        std::vector<Node> network_;
        std::vector<Node>::iterator node_iterator_;                
        Socket* socket_;
        int socket_fd_;
        std::string classname_, address_;            
        int n_network_;
        void createNetwork();
           
    public:
        Network();
        Network(Parameters* params, Socket* socket);
        void start();
        void createNode(short node_id);
        bool nodeExists(short node_id);

        Node getNode(short node_id);
        std::vector<Node> getNetwork();    
        std::vector<short> getNodeIds();

        void getSocketAddr(short node_id, sockaddr_in& sock_addr);

        int getCost(short node_id);
        void setCost(short node_id, int cost);
        
        size_t nNodes();
        
        bool hey();
        std::string name();

        void printNode(short node_id);
        void printNetwork();

};
#endif