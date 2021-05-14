#include "assert.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>


#include "Route.h"
#include "ForwardingTable.h"
#include "utils.h"
//===========================================================================//
//                                  TEST                                     // 
//===========================================================================//
class TestForwardingTable {
    public:
        TestForwardingTable() {}

        void testForwardingTable() {       
            std::cout << "\nTestForwardingTable: Add Route" << std::endl;  
            int i;
            Route route = Route();
            Routes* ft = new ForwardingTable();

            for (i=0;i<10;++i) {
                route.route_id_ = formatRouteId(i, i+10);
                route.neighbor_id_ = i;
                route.dest_id_ = i+10;
                route.link_cost_ = i+20;
                route.cost_ = i*30;
                route.path_ = makePath(5*i+2,i+2);
                ft->addRoute(route);
            }
            // should register some error
            route.route_id_ = formatRouteId(0, 0+10);
            route.neighbor_id_ = 0;
            route.dest_id_ = 0+10;
            route.link_cost_ = 0+20;
            route.cost_ = 0*30;
            route.path_ = makePath(5*0+2,0+2);
            //ft->addRoute(route); // works

            std::cout << "\nTestForwardingTable: Path Exists" << std::endl;  
            assert(ft->pathExists(route) == true);
            route.path_ = makePath(10,3);            
            assert(ft->pathExists(route) == false);

            std::cout << "\nTestForwardingTable: Route Exists" << std::endl;                          
            assert(ft->routeExists(route) == true);
            route.dest_id_ = 4;
            assert(ft->routeExists(route) == false);
        
            std::cout << "\nTestForwardingTable: Get Route" << std::endl;  
            for (i=0;i<10;++i) {
                route = ft->getRoute(i+10);
                route.print();
                //assert(route.neighbor_id_ != NEIGHBOR_NOT_FOUND);
            }

            std::cout << "\nTestForwardingTable: Activations" << std::endl;  
            for (i=0;i<10;++i) {
                if (i%2==0) {
                    ft->deactivateRoute(i+10);
                }
            };
            assert(ft->getInactiveRoutes().size() == ft->getInactiveDestinations().size());

            ft->deactivateRoutes();
            assert(ft->getRoutes().size() == ft->getInactiveRoutes().size());

            std::cout << "\nTestForwardingTable: Delete Route" << std::endl;  
            ft->deleteRoute(15);
            assert(ft->getRoute(15).neighbor_id_ == NEIGHBOR_NOT_FOUND);



            std::cout << "\nTestForwardingTable: Get Next Hop" << std::endl;  
            ft->activateRoute(16);
            assert(ft->getNextHop(16) == 6);

            std::cout << "\nTestForwardingTable: View" << std::endl;  
            for (i=0;i<10;++i) {
                route = ft->getRoute(i+10);
                std::cout << "Neighbor: " << route.neighbor_id_ << \
                             " Destination: " << route.dest_id_ << \
                             " Link Cost: " << route.link_cost_ << \
                             " Cost: " << route.cost_ << \
                             " Path Length: " << route.path_.size() << std::endl;
            }
            std::cout << "\nTestForwardingTable: Test Complete!" << std::endl;  
        }

        void main() {
            testForwardingTable();            
        };
};

int main() {
    TestForwardingTable().main();
}
 