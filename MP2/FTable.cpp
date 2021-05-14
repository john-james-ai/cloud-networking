#include <algorithm>

#include "FTable.h"
//===========================================================================//
//                                 FTABLE                                    //
//===========================================================================//
//---------------------------------------------------------------------------//
//                             CONSTRUCTOR                                   //
//---------------------------------------------------------------------------//
FTable::FTable() 
    : classname_("FTable")
    {
		iam(classname_, __FUNCTION__);        
    };

FTable::FTable(Parameters* parameters) 
    :   router_id_(parameters->router_id_), classname_("FTable")    {                
        
		iam(classname_, __FUNCTION__);
		std::string* address_ = (std::string*)this;            
    }
//---------------------------------------------------------------------------//
//                             UNARY FUNCTIONS                               //
//---------------------------------------------------------------------------//
FindHop::FindHop(short dest_id) 
    : dest_id_(dest_id)
    {}
bool FindHop::operator()(NextHop* hop) {
    return (dest_id_ == hop->dest_id_);
}
//---------------------------------------------------------------------------//
FindNeighborHop::FindNeighborHop(short neighbor_id, short dest_id) 
    : neighbor_id_(neighbor_id), dest_id_(dest_id)
    {}
bool FindNeighborHop::operator()(NextHop* hop) {
    return (neighbor_id_ == hop->neighbor_id_ &&
            dest_id_ == hop->dest_id_);
}
//---------------------------------------------------------------------------//
//                                  ADD HOP                                  //
//---------------------------------------------------------------------------//
void FTable::addHop(short neighbor_id, short dest_id) {
    iam(classname_, __FUNCTION__); 

    NextHop* hop = new NextHop();
    hop->neighbor_id_ = neighbor_id;
    hop->dest_id_ = dest_id;
    ftable_.push_back(hop);
}
//---------------------------------------------------------------------------//
//                                UPDATE HOP                                 //
//---------------------------------------------------------------------------// 
void FTable::updateHop(short neighbor_id, short dest_id) {
    // Changes the neighbor (hop) for a given destination. 
    iam(classname_, __FUNCTION__); 

    FindHop theHop = FindHop(dest_id);
    std::vector<NextHop*>::iterator it = 
        std::find_if(ftable_.begin(), ftable_.end(), theHop);    

    if (it != ftable_.end()) {        
        (*it)->neighbor_id_ = neighbor_id;        
        (*it)->dest_id_ = dest_id;
    } else {
        message_ = "Update failed. Route through " + shortToString(neighbor_id)\
            + " to " + shortToString(dest_id) + " was not found. ";
        note(classname_, __FUNCTION__, message_);
        exit(-1);
    }
}
//---------------------------------------------------------------------------//
//                               POST HOP                                    //
//---------------------------------------------------------------------------// 
void FTable::postHop(short neighbor_id, short dest_id) {
    iam(classname_, __FUNCTION__); 

    if (hopExists(dest_id)) {
        updateHop(neighbor_id, dest_id);
    } else {
        addHop(neighbor_id, dest_id);
    } 
}
//---------------------------------------------------------------------------// 
void FTable::postHop(Route* route) {
    iam(classname_, __FUNCTION__); 
    postHop(route->neighbor_id_, route->dest_id_);

}
//---------------------------------------------------------------------------//
//                              REMOVE ROUTE                                 //
//---------------------------------------------------------------------------//
void FTable::removeHop(Route* route) {
    // Removes a route from the forwarding table.
    iam(classname_, __FUNCTION__);          

    std::vector<NextHop*>::iterator it = ftable_.begin();
    while (it != ftable_.end()) {
    // (it=ftable_.begin(); it != ftable_.end(); ++it) {
        if ((*it)->neighbor_id_ == route->neighbor_id_ &&
            (*it)->dest_id_ == route->dest_id_) {
            it = ftable_.erase(it); 
            break;           
        } else {
            ++it;
        }
    }
}
//---------------------------------------------------------------------------//
void FTable::removeHop(short dest_id) {
    // Removes a route from the forwarding table.
    iam(classname_, __FUNCTION__);          

    std::vector<NextHop*>::iterator it = ftable_.begin();
    while (it != ftable_.end()) {
    // (it=ftable_.begin(); it != ftable_.end(); ++it) {
        if ((*it)->dest_id_ == dest_id) {
            it = ftable_.erase(it); 
            break;           
        } else {
            ++it;
        }
    }
}
//---------------------------------------------------------------------------//
//                             GET NEXT HOP                                  //
//---------------------------------------------------------------------------//
short FTable::getNextHop(short dest_id) {
    // returns the neighbor associated with the destination
    iam(classname_, __FUNCTION__);

    FindHop theHop = FindHop(dest_id);
    std::vector<NextHop*>::iterator it = 
        std::find_if(ftable_.begin(), ftable_.end(), theHop);     

    if (it == ftable_.end()) {
        return DESTINATION_NOT_FOUND;
    }  else {
        return (*it)->neighbor_id_;
    }
}
//---------------------------------------------------------------------------//
//                              HOP EXISTS                                   //
//---------------------------------------------------------------------------//
bool FTable::hopExists(short dest_id) {
    iam(classname_, __FUNCTION__);

    FindHop theHop = FindHop(dest_id);
    std::vector<NextHop*>::iterator it = 
        std::find_if(ftable_.begin(), ftable_.end(), theHop);     

    if (it==ftable_.end()) {
        return false;
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
bool FTable::hopExists(short neighbor_id, short dest_id) {
    iam(classname_, __FUNCTION__);

    FindNeighborHop theHop = FindNeighborHop(neighbor_id, dest_id);
    std::vector<NextHop*>::iterator it = 
        std::find_if(ftable_.begin(), ftable_.end(), theHop);     

    if (it==ftable_.end()) {
        return false;
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
bool FTable::hopExists(Route* route) {
    iam(classname_, __FUNCTION__);

    return hopExists(route->neighbor_id_, route->dest_id_);
}  
//---------------------------------------------------------------------------//
//                                  PRINT                                    //
//---------------------------------------------------------------------------//
void FTable::print() {
    iam(classname_, __FUNCTION__);

    if (VERBOSE) {

        std::cout << "\n=======================" << std::endl;
        std::cout << "    FORWARDING TABLE" << std::endl;
        std::cout << "------------------------" << std::endl;    

        printf("|%5s|%5s|%5s|\n", "From ", "  To ", " Via ");

        std::vector<NextHop*>::iterator it;    
        for (it=ftable_.begin(); it!=ftable_.end(); ++it) {
            printf("|%5s|%5s|%5s|\n", shortToString(router_id_).c_str(),
                                            shortToString((*it)->dest_id_).c_str(),
                                            shortToString((*it)->neighbor_id_).c_str());
        }
    }
}

//---------------------------------------------------------------------------//
//                                   HEY                                     //
//---------------------------------------------------------------------------//
bool FTable::hey() {
    holla(classname_, address_);
    return true;
}
