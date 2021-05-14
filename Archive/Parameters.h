#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

//===========================================================================//
//                                 PARAMETERS                                //
//===========================================================================//
struct Parameters { 
    short router_id_;     
    short n_network_;
    short heart_rate_;
    short ttl_factor_;
    short ad_factor_;    
    std::string cost_filename_;
    std::string log_filename_;
    std::string rib_log_filename_;
    std::string packet_log_filename_;    
};

#endif