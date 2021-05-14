/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <vector>


#define PORT "3490" // the port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once 
#define BUFFERSIZE 1024 // max number of bytes we can get at once 
#define CRLF "\r\n\r\n" // defines the end of the header 
#define LINE_BREAK "\r\n" // defines the end of line
#define SPACE " " // defines the space character
#define OUTFILENAME "output"
#define VERBOSE false

#define MAXRETRIES 5

// Defines the struct of a URL argument
struct Url {
		std::string protocol, node, service, port, resource, filename;		
};

struct Body {
	int start, end, length;
	char payload[MAXDATASIZE];
	//std::vector<unsigned char> payload;	
};


void jprint(std::string message){
	if (VERBOSE) std::cout << message << std::endl;	
};	

void kviprint(std::string k, int v){
	if (VERBOSE) std::cout << k << " : " << v << std::endl;	
};	

void kvsprint(std::string k, std::string v){
	if (VERBOSE) std::cout << k << " : " << v << std::endl;	
};

class Writer {
	public:
		void write();
		void close();
		int all_good(std::ofstream& file, std::string operation){
			if (!file.good()) {
				if (file.bad()) jprint(operation + " is bad");
				if (file.fail()) jprint(operation + " failed");
				if (file.eof()) jprint("End of file reached during " + operation);
				return 1;
			}
			return 0;
		}
};

class StringWriter: public Writer {
	public:
		int num_writes;
		int num_bytes;	
		bool started;	
		std::string operation;
		StringWriter() :
			started() {}		

		void report_counts(std::string content){
			if (VERBOSE) {
				if (!started) {
					num_writes = 0;
					num_bytes = 0;
					started = true;
				}			
				num_writes += 1;
				num_bytes += content.size();
				std::cout << "\r" << "Total Writes : " << num_writes
						<< "   Total Bytes : " << num_bytes 
						<< std::flush;
			}					  
		}

		int write(std::ofstream& file, std::string filename, 
				   std::string content){			
			
			if (!file.is_open()) {
				file.open(filename.c_str());
				operation = "Opening " + filename;
				if (int rc = all_good(file, operation) != 0) return rc;
			}

			file << content;
			
			operation = "Writing to " + filename;			
			if (int rc = all_good(file, operation) != 0) return rc;
			
			report_counts(content);
			return 0;
		}
		void close(std::ofstream& file, std::string filename) {
			file.close();
		}		
};

class BinaryWriter: public Writer {
	public:
		int num_writes;
		int num_bytes;		
		bool started;
		std::string operation;	
		BinaryWriter() :
			started() {}		

		void report_counts(char* content){
			if (VERBOSE) {
				if (!started) {
					num_writes = 0;
					num_bytes = 0;
					started = true;
				}			
				num_writes += 1;
				num_bytes += sizeof(content);
				std::cout << "\r" << "Total Writes : " << num_writes
						<< "   Total Bytes : " << num_bytes 
						<< std::flush;
			}					  
		}
		int write(std::ofstream& file, std::string filename, 
				   char* content, int nbytes) {
			
			if (!file.is_open()) {				
				file.open(filename.c_str(), std::ios::binary);
				operation = "Opening " + filename;
				if (int rc = all_good(file, operation) != 0) return rc;
			}
			
			file.write(content, nbytes);
			
			operation = "Writing to " + filename;			
			if (int rc = all_good(file, operation) != 0) return rc;

			report_counts(content);
			return 0;
		}
		void close(std::ofstream& file, std::string filename) {
			file.close();
		}

};

class HTTPClient {
	private:		
		bool header_processed;						
		bool body_started;
		char buffer[MAXDATASIZE];		
		int sockfd, nbytes, rv, rc, retries;	
		std::ofstream outfile;		
		std::string argument;	
		std::string content;		
		std::string filename;	
		std::string get_request; 			
		std::string resource; 
		struct addrinfo hints, *servinfo, *p;	
		Url url;
		BinaryWriter binary_writer;
		StringWriter string_writer;

		// get sockaddr, IPv4 or IPv6:
		void *get_in_addr(struct sockaddr *sa)
		{
			if (sa->sa_family == AF_INET) {
				return &(((struct sockaddr_in*)sa)->sin_addr);
			}

			return &(((struct sockaddr_in6*)sa)->sin6_addr);
		}

