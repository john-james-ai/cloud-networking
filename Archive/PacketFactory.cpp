#include "PacketFactory.h"
//===========================================================================//
//                   ABSTRACT PACKET FACTORY MODULE                          //
//___________________________________________________________________________//
//===========================================================================//
//===========================================================================//
//                           PACKET STATS                                    //
//===========================================================================//
void PStat::print(){
  std::cout << "============================================================" << std::endl;
  std::cout << "|                       PACKET SUMMARY                      |" << std::endl;
  std::cout << "============================================================" << std::endl;
  std::cout << "|                  Routes       Withdrawals     Failures    |" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;
  std::cout << "   Announced:       " << routes_announced << \
               "             " << withdraws_announced << \
               "             " << failures_announced << std::endl;  
  std::cout << "|  Processed:       " << routes_processed << \
               "             " << withdraws_processed << \
               "             " << failures_processed << std::endl;  
  std::cout << "|    Deleted:       " << route_objects_deleted << \
               "             " << withdraw_objects_deleted << \
               "             " << failure_objects_deleted << std::endl;  
  std::cout << "============================================================" << std::endl;               

};

//===========================================================================//
//                          ABSTRACT PACKET FACTORY                          //
//===========================================================================//
AbstractPacketFactory::AbstractPacketFactory() {};
AbstractPacketFactory::AbstractPacketFactory(FactoryParams* params) 
  : router_id_(params->parameters_->router_id_),
    neighborhood_(params->neighborhood_), routes_(params->routes_),    
    log_(params->log_), 
    classname_("AbstractPacketFactory")
    {
      reset();
    }
void AbstractPacketFactory::reset() {
  PStat* pstats_ = new PStat();
  pstats_->routes_announced = 0;
  pstats_->routes_processed = 0;
  pstats_->route_objects_deleted = 0;

  pstats_->withdraws_announced = 0;
  pstats_->withdraws_processed = 0;
  pstats_->withdraw_objects_deleted = 0;

  pstats_->failures_announced = 0;
  pstats_->failures_processed = 0;
  pstats_->failure_objects_deleted = 0;

}
AnnouncementPacket* AbstractPacketFactory::createAnnouncementPacket(Route* route, Neighbor* recipient) {};
ProcessPacket* AbstractPacketFactory::createProcessPacket(Route* route, Neighborhood* neighborhood) {};
void AbstractPacketFactory::deallocate(Packet* packet) {
    iam(classname_, __FUNCTION__); 
    if (packet->header.classname_ == "RouteAnnouncementPacket" ||
        packet->header.classname_ == "RouteProcessPacket") pstats_->route_objects_deleted +=1; 
    if (packet->header.classname_ ==  "WithdrawAnnouncementPacket" ||
        packet->header.classname_ ==  "WithdrawProcessPacket" ) pstats_->withdraw_objects_deleted +=1; 
    if (packet->header.classname_ ==  "FailureAnnouncementPacket" ||
        packet->header.classname_ ==  "FailureProcessPacket" ) pstats_->withdraw_objects_deleted +=1; 
    delete packet;
};
std::string AbstractPacketFactory::name() {
  return classname_;
};

bool AbstractPacketFactory::hey() {
  iam(classname_, __FUNCTION__); 
  assert(neighborhood_->hey());
  assert(routes_->hey());
  assert(log_->hey());
  return true;
};



//===========================================================================//
//                          ROUTE PACKET FACTORY                             //
//===========================================================================//
RoutePacketFactory::RoutePacketFactory() {};
RoutePacketFactory::RoutePacketFactory(FactoryParams* params) 
  : AbstractPacketFactory(params)
{
  AbstractPacketFactory::classname_ = "RoutePacketFactory";
};
//---------------------------------------------------------------------------//
AnnouncementPacket* RoutePacketFactory::createAnnouncementPacket(Route* route, Neighbor* recipient) {
  iam(classname_, __FUNCTION__); 
  RouteAnnouncementPacket* packet = new RouteAnnouncementPacket(route, recipient);
  packet->checkOut();
  pstats_->routes_announced += 1;

  return packet;
};
//---------------------------------------------------------------------------//
ProcessPacket* RoutePacketFactory::createProcessPacket(Route* route, Neighborhood* neighborhood) {
  iam(classname_, __FUNCTION__);   
  RouteProcessPacket* packet = new RouteProcessPacket(route, neighborhood);

  Route* route_ = new Route(0,0,0);  
  packet->checkIn();
  pstats_->routes_processed += 1;
  
  return packet;  
};



//===========================================================================//
//                          WITHDRAW PACKET FACTORY                          //
//===========================================================================//
WithdrawPacketFactory::WithdrawPacketFactory() {};
WithdrawPacketFactory::WithdrawPacketFactory(FactoryParams* params) 
: AbstractPacketFactory(params)
{
  AbstractPacketFactory::classname_ = "WithdrawPacketFactory";
};
//---------------------------------------------------------------------------//
AnnouncementPacket* WithdrawPacketFactory::createAnnouncementPacket(Route* route, Neighbor* recipient) {
  iam(classname_, __FUNCTION__); 
  WithdrawAnnouncementPacket* packet = new WithdrawAnnouncementPacket(route, recipient);
  pstats_->withdraws_announced += 1;
  return packet;
};
//---------------------------------------------------------------------------//
ProcessPacket* WithdrawPacketFactory::createProcessPacket(Route* route, Neighborhood* neighborhood) {
  iam(classname_, __FUNCTION__); 
  WithdrawProcessPacket* packet = new WithdrawProcessPacket(route,  neighborhood);  
  pstats_->withdraws_processed += 1;
  return packet;  
};



//===========================================================================//
//                          FAILURE PACKET FACTORY                           //
//===========================================================================//
FailurePacketFactory::FailurePacketFactory() {};
FailurePacketFactory::FailurePacketFactory(FactoryParams* params) 
: AbstractPacketFactory(params)
{
  AbstractPacketFactory::classname_ = "FailurePacketFactory";
};
//---------------------------------------------------------------------------//
AnnouncementPacket* FailurePacketFactory::createAnnouncementPacket(Route* route, Neighbor* recipient) {
  iam(classname_, __FUNCTION__); 
  FailureAnnouncementPacket* packet = new FailureAnnouncementPacket(route, recipient);
  packet->checkOut();
  pstats_->failures_announced += 1;
  return packet;
};
//---------------------------------------------------------------------------//
ProcessPacket* FailurePacketFactory::createProcessPacket(Route* route, Neighborhood* neighborhood) {
  iam(classname_, __FUNCTION__); 
  FailureProcessPacket* packet = new FailureProcessPacket(route,neighborhood);
  pstats_->failures_processed += 1;
  return packet;  
};
