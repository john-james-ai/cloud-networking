#ifndef PACKET_H
#define PACKET_H

#include <arpa/inet.h>
#include <string>
#include <vector>

#include "Constants.h"
#include "Routes.h"
#include "Neighborhood.h"   
#include "utils.h"

class Route;
class Neighbor;
class Neighborhood;
//===========================================================================//
//                                PACKETS                                    //
//___________________________________________________________________________//
struct PacketHeader {
    std::string packet_type_, classname_;
    short sender_;    
    std::vector<short> recipients_;    
};
//===========================================================================//
class Packet {
    protected:    
        Neighborhood* neighborhood_;      
        void printLinks();  
        std::string message_;
    public:
        short router_id_;
        PacketHeader header;
        Neighbor* recipient_;
        Route* route_;        
        char buffer_[MAX_BUFFER_LENGTH];
        short buffer_length_;
        short dest_id_;
        bool is_dataplane_type_;
        Packet();
        Packet(Route* route, Neighbor* recipient);
        Packet(Route* route,  Neighborhood* neighborhood);                
        void initialize();
        void checkIn();
        void checkOut();
        Route* getRoute();
        void serialize(char* buffer); 
        void deserialize(char* buffer);
        void print();
};
//===========================================================================//
//                        ANNOUNCEMENT PACKET                                //
//===========================================================================//
class AnnouncementPacket : public Packet  {
    public: 
        AnnouncementPacket();
        AnnouncementPacket(Route* route, Neighbor* recipient);        
        void checkOut(); 
};
//---------------------------------------------------------------------------//
//                      ROUTE ANNOUNCEMENT PACKET                            //
//---------------------------------------------------------------------------//
class RouteAnnouncementPacket : public AnnouncementPacket {
    public:
        RouteAnnouncementPacket();
        RouteAnnouncementPacket(Route* route, Neighbor* recipient);
};        
//---------------------------------------------------------------------------//
//                     WITHDRAW ANNOUNCEMENT PACKET                          //
//---------------------------------------------------------------------------//
class WithdrawAnnouncementPacket : public AnnouncementPacket {
    public:
        WithdrawAnnouncementPacket();
        WithdrawAnnouncementPacket(Route* route, Neighbor* recipient);
};    

//---------------------------------------------------------------------------//
//                      FAILURE ANNOUNCEMENT PACKET                          //
//---------------------------------------------------------------------------//
class FailureAnnouncementPacket : public AnnouncementPacket {
    public:
        FailureAnnouncementPacket();
        FailureAnnouncementPacket(Route* route, Neighbor* recipient);
        void checkOut();
};    


//===========================================================================//
//                        PROCESS PACKET                                     //
//===========================================================================//
struct ProcessPacketHeader   {
    std::string packet_type_, classname_, message_;
    short sender_;
};
//---------------------------------------------------------------------------//
class ProcessPacket : public Packet {
    protected:
        char* buffer_;
    public:
        ProcessPacketHeader header;
        ProcessPacket();
        ProcessPacket(Route* route, Neighborhood* neighborhood);
        void checkIn();
};
//---------------------------------------------------------------------------//
//                      ROUTE PROCESS PACKET                            //
//---------------------------------------------------------------------------//
class RouteProcessPacket : public ProcessPacket {
    public:
        RouteProcessPacket();
        RouteProcessPacket(Route* route, Neighborhood* neighborhood);        
};        
//---------------------------------------------------------------------------//
//                     WITHDRAW PROCESS PACKET                          //
//---------------------------------------------------------------------------//
class WithdrawProcessPacket : public ProcessPacket{
    public:
        WithdrawProcessPacket();
        WithdrawProcessPacket(Route* route, Neighborhood* neighborhood);
};    

//---------------------------------------------------------------------------//
//                      FAILURE PROCESS PACKET                               //
//---------------------------------------------------------------------------//
class FailureProcessPacket : public ProcessPacket{
    public:
        FailureProcessPacket();
        FailureProcessPacket(Route* route, Neighborhood* neighborhood);
};    

#endif