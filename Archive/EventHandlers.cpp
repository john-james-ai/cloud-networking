#include "EventHandlers.h"
//===========================================================================//
//                              CONTROLLER                                   //
//===========================================================================//
EventHandler::EventHandler() 
  : classname_("EventHandler")
    {
        iam(classname_, __FUNCTION__);          
        std::string* address_ = (std::string*)this;
    }
EventHandler::EventHandler(Parameters* parameters, RouteInfoBase* rib)
    : router_id_(parameters->router_id_), rib_(rib), 
      classname_("EventHandler")
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
//                                 ROUTE                                     //
//---------------------------------------------------------------------------//
RouteHandler::RouteHandler() {}
RouteHandler::RouteHandler(Parameters* parameters, RouteInfoBase* rib)
    : EventHandler(parameters, rib)
      {
        EventHandler::classname_ = "RouteHandler";
      }
//---------------------------------------------------------------------------//
void RouteHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header.packet_type_ == ROUTE_PACKET) {

    rib_->checkIn(packet);
    rib_->processRoute(packet);
  }
};

//---------------------------------------------------------------------------//
//                                  WITHDRAW                                 //
//---------------------------------------------------------------------------//
WithdrawHandler::WithdrawHandler() {}
WithdrawHandler::WithdrawHandler(Parameters* parameters, RouteInfoBase* rib)
    : EventHandler(parameters, rib)
      {
        EventHandler::classname_ = "WithdrawHandler";
      }
//---------------------------------------------------------------------------//
void WithdrawHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header.packet_type_ == WITHDRAW_ROUTE_PACKET) {
    rib_->checkIn(packet);
    rib_->processWithdraw(packet);
  }
};
//---------------------------------------------------------------------------//
//                                   FAILURE                                 //
//---------------------------------------------------------------------------//
FailureHandler::FailureHandler() {}
FailureHandler::FailureHandler(Parameters* parameters, RouteInfoBase* rib)
    : EventHandler(parameters, rib)
      {
        EventHandler::classname_ = "FailureHandler";
      }
//---------------------------------------------------------------------------//
void FailureHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header.packet_type_ == FAILURE_PACKET) {
    rib_->checkIn(packet);
    rib_->processFailure(packet);
  }
};
//---------------------------------------------------------------------------//
//                                   ADVERTISE                               //
//---------------------------------------------------------------------------//
AdvertiseHandler::AdvertiseHandler() {}
AdvertiseHandler::AdvertiseHandler(Parameters* parameters, RouteInfoBase* rib)
    : EventHandler(parameters, rib)
      {
        EventHandler::classname_ = "AdvertiseHandler";
      }
//---------------------------------------------------------------------------//
void AdvertiseHandler::process(Packet* packet) {
  iam(EventHandler::classname_, __FUNCTION__);

  if (packet->header.packet_type_ == ADVERTISE_PACKET) {
    rib_->processAdvertise(packet);
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

