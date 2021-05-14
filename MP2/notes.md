Design decision
# Route Disambiguation
Route is used in a number of control plane events. To avoid confusing
the route as presented by a neighbor with a route as would be presented
in the RIB, all methods will assume the later definition. This means
that a route, ready to be written to the RIB must be created from
packets before any evaluation or comparisons are made to existing routes. Hence the following changes must occur:

1. A packet will be converted to a route at checkIn by the ControlPlane
2. The checkIn method is the packet's end of life.
3. Creation and addition of routes must be kept separate.
4. Routes will be created during checkIn, then added or deleted

In essence, checkin extracts all information required from the packet, rendering the packet no longer useful.
        
The packet sender must be provided in all packets. This is the variable that will be used to represent neighbor id        


# Universality of Route
The route is the object that a packet conveys. If the packet type is withdraw, the route can be populated with __NOT FOUND for all fields except the dest_id, which is required. 

#TODO 
Assess whether periodic advertisements are necessary

# Deserialization
Serialization follows the packet type. Dataplane packets (send, forward) are deserialized differently then control plane packets. Fix deserialization in packet to reflect this. Also look at serialization.

# Routes and  Packets
Routes received from neighbors and used to create config packets will remain intact as received from the neighbor. Once the packet
arrives at the control plane, a new route will be created based upon
the route received.

# Packet Factory
Moved all creating logic for all packets to the factories. Pass factories to Control Plane, NOC, and Pulse during Build. Added methods to Factory to create route, withdraw, and failure packets with route and recipient signatures. Packets created by the listener call methods with the buffer as parameter. Here the route is created and filled in by deserialization. Packets that are created by and for control plane use public methods with route and recipient signatures. This expliots polymorphism in C in which multiple methods can have the same name as long as their signatures are different.

# Packet Deletion
Packets are deleted by the methods that create them!