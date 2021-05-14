#ifndef PACKET_H
#define PACKET_H

#include <arpa/inet.h>
#include <string>
#include <vector>

#include "Constants.h"
#include "Parameters.h"
#include "Routes.h"
#include "utils.h"

class Route;
class Parameters;
struct PacketHeader {
    std::string packet_type_;
    short sender_;
    char sender_ip_[15];
    std::vector<short> recipients_;    
};
//===========================================================================//
//                                PACKET                                     //
//===========================================================================//
class Packet {
    protected:    
        std::string message_, classname_;
    public:
        PacketHeader* header;        
        char buffer_[MAX_BUFFER_LENGTH];
        int buffer_length_;  
        short next_hop_; 
        short neighbor_id_;
        short dest_id_;
        std::string body_; // for dataplane packets, the message being sent.
        Route* route_;   
        Packet();  
        virtual ~Packet();
        virtual size_t serialize(char* buffer) = 0;
        virtual size_t deserialize(char* buffer) = 0;
        virtual void print() = 0;
};
//===========================================================================//
//                         DATA PLANE PACKET                                 //
//===========================================================================//
class DataPlanePacket : public Packet {
    public:
        DataPlanePacket();
        virtual size_t serialize(char* buffer);
        virtual size_t deserialize(char* buffer);
        virtual void print();                    
};
//---------------------------------------------------------------------------//
//                        SEND MESSAGE PACKET                                //
//---------------------------------------------------------------------------//
class SendMessage : public DataPlanePacket {
    public:        
        SendMessage();
        
};
//---------------------------------------------------------------------------//
//                       FORWARD MESSAGE PACKET                              //
//---------------------------------------------------------------------------//
class ForwardMessage : public DataPlanePacket {
    public:
        ForwardMessage();        
};


//===========================================================================//
//                       CONTROL PLANE PACKETS                               //
//===========================================================================//
class ControlPlanePacket : public Packet {
    public:
        ControlPlanePacket();
        virtual size_t serialize(char* buffer);
        virtual size_t deserialize(char* buffer);
        virtual void print();        
};    
//---------------------------------------------------------------------------//
//                        ROUTE PACKET                                       //
//---------------------------------------------------------------------------//
class RoutePacket : public ControlPlanePacket {
    public:        
        RoutePacket();
};
//---------------------------------------------------------------------------//
//                        NEIGHBOR PACKET                                    //
//---------------------------------------------------------------------------//
class NeighborPacket : public ControlPlanePacket {
    public:        
        NeighborPacket();
};
//---------------------------------------------------------------------------//
//                        WITHDRAW PACKET                                    //
//---------------------------------------------------------------------------//
class WithdrawPacket : public ControlPlanePacket {
    public:
        WithdrawPacket();
};
//===========================================================================//
//                          PACKET FACTORY                                   //
//===========================================================================//
class PacketFactory {
    protected:    
        std::string message_, classname_;  
    private:
        Packet* createSendMessagePacket(char* buffer, char* sender_ip_addr);
        Packet* createForwardMessagePacket(char* buffer, char* sender_ip_addr);
        Packet* createRoutePacket(char* buffer, char* sender_ip_addr);
        Packet* createNeighborPacket(char* buffer, char* sender_ip_addr);
        Packet* createWithdrawPacket(char* buffer, char* sender_ip_addr);   
        
    public:
        short router_id_;
        PacketFactory(Parameters* parameters);
        Packet* createPacket(char* buffer, char* sender_ip_addr);
        Packet* createRoutePacket(Route* route, std::vector<short> recipients);         
        Packet* createNeighborPacket(Route* route, std::vector<short> recipients);
        Packet* createWithdrawPacket(Route* route, std::vector<short> recipients);         

};
#endif