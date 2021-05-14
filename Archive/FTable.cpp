#include <algorithm>

#include "FTable.h"
//===========================================================================//
//                                 FTABLE                                    //
//===========================================================================//
//---------------------------------------------------------------------------//
//                            UNARY FUNCTIONS                                //
//---------------------------------------------------------------------------//
FindDestination::FindDestination(short dest_id) 
    : dest_id_(dest_id)
    {}

bool FindDestination::operator()(Route* route) {
    return route->dest_id_ == dest_id_;
}
//---------------------------------------------------------------------------//
//                             CONSTRUCTOR                                   //
//---------------------------------------------------------------------------//
FTable::FTable() {}
FTable::FTable(Parameters* parameters) 
    :   router_id_(parameters->router_id_)
    {}
//---------------------------------------------------------------------------//
//                              ADD ROUTE                                    //
//---------------------------------------------------------------------------//
void FTable::addRoute(Route* route) {
    iam(classname_, __FUNCTION__);            
                     
    FindDestination aRoute = FindDestination(route->dest_id_);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute); 

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
//                               GET ROUTE                                   //
//---------------------------------------------------------------------------//
Route* FTable::getRoute(short dest_id) {
    iam(classname_, __FUNCTION__);

    FindDestination aRoute = FindDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute);     

    if (it==routes_.end()) {
        return routeNotFound();
    } else {
        return (*it);
    }    
}
//---------------------------------------------------------------------------//
//                               GET ROUTES                                  //
//---------------------------------------------------------------------------//
std::vector<Route*> FTable::getRoutes() {
    iam(classname_, __FUNCTION__);
    return routes_;
}
//---------------------------------------------------------------------------//
//                            ROUTE NOT FOUND                                //
//---------------------------------------------------------------------------//
Route* FTable::routeNotFound() {
    std::vector<short> path;
    Route* rnf = new Route(DEFAULT_NODE, DEFAULT_NODE, DEFAULT_COST);
    return rnf;
}
//---------------------------------------------------------------------------//
//                              DELETE ROUTE                                 //
//---------------------------------------------------------------------------//
void FTable::deleteRoute(short dest_id) {
    iam(classname_, __FUNCTION__);

    Route* route;

    FindDestination aRoute = FindDestination(dest_id);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute);   

    for (it = routes_.begin(); it != routes_.end(); ++ it) {
        delete *it;
        it = routes_.erase(it);
    }
}    
//---------------------------------------------------------------------------//
//                              ROUTE EXISTS                                 //
//---------------------------------------------------------------------------//
bool FTable::equalRouteExists(Route* route) {
    iam(classname_, __FUNCTION__);

    FindExactRoute aRoute = FindExactRoute(route);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute);     

    if (it==routes_.end()) {
        return false;
    } else {
        return true;
    }
}
//---------------------------------------------------------------------------//
//                             ALT ROUTE EXISTS                              //
//---------------------------------------------------------------------------//
bool FTable::altRouteExists(Route* route) {
    iam(classname_, __FUNCTION__);

    FindDestination aRoute = FindDestination(route->dest_id_);
    std::vector<Route*>::iterator it = 
        std::find_if(routes_.begin(), routes_.end(), aRoute);     

    if (it==routes_.end()) {
        return false;
    } else {
        return true;
    }
}    
//---------------------------------------------------------------------------//
//                                   HEY                                     //
//---------------------------------------------------------------------------//
bool FTable::hey() {
    holla(classname_, address_);
    return true;
}
