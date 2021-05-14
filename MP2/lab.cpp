#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Constants.h"
#include "PacketFactory.h"
#include "utils.h"

 

int main() {
    char line[100];
    size_t len;
    sprintf(line, "node %i's route to %i through %i dropped. Reason: %s\n", 
            2,3,4,SEND_PACKET_TYPE.c_str());
    len = strlen(line);
    std::string message = std::string(line,len);
    std::cout << message << std::endl;
}
    
