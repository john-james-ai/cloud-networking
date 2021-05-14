#ifndef PACKET_FACTORY_H
#define PACKET_FACTORY_H

#include <assert.h>
#include <string>
#include <vector>

#include "Constants.h"
#include "Routes.h"
#include "Parameters.h"
#include "Packet.h"
#include "IO.h"
#include "FTable.h"
#include "Neighborhood.h"
#include "utils.h"
struct Parameters;
class Route;
class Routes;
class Neighbor;
class Neighborhood;
class FTable;
class CPLogger;
class Packet;
class AnnouncementPacket;
class ProcessPacket;
class DataPlanePacket;

//===========================================================================//
//                   ABSTRACT PACKET FACTORY MODULE                          //
//___________________________________________________________________________//
//===========================================================================//
//===========================================================================//
//                           PACKET PARAMS                                   //
//===========================================================================//
struct FactoryParams {
    Parameters* parameters_;
    Neighborhood* neighborhood_;
    Routes* routes_;
    CPLogger* log_;
};
//===========================================================================//
//                           PACKET STATS                                    //
//===========================================================================//
struct PStat {
    int routes_announced;    
    int withdraws_announced;
    int failures_announced;
    int routes_processed;
    int withdraws_processed;
    int failures_processed;
    int route_objects_deleted;
    int withdraw_objects_deleted;
    int failure_objects_deleted;
    void print();
};
//===========================================================================//
//                       ABSTRACT PACKET FACTORY                             //
//===========================================================================//
class AbstractPacketFactory {
    protected:
        short router_id_;
        std::string classname_, message_;
        Neighborhood* neighborhood_;
        Neighbor* neighbor_;
        Routes* routes_;
        Route* route_;
        CPLogger* log_;
        PStat* pstats_;
    public:
        void reset();
        AbstractPacketFactory();
        AbstractPacketFactory(FactoryParams* params);
        AnnouncementPacket* createAnnouncementPacket(Route* route, Neighbor* recipient);
        ProcessPacket* createProcessPacket(Route* route, Neighborhood* neighborhood);
        void deallocate(Packet* packet);
        std::string name();
        bool hey();
};
//===========================================================================//
//                               FACTORIES                                   //
//===========================================================================//
//---------------------------------------------------------------------------//
//                             ROUTE FACTORY                                 //
//---------------------------------------------------------------------------//
class RoutePacketFactory : public AbstractPacketFactory {
    public:
        RoutePacketFactory();
        RoutePacketFactory(FactoryParams* params);
        AnnouncementPacket* createAnnouncementPacket(Route* route, Neighbor* recipient);
        ProcessPacket* createProcessPacket(Route* route, Neighborhood* neighborhood);
};
//---------------------------------------------------------------------------//
//                           WITHDRAW FACTORY                                //
//---------------------------------------------------------------------------//
class WithdrawPacketFactory : public AbstractPacketFactory {
    public:
        WithdrawPacketFactory();
        WithdrawPacketFactory(FactoryParams* params);
        AnnouncementPacket* createAnnouncementPacket(Route* route, Neighbor* recipient);
        ProcessPacket* createProcessPacket(Route* route, Neighborhood* neighborhood);
};
//---------------------------------------------------------------------------//
//                           FAILURE FACTORY                                 //
//---------------------------------------------------------------------------//
class FailurePacketFactory : public AbstractPacketFactory {
    public:
        FailurePacketFactory();
        FailurePacketFactory(FactoryParams* params);    
        AnnouncementPacket* createAnnouncementPacket(Route* route, Neighbor* recipient);
        ProcessPacket* createProcessPacket(Route* route, Neighborhood* neighborhood);
};

#endif