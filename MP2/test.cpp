#include "assert.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include "Builder.h"
#include "Parameters.h"
#include "Router.h"
#include "Packet.h"

//===========================================================================//
//                                  TEST                                     // 
//===========================================================================//
class Test {
    public:
        Test() {}

        void testPacketFactories() {
            std::cout << "\nTestBuild" << std::endl;  

          	struct Parameters* parameters = new Parameters;
            parameters->router_id_ = 5;
            parameters->n_network_ = 256;
            parameters->ttl_ms_ = 15.0;	
            parameters->lifetime_ = 30000;
            parameters->cost_filename_ = "cost.txt";
            parameters->log_filename_ = "log_dp.txt";
            parameters->rib_plane_log_filename_ = "log_cp.txt";

            // Build and instantiate the Network;	
            Builder	builder = Builder(parameters);
            Director director = Director(builder);
            Router* router = director.buildRouter();            
            router->hey();
            router->startDataPlane();
            router->startPulse();
            router->startNOC();

            return router;
        }    
            
        void main() {
            std::cout << "\n\nTest" << std::endl;                        
            Router* router = testBuild();
            std::cout << "\nTest Complete!" << std::endl;                                    
        }
};
int main() {
    Test().main();
}
 