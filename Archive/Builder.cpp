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
    //                              DATA PLANE                               //
    //=======================================================================//    
    //-----------------------------------------------------------------------//
    //                               LOGGERS                                 //
    //-----------------------------------------------------------------------//
    CPLogger* cp_logger = new CPLogger(parameters_);   
    DPLogger* dp_logger = new DPLogger(parameters_);   
    
    //-----------------------------------------------------------------------//
    //                          NETWORK OBJECTS                              //
    //-----------------------------------------------------------------------//
    Socket* socket = new Socket(parameters_->router_id_);         
    //-----------------------------------------------------------------------//
    Network* network = new Network(parameters_, socket);     
    //-----------------------------------------------------------------------//
    //                           COMMUNICATIONS                              //
    //-----------------------------------------------------------------------//
    Listener* listener = new Listener(parameters_, socket);
    Messenger* messenger = new Messenger(parameters_, network, socket);




    //=======================================================================//
    //                           CONTROL PLANE                               //
    //=======================================================================//
    //-----------------------------------------------------------------------//
    //                       ROUTING INFORMATION BASE                        //
    //-----------------------------------------------------------------------//
    Neighborhood* neighborhood = new Neighborhood(parameters_);        
    //-----------------------------------------------------------------------//
    Routes* routes = new Routes(parameters_);
    //-----------------------------------------------------------------------//
    FTable* ftable = new FTable(parameters_);    
    //-----------------------------------------------------------------------//
    RIBPack* rib_pack = new RIBPack();    
    rib_pack->neighborhood_ = neighborhood;
    rib_pack->routes_ = routes;
    rib_pack->ftable_ = ftable;
    //-----------------------------------------------------------------------//
    RouteInfoBase* rib = new RouteInfoBase(parameters_, rib_pack, messenger, cp_logger);
    //-----------------------------------------------------------------------//




    //=======================================================================//
    //                          EVENT HANDLERS                               //
    //=======================================================================//
    //-----------------------------------------------------------------------//
    //                           ROUTE CONTROL                               //
    //-----------------------------------------------------------------------//
    EventHandler* route_control = new EventHandler(parameters_, rib);  
    RouteHandler* rh = new RouteHandler(parameters_, rib);
    WithdrawHandler* wrh = new WithdrawHandler(parameters_, rib);
    FailureHandler* fh = new FailureHandler(parameters_, rib);
    AdvertiseHandler* ah = new AdvertiseHandler(parameters_, rib);
    EventManager* event_manager = new EventManager(parameters_);
    event_manager->attach(rh);
    event_manager->attach(wrh);
    event_manager->attach(fh);
    event_manager->attach(ah);


    //-----------------------------------------------------------------------//
    //                             FORWARDERS                                //
    //-----------------------------------------------------------------------//
    Forwarder* forwarder = new Forwarder(parameters_, ftable, 
                                         messenger, dp_logger);
    Forwarders* forwarders = new Forwarders();
    forwarders->attach(forwarder);





    //=======================================================================//
    //                          NETWORK MANAGEMENT                           //
    //=======================================================================//
    EKG* ekg = new EKG(parameters_, neighborhood, routes, event_manager);
    Pulse* pulse = new Pulse(parameters_, neighborhood, messenger);
    Advertiser* advertiser = new Advertiser(parameters_, event_manager);




    //=======================================================================//
    //                               ASSEMBLY                                //
    //=======================================================================//
    //-----------------------------------------------------------------------//
    //                           DATA PLANE ASSEMBLY                         //
    //-----------------------------------------------------------------------//
    //-----------------------------------------------------------------------//
    //                           SET EVENT HANDLERS                          //
    //-----------------------------------------------------------------------//    
    listener->setForwarders(forwarders);
    listener->setEventManager(event_manager);    
    //-----------------------------------------------------------------------//
    DataPlane* data_plane = new DataPlane(parameters_, listener, messenger);
    //-----------------------------------------------------------------------//



    //-----------------------------------------------------------------------//
    //                          CONTROL PLANE ASSEMBLY                       //
    //-----------------------------------------------------------------------//
    NetworkManager* network_manager = new NetworkManager();
    network_manager->ekg_  = ekg;
    network_manager->pulse_  = pulse;
    network_manager->advertiser_  = advertiser;
    //-----------------------------------------------------------------------//
    ControlPlane* control_plane = new ControlPlane(rib, network_manager);



    //-----------------------------------------------------------------------//
    //                             LOAD ROUTER                               //
    //-----------------------------------------------------------------------//
    router_ = new Router(parameters_, control_plane, data_plane);
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