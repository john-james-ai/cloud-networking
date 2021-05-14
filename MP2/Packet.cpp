#include "Packet.h"
//===========================================================================//
//                                PACKET                                     //
//===========================================================================//
Packet::Packet() 
{
  classname_ = "Packet";
};

Packet::~Packet() {  
  delete header;
};
//===========================================================================//
//                            DATA PLANE PACKET                              //
//===========================================================================//
DataPlanePacket::DataPlanePacket() 
{
  classname_ = "DataPlanePacket";
  header = new PacketHeader();

};
//---------------------------------------------------------------------------//
//                         SEND MESSAGE SERIALIZER                           //
//---------------------------------------------------------------------------//
size_t DataPlanePacket::serialize(char* buffer) {
  // Sets the packet type
  iam(classname_, __FUNCTION__);
  
  short* short_ptr;
  short temp_short;
  int* int_ptr;  
  int temp_int;
  size_t length=0;
  std::string message;  

  // Initialize the memory location pointed to by buffer and assign pointer.
  memset(buffer,0,MAX_BUFFER_LENGTH);  
  char* char_ptr = buffer;

  // Packet type
  strncpy(char_ptr, header->packet_type_.c_str(),4);
  char_ptr = char_ptr + 4 * sizeof(char);  
  length += 4;  

  // Dest id
  short_ptr = (short* )char_ptr;
  temp_short = htons(dest_id_);
  memcpy(short_ptr,&temp_short, sizeof(short));
  ++short_ptr;
  length += sizeof(temp_short);

  // Packet body i.e. the message.
  char_ptr = (char* )short_ptr;
  strncpy(char_ptr,body_.c_str(),MAX_MESSAGE_LENGTH);
  length += strlen(char_ptr);

  return length;
}
//---------------------------------------------------------------------------//
//                        SEND MESSAGE DESERIALIZER                          //
//---------------------------------------------------------------------------//
size_t DataPlanePacket::deserialize(char* buffer) {
  // Deserializes gets the destination for routing table lookup.
  iam(classname_, __FUNCTION__);

  int temp_int;    
  size_t length=0;
  short temp_short;
  char* char_ptr;
  short* short_ptr;

  // Initialize the point to the start of the buffer.
  char_ptr = buffer;

  // Packet type
  header->packet_type_.assign(buffer,0,4);  
  printc(classname_, __FUNCTION__, "Deserialized packet_type", header->packet_type_, char_ptr);
  char_ptr = char_ptr + 4 * sizeof(char);
  length += 4;

  // Destination
  short_ptr = (short* )char_ptr;
  memcpy(&temp_short, short_ptr, sizeof(short));
  dest_id_ = ntohs(temp_short);
  prints(classname_, __FUNCTION__, "Deserialized dest id", dest_id_, &dest_id_);
  ++short_ptr;
  length += 2;

  // Message
  char_ptr = (char* )short_ptr;
  body_.assign(char_ptr,0,MAX_MESSAGE_LENGTH);  
  length += strlen(char_ptr);

  return length;
};
//---------------------------------------------------------------------------//
//                           SEND MESSAGE PRINT                              //
//---------------------------------------------------------------------------//
void DataPlanePacket::print() {
  std::cout << "\n=====================================" << std::endl;
  std::cout << "            Packet Header  " << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "            Type: " << header->packet_type_ << std::endl;   
  std::cout << "-------------------------------------" << std::endl;  
  std::cout << "     Destination: " << shortToString(dest_id_) << std::endl;
  std::cout << "        Next Hop: " << shortToString(next_hop_) << std::endl;
  std::cout << "         Message: " << body_ << std::endl;  
  std::cout << "\n=====================================" << std::endl;
}

//===========================================================================//
//                              SEND MESSAGE                                 //
//===========================================================================//
SendMessage::SendMessage() 
  : DataPlanePacket()
  {
    classname_ = "SendMessage";
    header = new PacketHeader();
    header->packet_type_ = SEND_PACKET_TYPE;
    iam(classname_, __FUNCTION__);
  };

