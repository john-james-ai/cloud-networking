#include "Socket.h"


//===========================================================================//
//                                 SOCKET                                    //
//===========================================================================//
//---------------------------------------------------------------------------//
//                                CONSTRUCTORS                               //
//---------------------------------------------------------------------------//
Socket::Socket() 
	: classname_("Socket")
{
	iam(classname_, __FUNCTION__);
	std::string* address_ = (std::string*)this;
}
Socket::Socket(short router_id) 
    : router_id_(router_id), classname_("Socket")
    {
		iam(classname_, __FUNCTION__);
		std::string* address_ = (std::string*)this;
		openSocket();
	}
//---------------------------------------------------------------------------//
//                             BIND GLOBAL SOCKET                            //
//---------------------------------------------------------------------------//
void Socket::openSocket() {
	iam(classname_, __FUNCTION__);

	if((global_UDP_socket_=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}
	
	sprintf(my_ip_addr_, "10.1.1.%d", router_id_);		
	memset(&socket_addr_, 0, sizeof(socket_addr_));
	socket_addr_.sin_family = AF_INET;
	socket_addr_.sin_port = htons(7777);
	inet_pton(AF_INET, my_ip_addr_, &socket_addr_.sin_addr);
	if(bind(global_UDP_socket_, (struct sockaddr*)&socket_addr_, sizeof(struct sockaddr_in)) < 0)
	{
		perror("bind");
		close(global_UDP_socket_);
		exit(1);
	}    
}
//---------------------------------------------------------------------------//
//                              CLOSE SOCKET                                 //
//---------------------------------------------------------------------------//
void Socket::closeSocket() {
	close(global_UDP_socket_);
};
//---------------------------------------------------------------------------//
//                              GET SOCKET                                   //
//---------------------------------------------------------------------------//
int Socket::getSocket() {
	return global_UDP_socket_;
}
//---------------------------------------------------------------------------//
//                                PRINT                                      //
//---------------------------------------------------------------------------//
void Socket::print() {

    std::cout << "\n=======================================" << std::endl;
    std::cout << "                   Socket " << my_ip_addr_ << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "          Socket Family: " << socket_addr_.sin_family << std::endl;
    std::cout << "            Socket Port: " << ntohl(socket_addr_.sin_port) << std::endl;
    std::cout << "         Socket Address: " << ntohl(socket_addr_.sin_addr.s_addr) << std::endl;
    std::cout << "---------------------------------------" << std::endl;	
}
//---------------------------------------------------------------------------//
//                                  HEY                                      //
//---------------------------------------------------------------------------//
bool Socket::hey() {
	holla(classname_, address_);
	return true;
}
//---------------------------------------------------------------------------//
//                                  NAME                                     //
//---------------------------------------------------------------------------//
std::string Socket::name() {
	return classname_;
}