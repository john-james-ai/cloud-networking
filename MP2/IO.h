#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>

#include "Parameters.h"
#include "Packet.h"
#include "Routes.h"
#include "utils.h"

class Parameters;
class Packet;
class Route;

class Logger {
    private:
        short router_id_;
        std::string filename_;
        std::string classname_, address_;
        FILE* file_pointer_;
        char line_[100];
        std::string message_;
        
    public:
        Logger();
        Logger(Parameters* parameters);
        void verbose();
        void send(Packet* packet);
        void forward(Packet* packet);
        void receive(Packet* packet);
        void drop(Packet* packet);
        void addRoute(Route* route, std::string fn);
        void bestRoute(Route* route, std::string fn);
        void withdraw(Packet* Packet, std::string fn);
        void advertiseRoute(Packet* packet, std::string fn);
        void announceRoute(Packet* packet, std::string fn);        
        void announceWithdraw(Packet* packet, std::string fn);
        void failedNeighbor(short node_id, std::string fn);
        void failedRoute(Route* route, std::string fn);
        void activatedRoute(Route* route, std::string fn);
        void deactivatedRoute(Packet* packet, std::string fn);
        void deactivatedRoute(Route* route, std::string fn);
        void deactivatedRoutesNeighbor(short neighbor_id, int n, std::string fn);
        void deactivatedRoutesDestination(short neighbor_id, int n, std::string fn);
        void discoverNeighbor(short neighbor_id, std::string fn);

        bool hey();
};
  
#endif