		// parses url in argument into its constituent parts
		Url parse_argument() {
			jprint("Inside parse_argument");
			// Compute starts and lengths of components
			int protocol_start = 0;
			int protocol_len = argument.find_first_of(':');
			int node_start = argument.find("//") + 2;
			int node_len = argument.find_first_of("/:", node_start) - node_start;	
			int port_start = 0;
			int port_len = 0;
			int resource_start = node_start + node_len + 1;
			int resource_len = argument.size() - resource_start + 1;		

			if (argument.find_first_of(':') != argument.find_last_of(':')){
				port_start = node_start + node_len + 1;
				port_len = argument.find('/',port_start) - port_start;
				resource_start = port_start + port_len + 1;
				resource_len = argument.size() - resource_start + 1;
			}
			
			int filename_start = argument.find_last_of("/") + 1;
			int filename_len = argument.size() - filename_start + 1;

			// Parse components into struct
			Url u;
			u.protocol = argument.substr(0,protocol_len);	
			u.node = argument.substr(node_start, node_len);
			u.port = "";
			u.service = u.protocol;	

			if (port_len != 0) {
				u.port = argument.substr(port_start, port_len);
				u.service = u.port;
			};	

			u.resource = argument.substr(resource_start, resource_len);
			u.filename = argument.substr(filename_start, filename_len);	

			return u;	
		} 
		std::string parse_header(std::string header, std::string key, 
								   std::string delimiter){		
			jprint("Inside parse_header");
			int key_pos;
			std::string value;
			
			if (key_pos = header.find(key) != std::string::npos) {				
				int value_start = key_pos + key.length() - 1;
				int value_end = header.find(delimiter, value_start);
				int value_len = value_end - value_start;
				value = header.substr(value_start, value_len);
			}	
			return value;
		}

		int get_return_code(std::string header, std::string &return_code){			
			jprint("Inside get return code");
			return_code = parse_header(header, "HTTP/1.0 ", SPACE);

			// Get return code
			if (return_code.empty()) {
				return_code = parse_header(header, "HTTP/1.1 ", SPACE);
			}
			if (return_code.empty()) {
				return_code = parse_header(header, "HTTPS/1.0 ", SPACE);
			}
			if (return_code.empty()) {
				return_code = parse_header(header, "HTTPS/1.1 ", SPACE);
			}
			if (return_code.empty()) {
				content = "NORESPONSECODE";
				string_writer.write(outfile, OUTFILENAME, content);
				jprint(content);
				outfile.close();
				return 1;
			}	
			return 0;
		}

		void process_error(std::string message){
			jprint("Inside process_error");
			string_writer.write(outfile, OUTFILENAME, message);
			jprint(message);
			outfile.close();
		}

		int validate_return_code(std::string header) {
			jprint("Inside validate_return_code");
			std::string return_code;

			if ((rc = get_return_code(header, return_code)) != 0) return rc;			

			if (return_code == "200") return 0;

			if (return_code == "301") {
				process_error("PERMANENTLYMOVED");
				return 1;
			}
			if (return_code == "404") {
				process_error("FILENOTFOUND");
				return 2;
			}
			content = "\nclient response: " + return_code;
			process_error(content);
			return 3;		
		}

		int process_header(char * buffer, int nbytes) {
			jprint("\nInside process header");			
			std::string header;			
			header.append(buffer, nbytes);

			if (!header_processed) {
				jprint("...header processing...");
				header_processed = true;
				return validate_return_code(header);
			}

			if (!body_started) {

				if ((rc = header.find(CRLF)) == header.npos) {
					jprint("...header processed, no body yet...");
					return 0;
				} else {
					
					kviprint("/n...body found at ",rc);
					// Identify start and length, and create buffer.
					int body_start = rc + 4;
					int body_length = nbytes - body_start;
					char buf[body_length];
					kviprint("...start", body_start);
					kviprint("...length", body_length);
					
					// Copy body content to new buffer and write.
					strncpy(buf, buffer+body_start, sizeof(buf));
					if (int rc = binary_writer.write(outfile, OUTFILENAME, 
									buf, sizeof(buf)) != 0) { return rc; }					
					body_started = true;
					return 0;
				}
			}
		}
		
	public:
		HTTPClient() :
			header_processed(), body_started() {}	

