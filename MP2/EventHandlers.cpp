#include "EventHandlers.h"
//===========================================================================//
//                           EVENT HANDLER                                   //
//===========================================================================//
EventHandler::EventHandler() 
  : classname_("EventHandler")
    {
        iam(classname_, __FUNCTION__);          
        std::string* address_ = (std::string*)this;
    }
EventHandler::EventHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane)
    : router_id_(parameters->router_id_), control_plane_(control_plane), 
      data_plane_(data_plane), classname_("EventHandler")
    {
        iam(classname_, __FUNCTION__);  
        std::string* address_ = (std::string*)this;
    }
//---------------------------------------------------------------------------//      
void EventHandler::process(Packet* packet) {;};
//---------------------------------------------------------------------------//
bool EventHandler::hey() {
    
    holla(classname_, address_);
    return true;
}  
//---------------------------------------------------------------------------//
//                                  SEND                                     //
//---------------------------------------------------------------------------//
DataPlaneHandler::DataPlaneHandler() {}
DataPlaneHandler::DataPlaneHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane)
    : EventHandler(parameters, control_plane, data_plane)
      {
        EventHandler::classname_ = "RouteHandler";
      }
//---------------------------------------------------------------------------//
void DataPlaneHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header->packet_type_ == SEND_PACKET_TYPE ||
      packet->header->packet_type_ == FORWARD_PACKET_TYPE) {
    // If this router is the destination, process receive packet.
    if (packet->dest_id_ == router_id_) {
      data_plane_->receive(packet);
    } else {    
      // Obtain next hop, if found. If not, drop packet.      
      packet->next_hop_ = control_plane_->getNextHop(packet);

      if (packet->next_hop_ < 0 || packet->next_hop_  == DESTINATION_NOT_FOUND) {
        data_plane_->drop(packet);
      
      } else {
        packet->header->recipients_.clear();     
        packet->header->recipients_.push_back(packet->next_hop_);                  

        if(strstr(packet->header->sender_ip_, "10.1.1.")) {
          // If the packet originated from a neighbor, change the packet
          // type to forward. Only originators, receiving command from
          // the manager, 'send' packets. Downstream routers forward.
          packet->header->packet_type_ = FORWARD_PACKET_TYPE;
        }

        if (packet->header->packet_type_ == SEND_PACKET_TYPE) {
          data_plane_->send(packet);
        } else {
          data_plane_->forward(packet);
        }   
      }
    }
  }
};

//---------------------------------------------------------------------------//
//                            ROUTE HANDLER                                  //
//---------------------------------------------------------------------------//
RouteHandler::RouteHandler() {}
RouteHandler::RouteHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane)
    : EventHandler(parameters, control_plane, data_plane)
      {
        EventHandler::classname_ = "RouteHandler";
      }
//---------------------------------------------------------------------------//
void RouteHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header->packet_type_ == ROUTE_PACKET) {
    if (packet->header->sender_ == router_id_) {
      data_plane_->announce(packet);
    } else {
      control_plane_->configRoute(packet);
    }
  }
};

//---------------------------------------------------------------------------//
//                             NEIGHBOR HANDLER                              //
//---------------------------------------------------------------------------//
NeighborHandler::NeighborHandler() {}
NeighborHandler::NeighborHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane)
    : EventHandler(parameters, control_plane, data_plane)
      {
        EventHandler::classname_ = "RouteHandler";
      }
//---------------------------------------------------------------------------//
void NeighborHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header->packet_type_ == NEIGHBOR_PACKET) {
    if (packet->header->sender_ == router_id_) {
      data_plane_->announce(packet);
    } else {      
      control_plane_->configNeighbor(packet);
    }
  }
};

//---------------------------------------------------------------------------//
//                                  WITHDRAW                                //
//---------------------------------------------------------------------------//
WithdrawHandler::WithdrawHandler() {}
WithdrawHandler::WithdrawHandler(Parameters* parameters, ControlPlane* control_plane, DataPlane* data_plane)
    : EventHandler(parameters, control_plane, data_plane)
      {
        EventHandler::classname_ = "WithdrawHandler";
      }
//---------------------------------------------------------------------------//
void WithdrawHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header->packet_type_ == WITHDRAW_PACKET) {    
    if (packet->header->sender_ == router_id_) {
      data_plane_->announce(packet);
    } else {
      control_plane_->configWithdraw(packet);
    }    
  }
};
//===========================================================================//
//                            EVENT MANAGER                                  //
//===========================================================================//
EventManager::EventManager() 
  : classname_("EventManager")

    {
        iam(classname_, __FUNCTION__);   
        std::string* address_ = (std::string*)this;
    }

EventManager::EventManager(Parameters* parameters) 
    : router_id_(parameters->router_id_), 
      classname_("EventManager")
    {
        iam(classname_, __FUNCTION__);   
        std::string* address_ = (std::string*)this;
    }


void EventManager::attach(EventHandler* handler) {
    
    handlers_.push_back(handler);
};

void EventManager::notify(Packet* packet) {
    std::vector<EventHandler*>::iterator it;
    
    for (it=handlers_.begin();it != handlers_.end(); ++it) {
        (*it)->process(packet);
    }        
};

bool EventManager::hey() {

    holla(classname_, address_);

    std::vector<EventHandler*>::iterator it;
    for (it = handlers_.begin();it != handlers_.end();++it) {
        assert((*it)->hey());
    }
    return true;
}

