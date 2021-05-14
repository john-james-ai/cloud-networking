#include "assert.h"
#include <string>
#include <vector>


#include "PacketFactory.h"
#include "Parameters.h"
#include "Neighborhood.h"
//===========================================================================//
//                                  TEST                                     // 
//===========================================================================//
class TestNeighborhood {
    public:
        TestNeighborhood() {}

        void testNeighborhood() {
            std::cout << "\nTestNeighborhood: Create Neighbor" << std::endl; 
            int i;
            std::vector<short> ids;
            std::vector<short> path;
            Parameters* p = new Parameters();
            p->router_id_ = 22;
            p->ttl_ms_ = 120;
            Neighborhood* hood = new Neighborhood(p);                        
            Neighbor n;

            for (i=0;i<10;++i) {
                hood->createNeighbor(i,i+10);
            };
            for (i=0;i<10;++i) {
                assert(hood->neighborExists(i));
                assert(hood->neighborActive(i));
            }            
            std::cout << "\nTestNeighborhood: Create Neighbor: Complete!" << std::endl; 
            
            std::cout << "\nTestNeighborhood: Get Neighbor Methods" << std::endl; 

            for (i=0;i<10;++i) {
                n = hood->getNeighbor(i);
                assert(n.getNeighborId() == i);
                assert(n.getCost() == i+10);
            }

            ids = hood->getNeighborIds();
            for (i=0;i<10;++i) {
                assert(ids.at(i) == i);
            }

            for (i=0;i<10;++i) {
                if (i%2 == 0) {
                    path.push_back(i);
                }
            }            
            ids = hood->getRecipients(path);
            assert(ids.size() == 5);

            std::cout << "\nTestNeighborhood: Get Neighbor Methods: Complete" << std::endl;
            std::cout << "\nTestNeighborhood: Deactivate Method" << std::endl; 
            for (i=0;i<10;++i) {
                if (i%2 == 0) {
                    hood->dropNeighbor(i);
                }
            }    
            std::cout << "\nTestNeighborhood: Deactivate Method: Complete" << std::endl; 
            std::cout << "\nTestNeighborhood: Neighbor State Methods" << std::endl; 
            for (i=0;i<10;++i) {
                if (i%2 == 0) {
                    assert(hood->neighborExists(i) == true);
                    assert(hood->neighborActive(i) == false);
                } else {
                    assert(hood->neighborExists(i) == true);
                    assert(hood->neighborActive(i) == true);
                }
            }    
            for (i=0;i<10;++i) {
                if (i%2 == 0) {
                    hood->deleteNeighbor(i);
                }
            }
            ids = hood->getNeighborIds();
            assert(ids.size() == 5);    

            for (i=0;i<10;++i) {
                if (i%2 == 0) {
                    assert(hood->neighborExists(i) == false);
                } else {
                    assert(hood->neighborExists(i) == true);
                }
            }    
            delete p;
            delete hood;
            std::cout << "\nTestNeighborhood: Neighbor State Methods: Complete" << std::endl; 
        };
        void main() {
            std::cout << "\n\nTestNeighborhood" << std::endl;  
            testNeighborhood();
            std::cout << "\n\nTestNeighborhood Complete" << std::endl;  

        };
};

int main() {
    TestNeighborhood().main();
}
 