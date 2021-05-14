#ifndef FORWARDERS_H
#define FORWARDERS_H

#include "assert.h"
#include <iostream>
#include <string>
#include <vector>

#include "IO.h"
#include "Messenger.h"
#include "Parameters.h"
#include "FTable.h"
#include "Routes.h"

class Messenger;
class DPLogger;
class Packet;
struct Parameters;
class FTable;

//===========================================================================//
//                               D_HANDLER                                   //
//===========================================================================//
class Forwarder {
    private:
        short router_id_;
        std::string classname_, address_;                   
        FTable* ftable_;
        Messenger* messenger_;
        DPLogger* log_;
    public:
        Forwarder();
        Forwarder(Parameters* parameters, FTable* ftable, 
                    Messenger* messenger, DPLogger* logger);
        void process(Packet* packet);
        bool hey();
        std::string name();
        
        
};

//===========================================================================//
//                        FORWARDER EVENT MANAGER                            //
//===========================================================================//
class Forwarders {
    private:
        short router_id_;
        std::vector<Forwarder*> forwarders_;
        std::string classname_, address_;

    public:
        Forwarders();
        void attach(Forwarder* forwarder);
        void notify(Packet* packet);
        bool hey();
        std::string name();

};
#endif