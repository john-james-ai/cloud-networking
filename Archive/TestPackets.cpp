#include "assert.h"
#include <iostream>
#include <stdio.h>
#include <string>

#include "Routes.h"
#include "Neighborhood.h"
#include "Packet.h"
#include "Constants.h"
#include "PacketFactory.h"
#include "Parameters.h"
#include "FTable.h"
#include "utils.h"

class Neighbor;
class Neighborhood;
class AbstractPacketFactory;
class RoutePacketFactory;
class WithdrawPacketFactory;
class FailurePacketFactory;
class Route;
class Routes;
struct Link;

//===========================================================================//
//                                TEST LINKS                                 // 
//===========================================================================//
class TestPackets {
    public:
        TestPackets() {}

        int link_cost;
        short dest_id, ttl_ms;

        Routes* routes;
        Route* route;
        Route* exp;
        Route* act;
        Neighbor* recipient;
        Neighborhood* neighborhood;        
        std::vector<short> path;
        std::vector<Link> links;
        
        RouteAnnouncementPacket* rap;
        WithdrawAnnouncementPacket* wap;
        FailureAnnouncementPacket* fap;
        
        RouteProcessPacket* rpp;
        WithdrawProcessPacket* wpp;
        FailureProcessPacket* fpp;

        DataPlanePacket* dpp;
        
        Route* buildRoute(short router_id, short neighbor_id, short dest_id, 
                                int link_cost) {                

            int n_nodes;
            std::vector<short> path;
            std::vector<Link> links;
            Link link = Link();
            n_nodes = std::max(router_id +1 - dest_id,0);

            path.clear();
            links.clear();
            path.reserve(n_nodes);
            links.reserve(n_nodes);

            
            for (int i=dest_id;i < router_id;++i) {
                path.insert(path.begin(),i);                
                link.router_id_ = i;
                link.neighbor_id_ = i+1;
                link.version_ = 0;
                links.insert(links.begin(),link);         
            }

            if (router_id==0) link_cost = 0;

            Route* route = new Route(router_id, neighbor_id, dest_id, link_cost, router_id*link_cost, path, links);
            
            return route;
        };         

        void testRoute() {

            std::cout << "TestPackets: Route" << std::endl;             

            short dest_id = 0;
            short start = 1;
            short end = 20;
            short i = 0;
            link_cost = 10;
            ttl_ms = 150;
            Route* rp;
            Route* null_route;
            std::string message, classname_;
            classname_ = "TestPackets";

            char buffer[MAX_BUFFER_LENGTH]; 

            std::cout << "\n==============BUILD DIRECT ROUTE TO DESTINATION =============  " << std::endl;
            std::cout << "Build direct route to destination from the destination node " << intToString(i) << std::endl;
            act = new Route(i,i,0);          
            routes = new Routes();  
           
            for (i=start;i<end;++i) {

                exp = buildRoute(i,i-1,dest_id,link_cost);
                exp->print();

                std::cout << "\n=============== BUILD NEIGHBORHOOD ===================== " << std::endl;
                std::cout << "Build Neighborhood for Router "<< i << std::endl;                
                recipient = new Neighbor(i,i-1,link_cost ,150);                
                recipient->print();

                rap = new RouteAnnouncementPacket(act,recipient);
                std::cout << "\n=========== " << rap->router_id_ << " CREATES AN ANNOUCEMENT PACKAGE FOR " << recipient->getRouterId() << std::endl;
                act = rap->getRoute();
                act->print();
                
                rap->checkOut();
                std::cout << "\n=========== " << rap->router_id_ << " CHECKS OUT ============== " << std::endl;
                act = rap->getRoute();
                act->print();

                std::cout << "\n=========== " << rap->router_id_ << " SERIALIZES THE ANNOUNCEMENT ============== " << std::endl;
                rap->serialize(buffer);                
                act = rap->getRoute();
                act->print();                                        
                std::cout << "\n=========== " << act->router_id_ << " ANNOUNCES ROUTE TO " << recipient->getRouterId() << std::endl;


                std::cout << "\n\n------------- ROUTER " << i << " DESERIALIZES THE ANNOUNCEMENT FROM ------" << act->router_id_ << "======== " << std::endl;
                neighborhood = new Neighborhood(i);
                neighborhood->setCost(i-1,link_cost);
                null_route = routes->nullRoute();
                rpp =  new RouteProcessPacket(null_route, neighborhood);                
                rpp->deserialize(buffer);
                act = rpp->getRoute();
                message = "Iteration " + intToString(i) + ":  Deserialized Results.";
                note(classname_,__FUNCTION__, message);                         
                act->print();                

                std::cout << "\n------------ CHECK IN ----------------." << std::endl;
                rpp->checkIn();
                message = "Iteration " + intToString(i) + ":  Actual results after checkin.";
                note(classname_,__FUNCTION__, message);         
                act = rpp->getRoute();
                act->print();                         

                std::cout << "\n---------------  EXPECTED RESULT -------------- " << std::endl;
                message = "Iteration " + intToString(i) + " The expected route is as follows.";
                note(classname_,__FUNCTION__, message);    
                exp->print();
                assert(routes->routesEqual(exp,act));

                std::cout << "\n========== ITERATION " << i << " COMPLETE!   =============" << std::endl;

                delete recipient;
                delete neighborhood;
                delete exp;
                delete rpp;
                delete rap;
            }
            std::cout << "TestPackets: Route Complete!" << std::endl;           
        };
        void testWithdraw() {
            std::cout << "\n\nTestPackets: Withdraw" << std::endl;   
            routes = new Routes();          
            exp = new Route(1,2,3);    
            exp->appendLink(1,3,10);
            neighborhood = new Neighborhood(1);
            recipient = neighborhood->getNeighbor(3);
            wap = new WithdrawAnnouncementPacket(exp, recipient);
            wap->checkOut();
            act = wap->getRoute();
            std::cout << "\n\nEXPECTED RESULTS" << std::endl;
            exp->print();
            std::cout << "ACTUAL RESULTS" << std::endl;
            act->print();
            assert (routes->routesEqual(exp,act));
            wap->header.packet_type_ == WITHDRAW_ROUTE_PACKET;        
            delete exp;
            delete routes;
            delete neighborhood;
            delete wap;
            std::cout << "TestPackets: Withdraw Complete!" << std::endl;
        };

