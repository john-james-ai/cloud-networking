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

class Packet;
class Route;
class Link;

class DPLogger {
    private:
        short router_id_;
        std::string filename_;
        std::string classname_, address_;
        FILE* file_pointer_;
        char line_[100];
        std::string message_;
        
    public:
        DPLogger();
        DPLogger(Parameters* parameters);
        void verbose();
        void send(Packet* packet);
        void forward(Packet* packet);
        void receive(Packet* packet);
        void drop(Packet* packet);

        bool hey();
};

class CPLogger {
    private:
        short router_id_;
        std::string filename_;
        std::string classname_, address_;
        FILE* file_pointer_;
        char line_[100];
        std::string message_;
        std::string datetime_;    
    public:
        CPLogger();
        CPLogger(Parameters* parameters);
        char* getCurrentDateTime(char& line);
        void verbose();
        void setCost(Packet* packet);
        void processRoute(Route* route);
        void processRoute(Packet* packet);
        void processDeleteRoute(Route* route);
        void processForwarding(Route* route);
        void processDeleteForwarding(Route* route);
        void processWithDraw(Route* route);
        void processWithDraw(Route* best, Route* better);
        void processFailure(Route* best);   
        void processFailure(Route* best, Route* better, Link root_cause);   
        bool hey();
};

class PacketLogger {
    private:
        short router_id_;
        std::string filename_;
        std::string classname_, address_;
        FILE* file_pointer_;
        char line_[100];
        std::string message_;
        std::string datetime_;    
    public:
        PacketLogger();
        PacketLogger(Parameters* parameters);
        char* getCurrentDateTime(char& line);
        void verbose();
        void route(Packet* packet);
        void withdraw(Packet* packet);
        void failure(Packet* packet);
        bool hey();
};
  
#endif