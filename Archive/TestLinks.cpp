#include "assert.h"
#include <iostream>
#include <stdio.h>
#include <string>
struct Link;
//===========================================================================//
//                                TEST LINKS                                 // 
//===========================================================================//
class TestLinks {
    public:
        TestLinks() {}

        void testLink() {
            std::cout << "TestLink" << std::endl; 

            Link l1 = Link(5,10);
            Link l2  = Link(5,10);
            Link l3 = Link(8,9);

            std::cout << "TestLink: instantiation" << std::endl; 
            assert(l1->router_id_ == 5);
            assert(l1->neighbor_id_== 10);
            assert(l1->version_ == 0);

            std::cout << "TestLink: equality" << std::endl; 
            assert(l1->linkEqual(l2));
            assert(l1->linkEqual(5,10));
            assert(!l1->linkEqual(2,3));
            assert(!l1->linkEqual(l3));

            std::cout << "TestLink: bumVersion" << std::endl; 
            l2->bumpVersion();
            assert(l2->version_ == 1);

            std::cout << "TestLink: serialization" << std::endl; 
            char buffer[6];
            size_t offset = 0;
            l1->serialize(buffer, offset);
            offset = 0;
            l3->deserialize(buffer, offset);
            assert(l1->linkEqual(l3));
            delete l1;
            delete l2;
            delete l3;

            std::cout <<  "TestLink: Complete!" << std::endl; 
        };
        void testLinks() {
            std::cout << "TestLinks" << std::endl; 

            Link l1 = Link(5,10);
            Link l2 = Link(8,9);
            Links* links = Links();
            Links* links2 = Links();

            std::cout << "TestLinks: prependLink" << std::endl; 
            links->prependLink(l1);
            l2->bumpVersion();
            links->prependLink(l2);

            std::cout << "TestLinks: link exists" << std::endl; 
            assert(links->linkExists(l1));
            assert(links->linkExists(8,9));

            std::cout << "TestLinks: bumVersion" << std::endl; 
            links->bumpVersion(5,10);
            assert(l2->version_ == 1);

            std::cout << "TestLinks: serialization" << std::endl; 
            char buffer[32];
            memset(buffer,0,32);
            size_t offset = 0;
            links->serialize(buffer, offset);
            offset = 0;
            links2->deserialize(buffer, offset);

            std::cout << "Links 1:" << std::endl; 
            links->print();
            std::cout << "Links 2:" << std::endl; 
            links2->print();
            
            std::vector<Link> ls1 = links->getLinks();            
            std::vector<Link> ls2 = links2->getLinks();
            std::vector<Link>::iterator it;
            for (it=ls1.begin();it!=ls1.end();++it) {
                assert(links2->linkExists(*it));
            }

            delete l1;
            delete l2;
            delete links;
            delete links2;

            std::cout <<  "TestLinks: Complete!" << std::endl;  
        };       

        void main() {
            std::cout << "\nTestLinks" << std::endl;  
            testLink();
            testLinks();
            std::cout << "\nTestLinks Complete" << std::endl;  
        };
};

int main() {
    TestLinks().main();
}
 