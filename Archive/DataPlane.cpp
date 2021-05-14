#include "DataPlane.h"

//===========================================================================//
//                               DATA PLANE                                  //
//===========================================================================//

//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
DataPlane::DataPlane() 
	: classname_("DataPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
DataPlane::DataPlane(Parameters* parameters, Listener* listener, Messenger* messenger)
    : parameters_(parameters), listener_(listener), messenger_(messenger),
	  classname_("DataPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}

//---------------------------------------------------------------------------//
void DataPlane::startListening() {
	iam(classname_, __FUNCTION__);

	listener_->start();
}

//---------------------------------------------------------------------------//
bool DataPlane::hey() {
	holla(classname_, address_);

	assert(listener_->hey());
	assert(messenger_->hey());
	

    return true;
};