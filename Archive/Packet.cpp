#include "Packet.h"
//===========================================================================//
//                                PACKETS                                    //
//___________________________________________________________________________//
//===========================================================================//
Packet::Packet() {};
Packet::Packet(Route* route, Neighbor* recipient)
  : router_id_(route->router_id_), route_(route)
  {
    PacketHeader header = PacketHeader();
    header.classname_ = "Packet";    
    header.recipients_.push_back(recipient->getNeighborId());
    header.sender_ = router_id_;           
    recipient_ = recipient;    
    iam(header.classname_, __FUNCTION__);

  };
Packet::Packet(Route* route, Neighborhood* neighborhood) 
  : neighborhood_(neighborhood), route_(route)
  {        
    PacketHeader header = PacketHeader();    
    header.classname_ = "Packet";
    header.sender_ = router_id_;           
    router_id_ = neighborhood->getRouterId();
    iam(header.classname_, __FUNCTION__);
  };
//---------------------------------------------------------------------------//
//                               INITIALIZE                                  //
//---------------------------------------------------------------------------//  
void Packet::initialize() {
  iam(header.classname_, __FUNCTION__);

  router_id_ = INIT_NODE;

  PacketHeader header = PacketHeader();    
  header.packet_type_ = "";
  header.sender_ = INIT_NODE;
  header.classname_ = "";
  dest_id_ = INIT_NODE;
  memset(buffer_,0,MAX_BUFFER_LENGTH);
}
//---------------------------------------------------------------------------//
//                      CHECK IN / CHECK OUT                                 //
//---------------------------------------------------------------------------//
void Packet::checkIn() {
  // Overriden by Announce classes
  iam(header.classname_, __FUNCTION__); 

  leaving(header.classname_, __FUNCTION__);
}
void Packet::checkOut() {
  // Overriden by Announce classes
  iam(header.classname_, __FUNCTION__); 

  leaving(header.classname_, __FUNCTION__);
}
//---------------------------------------------------------------------------//
//                             GET ROUTE                                     //
//---------------------------------------------------------------------------//
Route* Packet::getRoute() {
  iam(header.classname_, __FUNCTION__);
  return route_;
}
//---------------------------------------------------------------------------//
//                              SERIALIZE                                    //
//---------------------------------------------------------------------------//
void Packet::serialize(char* buffer) {
  iam(header.classname_, __FUNCTION__);

  size_t offset = 0;
  buffer[MAX_BUFFER_LENGTH];
  memset(buffer,0,MAX_BUFFER_LENGTH);  

  // delete this
  std::cout << "\nPrinting the route before serialization. " << std::endl;
  route_->print();    

  strncpy(buffer, header.packet_type_.c_str(),4);
  offset += 4;
  route_->serialize(buffer, offset);

  leaving(header.classname_, __FUNCTION__);
}
//---------------------------------------------------------------------------//
//                              DESERIALIZE                                  //
//---------------------------------------------------------------------------//
void Packet::deserialize(char* buffer) {
  iam(header.classname_, __FUNCTION__);

  size_t offset = 0;
  char temp_buffer[4];

  // Initialize packet, route and recipient
  initialize();
  message_ = "Packet initialization complete";
  note(header.classname_, __FUNCTION__,  message_);  
  route_->initialize();
  message_ = "Route initialization complete";
  note(header.classname_, __FUNCTION__,  message_);  


  // packet type
  strncpy(temp_buffer,buffer,4);
  header.packet_type_ = std::string(temp_buffer);
  offset += 4;  

  // Route  
  route_->deserialize(buffer, offset);  

  
  leaving(header.classname_, __FUNCTION__);
}

//---------------------------------------------------------------------------//
//                                 PRINT                                     //
//---------------------------------------------------------------------------//
void Packet::printLinks() {
  std::vector<Link>::iterator it;
  
  for (it=route_->links_.begin();it!=route_->links_.end(); ++it) {
    (*it).print();
  }  
}
//---------------------------------------------------------------------------//
void Packet::print() {
        
  std::cout << "\n=====================================" << std::endl;
  std::cout << "            Packet Header  " << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "            Type: " << header.packet_type_ << std::endl;   
  std::cout << "          Sender: " << header.sender_ << std::endl;  
  std::cout << "       Recipient: " << header.recipients_.front() << std::endl;  
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "                Route " << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "        Neighbor: " << route_->router_id_ << std::endl;
  std::cout << "     Destination: " << route_->dest_id_ << std::endl;
  std::cout << "       Link Cost: " << route_->link_cost_ << std::endl;
  std::cout << "            Cost: " << route_->cost_ << std::endl;
  std::cout << "-------------------------------------" << std::endl;  
  std::cout << "               Path " << std::endl;
  std::cout << "     Path Length: " << route_->path_.size() << std::endl;
  print_vec(route_->path_);
    std::cout << "-------------------------------------" << std::endl;  
  std::cout << "              Links " << std::endl;
  std::cout << "         # Links: " << route_->links_.size() << std::endl;
  printLinks();
  std::cout << "=====================================\n" << std::endl;   
};