        void testFailure() {
            std::cout << "\n\nTestPackets: Failure" << std::endl; 
            routes = new Routes();     
            exp = buildRoute(1,0,10,10);
            exp->setFailedLink(2,3,0);  
            neighborhood = new Neighborhood(1);          
            recipient = neighborhood->getNeighbor(2);
            fap = new FailureAnnouncementPacket(exp, recipient);
            act = fap->getRoute();
            assert (routes->routesEqual(exp,act));
            fap->header.packet_type_ == FAILURE_PACKET;
            delete neighborhood;
            delete fap;
            delete routes;
            std::cout << "TestPackets: Failure: Complete!" << std::endl;
        };

        void main() {
            std::cout << "\nTestPackets" << std::endl; 
            testRoute();
            testWithdraw();
            testFailure();

            std::cout << "\nTestPackets Complete" << std::endl;  
        };
};


class TestFactories {
    public:
        FactoryParams* factory_params;
        Parameters* params;
        CPLogger* logger;
        Route* next_door;
        Route* route_in;
        Route* route_out;
        Link* link;
        TestPackets tp;
        Neighborhood* neighborhood;
        Neighbor* recipient;
        std::string logfilename;
        RoutePacketFactory* rpf;
        WithdrawPacketFactory* wpf;
        FailurePacketFactory* fpf;
        AnnouncementPacket* ap;
        ProcessPacket* pp;
        short router_id, neighbor_id, dest_id, recipient_id, n_nodes;
        int link_cost;
        
        TestFactories() {}

        void startup() {            
            n_nodes = 20;
            router_id = 10;
            neighbor_id = 9;
            dest_id = 0;
            recipient_id = 11;
            link_cost = 15;
            logfilename = std::string("cp_log.txt");

            params = new Parameters();                        
            params->router_id_ = router_id;
            params->rib_log_filename_ = logfilename;            
            factory_params = new FactoryParams();            
            factory_params->parameters_ = params;
            factory_params->neighborhood_ = new Neighborhood(router_id);
            factory_params->routes_ = new Routes();
            factory_params->log_ = new CPLogger(params);            
            next_door = new Route(router_id,neighbor_id, link_cost);
            tp = TestPackets();         
            recipient = new Neighbor(router_id, recipient_id, link_cost, 150);              
        }

