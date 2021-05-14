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

void* ad(void* param) {
	((Router*)param)->startAdvertiser();
};


int main(int argc, char** argv) {

	char temp[100];
	
	if(argc < 4)
	{
		fprintf(stderr, "Usage: %s mynodeid initialcostsfile logfile\n\n", argv[0]);
		exit(1);
	}
	short router_id = atoi(argv[1]);
	std::string cost_filename(argv[2]);
	std::string log_filename(argv[3]);
	sprintf(temp, "RIB_logfile_node_%hi.txt", router_id);	
	std::string rib_log_filename(temp);	
	sprintf(temp, "Packet_logfile_node_%hi.txt", router_id);	
	std::string packet_log_filename(temp);	
	int lifetime = 0;
	if (argc == 5)  {
		int lifetime = atoi(argv[4]);
	}

	// Pack up parameters for the trip.
	struct Parameters* parameters = new Parameters;
	parameters->router_id_ = router_id;
	parameters->n_network_ = 256;
	parameters->heart_rate_ = 300;
	parameters->ttl_factor_ = 3;
	parameters->ad_factor_ = 5;
	parameters->cost_filename_ = cost_filename;
	parameters->log_filename_ = log_filename;
	parameters->rib_log_filename_ = rib_log_filename;
	parameters->packet_log_filename_ = packet_log_filename;

	// Build and instantiate the Network;	
	Builder	builder = Builder(parameters);
	Director director = Director(builder);
	Router* router = director.buildRouter();            
	router->hey();	

	pthread_t pulseThread;
	pthread_create(&pulseThread, NULL, pulse, (void*)router);

	pthread_t ekgThread;
	pthread_create(&ekgThread, NULL, ekg, (void*)router);	

	pthread_t adThread;
	pthread_create(&adThread, NULL, ad, (void*)router);
	
	router->startDataPlane();
}