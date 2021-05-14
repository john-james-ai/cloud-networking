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

struct Link {
    short router_id_;
    short neighbor_id_;
    short version_;    
    void print() {
        std::cout << "LINK: Router id: " << router_id_ << \
                    " Neighbor id: " << neighbor_id_ << \
                    " Version: " << version_ << std::endl;
    }        
    
};
//===========================================================================//
//                                 ROUTE                                     //
//===========================================================================//
class  Route {
    private:
        std::string classname_, message_, address_;
        void formatRouteId();
    public:
        short router_id_;
        std::string route_id_;
        short neighbor_id_;    
        short dest_id_;
        int link_cost_;     
        int cost_;
        std::vector<short> path_;
        std::vector<Link> links_;
        Link failed_link_;
        bool has_failed_link_;
        Route();
        // Direct Route
        Route(short router_id, short neighbor_id,int link_cost); 
        // Testing
        Route(short router_id, short neighbor_id, short dest_id, int link_cost, 
              int cost, std::vector<short> path, std::vector<Link> links);        
        ~Route();
        void initialize();
        void appendLink(short router_id, short neighbor_id, short version);
        void prependLink(Link link); 
        void prependLink(short router_id, short neighbor_id, short version); 
        void prependPath(short router_id);        
        Link getLastLink();
        Link getFailedLink();
        void setFailedLink(short router_id, short neighbor_id, short version);        
        bool linksEqual(Link a, Link b);
        bool linkVectorsEqual(std::vector<Link> a, std::vector<Link> b);   
        void serialize(char* buffer, size_t& offset);
        void deserialize(char* buffer, size_t& offset);
        bool isDirect();
        void print();   
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
class FindRoute {
    private:
        Route* route_;
    public:
        FindRoute(Route* route);             
        bool operator()(Route* route);
};
//---------------------------------------------------------------------------//
class FindLink {
    private:
        Link link_;
    public:
        FindLink(Link link);             
        bool operator()(Link link);
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
class FindRoutesNeighbor {
    private:
        short neighbor_id_;
    public:
        FindRoutesNeighbor(short neighbor_id);
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
class SearchPath {
    private:
        short node_id_;
    public:
        SearchPath(short node_id);
        bool operator()(short node_id);
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
        Route* buildRoute();
        Route* nullRoute();
        void createRoute(short neighbor_id, int link_cost);              
        void deleteRoute(Route* route);       
        void deleteRoute(short neighbor_id, short dest_id);

        // Costs
        void updateCosts(short neighbor_id, int link_cost, int cost_differential); 

        // Boolean Queries         
        bool routesEquivalent(Route* a, Route* b);
        bool routesEqual(Route* a, Route* b);
        bool aRouteExists(short dest_id);
        bool aRouteExists(short neighbor_id, short dest_id);
        bool altRouteExists(Route* route);
        bool equalRouteExists(Route* route);

        // Queries
        Route* getRoute(short neighbor_id, short dest_id);
        Route* getBestRoute(short dest_id);
        short getNextHop(short dest_id);
        std::vector<Route*> getInvalidRoutes(Link failed_link);            
        std::vector<Route*> getBestRoutes();
        std::set<short> getDestinations();         

        bool hey();

        void print();

};
#endif