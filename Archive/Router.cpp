#include "Router.h"

//---------------------------------------------------------------------------//
//                                ROUTER                                     //
//---------------------------------------------------------------------------//
Router::Router() 
    : classname_("Router")      
{
    iam(classname_, __FUNCTION__);
    std::string* address_ = (std::string*)this;
}

Router::Router(Parameters* parameters, ControlPlane* control_plane,
                DataPlane* data_plane) 
    : parameters_(parameters), 
      router_id_(parameters->router_id_),
      control_plane_(control_plane),
      data_plane_(data_plane),
      classname_("Router")      
      {
          iam(classname_, __FUNCTION__);
          std::string* address_ = (std::string*)this;
      }

//---------------------------------------------------------------------------//
short Router::getRouterId() {
    iam(classname_, __FUNCTION__);
    return router_id_;
};
//---------------------------------------------------------------------------//
ControlPlane* Router::getControlPlane() {
    iam(classname_, __FUNCTION__);
    return control_plane_;
};
//---------------------------------------------------------------------------//
DataPlane* Router::getDataPlane() {
    iam(classname_, __FUNCTION__);
    return data_plane_;
};
//---------------------------------------------------------------------------//
void Router::startDataPlane() {        
    data_plane_->startListening();
};
//---------------------------------------------------------------------------//
void Router::startPulse() {    
    control_plane_->startPulse();
};
//---------------------------------------------------------------------------//
void Router::startEKG() {    
    control_plane_->startEKG();
};
//---------------------------------------------------------------------------//
void Router::startAdvertiser() {    
    control_plane_->startAdvertiser();
};
//---------------------------------------------------------------------------//
bool Router::hey() {    

	holla(classname_, address_);
   
    assert((*control_plane_).hey());
    if (TRACE) {
        std::cout << "\n\n========================================" << std::endl;
        
        std::cout << "      Control Plane Build Complete!" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
    assert((*data_plane_).hey());
    if (TRACE) {
        std::cout << "        Data Plane Build Complete!" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "           Router Build Complete!" << std::endl;
        std::cout << "========================================\n\n" << std::endl;
    }
    
    return true;
};
