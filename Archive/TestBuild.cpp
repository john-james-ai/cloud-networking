#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <time.h>
#include <pthread.h>

#include "Builder.h"
#include "Constants.h"
#include "Parameters.h"
#include "Pulse.h"
#include "EKG.h"

struct timeval launch;    

void* pulse(void* param){
	((Router*)param)->startPulse();
};

void* ekg(void* param) {
	((Router*)param)->startEKG();
};

Router* testBuild(int router_id, std::string cost_filename, std::string log_filename) {

	char temp[100];
	
	sprintf(temp, "rib_plane_logfile_node_%hi.txt", router_id);	
	std::string rib_plane_log_filename(temp);	

	// Pack up parameters for the trip.
	struct Parameters* parameters = new Parameters;
	parameters->router_id_ = router_id;
	parameters->n_network_ = 256;
	parameters->ttl_ms_ = 1000.0;	
	parameters->heart_rate_ = 400.0;
	parameters->cost_filename_ = cost_filename;
	parameters->log_filename_ = log_filename;
	parameters->rib_plane_log_filename_ = rib_plane_log_filename;

	// Build and instantiate the Network;	
	Builder	builder = Builder(parameters);
	Director director = Director(builder);
	Router* router = director.buildRouter();            
	router->hey();	
	
	pthread_t pulseThread;
	//pthread_create(&pulseThread, NULL, pulse, (void*)router);

	pthread_t ekgThread;
	//pthread_create(&ekgThread, NULL, ekg, (void*)router);
	
	//router->startDataPlane();

    return router;
	
}