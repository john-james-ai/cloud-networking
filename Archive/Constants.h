#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <vector>

const size_t MAX_BUFFER_LENGTH = 1024;
const size_t MAX_MESSAGE_LENGTH = 128;

const short MANAGER = 999;

const std::string COST_PACKET_TYPE = "cost";
const std::string SEND_PACKET_TYPE = "send";
const std::string FORWARD_PACKET_TYPE = "fwdp";


const std::string ROUTE_PACKET = "rout";
const std::string WITHDRAW_ROUTE_PACKET = "wrte";
const std::string FAILURE_PACKET = "fail";
const std::string ADVERTISE_PACKET = "ad4t";

// DEFAULTS 
const std::string DEFAULT_PACKET_TYPE = "dflt";
const short DEFAULT_NODE = 0;
const int DEFAULT_COST = 1;
const int ZERO_COST = 0;
const short DEFAULT_VERSION = 0;

// INITIAL VALUES
const short INIT_NODE = 0;
const short INIT_COST = 0;
const short INIT_VERSION = 0;

// NOT FOUND
const short HEARTBEAT_NOT_FOUND = -1;
const short COST_NOT_FOUND = -1;
const short NODE_NOT_FOUND = -1;
const short NEIGHBOR_NOT_FOUND = -1;
const std::string IP_ADDR_NOT_FOUND = "99.9.9.9";
const std::string ROUTE_NOT_FOUND = "999-999";
const short DESTINATION_NOT_FOUND = -1;

const double BIG_NUM = 999999;
const short N_NODES = 256;
const bool CONTROL_PLANE_LOG = true;
const bool BRIEF = true;
const bool VERBOSE = true;
const bool TRACE = true;
const bool DEBUGGING = false;

const std::string CLASSES[3] = {"EKG","RouteHandler", "RouteInfoBase"};
#endif