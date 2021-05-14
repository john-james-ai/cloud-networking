#include "IO.h"

//---------------------------------------------------------------------------//
//                                LOGGER                                     //
//---------------------------------------------------------------------------//
DPLogger::DPLogger() 
	: classname_("Logger")
{
	iam(classname_, __FUNCTION__);
}
DPLogger::DPLogger(Parameters* parameters)
    : router_id_(parameters->router_id_), 
      filename_(parameters->log_filename_),
      classname_("Logger")
    {
        iam(classname_, __FUNCTION__);
        std::remove(filename_.c_str());
        file_pointer_ = fopen(filename_.c_str(), "wb");
    };
void DPLogger::verbose() {

    size_t len = strlen(line_);
    std::string message = std::string(line_,len);
    std::cout << message << std::endl;
}
void DPLogger::send(Packet* packet) {
    
    const char* m = packet->buffer_;    
    sprintf(line_, "sending packet dest %hi nexthop %hi message %s\n", 
            packet->route_->dest_id_,
            packet->route_->neighbor_id_, m);    
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
}

void DPLogger::forward(Packet* packet) {

    const char* m = packet->buffer_;    
    sprintf(line_, "forward packet dest %hi nexthop %hi message %s\n",
            packet->route_->dest_id_,
            packet->route_->neighbor_id_, m);    
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
}
void DPLogger::receive(Packet* packet) {

    const char* m = packet->buffer_;    
    sprintf(line_, "receive packet message %s\n", m);
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
}
void DPLogger::drop(Packet* packet) {

    sprintf(line_, "unreachable dest %hi\n", packet->route_->dest_id_);
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
};

bool DPLogger::hey() {
    holla(classname_, address_);
    return true;
};

//---------------------------------------------------------------------------//
//                    ROUTE INFORMATION BASE LOGGER                          //
//---------------------------------------------------------------------------//
CPLogger::CPLogger() 
	: classname_("CPLogger")
{
	iam(classname_, __FUNCTION__);
}
CPLogger::CPLogger(Parameters* parameters)
    : router_id_(parameters->router_id_), 
      filename_(parameters->rib_log_filename_),
      classname_("CPLogger")
    {
        iam(classname_, __FUNCTION__);
        std::remove(filename_.c_str());
        file_pointer_ = fopen(filename_.c_str(), "wb");
    }
void CPLogger::verbose() {

    size_t len = strlen(line_);
    std::string message = std::string(line_,len);
    std::cout << message << std::endl;
}
void CPLogger::setCost(Packet* packet) {
    if (CONTROL_PLANE_LOG) {        
        sprintf(line_, "node %hi 's cost to node %hi is now  %d\n", 
                router_id_,
                packet->route_->neighbor_id_,
                packet->route_->cost_);    
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
}
void CPLogger::processRoute(Route* route) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "node %hi has a new route to %hi through %hi. Cost is %hi\n", 
                router_id_,
                route->dest_id_,
                route->neighbor_id_,
                route->cost_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};

void CPLogger::processRoute(Packet* packet) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "node %hi has a new route to %hi through %hi. Cost is %hi\n", 
                router_id_,
                packet->route_->dest_id_,
                packet->route_->neighbor_id_,
                packet->route_->cost_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};
void CPLogger::processDeleteRoute(Route* route) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "Deleted route from %hi to %hi through %hi from Routing.\n", 
                router_id_,
                route->dest_id_,
                route->neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};
void CPLogger::processForwarding(Route* route) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "Route from %hi to %hi through %hi added to Forwarding.\n", 
                router_id_,
                route->dest_id_,
                route->neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};
void CPLogger::processDeleteForwarding(Route* route) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "Route from %hi to %hi through %hi deleted from Forwarding.\n", 
                router_id_,
                route->dest_id_,
                route->neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};
void CPLogger::processWithDraw(Route* route) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "node %hi 's route to %hi through %hi withdrawn.\n", 
                router_id_,
                route->dest_id_,
                route->neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};
void CPLogger::processWithDraw(Route* best, Route* better) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "node %hi 's route to %hi through %hi withdrawn and replaced by route through %hi.\n", 
                router_id_,
                best->dest_id_,
                best->neighbor_id_,
                better->neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
};
void CPLogger::processFailure(Route* route) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "node %hi 's connection to node %hi has failed.\n",
                router_id_,
                route->neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
}
void CPLogger::processFailure(Route* best, Route* better, Link root_cause) {

    if (CONTROL_PLANE_LOG) {

        sprintf(line_, "node %hi 's route to %hi through %hi has been replaced by route through %hi. Root cause link: %hi -> %hi\n", 
                router_id_,
                best->dest_id_,
                best->neighbor_id_,
                better->neighbor_id_,
                root_cause.router_id_,
                root_cause.neighbor_id_);
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
}

bool CPLogger::hey() {
    holla(classname_, address_);
    return true;
};



//---------------------------------------------------------------------------//
//                                PACKET LOG                                 //
//---------------------------------------------------------------------------//
PacketLogger::PacketLogger() 
	: classname_("Logger")
{
	iam(classname_, __FUNCTION__);
}
PacketLogger::PacketLogger(Parameters* parameters)
    : router_id_(parameters->router_id_), 
      filename_(parameters->log_filename_),
      classname_("Logger")
    {
        iam(classname_, __FUNCTION__);
        std::remove(filename_.c_str());
        file_pointer_ = fopen(filename_.c_str(), "wb");
    };
void PacketLogger::verbose() {

    size_t len = strlen(line_);
    std::string message = std::string(line_,len);
    std::cout << message << std::endl;
}
void PacketLogger::route(Packet* packet) {

    if (CONTROL_PLANE_LOG) {
    
        sprintf(line_, "%s: Route %s from %hi to %hi through %hi was created.\n", 
                packet->header.packet_type_.c_str(),
                packet->route_->route_id_.c_str(),
                packet->route_->router_id_,
                packet->route_->dest_id_,
                packet->route_->neighbor_id_);    
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
}
void PacketLogger::withdraw(Packet* packet) {

    if (CONTROL_PLANE_LOG) {
    
        sprintf(line_, "%s: Route %s from %hi to %hi through %hi was withdrawn.\n", 
                packet->header.packet_type_.c_str(),
                packet->route_->route_id_.c_str(),
                packet->route_->router_id_,
                packet->route_->dest_id_,
                packet->route_->neighbor_id_);    
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();
    }
}
void PacketLogger::failure(Packet* packet) {

    if (CONTROL_PLANE_LOG) {
    
        sprintf(line_, "%s: Route %s from %hi to %hi through %hi was withdrawn due to link failure from %hi to %hi.\n", 
                packet->header.packet_type_.c_str(),
                packet->route_->route_id_.c_str(),
                packet->route_->router_id_,
                packet->route_->dest_id_,
                packet->route_->neighbor_id_,
                packet->route_->failed_link_.router_id_,
                packet->route_->failed_link_.neighbor_id_);    
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
        if (VERBOSE) verbose();

    }
}
bool PacketLogger::hey() {
    holla(classname_, address_);
    return true;
};