//===========================================================================//
//                              FORWARD MESSAGE                              //
//===========================================================================//
ForwardMessage::ForwardMessage() 
  : DataPlanePacket()
  {
    classname_ = "ForwardMessage";
    header = new PacketHeader();
    header->packet_type_ = FORWARD_PACKET_TYPE;
    iam(classname_, __FUNCTION__);
  };




//===========================================================================//
//                             PACKET                                 //
//===========================================================================//
ControlPlanePacket::ControlPlanePacket() 
  : Packet()
  {
    classname_ = "ControlPlanePacket";
    header = new PacketHeader();  
    route_ = new Route();          
    iam(classname_, __FUNCTION__);
  };
//---------------------------------------------------------------------------//
//                            SERIALIZE                               //
//---------------------------------------------------------------------------//
size_t ControlPlanePacket::serialize(char* buffer) {
  iam(classname_, __FUNCTION__);

  
  short* short_ptr;
  short  temp_short;
  int* int_ptr;
  int temp_int;
  size_t* size_t_ptr;
  size_t vec_length, temp_size_t, length=0;
  std::string message;

  // Initialize the memory location pointed to by buffer and assign pointer.
  memset(buffer,0,MAX_BUFFER_LENGTH);  
  char* char_ptr = buffer;

  // Packet type
  strncpy(buffer, header->packet_type_.c_str(),4);
  printc(classname_, __FUNCTION__, "Serialized packet_type", std::string(char_ptr).c_str(), char_ptr);
  char_ptr = char_ptr + 4;  
  length += 4;

  // Path length
  size_t_ptr = (size_t* )char_ptr;
  temp_size_t = htonl(route_->path_.size());
  memcpy(size_t_ptr,&temp_size_t, sizeof(temp_size_t));
  printt(classname_, __FUNCTION__, "Serialized path_length", route_->path_.size(), size_t_ptr);
  ++size_t_ptr;
  length += sizeof(temp_size_t);

  // Link Cost
  int_ptr = (int* )size_t_ptr;
  temp_int = htonl(route_->link_cost_);
  memcpy(int_ptr,&temp_int, sizeof(int));
  printi(classname_, __FUNCTION__, "Serialized link_cost", route_->link_cost_, int_ptr);
  ++int_ptr;
  length += sizeof(temp_int);

  // Cost
  temp_int = htonl(route_->cost_);
  memcpy(int_ptr,&temp_int, sizeof(int));
  printi(classname_, __FUNCTION__, "Serialized link_cost", route_->cost_, int_ptr);
  ++int_ptr;  
  length += sizeof(temp_int);


  //  Router id
  short_ptr = (short* )int_ptr;
  temp_short = htons(route_->router_id_);
  memcpy(short_ptr,&temp_short, sizeof(short));
  prints(classname_, __FUNCTION__, "Serialized router_id", route_->router_id_, short_ptr);
  ++short_ptr;
  length += sizeof(temp_short);
  
  // Neighbor id
  temp_short = htons(route_->neighbor_id_);
  memcpy(short_ptr,&temp_short, sizeof(short));
  prints(classname_, __FUNCTION__, "Serialized neighbor_id", route_->neighbor_id_, short_ptr);
  ++short_ptr;
  length += sizeof(temp_short);


  // Dest id
  temp_short = htons(route_->dest_id_);
  memcpy(short_ptr,&temp_short, sizeof(short));
  prints(classname_, __FUNCTION__, "Serialized dest_id", route_->dest_id_, short_ptr);
  ++short_ptr;
  length += sizeof(temp_short);
  
 // Path
  std::vector<short>::iterator it;
  for (it=route_->path_.begin();it != route_->path_.end();++it) {

      temp_short = htons(*it);
      memcpy(short_ptr,&temp_short, sizeof(short));
      ++short_ptr;
      length += sizeof(temp_short);
  }
  
  leaving(classname_, __FUNCTION__);

  return length;
  
}
//---------------------------------------------------------------------------//
//                           DESERIALIZE                              //
//---------------------------------------------------------------------------//
size_t ControlPlanePacket::deserialize(char* buffer) {
  // Deserialize into a route object.
  iam(classname_, __FUNCTION__);  

  int temp_int;
  size_t vec_length, temp_size_t, length=0;    
  size_t* size_t_ptr;
  short temp_short, node_id, router_id, neighbor_id;
  short* short_ptr;
  int* int_ptr;
  char* char_ptr = buffer;
  char* temp_buffer =  (char*)calloc(sizeof(char),MAX_BUFFER_LENGTH);
  std::string message;
  

  // packet type
  header->packet_type_.assign(buffer,0,4);  
  printc(classname_, __FUNCTION__, "Deserialized packet_type", header->packet_type_, char_ptr);
  char_ptr += 4*sizeof(char);  
  length += 4;

  // Path length
  size_t_ptr = (size_t* )char_ptr;
  memcpy(&temp_size_t,size_t_ptr,sizeof(size_t));
  vec_length = ntohl(temp_size_t);
  printt(classname_, __FUNCTION__, "Deserialized path length", vec_length, &vec_length);
  ++size_t_ptr;
  length += sizeof(vec_length);

  // Link Cost
  int_ptr = (int* )size_t_ptr;
  memcpy(&temp_int,int_ptr,sizeof(int));
  route_->link_cost_ = ntohl(temp_int);
  printi(classname_, __FUNCTION__, "Deserialized link cost", route_->link_cost_, &route_->link_cost_);
  ++int_ptr;  
  length += sizeof(int);

  // Cost
  memcpy(&temp_int,int_ptr,sizeof(int));
  route_->cost_ = ntohl(temp_int);
  printi(classname_, __FUNCTION__, "Deserialized cost", route_->cost_, &route_->cost_);
  ++int_ptr;
  length += sizeof(int);

  //  Router id  
  short_ptr = (short* )int_ptr;
  memcpy(&temp_short,short_ptr,sizeof(short));
  route_->router_id_ = ntohs(temp_short);
  prints(classname_, __FUNCTION__, "Deserialized router_id", route_->router_id_, &route_->router_id_);
  ++short_ptr;
  length += sizeof(short);

  // Neighbor id  
  memcpy(&temp_short,short_ptr,sizeof(short));
  route_->neighbor_id_ = ntohs(temp_short);
  prints(classname_, __FUNCTION__, "Deserialized neighbor id", route_->neighbor_id_, &route_->neighbor_id_);
  ++short_ptr;
  length += sizeof(short);

  // Dest id
  memcpy(&temp_short,short_ptr,sizeof(short));
  route_->dest_id_ = ntohs(temp_short);
  prints(classname_, __FUNCTION__, "Deserialized dest id", route_->dest_id_, &route_->dest_id_);
  ++short_ptr;
  length += sizeof(short);

  route_->formatRouteId();  

  // Path
  for (size_t i = 0;i<vec_length;++i) {
    memcpy(&temp_short,short_ptr,sizeof(short));
    node_id = ntohs(temp_short);
    route_->path_.push_back(node_id);
    ++short_ptr;
    length += sizeof(short);
  }

  // Obtain sender from router_id_ member of route object.
  header->sender_ = route_->router_id_;

  leaving(classname_, __FUNCTION__);

  return length;

}
//---------------------------------------------------------------------------//
//                              PRINT                                 //
//---------------------------------------------------------------------------//
void ControlPlanePacket::print() {
        
  std::cout << "\n=====================================" << std::endl;
  std::cout << "            Packet Header  " << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "            Type: " << header->packet_type_ << std::endl;   
  std::cout << "          Sender: " << header->sender_ << std::endl;  
  std::cout << "      Recipients: " << header->recipients_.size() << std::endl;  
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "                Route " << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "        Neighbor: " << route_->neighbor_id_ << std::endl;
  std::cout << "     Destination: " << route_->dest_id_ << std::endl;
  std::cout << "       Link Cost: " << route_->link_cost_ << std::endl;
  std::cout << "            Cost: " << route_->cost_ << std::endl;
  std::cout << "-------------------------------------" << std::endl;  
  std::cout << "               Path " << std::endl;
  std::cout << "     Path Length: " << route_->path_.size() << std::endl;
  print_vec(route_->path_);
  std::cout << "=====================================\n" << std::endl;   
}
//===========================================================================//
//                           ROUTE PACKET                                    //
//===========================================================================//
RoutePacket::RoutePacket() 
  : ControlPlanePacket()
  {
    classname_ = "RoutePacket";    
    header = new PacketHeader();
    header->packet_type_ = ROUTE_PACKET;    
    iam(classname_, __FUNCTION__);
  };
