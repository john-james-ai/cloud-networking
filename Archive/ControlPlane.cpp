#include "ControlPlane.h"

//===========================================================================//
//                               DATA PLANE                                  //
//===========================================================================//

//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
ControlPlane::ControlPlane() 
	: classname_("ControlPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
ControlPlane::ControlPlane(RouteInfoBase* rib, NetworkManager* manager)
    : rib_(rib_),   
	  ekg_(manager->ekg_), pulse_(manager->pulse_), 
	  advertiser_(manager->advertiser_),
	  classname_("ControlPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}

//---------------------------------------------------------------------------//
void ControlPlane::startPulse() {
	iam(classname_, __FUNCTION__);

	pulse_->start();
}
//---------------------------------------------------------------------------//
void ControlPlane::startEKG() {
	iam(classname_, __FUNCTION__);

	ekg_->start();
}
//---------------------------------------------------------------------------//
void ControlPlane::startAdvertiser() {
	iam(classname_, __FUNCTION__);

	advertiser_->start();
}

//---------------------------------------------------------------------------//
bool ControlPlane::hey() {
	holla(classname_, address_);

	assert(ekg_->hey());
	assert(pulse_->hey());
	assert(advertiser_->hey());
	

    return true;
};