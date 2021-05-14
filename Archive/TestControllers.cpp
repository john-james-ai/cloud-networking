#include "assert.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>


#include "EventHandlers.h"
#include "RouteInfoBase.h"
#include "PacketFactory.h"
#include "TestBuild.cpp"
//===========================================================================//
//                              TEST CONTROLLERS                             // 
//===========================================================================//
class Test EventHandler {
    public:
        Test EventHandler() {}

        Router* getRouter() {
            short router_id = 1;
            std::string cost_filename = "testinitcosts1";
            std::string log_filename = "log1.txt";
            Router* router = testBuild(router_id, cost_filename, log_filename);
            return router;
        }

        void test  EventHandler(Router* router) {
            std::cout << "\nTest  EventHandler" << std::endl;  
            std::cout << "\nTest  EventHandler: Building Router" << std::endl;
              EventHandler* controllers = new   EventHandler();
            Cost* cost = new Cost();
            AddRoute* add_route = new AddRoute();
            DropRoute* drop_destination = new DropRoute();
            AckNeighbor* ack_node = new AckNeighbor();
            DeactivateNeighbor* drop_neighbor = new DeactivateNeighbor();
            controllers->attach(cost);
            controllers->attach(add_route);
            controllers->attach(drop_destination);
            controllers->attach(ack_node);
            controllers->attach(drop_neighbor);

            Packet* p = new Packet();
            p->header.packet_type_ = COST_PACKET_TYPE;
            p->neighbor_id_ = 5;
            p->cost_ = 420;

            DataPlane* dp = router->getDataPlane();
            Messenger* messenger = dp->getMessenger();
            RouteInfoBase* cp = router->getRouteInfoBase();
            Network* network = cp->getNetwork();
            Routes* routes = cp->getRoutes();
            Routes* ftable = cp->getForwardingTable();    

            std::cout << "\nTest  EventHandler: Cost  EventHandler" << std::endl;    
            controllers->notify(p);
            std::cout << "\nTest  EventHandler: Cost  EventHandler: Node" << std::endl;    
            Node node = network->getNode(5);            
            assert(node.getCost() == 420);

            std::cout << "\nTest  EventHandler: Cost  EventHandler: Route" << std::endl;    
            Route route = routes->getRoute(formatRouteId(5,5));
            assert(route.node_id_ == 5);
            assert(route.dest_id_ = 5);
            assert(route.cost_ = 420);
            std::vector<short> path;
            path.push_back(1);
            path.push_back(5);
            assert(route.path_ == path);            

            std::cout << "\nTest  EventHandler: Cost  EventHandler: Destination" << std::endl;    
            Destination destination = ftable->getRoute(5);
            destination.node_id_ = 5;
            destination.dest_id_ = 5;
            destination.active_ = true;
            destination.route_.active_ = true;
            destination.route_.cost_ = 420;
            destination.route_.node_id_ = 5;
            destination.route_.dest_id_ = 5;
            assert(destination.route_.path_ == path);

            std::cout << "\nTest  EventHandler: Add Route  EventHandler" << std::endl;    
            std::cout << "\nTest  EventHandler: Add Route  EventHandler: Existing Node" << std::endl;    
            std::vector<short> path_1 = makePath(4,12);
            std::vector<short> path_sans_self_1 = path_1;
            path_1.resize(path_1.size()+1);
            path_1.insert(path_1.begin(),1);            
            p->header.packet_type_ = ORIGINATE_ROUTE_PACKET;
            p->header.recipients_.push_back(1);
            p->neighbor_id_ = 5;
            p->dest_id_ = 120;
            p->cost_ = 17;
            p->path_ = path_sans_self_1;

            controllers->notify(p);

            std::cout << "\nTest  EventHandler: Add Route  EventHandler: Existing Node: Node" << std::endl;    
            node = network->getNode(10);            
            assert(node.getCost() == 17);
            std::vector<Node> nvec = network->getNetwork();
            assert(nvec.size() == 1);

            std::cout << "\nTest  EventHandler: Add Route  EventHandler: Existing Node: Route" << std::endl;                
            route = routes->getRoute(formatRouteId(10,120));
            assert(route.node_id_ == 5);
            assert(route.dest_id_ == 120);
            assert(route.cost_ == 17);
            assert(route.path_ == path_1);
            assert(routes->nRoutes() == 2);

            std::cout << "\nTest  EventHandler: Add Route  EventHandler: Existing Node: Destination" << std::endl;                
            destination = ftable->getRoute(120);
            assert(destination.node_id_ == 5);
            assert(destination.dest_id_ == 120);
            assert(destination.active_ == true);
            assert(destination.route_.active_ == true);
            assert(destination.route_.node_id_ == 10);
            assert(destination.route_.dest_id_ == 120);
            assert(destination.route_.cost_ == 17);
            assert(destination.route_.path_ == path_1);
            std::vector<Destination> dvec = ftable->getForwardingTable();
            assert(dvec.size() == 2);
            


            std::cout << "\nTest  EventHandler: Add Route  EventHandler: New Node" << std::endl;    
            std::vector<short> path_2 = makePath(3,11);
            std::vector<short> path_sans_self_2 = path_2;
            path_2.resize(path_2.size()+1);
            path_2.insert(path_2.begin(),1);            
            p->header.packet_type_ = ORIGINATE_ROUTE_PACKET;
            p->header.recipients_.push_back(1);
            p->neighbor_id_ = 55;
            p->dest_id_ = 120;
            p->cost_ = 10;
            p->path_ = path_sans_self_2;

            controllers->notify(p);

            std::cout << "\nTest  EventHandler: Add Route  EventHandler: New Node: Node" << std::endl;    
            node = network->getNode(55);            
            assert(node.getCost() == 10);
            nvec = network->getNetwork();
            assert(nvec.size() == 2);
            

            std::cout << "\nTest  EventHandler: Add Route  EventHandler: new Node: Route" << std::endl;                
            route = routes->getRoute(formatRouteId(55,120));
            assert(route.node_id_ == 55);
            assert(route.dest_id_ == 120);
            assert(route.cost_ == 10);
            assert(route.path_ == path_2);
            assert(routes->nRoutes() == 3);
            assert(routes->nActiveRoutesThruNeighbor(55) == 2);
            

            std::cout << "\nTest  EventHandler: Add Route  EventHandler: new Node: Destination" << std::endl;                
            destination = ftable->getRoute(120);
            assert(destination.node_id_ == 55);
            assert(destination.dest_id_ == 120);
            assert(destination.active_ == true);
            assert(destination.route_.active_ == true);
            assert(destination.route_.node_id_ == 55);
            assert(destination.route_.dest_id_ == 120);
            assert(destination.route_.cost_ == 10);
            assert(destination.route_.path_ == path_2);  
            dvec = ftable->getForwardingTable();
            assert(dvec.size() == 3);          



            std::cout << "\nTest  EventHandler: Drop Route  EventHandler" << std::endl;    
            p->header.packet_type_ = ORIGINATE_WITHDRAW_ROUTE_PACKET;
            p->header.recipients_.push_back(1);
            p->neighbor_id_ = 55;
            p->dest_id_ = 120;

            controllers->notify(p);

            std::cout << "\nTest  EventHandler: Drop Route  EventHandler: Node" << std::endl;    
            node = network->getNode(10);            
            assert(node.getCost() == 10);
            nvec = network->getNetwork();
            assert(nvec.size() == 2);
            
  
            std::cout << "\nTest  EventHandler: Drop Route  EventHandler: Route" << std::endl;                
            route = routes->getRoute(formatRouteId(10,120));
            assert(route.node_id_ == 55);
            assert(route.dest_id_ == 120);
            assert(route.cost_ == 10);
            assert(route.path_ == path_1);
            assert(routes->nRoutes() == 3);
            assert(routes->nActiveRoutes() ==2);
            assert(routes->nActiveRoutesThruNeighbor(55) == 1);

            std::cout << "\nTest  EventHandler: Drop Route  EventHandler: Destination" << std::endl;                
            destination = ftable->getRoute(120);
            assert(destination.node_id_ == 5);
            assert(destination.dest_id_ == 120);
            assert(destination.active_ == true);
            assert(destination.route_.active_ == true);
            assert(destination.route_.node_id_ == 5);
            assert(destination.route_.dest_id_ == 120);
            assert(destination.route_.cost_ == 17);
            assert(destination.route_.path_ == path_1);  
            dvec = ftable->getForwardingTable();
            assert(dvec.size() == 2);          


            std::cout << "\nTest  EventHandler: Acknowledge Node  EventHandler" << std::endl;                
            p->header.packet_type_ = ADD_NEIGHBOR_PACKET_TYPE;
            p->header.recipients_.push_back(1);
            p->neighbor_id_ = 200;            

            controllers->notify(p);

            std::cout << "\nTest  EventHandler: Acknowledge Node  EventHandler: Node" << std::endl;    
            node = network->getNode(200);            
            assert(node.getCost() == 1);
            nvec = network->getNetwork();
            assert(nvec.size() == 3);
            
            std::cout << "\nTest  EventHandler: Acknowledge Node  EventHandler: Route" << std::endl;                
            route = routes->getRoute(formatRouteId(200,200));
            std::vector<short> path_3;
            path_3.push_back(200);            
            std::vector<short> path_sans_self_3 = path_3;
            path_3.resize(path_3.size()+1);
            path_3.insert(path_3.begin(),1); 
            assert(route.node_id_ == 200);
            assert(route.dest_id_ == 200);
            assert(route.cost_ == 1);
            assert(route.path_ == path_3);
            assert(routes->nRoutes() == 4);
            assert(routes->nActiveRoutes() ==3);
            assert(routes->nActiveRoutesThruNeighbor(200) == 1);

            std::cout << "\nTest  EventHandler: Acknowledge Node   EventHandler: Destination" << std::endl;                
            destination = ftable->getRoute(200);
            assert(destination.node_id_ == 200);
            assert(destination.dest_id_ == 200);
            assert(destination.active_ == true);
            assert(destination.route_.active_ == true);
            assert(destination.route_.node_id_ == 200);
            assert(destination.route_.dest_id_ == 200);
            assert(destination.route_.cost_ == 1);
            assert(destination.route_.path_ == path_3);  
            dvec = ftable->getForwardingTable();
            assert(dvec.size() == 3);                      


            std::cout << "\nTest  EventHandler: Drop Node  EventHandler" << std::endl;                
            p->header.packet_type_ = DROP_NEIGHBOR_PACKET_TYPE;
            p->header.recipients_.push_back(1);
            p->neighbor_id_ = 200;            

            controllers->notify(p);

            std::cout << "\nTest  EventHandler: Drop Node  EventHandler: Node" << std::endl;    
            node = network->getNode(200);            
            assert(node.getNodeId() == NODE_NOT_FOUND);
            nvec = network->getNetwork();
            assert(nvec.size() == 2);
            
            std::cout << "\nTest  EventHandler: Drop Node  EventHandler: Route" << std::endl;                
            route = routes->getRoute(formatRouteId(200,200));
            assert(route.node_id_ == NODE_NOT_FOUND);
            assert(routes->nRoutes() == 3);
            assert(routes->nActiveRoutes() ==2);
            assert(routes->nActiveRoutesThruNeighbor(200) == 0);

            std::cout << "\nTest  EventHandler: Drop Node   EventHandler: Destination" << std::endl;                
            destination = ftable->getRoute(200);
            assert(destination.dest_id_ == DESTINATION_NOT_FOUND);
            dvec = ftable->getForwardingTable();
            assert(dvec.size() == 2);  

            // Clean up
            delete p;
            delete controllers;
            delete cost;
            delete add_route;
            delete drop_destination;
            delete ack_node;
            delete drop_neighbor;

        };

        void main() {            
            std::cout << "\n\nTest EventHandler" << std::endl;  
            Router* router = getRouter();
            test  EventHandler(router);
            delete router;
            std::cout << "\n\nTest EventHandler Complete" << std::endl;  

        };
};

int main() {

    Test EventHandler().main();
}
 