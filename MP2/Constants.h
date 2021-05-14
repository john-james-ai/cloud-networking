#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>

const size_t MAX_BUFFER_LENGTH = 512;
const size_t MAX_MESSAGE_LENGTH = 128;

const int MANAGER = 999;

//Dataplane Packet Types
const std::string COST_PACKET_TYPE = "cost";
const std::string SEND_PACKET_TYPE = "send";
const std::string FORWARD_PACKET_TYPE = "frwd";

// Controlplane packet types
const std::string ROUTE_PACKET = "rout";
const std::string NEIGHBOR_PACKET = "nbor";
const std::string WITHDRAW_PACKET = "wdrw";
const std::string FAILURE_PACKET = "fail";

// Route Status
const std::string ACTIVE = "a";
const std::string SERVICABLE = "s";
const std::string FAILED = "f";
const std::string WITHDRAWN = "w";


// DEFAULTS 
const std::string DEFAULT_PACKET_TYPE = "dflt";
const short DEFAULT_NODE = 0;
const int DEFAULT_COST = 1;
const int ZERO_COST = 0;

// INITIAL VALUES
const short INIT_NODE = 0;
const int INIT_COST = 0;

// NOT FOUND
const int HEARTBEAT_NOT_FOUND = -1;
const short ROUTER_NOT_FOUND = -1;
const int COST_NOT_FOUND = -1;
const short NODE_NOT_FOUND = -1;
const short NEIGHBOR_NOT_FOUND = -1;
const short DESTINATION_NOT_FOUND = -1;
const std::string IP_ADDR_NOT_FOUND = "99.9.9.9";
const std::string ROUTE_NOT_FOUND = "999-999";

// Constants to change prior to submission.
const double BIG_NUM = 999999;
const int N_NODES = 256;
const bool CONTROL_PLANE_LOG = false;
const bool BRIEF = true;
const bool VERBOSE = true;
const bool TRACE = false;
const bool DEBUGGING = true;

const std::string CLASSES[0] = {};
#endif