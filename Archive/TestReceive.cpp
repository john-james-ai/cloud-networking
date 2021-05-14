#include "assert.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include "Manager.h"
#include "PacketFactory.h"
#include "Serializer.h"
//===========================================================================//
//                                  TEST                                     // 
//===========================================================================//
class TestReceive {
    public:
        TestReceive() {}

        void testCreatePacket() {
            std::cout << "\nTestReceive: CreatePacket" << std::endl; 
            // Create command
            Command c1;
            strcpy(c1.command,"send");
            c1.node_id = 5;
            c1.dest_id = 0;
            strcpy(c1.message,"Its time.");
            // Create commands vector
            std::vector<Command> commands;            
            commands.push_back(c1);
            // Instantiate and run manager
            Manager manager = Manager(commands);
            manager.run();
            std::cout << "\nTestReceive: CreatePacket - Complete!" << std::endl; 
        };

        void main() {
            std::cout << "\n\nTestReceive" << std::endl;  
            testCreatePacket();
            std::cout << "\n\nTestReceive Complete" << std::endl;  

        };
};

int testRecv() {
    TestReceive().main();
}
 