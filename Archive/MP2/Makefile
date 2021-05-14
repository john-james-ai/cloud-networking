all: vec ls lab test packet

vec: main.c monitor_network.h
	gcc -pthread -o vec_router main.c monitor_network.h

ls: main.c monitor_network.h
	gcc -pthread -o ls_router main.c monitor_network.h

lab: lab.cpp  utils.h utils.cpp PacketFactory.h Packet.cpp
	g++ -o lab Packet.cpp lab.cpp utils.cpp

manager: manager_send.c
	g++ -o manager manager_send.c

pvr: main.cpp Constants.h Parameters.h Packet.h Packet.cpp IO.h IO.cpp utils.h  \
utils.cpp Socket.h Socket.cpp Network.h Network.cpp \
Listener.h Listener.cpp Messenger.h Messenger.cpp \
Neighborhood.h Neighborhood.cpp Routes.h Routes.cpp FTable.h FTable.cpp \
ControlPlane.h ControlPlane.cpp \
EventHandlers.h   EventHandlers.cpp \
Pulse.h Pulse.cpp NOC.h NOC.cpp \
DataPlane.h DataPlane.cpp  \
Router.h Router.cpp Builder.h Builder.cpp
	g++ -pthread -g -o pvr main.cpp Constants.h Parameters.h  Packet.cpp IO.cpp \
utils.cpp Socket.cpp Network.cpp Listener.cpp Messenger.cpp Neighborhood.cpp \
Routes.cpp FTable.cpp ControlPlane.cpp EventHandlers.cpp \
Pulse.cpp NOC.cpp DataPlane.cpp  \
Router.cpp Builder.cpp


.PHONY: clean pvr recv
clean:
	rm *.o vec_router ls_router pvr recv
