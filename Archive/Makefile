all: vec ls lab test packet

vec: main.c monitor_network.h
	gcc -pthread -o vec_router main.c monitor_network.h

ls: main.c monitor_network.h
	gcc -pthread -o ls_router main.c monitor_network.h

lab: lab.cpp  utils.h utils.cpp PacketFactory.h Packet.cpp
	g++ -o lab Packet.cpp lab.cpp utils.cpp

manager: manager_send.c
	g++ -o manager manager_send.c

ser: Constants.h TestSerializer.cpp Neighborhood.h Neighborhood.cpp \
Routes.h Routes.cpp PacketFactory.h Packet.cpp utils.h utils.cpp 
	g++ -o ser TestSerializer.cpp Constants.h utils.cpp Links.cpp \
	Neighborhood.cpp Routes.cpp Packet.cpp 

neighborhood: TestNeighborhood.cpp Neighborhood.h Neighborhood.cpp utils.h utils.cpp
	g++ -o neighborhood TestNeighborhood.cpp utils.cpp Neighborhood.cpp 

utils: TestUtils.cpp 
	g++ -o utils TestUtils.cpp 

packets: TestPackets.cpp Constants.h IO.h IO.cpp Neighborhood.h Neighborhood.cpp Routes.h Routes.cpp \
Packet.h Packet.cpp PacketFactory.h PacketFactory.cpp FTable.h FTable.cpp utils.h utils.cpp
	g++ -o packets Constants.h Neighborhood.cpp TestPackets.cpp utils.cpp Routes.cpp IO.cpp  \
	FTable.cpp PacketFactory.cpp Packet.cpp 

test: test.cpp IO.h IO.cpp utils.h utils.cpp Constants.h Serializer.h \
Serializer.cpp PacketFactory.h Packet.cpp Network.h Network.cpp Routes.h \
Routes.cpp  RouteInfoBase.h \
RouteInfoBase.cpp Network.h Network.cpp Socket.h Socket.cpp  EKG.h EKG.cpp \
Listener.h Listener.cpp Messenger.h Messenger.cpp \
DataPlane.h DataPlane.cpp Events.h Events.cpp Router.h \
Router.cpp Builder.h Builder.cpp   
	g++ -o test test_data_plane.cpp Constants.h utils.cpp Packet.cpp IO.cpp Serializer.cpp \
	Network.cpp Routes.cpp RouteInfoBase.cpp  \
	Network.cpp  Socket.cpp Messenger.cpp Listener.cpp \
	Events.cpp EKG.cpp DataPlane.cpp Router.cpp Builder.cpp    

controllers: Test  EventHandler.cpp Parameters.h PacketFactory.h Packet.cpp IO.h IO.cpp utils.h utils.cpp Constants.h  \
Serializer.h Serializer.cpp  \
Socket.h Socket.cpp Network.h Network.cpp Listener.h Listener.cpp Messenger.h Messenger.cpp \
Pulse.h Pulse.cpp EKG.h EKG.cpp DataPlane.h DataPlane.cpp \
Routes.h Routes.cpp  \
RouteInfoBase.h RouteInfoBase.cpp Bundles.h   EventHandlers.h   EventHandler.cpp \
Marketers.h Marketers.cpp Forwarders.h Forwarders.cpp Router.h Router.cpp Builder.h Builder.cpp \
main.cpp
	g++ -pthread -o controllers Test  EventHandler.cpp Parameters.h PacketFactory.h Packet.cpp IO.cpp utils.cpp Constants.h \
Serializer.h Serializer.cpp Socket.cpp Network.cpp Listener.cpp Messenger.cpp Pulse.cpp EKG.cpp DataPlane.cpp \
Routes.cpp RouteInfoBase.cpp Bundles.h   EventHandler.cpp \
Marketers.cpp Forwarders.cpp Router.cpp Builder.cpp 



pvr: main.cpp Constants.h Parameters.h PacketFactory.h Packet.cpp IO.h IO.cpp utils.h  \
utils.cpp Socket.h Socket.cpp Network.h Network.cpp \
Listener.h Listener.cpp Messenger.h Messenger.cpp \
Neighborhood.h Neighborhood.cpp Routes.h Routes.cpp FTable.h FTable.cpp \
RouteInfoBase.h RouteInfoBase.cpp \
EventHandlers.h   EventHandlers.cpp  Forwarders.h Forwarders.cpp \
Pulse.h Pulse.cpp EKG.h EKG.cpp Advertiser.h Advertiser.cpp \
DataPlane.h DataPlane.cpp ControlPlane.h ControlPlane.cpp \
Router.h Router.cpp Builder.h Builder.cpp
	g++ -pthread -g -o pvr main.cpp Constants.h Parameters.h  Packet.cpp IO.cpp \
utils.cpp Socket.cpp Network.cpp Listener.cpp Messenger.cpp Neighborhood.cpp \
Routes.cpp FTable.cpp RouteInfoBase.cpp EventHandlers.cpp Forwarders.cpp \
Pulse.cpp EKG.cpp Advertiser.cpp DataPlane.cpp ControlPlane.cpp \
Router.cpp Builder.cpp


.PHONY: clean pvr recv
clean:
	rm *.o vec_router ls_router pvr recv
