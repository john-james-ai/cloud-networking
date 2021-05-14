#include "assert.h"
#include <vector>


#include "PacketFactory.h"
#include "IO.h"
#include "Costs.h"
#include "Constants.h"
//===========================================================================//
//                                  TEST                                     // 
//===========================================================================//
class TestCost {
    public:
        TestCost() {}

       void main() {
            std::cout << "\n\nTestCost" << std::endl; 


            Parameters* params = new Parameters();
            params->router_id_ = 1;
            params->cost_filename_ = "testinitcosts1";


            Costs* costs = new Costs(params);
            for (int i=0;i<N_NODES;++i) {
                if (i != 1) {
                
                    if (i==2) {
                        assert(costs->getCost(i) == 54);
                    } else if (i==5) {
                        assert(costs->getCost(i) == 2);
                    } else {
                        assert(costs->getCost(i) == 1);
                    }
                    if (i%2 == 0) {
                        costs->setCost(i, i*2);
                    }
                }
            }

            for (int i=0;i<N_NODES && i != 1;++i) {

                if (i%2 == 0) {
                    assert(costs->getCost(i) == i*2);
                } else if (i==5) {
                    assert(costs->getCost(i) == 2);
                } else {
                    assert(costs->getCost(i) == 1);
                }
            }
            std::cout << "\n\nTestCost Complete" << std::endl;  

        };
};

int main() {
    TestCost().main();
}
 