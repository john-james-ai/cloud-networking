#ifndef DATA_PLANE_H
#define DATA_PLANE_H

#include "assert.h"

#include "EKG.h"
#include "Listener.h"
#include "Messenger.h"
#include "Parameters.h"
#include "Network.h"
#include "Socket.h"
#include "Pulse.h"
#include "utils.h"

class Parameters;
class   EventHandler;
struct NPack;
//===========================================================================//
//                               DATAPLANE                                   //
//===========================================================================//
class DataPlane {
    private:  
        std::string classname_, address_;
        Parameters* parameters_;
        Listener* listener_;
        Messenger* messenger_;
        NPack* n_pack_;
        EventHandler* controllers_;

    public:
        DataPlane();
        DataPlane(Parameters* params, Listener* listener, Messenger* messenger);         
        void startListening();
        bool hey();

};        
#endif