//===========================================================================//
//                           NEIGHBOR PACKET                                 //
//===========================================================================//
NeighborPacket::NeighborPacket() 
  : ControlPlanePacket()
  {
    classname_ = "NeighborPacket";    
    header = new PacketHeader();
    header->packet_type_ = NEIGHBOR_PACKET;    
    iam(classname_, __FUNCTION__);
  };

//===========================================================================//
//                          WITHDRAW PACKET                                  //
//===========================================================================//
WithdrawPacket::WithdrawPacket() 
  : ControlPlanePacket()
  {
    classname_ = "WithdrawPacket";    
    header = new PacketHeader();
    header->packet_type_ = WITHDRAW_PACKET;    
    iam(classname_, __FUNCTION__);
  };

//===========================================================================//
//                             PACKET FACTORY                                //
//===========================================================================//
PacketFactory::PacketFactory(Parameters* parameters)
  : router_id_(parameters->router_id_)  
  {
    classname_ = "PacketFactory";
    iam(classname_, __FUNCTION__);
  };

//---------------------------------------------------------------------------//
//                            CREATE PACKET                                  //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createPacket(char* buffer, char* sender_ip_addr) {
  // Creates a packet via delegation 
  iam(classname_, __FUNCTION__);

  // Process packet from manager
  if (!strstr(sender_ip_addr, "10.1.1.")) {
    return createSendMessagePacket(buffer, sender_ip_addr);
  }

  // Process packets from neighbors
  if (strstr(sender_ip_addr, "10.1.1.")) {

    if (!strncmp(buffer, SEND_PACKET_TYPE.c_str(),4)) {
      return createForwardMessagePacket(buffer, sender_ip_addr);
    }

    if (!strncmp(buffer, FORWARD_PACKET_TYPE.c_str(),4)) {
      return createForwardMessagePacket(buffer, sender_ip_addr);
    }

    if (!strncmp(buffer, ROUTE_PACKET.c_str(),4)) {
      return createRoutePacket(buffer, sender_ip_addr);
    }

    if (!strncmp(buffer, NEIGHBOR_PACKET.c_str(),4)) {
      return createNeighborPacket(buffer, sender_ip_addr);      
    }

    if (!strncmp(buffer, WITHDRAW_PACKET.c_str(), 4)) {
    return createWithdrawPacket(buffer, sender_ip_addr);      
    }

    std::string msg = "Unable to obtain a valid packet type from the buffer";
        error(classname_, __FUNCTION__, msg);
        exit(-1);
  }
}
//---------------------------------------------------------------------------//
//                       CREATE SEND MESSAGE PACKET                          //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createSendMessagePacket(char* buffer, char* sender_ip_addr) {
  iam(classname_, __FUNCTION__);
  

  SendMessage* packet = new SendMessage();
  packet->deserialize(buffer);
  packet->header->packet_type_ = SEND_PACKET_TYPE;  
  strncpy(packet->header->sender_ip_, sender_ip_addr,12);
  packet->header->sender_ = atoi(
					strchr(strchr(strchr(sender_ip_addr,'.')+1,'.')+1,'.')+1);  
  return packet;
}