		// Member functions
		int initialize(int argc, char *argv[]) {
			jprint("Inside initialize");

			remove(OUTFILENAME);
			rv = 0;
			rc = 0;
			retries = 0;
			nbytes = 0;
			header_processed = false;
			body_started = false;

			if (argc != 2) {
				content = "usage: client hostname\n"; 
				string_writer.write(outfile, OUTFILENAME, content);
				outfile.close();
				return 1;
			}
			argument = argv[1];
			std::string protocol = argument.substr(0,4);

			if ((protocol.find("http") == std::string::npos) &&
				(protocol.find("HTTP") == std::string::npos)) {
				content = "INVALIDPROTOCOL";
				process_error(content);
				outfile.close();
				return 2;
			}
			retries = 0;
			return 0;
		}

		int connect_to_host(){
			jprint("Inside connect_to_host");
			url = parse_argument();
			kvsprint("\nFilename", url.filename);
			kvsprint("url", url.node);
			kvsprint("Port", url.port);
			kvsprint("Protocol", url.protocol);
			kvsprint("Resource", url.resource);
			kvsprint("Service", url.service);

			memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;		

			if ((rv = getaddrinfo(url.node.c_str(), url.service.c_str(), &hints, &servinfo)) != 0) {		
				content = "getaddrinfo: " + std::string(gai_strerror(rv)) + "\n";
				string_writer.write(outfile, OUTFILENAME, content);
				outfile.close();		
				return 3;
			}
			
			// loop through all the results and connect to the first we can
			for(p = servinfo; p != NULL; p = p->ai_next) {
				if ((sockfd = socket(p->ai_family, p->ai_socktype,
						p->ai_protocol)) == -1) {			
					//content = "client: socket " + std::string(strerror(errno)) + "\n";
					//string_writer.write(outfile, OUTFILENAME, content);					
					continue;
				}

				if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
					close(sockfd);
					content = "client: socket " + std::string(strerror(errno)) + "\n";
					string_writer.write(outfile, OUTFILENAME, content);
					continue;
				}

				break;
			}

			if (p == NULL) {
				content = "NOCONNECTION";
				string_writer.write(outfile, OUTFILENAME, content);
				outfile.close();		
				return 4;			
			}			
			return 0;	
		}

		int request(){
			jprint("Inside request");			
			get_request = "GET /" + url.resource + " HTTP/1.1\r\n\r\n";	
			int len;
			len = strlen(get_request.c_str());
			kviprint("message length", len);
			kvsprint("message", get_request);


			// send get request 
			if ((rv = send(sockfd, get_request.c_str(), len,0)) == -1){		
				content = "Send failed: " + std::string(strerror(errno)) + "\n";
				string_writer.write(outfile, OUTFILENAME, content);
				outfile.close();				
				return rv;
			}		

			freeaddrinfo(servinfo); // all done with this structure		
			return 0;
		}

		int receive(){
			jprint("Inside receive");									
			int n = 0;

			// receive data from host in chunks
			while ((nbytes = recv(sockfd, buffer, MAXDATASIZE-1, 0)) > 0) {				

				if (nbytes < 0) {
					content = "Receive failed: " + std::string(strerror(errno)) + "\n";	
					process_error(content);
					continue;
				}
				if (nbytes == 0) {
					if (retries < MAXRETRIES) {
						content = "Connection is closed. Restarting connection.";
						process_error(content);
						connect_to_host();	
					} else {
						content = "Connection is closed. Maximum retries met. Giving up:-(";	
						process_error(content);
						return 1;
					}		
				}


				if (!body_started) {					
					if ((rc = process_header(buffer, nbytes)) != 0) { 
						return rc; 
					} else {
						continue;
					}
				} else {
					if ((rc = binary_writer.write(outfile, OUTFILENAME, 
								buffer, nbytes)) != 0) { return rc; }					
				}
			}

			close(sockfd);
			binary_writer.close(outfile, OUTFILENAME);
			jprint("\nCompleted successfully.");			

			return 0;
		}

};

int main(int argc, char *argv[])
{	
	HTTPClient client;	
	int rv;
	if ((rv = client.initialize(argc, argv)) != 0) {		
		return rv;
	}
	if ((rv = client.connect_to_host()) != 0) {		
		return rv;
	}
	if ((rv = client.request()) != 0) {		
		return rv;
	}
	client.receive();
}