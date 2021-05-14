#ifndef FTABLE_H
#define FTABLE_H

#include <ctime>
#include <string>
#include <vector>

#include "Parameters.h"
#include "Constants.h"
#include "Routes.h"

//===========================================================================//
//                                 FTABLE                                    //
//===========================================================================//
struct NextHop {    
    short neighbor_id_;
    short dest_id_;
    
};
//---------------------------------------------------------------------------//
class FindHop {
    private:
        short dest_id_;
    public:
        FindHop(short dest_id);             
        bool operator()(NextHop* hop);
};
//---------------------------------------------------------------------------//
class FindNeighborHop {
    private:
        short neighbor_id_;
        short dest_id_;
    public:
        FindNeighborHop(short neighbor_id, short dest_id);             
        bool operator()(NextHop* hop);
};
//---------------------------------------------------------------------------//
class FTable {
    private:
        // Initialization data
        short router_id_;
        std::string classname_, message_, address_;           
        std::vector<NextHop*> ftable_;              

    public: 
        
        FTable();
        FTable(Parameters* parameters);

        // core methods
        void addHop(short neighbor_id, short dest_id); 
        void updateHop(short neighbor_id, short dest_id);     
        void postHop(Route* route);
        void postHop(short neighbor_id, short dest_id);
        void removeHop(Route* route);      
        void removeHop(short dest_id);        
        short getNextHop(short dest_id);      
        bool hopExists(short dest_id);                
        bool hopExists(short neighbor_id, short dest_id);                
        bool hopExists(Route* route); 
        void print();      
        bool hey();
};
#endif