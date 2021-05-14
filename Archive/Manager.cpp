#include "Manager.h"

Manager::Manager() {}
Manager::Manager(std::vector<Command> commands)
	: commands_(commands)
	{}

void Manager::connect() {
	short socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in source_addr_;
	memset(&source_addr_, 0, sizeof(source_addr_));
	source_addr_.sin_family = AF_INET;
	source_addr_.sin_port = htons(8999);
	inet_pton(AF_INET, "10.0.2.15", &source_addr_.sin_addr);
	if(bind(socket_, (struct sockaddr*)&source_addr_, sizeof(source_addr_)) < 0)
		perror("bind()");	

};
void Manager::disconnect() {
	close(socket_);
};
void Manager::send(short node_id, short dest_id, char* message) {	

	short no_dest_id = htons(dest_id);

	struct sockaddr_in node_addr;
	char tempaddr[100];
	sprintf(tempaddr, "10.1.1.%d", node_id);
	memset(&node_addr, 0, sizeof(node_addr));
	node_addr.sin_family = AF_INET;
	node_addr.sin_port = htons(7777);
	inet_pton(AF_INET, tempaddr, &node_addr.sin_addr);

	short msgLen = 4+sizeof(short)+strlen(message);
	char* sendBuf = (char*) malloc(msgLen);

	strcpy(sendBuf, "send");
	memcpy(sendBuf+4, &no_dest_id, sizeof(short int));
	memcpy(sendBuf+4+sizeof(short int), message, strlen(message));

	if(sendto(socket_, sendBuf, msgLen, 0, (struct sockaddr*)&node_addr, sizeof(node_addr)) < 0)
		perror("sendto()");
	free(sendBuf);	
};

void Manager::cost(short node_id, short node_id, int cost) {

	short no_node_id = htons(node_id);


	struct sockaddr_in node_addr;
	char tempaddr[100];
	sprintf(tempaddr, "10.1.1.%d", node_id);
	memset(&node_addr, 0, sizeof(node_addr));
	node_addr.sin_family = AF_INET;
	node_addr.sin_port = htons(7777);
	inet_pton(AF_INET, tempaddr, &node_addr.sin_addr);

	short no_newCost = htonl(cost);

	char sendBuf[4+sizeof(short int)+sizeof(int)];

	strcpy(sendBuf, "cost");
	memcpy(sendBuf+4, &no_node_id, sizeof(short int));
	memcpy(sendBuf+4+sizeof(short int), &no_newCost, sizeof(int));

	if(sendto(socket_, sendBuf, 4+sizeof(short int)+sizeof(int), 0,
				(struct sockaddr*)&node_addr, sizeof(node_addr)) < 0)
		perror("sendto()");	

};
void Manager::run() {

	std::vector<Command>::iterator it;

	connect();
	for (it=commands_.begin();it!=commands_.end();++it) {
		if (std::string((*it).command) == "send") {
			send((*it).node_id, (*it).dest_id, (*it).message);
		} else {
			cost((*it).node_id, (*it).node_id, (*it).cost);
		}
	}
	disconnect();	
}