        void testRouteFactory() {
            std::cout << "TestFactories: Route Factory" << std::endl; 
            rpf = new RoutePacketFactory(factory_params);  
            route_in = new Route(0,0,0);

            for (short i=0; i<n_nodes; ++i) {
                recipient = new Neighbor(i,i+1, link_cost*i, 150);
                ap = rpf->createAnnouncementPacket(route_in, recipient);
                std::cout << "TestFactories: Route Factory: Announcement Iteration: " << i << std::endl; 
                route_out = ap->getRoute();
                assert(ap->header.packet_type_ == ROUTE_PACKET);
                assert(route_in->route_id_ == route_out->route_id_);
                assert(route_in->neighbor_id_ == route_out->neighbor_id_);
                assert(route_in->dest_id_ == route_out->dest_id_);
                assert(route_in->path_ == route_out->path_);
                assert(route_in->links_.size() == i);
                assert(route_out->links_.size() == i+1);
                std::cout << "\nAnnouncement Route" << std::endl;
                route_out->print();
                std::cout << "TestFactories: Route Factory: Process Iteration: " << i << std::endl; 
                router_id = i+1;
                route_in = route_out;
                neighborhood = new Neighborhood(router_id);
                pp = rpf->createProcessPacket(route_in, neighborhood);
                route_out = pp->getRoute();
                assert(pp->header.packet_type_ == ROUTE_PACKET);
                assert(route_out->router_id_ == router_id);
                assert(route_out->neighbor_id_ == route_in->router_id_);
                assert(route_out->dest_id_ == route_in->dest_id_);
                assert(route_out->link_cost_ == link_cost);
                assert(route_out->cost_ == (i+1) * link_cost);
                assert(route_out->path_.size() == (i+2));
                assert(route_out->links_.size() == i);
                std::cout << "\nProcess Route" << std::endl;
                route_out->print();
            }
            std::cout << "TestFactories: Route Factory: Complete!" << std::endl; 
        };

        void testWithdrawFactory() {
            std::cout << "TestFactories: Withdraw Factory" << std::endl;  
            wpf = new WithdrawPacketFactory(factory_params);                     
            route_in = new Route(1,0,10);
            route_in->setFailedLink(1,0,0);            

            for (short i=1; i<n_nodes; ++i) {
                recipient = new Neighbor(i,i+1, link_cost*i, 150);
                ap = wpf->createAnnouncementPacket(route_in, recipient);
                std::cout << "TestFactories: Withdraw Factory: Announcement Iteration: " << i << std::endl; 
                route_out = ap->getRoute();
                assert(pp->header.packet_type_ == WITHDRAW_ROUTE_PACKET);
                assert(route_in->route_id_ == route_out->route_id_);
                assert(route_in->neighbor_id_ == route_out->neighbor_id_);
                assert(route_in->dest_id_ == route_out->dest_id_);
                assert(route_in->path_ == route_out->path_);
                assert(route_in->links_.size() == i);
                assert(route_out->links_.size() == i+1);
                assert(route_in->getFailedLink().router_id_ == route_out->getFailedLink().router_id_);
                assert(route_in->getFailedLink().neighbor_id_ == route_out->getFailedLink().neighbor_id_);
                assert(route_in->getFailedLink().version_ == route_out->getFailedLink().version_);
                std::cout << "\nAnnouncement Route" << std::endl;
                route_out->print();
                std::cout << "TestFactories: Withdraw Factory: Process Iteration: " << i << std::endl; 
                router_id = i+1;
                route_in = route_out;
                neighborhood = new Neighborhood(router_id);
                pp = rpf->createProcessPacket(route_in, neighborhood);
                route_out = pp->getRoute();
                assert(route_out->router_id_ == router_id);
                assert(route_out->neighbor_id_ == route_in->router_id_);
                assert(route_out->dest_id_ == route_in->dest_id_);
                assert(route_out->link_cost_ == link_cost);
                assert(route_out->cost_ == (i+1) * link_cost);
                assert(route_out->path_.size() == (i+2));
                assert(route_out->links_.size() == i);
                assert(route_in->getFailedLink().router_id_ == route_out->getFailedLink().router_id_);
                assert(route_in->getFailedLink().neighbor_id_ == route_out->getFailedLink().neighbor_id_);
                assert(route_in->getFailedLink().version_ == route_out->getFailedLink().version_);
                std::cout << "\nProcess Route" << std::endl;
                route_out->print();
                route_in = route_out;
            }            

            std::cout << "TestFactories: Withdraw Factory: Complete!" << std::endl; 
        };        

        void testFailureFactory() {
            std::cout << "TestFactories: Failure Factory" << std::endl; 
            std::cout << "TestFactories: Failure Factory: Announce" << std::endl; 
            std::cout << "TestFactories: Failure Factory: Process" << std::endl; 
        }; 

        void takedown() {
            delete params;
            delete logger;
            delete next_door;
            delete route_in;
            delete route_out;
            delete rpf;
            delete wpf;
            delete recipient;
            delete factory_params;
            delete neighborhood;
        };               


        void main() {
            std::cout << "\nTestFactories" << std::endl;  
            startup();
            testRouteFactory();
            testWithdrawFactory();
            //testFailureFactory();
            takedown();
            
            std::cout << "\nTestFactories Complete" << std::endl;  
        };
};

int main() {
    //TestPackets().main();
    TestFactories().main();
}
 