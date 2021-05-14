#include "IO.h"

//---------------------------------------------------------------------------//
//                                LOGGER                                     //
//---------------------------------------------------------------------------//
Logger::Logger() 
	: classname_("Logger")
{
	iam(classname_, __FUNCTION__);
}
Logger::Logger(Parameters* parameters)
    : router_id_(parameters->router_id_), 
      filename_(parameters->log_filename_),
      classname_("Logger")
    {
        iam(classname_, __FUNCTION__);
        std::remove(filename_.c_str());
        file_pointer_ = fopen(filename_.c_str(), "wb");
    };
void Logger::verbose() {

    size_t len = strlen(line_);
    std::string message = std::string(line_,len);
    std::cout << message << std::endl;
}
void Logger::send(Packet* packet) {
    
    const char* m = packet->body_.c_str();    
    sprintf(line_, "sending packet dest %hi nexthop %hi message %s\n", 
            packet->dest_id_,
            packet->next_hop_, m);    
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
}

void Logger::forward(Packet* packet) {

    const char* m = packet->body_.c_str();    
    sprintf(line_, "forward packet dest %hi nexthop %hi message %s\n", 
            packet->dest_id_,
            packet->next_hop_, m);    
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
}

void Logger::receive(Packet* packet) {

    const char* m = packet->body_.c_str();    
    sprintf(line_, "receive packet message %s\n", m);
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
}
void Logger::drop(Packet* packet) {

    sprintf(line_, "unreachable dest %hi\n", packet->dest_id_);
    fwrite(line_,1,strlen(line_), file_pointer_);
    fflush(file_pointer_);
    if (VERBOSE) verbose();
};
void Logger::addRoute(Route* route, std::string fn) {

    std::string path = vecToString(route->path_);
    sprintf(line_, "CP - %s: %hi added route to %hi through %hi with path %s\n", 
                        fn.c_str(),
                        route->router_id_,
                        route->dest_id_,
                        route->neighbor_id_,
                        path.c_str());
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);        
    }
    if (VERBOSE) verbose();
};
void Logger::bestRoute(Route* route, std::string fn) {

    sprintf(line_, "CP - %s: %hi added route to %hi through %hi to the forwarding table\n", 
                        fn.c_str(),
                        route->router_id_,
                        route->dest_id_,
                        route->neighbor_id_);
    
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::withdraw(Packet* packet, std::string fn) {

    std::string path = vecToString(packet->route_->path_);
    sprintf(line_, "CP - %s: %hi withdrew route to %hi thru %hi, with path %s\n", 
                        fn.c_str(),
                        packet->header->sender_,
                        packet->route_->dest_id_,
                        packet->route_->neighbor_id_,
                        path.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::announceRoute(Packet* packet, std::string fn) {

    std::string path = vecToString(packet->route_->path_);
    std::string recipients = vecToString(packet->header->recipients_);
    sprintf(line_, "CP - %s: %hi announcing new route to %hi through %hi with path %s. Recipients: %s\n", 
                        fn.c_str(),
                        packet->header->sender_,
                        packet->route_->dest_id_,
                        packet->route_->neighbor_id_,
                        path.c_str(),
                        recipients.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);        
    }
    if (VERBOSE) verbose();
};
void Logger::advertiseRoute(Packet* packet, std::string fn) {

    std::string path = vecToString(packet->route_->path_);
    std::string recipients = vecToString(packet->header->recipients_);
    sprintf(line_, "CP - %s: %hi advertise new route to %hi through %hi with path %s. Recipients: %s\n", 
                        fn.c_str(),
                        packet->header->sender_,
                        packet->route_->dest_id_,
                        packet->route_->neighbor_id_,
                        path.c_str(),
                        recipients.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);        
    }
    if (VERBOSE) verbose();
};

void Logger::announceWithdraw(Packet* packet, std::string fn) {

    std::string path = vecToString(packet->route_->path_);
    std::string recipients = vecToString(packet->header->recipients_);
    sprintf(line_, "CP - %s: %hi announcing withdraw of route to %hi thru %hi with path %s. Recipients: %s\n",
                        fn.c_str(), 
                        packet->header->sender_,
                        packet->route_->dest_id_,
                        packet->route_->neighbor_id_,
                        path.c_str(),
                        recipients.c_str());
    
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::failedNeighbor(short node_id, std::string fn) {

    sprintf(line_, "CP - %s: Neighbor %hi has failed.\n", 
                        fn.c_str(),
                        node_id);
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::failedRoute(Route* route, std::string fn) {

    std::string path = vecToString(route->path_);
    sprintf(line_, "CP - %s: Route to %hi through %hi with path %s has failed\n", 
            fn.c_str(),
            route->dest_id_,
            route->neighbor_id_,
            path.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::activatedRoute(Route* route, std::string fn) {

    std::string path = vecToString(route->path_);
    sprintf(line_, "CP - %s: %hi activated route from %hi to %hi with path %s\n", 
            fn.c_str(),
            router_id_,
            route->router_id_,
            route->dest_id_,
            path.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::deactivatedRoute(Packet* packet, std::string fn) {

    std::string path = vecToString(packet->route_->path_);
    sprintf(line_, "CP - %s: %hi deactivated route from %hi to %hi with path %s\n", 
            fn.c_str(),
            router_id_,
            packet->route_->router_id_,
            packet->route_->dest_id_,
            path.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::deactivatedRoute(Route* route, std::string fn) {

    std::string path = vecToString(route->path_);
    sprintf(line_, "CP - %s: %hi deactivated route from %hi to %hi with path %s\n", 
            fn.c_str(),
            router_id_,
            route->router_id_,
            route->dest_id_,
            path.c_str());

    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::deactivatedRoutesNeighbor(short neighbor_id, int n, std::string fn) {

    sprintf(line_, "CP - %s: %hi deactivated %d routes through %hi\n", 
            fn.c_str(),
            router_id_,
            n,
            neighbor_id);
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);        
    }
    if (VERBOSE) verbose();
};
void Logger::deactivatedRoutesDestination(short dest_id, int n, std::string fn) {

    sprintf(line_, "CP - %s: %hi deactivated %d routes to %hi\n", 
            fn.c_str(),
            router_id_,
            n,
            dest_id);
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);
    }
    if (VERBOSE) verbose();
};
void Logger::discoverNeighbor(short neighbor_id, std::string fn) {

    sprintf(line_, "\nCP - %s: %hi DISCOVERED NEW NEIGHBOR %hi WOOHOOO\n", 
            fn.c_str(),
            router_id_,
            neighbor_id);
    if (CONTROL_PLANE_LOG) {
        fwrite(line_,1,strlen(line_), file_pointer_);
        fflush(file_pointer_);        
    }
    if (VERBOSE) verbose();
};
bool Logger::hey() {
    holla(classname_, address_);
    return true;
};
