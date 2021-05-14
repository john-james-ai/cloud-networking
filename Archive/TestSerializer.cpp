#include "assert.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include "Routes.h"
#include "Neighborhood.h"
#include "PacketFactory.h"
#include "utils.h"
struct Link;
struct Links;
class Routes;
class Route;
class Neighbor;
//===========================================================================//
//                                  TEST                                     // 
//===========================================================================//
class TestSerializer {
    public:
        TestSerializer() {}

        Route* buildRoute(short path_len) {
            
            std::vector<short> path;
            Route* route = new Route();
            Links* links = Links();
            for (int i=0;i<path_len;++i) {
                path.push_back(i);
                if (i<path_len-1) {
                    links->createLink(i,i+1);
                }
            }
            route->router_id_  = path_len;
            route->neighbor_id_ = path_len-1;
            route->dest_id_ = 0;
            route->link_cost_ = 10;
            route->cost_ = path_len* 10;
            route->path_ = path;    
            route->links_ = links;
            return route;
        }        

        void testSerialization() {

            std::cout << "TestSerializer: Route" << std::endl;  
            Routes routes = Routes();
            Route* route = buildRoute(10); 
            char* buffer = new char[MAX_BUFFER_LENGTH];
            
            Packet* packet = new Packet();
            packet->header.packet_type_ = ROUTE_PACKET;
            packet->announceRoute(route);
            packet->serialize(buffer);

            Packet* unpack = new Packet();
            unpack->deserialize(buffer);

            Route* r1 = packet->route_;
            Route* r2 = unpack->route_;
            r1->print();
            r2->print();
            assert(routes.routesEqual(r1,r2));
            std::cout << "TestSerializer: Route Complete" << std::endl;  
        }
     
        void main() {
            std::cout << "\nTestSerializer" << std::endl;  
            testSerialization();            
            std::cout << "\n\nTestSerializer Complete" << std::endl;  
        };
};

int main() {
    TestSerializer().main();
}
 