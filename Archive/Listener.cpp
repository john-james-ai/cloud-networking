#include "Listener.h"

//===========================================================================//
//                                 LISTENER                                  //
//===========================================================================//

//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
Listener::Listener()
	: classname_("Listener")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
Listener::Listener(Parameters* parameters, Socket* socket) 
    : router_id_(parameters->router_id_),
	  socket_(socket),
	  classname_("Listener")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;	
}
//---------------------------------------------------------------------------//
//                                LISTEN                                     //
//---------------------------------------------------------------------------//
void Listener::listen() {
	iam(classname_, __FUNCTION__);
	
	short this_socket = socket_->getSocket();
	sockaddr_in sender_socket_addr;
	socklen_t sender_socket_addr_length;
	char sender_ip_addr[100];
	char buffer[MAX_BUFFER_LENGTH];
	short bytes_received;
	char* manager;	
	Packet* packet = new Packet();	
	
	// Listen to the socket
	while(1) {

		memset(buffer,0,MAX_BUFFER_LENGTH);	
		
		sender_socket_addr_length = sizeof(sender_socket_addr);
		
		if ((bytes_received = recvfrom(this_socket, (char*)buffer, MAX_BUFFER_LENGTH , 0, 
					(struct sockaddr*)&sender_socket_addr, &sender_socket_addr_length)) == -1)
		{
			perror("connectivity listener: recvfrom failed");
			exit(1);
		}
		// Initialize packet, deserialize, and current node 		
		packet->initialize();		
		packet->deserialize(buffer);				

		// Dispatch
		if (packet->is_dataplane_type_) {
			forwarders_->notify(packet);
		} else {
			manager_->notify(packet);
		}
	}
	delete packet;
}
//---------------------------------------------------------------------------//
//                            SET CONTROLLERS                                //
//---------------------------------------------------------------------------//
void Listener::setEventManager(EventManager* controllers) {
	iam(classname_, __FUNCTION__);
	manager_ = controllers;
};
//---------------------------------------------------------------------------//
//                              SET FORWARDERS                               //
//---------------------------------------------------------------------------//
void Listener::setForwarders(Forwarders* forwarders) {	
	iam(classname_, __FUNCTION__);
	forwarders_ = forwarders;
};
//---------------------------------------------------------------------------//
//                                 START                                     //
//---------------------------------------------------------------------------//
void Listener::start() {
	iam(classname_, __FUNCTION__);	
	listen();
}
//---------------------------------------------------------------------------//
//                                 ID AND NAME                               //
//---------------------------------------------------------------------------//
std::string Listener::name() {
	return classname_;
}
bool Listener::hey() {
	iam(classname_, __FUNCTION__);
	return true;
}
