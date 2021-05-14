#include "Builder.h"

//===========================================================================//
//                                 BUILDER                                   //
//===========================================================================//
Builder::Builder() 
    : classname_("Builder")
    {
        iam(classname_, __FUNCTION__);
    }
Builder::Builder(Parameters* parameters)                    
    : parameters_(parameters),
      classname_("Builder")
    {
        iam(classname_, __FUNCTION__);
    }
//---------------------------------------------------------------------------//
//                              BUILD ROUTER                                 //
//---------------------------------------------------------------------------//

void Builder::buildRouter() {
    iam(classname_, __FUNCTION__);

    //=======================================================================//
    //                               LOGGER                                  //
    //=======================================================================//
    Logger* logger = new Logger(parameters_);           

    //=======================================================================//
    //                           PACKET FACTORY                              //
    //=======================================================================//
    PacketFactory* packet_factory = new PacketFactory(parameters_);

    //=======================================================================//
    //                           CONTROL PLANE                               //
    //=======================================================================//
    Neighborhood* neighborhood = new Neighborhood(parameters_);        
    //-----------------------------------------------------------------------//
    Routes* routes = new Routes(parameters_);
    //-----------------------------------------------------------------------//
    FTable* ftable = new FTable(parameters_);
    //-----------------------------------------------------------------------//
    ControlPlane* control_plane_ = new ControlPlane(parameters_, routes,
                                                    neighborhood, ftable,
                                                    logger, packet_factory);
    //=======================================================================//
    //                      NETWORK OPERATIONS CENTER                        //
    //=======================================================================//    
    NOC* noc = new NOC(parameters_, control_plane_, packet_factory);

    //=======================================================================//
    //                              DATA PLANE                               //
    //=======================================================================//
    //-----------------------------------------------------------------------//
    //                          NETWORK OBJECTS                              //
    //-----------------------------------------------------------------------//
    Socket* socket = new Socket(parameters_->router_id_);         
    //-----------------------------------------------------------------------//
    Network* network = new Network(parameters_, socket);     
    //-----------------------------------------------------------------------//
    //                           COMMUNICATIONS                              //
    //-----------------------------------------------------------------------//
    Listener* listener = new Listener(parameters_, socket, packet_factory);
    Messenger* messenger = new Messenger(parameters_, network, socket);
    //-----------------------------------------------------------------------//
    Pulse* pulse = new Pulse(parameters_,neighborhood, messenger, packet_factory);
    //-----------------------------------------------------------------------//
    DataPlane* data_plane_ = new DataPlane(parameters_, pulse, listener, 
                                           messenger, logger);    

    //=======================================================================//
    //                          EVENT HANDLERS                               //
    //=======================================================================//
    DataPlaneHandler* data_plane_handler = new DataPlaneHandler(parameters_, control_plane_, data_plane_); 
    NeighborHandler* neighbor_handler = new NeighborHandler(parameters_, control_plane_, data_plane_);
    RouteHandler* route_handler = new RouteHandler(parameters_, control_plane_, data_plane_);
    WithdrawHandler* withdraw_handler = new WithdrawHandler(parameters_, control_plane_, data_plane_);    
    //-----------------------------------------------------------------------//
    EventManager* event_manager = new EventManager(parameters_);
    event_manager->attach(data_plane_handler);
    event_manager->attach(route_handler);
    event_manager->attach(neighbor_handler);
    event_manager->attach(withdraw_handler);
    
    //=======================================================================//
    //                              ASSEMBLY                                 //
    //=======================================================================//    
    // Event manager
    control_plane_->setEventManager(event_manager);
    data_plane_->setEventManager(event_manager);

    // Final assembly
    router_ = new Router(parameters_, control_plane_, noc, data_plane_);

    //=======================================================================//
    //                                CHECK                                  //
    //=======================================================================//        
    router_->hey();
    
    }


//---------------------------------------------------------------------------//
//                                GET ROUTER                                 //
//---------------------------------------------------------------------------//
Router* Builder::getRouter() {
    return router_;
}
//===========================================================================//
//                                 DIRECTOR                                  //
//===========================================================================//
Director::Director() {}
Director::Director(Builder& builder) 
    : builder_(builder), 
      classname_("Director")      
    {}

Router* Director::buildRouter() {        
    builder_.buildRouter();
    return builder_.getRouter();
}