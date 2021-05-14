#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

//===========================================================================//
//                                 PARAMETERS                                //
//===========================================================================//
struct Parameters { 
    short router_id_;     
    int n_network_;
    int heart_rate_;
    int ttl_factor_;
    int ad_factor_;    
    std::string cost_filename_;
    std::string log_filename_;
};

#endif