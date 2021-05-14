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
DataPlane::DataPlane(Parameters* parameters, Pulse* pulse,  
  		             Listener* listener, Messenger* messenger, 
					 Logger* logger)
    : parameters_(parameters), pulse_(pulse),
	  listener_(listener), messenger_(messenger), logger_(logger),	  
	  classname_("DataPlane")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
//---------------------------------------------------------------------------//
void DataPlane::setEventManager(EventManager* event_manager) {
	iam(classname_, __FUNCTION__);

	listener_->setEventManager(event_manager);
}
//---------------------------------------------------------------------------//
void DataPlane::start() {
	iam(classname_, __FUNCTION__);
	listener_->start();
}
//---------------------------------------------------------------------------//
void DataPlane::startPulse() {
	iam(classname_, __FUNCTION__);	
	pulse_->start();
}
//---------------------------------------------------------------------------//
void DataPlane::send(Packet* packet) {
	iam(classname_, __FUNCTION__);

	messenger_->send(packet);
	logger_->send(packet);
	
}
//---------------------------------------------------------------------------//
void DataPlane::forward(Packet* packet) {
	iam(classname_, __FUNCTION__);

	messenger_->send(packet);
	logger_->forward(packet);
	
}
//---------------------------------------------------------------------------//
void DataPlane::announce(Packet* packet) {
	iam(classname_, __FUNCTION__);

	messenger_->send(packet);
}
//---------------------------------------------------------------------------//
void DataPlane::receive(Packet* packet) {
	iam(classname_, __FUNCTION__);
	
	logger_->receive(packet);
	
}
//---------------------------------------------------------------------------//
void DataPlane::drop(Packet* packet) {
	iam(classname_, __FUNCTION__);
	
	logger_->drop(packet);
	
}
//---------------------------------------------------------------------------//
bool DataPlane::hey() {
	holla(classname_, address_);

	assert(pulse_->hey());
	assert(listener_->hey());	
	assert(messenger_->hey());
	

    return true;
};