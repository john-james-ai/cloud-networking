#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include <arpa/inet.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <vector>

#include "Packet.h"
#include "Constants.h"
#include "Parameters.h"
#include "utils.h"

struct Parameters;
//---------------------------------------------------------------------------//
//                                 NEIGHBOR                                  //
//---------------------------------------------------------------------------//
struct Monitor {
    int ttl_ms_;
    struct timeval now_;
    struct timeval last_heartbeat_;    
};
class Neighbor {
    protected:        
        std::string classname_, address_;  
        short router_id_;             
        short neighbor_id_;         
        int version_;       
        int link_cost_;        
        struct Monitor monitor;
        bool discovered_; 
        bool connected_;
    public:
        bool prior_state_;
        bool current_state_;           
        Neighbor();         
        Neighbor(short router_id_, short neighbor_id, int link_cost, int ttl_ms);    
        void initialize();       
        short getRouterId();     
        short getNeighborId();
        int getCost();         
        void setCost(int link_cost);

        void setLastHeartbeat();        
        void connect();
        void activate();
        void deactivate(); 
        void discover(); 

        bool currentState();
        bool isActive();
        bool isConnected();
        bool isFailing();
        bool isRecovering();
        bool isDiscovered();
               

        void print();

        void serialize(char* buffer, size_t& offset);
        void deserialize(char* buffer, size_t& offset);

        bool hey();

};
//---------------------------------------------------------------------------//
//                               UNARY FUNCTIONS                             //
//---------------------------------------------------------------------------//
class FindNeighbor {
    private:
        short neighbor_id_;
    public:
        FindNeighbor();
        FindNeighbor(short neighbor_id);             
        bool operator()(Neighbor* neighbor);
};
//---------------------------------------------------------------------------//
class FindConnectedNeighbors {
    public:
        FindConnectedNeighbors();
        bool operator()(Neighbor* neighbor);
};
//---------------------------------------------------------------------------//
class FindActiveNeighbors {
    public:
        FindActiveNeighbors();
        bool operator()(Neighbor* neighbor);
};
//---------------------------------------------------------------------------//
//                              NEIGHBORS                                    //
//---------------------------------------------------------------------------//
class Neighborhood {
    private:
        std::string classname_, cost_filename_, message_, address_;    
        short router_id_;
        int ttl_ms_;                      
        std::vector<Neighbor*> neighbors_;  

        void loadCosts(); 
        void createNeighborhood();                 
    public:
        Neighborhood();
        Neighborhood(short router_id);
        Neighborhood(Parameters* parameters);        
        void createNeighbor(short neighbor_id, int link_cost);     

        Neighbor* getNeighbor(short neighbor_id);        
        std::vector<Neighbor*> getAllNeighbors();
        std::vector<Neighbor*> getConnectedNeighbors();
        std::vector<short> getConnectedNeighborIds();
        std::vector<Neighbor*> getActiveNeighbors();
        std::vector<short> getActiveNeighborIds();
        std::vector<Neighbor*> getRecipients(std::vector<short> path);
        std::vector<short> getRecipientIds(std::vector<short> path);
        std::vector<short> getNeighborIds(); 
        short getRouterId();
           
                            
        Neighbor* neighborNotFound();        

        std::vector<short> getDownNeighborIds();                   

        bool isDiscovered(short neighbor_id);
        void discover(short neighbor_id);

        int getCost(short neighbor_id);
        int setCost(short neighbor_id, int link_cost);
        
        void setLastHeartbeat(short neighbor_id);
        void activateNeighbor(short neighbor_id);        
        void deactivateNeighbor(short neighbor_id);
        void hearFromNeighbor(short neighbor_id);


        

        bool neighborExists(short neighbor_id);
        bool neighborActive(short neighbor_id);
        bool neighborConnected(short neighbor_id);

        void print();

        bool hey();
};
#endif