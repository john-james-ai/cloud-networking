#ifndef ROUTES_H
#define ROUTES_H

#include <algorithm>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <iostream>
#include "Parameters.h"
#include "Constants.h"
#include "utils.h"

//===========================================================================//
//                                 ROUTE                                     //
//===========================================================================//
class  Route {
    private:
        std::string classname_, message_, address_;        
    public:
        short router_id_;
        std::string route_id_;
        short neighbor_id_;    
        short dest_id_;
        int link_cost_;     
        int cost_;
        std::string state_;
        bool active_;
        std::vector<short> path_;
        Route();
        // Direct Route
        Route(short router_id, short neighbor_id,int link_cost); 
        // Standard Route
        Route(short router_id, short neighbor_id, short dest_id, int link_cost, 
              int cost, std::vector<short> path);        
        //~Route();    
        void formatRouteId();

        void activate();
        void service();
        void fail();
        void withdraw();

        bool isActive();
        bool isServicable();
        bool hasFailed();        
        bool isWithdrawn();
        
        void prependPath(short router_id);           
        bool onPath(short router_id);    
        bool notOnPath(short router_id);     
        
        void print();   
        void print(std::string note);
        void print(std::string note, std::vector<short> recipients);
};
//===========================================================================//
//                                 ROUTES                                    //
//===========================================================================//

//---------------------------------------------------------------------------//
//                   UNARY FUNCTIONS: ROUTES & PATHS                         //
//---------------------------------------------------------------------------//
struct SortRoutes {
    SortRoutes() {}
    bool operator()(Route* lhs, Route* rhs) const;
};
//---------------------------------------------------------------------------//
class FindExactRoute {
    private:
        Route* route_;
    public:
        FindExactRoute(Route* route);             
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindExactActiveRoute {
    private:
        Route* route_;
    public:
        FindExactActiveRoute(Route* route);             
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindRoute {
    private:
        Route* route_;
    public:
        FindRoute(Route* route);             
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindActiveRoute {
    private:
        Route* route_;
    public:
        FindActiveRoute(Route* route);             
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindNeighborDestination {
    private:
        short neighbor_id_;
        short dest_id_;
    public:
        FindNeighborDestination(short neighbor_id, short dest_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindActiveNeighborDestination {
    private:
        short neighbor_id_;
        short dest_id_;
    public:
        FindActiveNeighborDestination(short neighbor_id, short dest_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindRoutesNeighbor {
    private:
        short neighbor_id_;
    public:
        FindRoutesNeighbor(short neighbor_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindActiveRoutesNeighbor {
    private:
        short neighbor_id_;
    public:
        FindActiveRoutesNeighbor(short neighbor_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindRoutesDestination {
    private:
        short dest_id_;
    public:
        FindRoutesDestination(short dest_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindActiveRoutesDestination {
    private:
        short dest_id_;
    public:
        FindActiveRoutesDestination(short dest_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindActiveRoutesDestinationAroundNeighbor {
    private:
        short neighbor_id_, dest_id_;
    public:
        FindActiveRoutesDestinationAroundNeighbor(short neighbor_id, short dest_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindActiveRoutesDestinationThruNeighbor {
    private:
        short neighbor_id_, dest_id_;
    public:
        FindActiveRoutesDestinationThruNeighbor(short neighbor_id, short dest_id);
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class SearchPath {
    private:
        short node_id_;
    public:
        SearchPath(short router_id);
        bool operator()(short router_id);
};
//---------------------------------------------------------------------------//
//                               ROUTES                                      //
//---------------------------------------------------------------------------//
class Routes {
    private:
        // Initialization data
        short router_id_;
        std::string classname_, address_;           

        // RouteInfoBase Components
        Route* route_;
        std::vector<Route*> routes_;        
        Route* routeNotFound();

    public: 
        
        Routes();
        Routes(Parameters* parameters);

        // core methods
        void addRoute(Route* route);                           
        Route* createRoute(short neighbor_id, short dest_id, int cost, int link_cost,
                         std::vector<short> path);                           
        void updateRoute(Route* route);   
        void postRoute(Route* route); 

        // Activations / Deactivations  
        void activateRoute(Route* route);

        void deactivateRoute(Route* route);
        void deactivateRoute(short neighbor_id, short dest_id);
        void deactivateRoutesNeighbor(short neighbor_id);
        void deactivateRoutesDestination(short dest_id);
        

        // Comparisons
        bool routesEquivalent(Route* a, Route* b);
        bool routesEqual(Route* a, Route* b);

        // Boolean Queries         
        bool routeExists(short dest_id);
        bool routeExists(short neighbor_id, short dest_id);
        bool routeExists(Route* route);
        bool activeRouteExists(short dest_id);
        bool activeRouteExists(short neighbor_id, short dest_id);
        bool activeRouteExists(Route* route);
        bool routeAroundExists(short neighbor_id, short dest_id);
        bool isBestRoute(Route* route);        

        // Queries
        Route* getRoute(short neighbor_id, short dest_id);                
        Route* getBestRoute(short dest_id);
        Route* getBestRoute(short neighbor_id, short dest_id);  
        Route* getActiveRoute(short neighbor_id, short dest_id);              
        std::vector<Route*> getRoutes(short neighbor_id);
        std::vector<Route*> getBestRoutes();
        std::vector<Route*> getBestRoutes(short neighbor_id);        
        std::vector<Route*> getActiveRoutesNeighbor(short neighbor_id);
        std::vector<Route*> getActiveRoutesNeighbors(std::vector<short> neighbor_ids);
        std::vector<Route*> getActiveRoutesDestination(short dest_id);
        std::vector<Route*> getRoutesToAdvertise(short neighbor_id);
        std::set<short> getActiveDestinations();                 
        std::set<short> getActiveDestinations(short neighbor_id);  
        std::set<short> getDestinations(short neighbor_id);
        std::set<short> getDestinations(std::vector<short> neighbor_ids);


        bool hey();

        void print();
        

};
#endif