//===========================================================================//
//                          ANNOUNCEMENT BASE PACKET                         //
//===========================================================================//
AnnouncementPacket::AnnouncementPacket(){};
AnnouncementPacket::AnnouncementPacket(Route* route, Neighbor* recipient) 
  : Packet(route, recipient)
{
  header.classname_ = "AnnouncementPacket";
  header.sender_ = router_id_;   
  Packet::is_dataplane_type_ = false;
}

//---------------------------------------------------------------------------//
//                               CHECKOUT                                    //
//---------------------------------------------------------------------------//
void AnnouncementPacket::checkOut() {
  iam(header.classname_, __FUNCTION__);

  short version = recipient_->getVersion();         
  route_->prependLink(router_id_, recipient_->getRouterId(), recipient_->getVersion());    
  leaving(header.classname_, __FUNCTION__);
};
//---------------------------------------------------------------------------//
//                          ROUTE ANNOUNCEMENT PACKET                        //
//---------------------------------------------------------------------------//
RouteAnnouncementPacket::RouteAnnouncementPacket(){}
RouteAnnouncementPacket::RouteAnnouncementPacket(Route* route, Neighbor* recipient) 
  : AnnouncementPacket(route, recipient)
{
  header.classname_ = "RouteAnnouncementPacket";
  header.packet_type_ = ROUTE_PACKET;
}
//---------------------------------------------------------------------------//
//                        WITHDRAW ANNOUNCEMENT PACKET                       //
//---------------------------------------------------------------------------//
WithdrawAnnouncementPacket::WithdrawAnnouncementPacket(){}
WithdrawAnnouncementPacket::WithdrawAnnouncementPacket(Route* route, Neighbor* recipient) 
  : AnnouncementPacket(route, recipient)
{
  header.classname_ = "WithdrawAnnouncementPacket";
  header.packet_type_ = WITHDRAW_ROUTE_PACKET;
}

//---------------------------------------------------------------------------//
//                         FAILURE ANNOUNCEMENT PACKET                       //
//---------------------------------------------------------------------------//
FailureAnnouncementPacket::FailureAnnouncementPacket(){}
FailureAnnouncementPacket::FailureAnnouncementPacket(Route* route, Neighbor* recipient) 
  : AnnouncementPacket(route, recipient)
{
  header.classname_ = "FailureAnnouncementPacket";
  header.packet_type_ = FAILURE_PACKET;
}
//---------------------------------------------------------------------------//
//                               CHECKOUT                                    //
//---------------------------------------------------------------------------//
void FailureAnnouncementPacket::checkOut() {
  iam(header.classname_, __FUNCTION__);

  if (route_->getFailedLink().neighbor_id_ == DEFAULT_NODE) {
    route_->setFailedLink(recipient_->getRouterId(), recipient_->getNeighborId(),
                          recipient_->getVersion());
  }
};


//===========================================================================//
//                          PROCESS BASE PACKET                              //
//===========================================================================//
ProcessPacket::ProcessPacket(Route* route, Neighborhood* neighborhood)
  : Packet(route, neighborhood)
{
  header.classname_ = "ProcessPacket";  
  Packet::is_dataplane_type_ = false;
}
//---------------------------------------------------------------------------//
//                              CHECK IN                                     //
//---------------------------------------------------------------------------//
void ProcessPacket::checkIn() {
  // Prepends path and adjust costs
  iam(header.classname_, __FUNCTION__); 

  short link_cost = neighborhood_->getCost(route_->router_id_);
  
  // Neighbor id is the send or incoming route_id
  route_->neighbor_id_ = route_->router_id_;

  // This router's id can be retrieved from any object on the router
  // e.g. the neighborhood object.
  route_->router_id_ = neighborhood_->getRouterId();

  // Add link cost to route cost.
  route_->link_cost_ = link_cost;
  route_->cost_ += link_cost;

  // Prepend the route to the path list
  route_->prependPath(route_->router_id_);
  
  leaving(header.classname_, __FUNCTION__);
}
//---------------------------------------------------------------------------//
//                          ROUTE PROCESS PACKET                             //
//---------------------------------------------------------------------------//
RouteProcessPacket::RouteProcessPacket(Route* route, Neighborhood* neighborhood)
  : ProcessPacket(route, neighborhood)
{
  header.classname_ = "RouteProcessPacket";
  header.packet_type_ = ROUTE_PACKET;
}
//---------------------------------------------------------------------------//
//                        WITHDRAW PROCESS PACKET                            //
//---------------------------------------------------------------------------//
WithdrawProcessPacket::WithdrawProcessPacket(Route* route, Neighborhood* neighborhood)
  : ProcessPacket(route, neighborhood)
{
  header.classname_ = "WithdrawProcessPacket";
  header.packet_type_ = WITHDRAW_ROUTE_PACKET;
}

//---------------------------------------------------------------------------//
//                         FAILURE PROCESS PACKET                            //
//---------------------------------------------------------------------------//
FailureProcessPacket::FailureProcessPacket(Route* route, Neighborhood* neighborhood)
  : ProcessPacket(route, neighborhood)
{
  header.classname_ = "FailureProcessPacket";
  header.packet_type_ = FAILURE_PACKET;
}