//---------------------------------------------------------------------------//
//                     CREATE FORWARD MESSAGE PACKET                         //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createForwardMessagePacket(char* buffer, char* sender_ip_addr) {
  // Creates a message packet via delegation to the Send or Forward Methods  
  iam(classname_, __FUNCTION__);

  ForwardMessage* packet = new ForwardMessage();
  packet->deserialize(buffer);
  packet->header->packet_type_ = FORWARD_PACKET_TYPE;
  strncpy(packet->header->sender_ip_, sender_ip_addr,12); 
  packet->header->sender_ = atoi(
					strchr(strchr(strchr(sender_ip_addr,'.')+1,'.')+1,'.')+1);   
  return packet;
}
//---------------------------------------------------------------------------//
//                           CREATE ROUTE PACKET                             //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createRoutePacket(char* buffer, char* sender_ip_addr) {
  iam(classname_, __FUNCTION__);  

  RoutePacket* packet = new RoutePacket();
  packet->deserialize(buffer);
  packet->header->packet_type_ = ROUTE_PACKET;
  strncpy(packet->header->sender_ip_, sender_ip_addr,12);
  packet->header->sender_ = atoi(
					strchr(strchr(strchr(sender_ip_addr,'.')+1,'.')+1,'.')+1);  
  return packet;
}
//---------------------------------------------------------------------------//
//                         CREATE NEIGHBOR PACKET                            //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createNeighborPacket(char* buffer, char* sender_ip_addr) {
  iam(classname_, __FUNCTION__);  

  RoutePacket* packet = new RoutePacket();
  packet->deserialize(buffer);
  packet->header->packet_type_ = NEIGHBOR_PACKET;
  strncpy(packet->header->sender_ip_, sender_ip_addr,12);
  packet->header->sender_ = atoi(
					strchr(strchr(strchr(sender_ip_addr,'.')+1,'.')+1,'.')+1);
  return packet;
}
//---------------------------------------------------------------------------//
//                          CREATE WITHDRAW PACKET                           //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createWithdrawPacket(char* buffer, char* sender_ip_addr) {
  iam(classname_, __FUNCTION__);  

  WithdrawPacket* packet = new WithdrawPacket();
  packet->deserialize(buffer);
  packet->header->packet_type_ = WITHDRAW_PACKET;
  strncpy(packet->header->sender_ip_, sender_ip_addr,12);
  packet->header->sender_ = atoi(
					strchr(strchr(strchr(sender_ip_addr,'.')+1,'.')+1,'.')+1);
  return packet;
}
//---------------------------------------------------------------------------//
//                           CREATE ROUTE PACKET                             //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createRoutePacket(Route* route, std::vector<short> recipients) {
  // Creates a packet via delegation 
  iam(classname_, __FUNCTION__);
  
  RoutePacket* packet = new RoutePacket();
  packet->header->packet_type_ = ROUTE_PACKET;
  packet->header->sender_ = router_id_;
  packet->header->recipients_ = recipients;
  packet->route_ = route;
  return packet;
};
//---------------------------------------------------------------------------//
//                         CREATE NEIGHBOR PACKET                            //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createNeighborPacket(Route* route, std::vector<short> recipients) {
  // Creates a packet via delegation 
  iam(classname_, __FUNCTION__);
  
  RoutePacket* packet = new RoutePacket();
  packet->header->packet_type_ = NEIGHBOR_PACKET;
  packet->header->sender_ = router_id_;
  packet->header->recipients_ = recipients;
  packet->route_ = route;
  return packet;
};

//---------------------------------------------------------------------------//
//                         CREATE WITHDRAW PACKET                            //
//---------------------------------------------------------------------------//
Packet* PacketFactory::createWithdrawPacket(Route* route, std::vector<short> recipients) {
  // Creates a packet via delegation 
  iam(classname_, __FUNCTION__);
  
  WithdrawPacket* packet = new WithdrawPacket();
  packet->header->packet_type_ = WITHDRAW_PACKET;
  packet->header->sender_ = router_id_;
  packet->header->recipients_ = recipients;
  packet->route_ = route;
  return packet;
};
