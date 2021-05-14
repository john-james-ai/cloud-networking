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
Listener::Listener(Parameters* parameters, Socket* socket, 
				   PacketFactory* packet_factory) 
    : router_id_(parameters->router_id_),
	  socket_(socket),
	  packet_factory_(packet_factory),
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
	
	int this_socket = socket_->getSocket();
	sockaddr_in sender_socket_addr;
	socklen_t sender_socket_addr_length;
	char sender_ip_addr[100];
	char buffer[MAX_BUFFER_LENGTH];
	int bytes_received;
	char* manager;	
	std::string message;
	size_t blen;
	
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

		inet_ntop(AF_INET, &sender_socket_addr.sin_addr, sender_ip_addr, 100);

		// Get packet and deserialize buffer 
		Packet* packet = packet_factory_->createPacket(buffer, sender_ip_addr);
		// Dispatch
		event_manager_->notify(packet);		

		// Clean up the heap.
		delete packet;		
	}
}
//---------------------------------------------------------------------------//
//                            SET EVENT MANAGER                              //
//---------------------------------------------------------------------------//
void Listener::setEventManager(EventManager* event_manager) {
	iam(classname_, __FUNCTION__);
	event_manager_ = event_manager;
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
