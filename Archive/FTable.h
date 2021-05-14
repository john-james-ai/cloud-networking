#ifndef FTABLE_H
#define FTABLE_H

#include <ctime>
#include <string>
#include <vector>

#include "Parameters.h"
#include "Constants.h"
#include "Routes.h"
#include "Neighborhood.h"

class Route;
class FindRoute;
class Link;

//===========================================================================//
//                                 FTABLE                                    //
//===========================================================================//
//---------------------------------------------------------------------------//
//                            UNARY FUNCTIONS                                //
//---------------------------------------------------------------------------//
class FindDestination {
    private:
        short dest_id_;
    public:
        FindDestination(short dest_id);             
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FTable {
    private:
        // Initialization data
        short router_id_;
        std::string classname_, address_;           

        // RouteInfoBase Components
        std::vector<Link> links_;
        std::vector<Route*> routes_;        
        Route* routeNotFound();

    public: 
        
        FTable();
        FTable(Parameters* parameters);

        // core methods
        void addRoute(Route* route);
        Route* getRoute(short dest_id);          
        std::vector<Route*> getRoutes();      
        void deleteRoute(short dest_id);
        bool equalRouteExists(Route* route);
        bool altRouteExists(Route* route);       
        bool hey();
};
#endif