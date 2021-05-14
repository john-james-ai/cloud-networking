#include "Messenger.h"

//===========================================================================//
//                                MESSENGER                                  //
//===========================================================================//

//---------------------------------------------------------------------------//
//                               CONSTRUCTORS                                //
//---------------------------------------------------------------------------//
Messenger::Messenger() 
	: classname_("Messenger")
	{
		iam(classname_, __FUNCTION__);
		std::string* address_ = (std::string*)this;
	}
Messenger::Messenger(Parameters* parameters, Network* network, Socket* socket)
	: router_id_(parameters->router_id_), 	 
	  socket_(socket), 	  	  
	  network_(network),
	  classname_("Messenger")
	{
		global_UDP_socket_ = socket_->getSocket();		
		std::string* address_ = (std::string*)this;
	}
//---------------------------------------------------------------------------//
//                               SEND COST                                   //
//---------------------------------------------------------------------------//
void Messenger::send(Packet* packet) {
	iam(classname_, __FUNCTION__);
	
	std::string message;
	size_t length=0;

	struct sockaddr_in dest_addr;
	std::vector<short>::iterator it;
	char* buffer =  (char*)calloc(sizeof(char),MAX_BUFFER_LENGTH);
	//char* buffer =  new char[MAX_BUFFER_LENGTH];


	if (packet->header->recipients_.size() == 0) {
		message = "No recipients for message.";
		error(classname_, __FUNCTION__, message);
		exit(1);
	}	

	// Serialize 
	length = packet->serialize(buffer);		

	// Iterate over recipients and send.
	for (it=packet->header->recipients_.begin();it != packet->header->recipients_.end(); ++ it) {		
		network_->getSocketAddr(*it, dest_addr);
		sendto(global_UDP_socket_, buffer, MAX_BUFFER_LENGTH,0,
					(struct sockaddr*)&dest_addr, sizeof(dest_addr));					
	}
	
	leaving(classname_,__FUNCTION__);
};
//---------------------------------------------------------------------------//
//                                 ID AND NAME                               //
//---------------------------------------------------------------------------//
std::string Messenger::name() {
	return classname_;
}
bool Messenger::hey() {
	holla(classname_, address_);
	return true;
}
