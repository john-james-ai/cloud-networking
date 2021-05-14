/*
** client.c -- a stream socket client demo
*/

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>

struct stat buffer_infile, buffer_outfile;

char* read_file(std::string filepath, std::size_t filesize){

	// declare file object 
	//std::ifstream file(filepath.c_str(), std::ifstream::binary);
	std::ifstream file(filepath.c_str(), std::ifstream::binary);
	// get pointer to associated buffer object
	std::filebuf* buffer_pointer = file.rdbuf();

	// allocate memory to contain file data
	char* buffer = new char[filesize];

	// read file data
	buffer_pointer->sgetn (buffer, filesize);
	
	file.close();

	return buffer;	
}

int check_contents(std::string infilehost, std::string infile, 
				   struct stat& buffer_in, std::string outfile, 
				   struct stat& buffer_out){
	
	// read files into memory
	char* in_content = read_file(infile, buffer_in.st_size);
	char* out_content = read_file(outfile, buffer_out.st_size);

	// compare files
	int n = memcmp(in_content, out_content, sizeof(in_content));
	if (n > 0){
		std::cout << infile << " is greater than " << outfile << std::endl;
		return 1;
	} else if (n < 0) {
		std::cout << infile << " is less than " << outfile << std::endl;
		return 2;
	} else {
		std::cout << "Files for "<< infilehost << " are both " << buffer_in.st_size << "! \nCongratulations!!\n" << std::endl;
		return 0;
	}	

}
int check_filesize(std::string infilehost, std::string infile, std::string outfile){	

	// get file information
	int status;
	status = stat(infile.c_str(), &buffer_infile);
	status = stat(outfile.c_str(), &buffer_outfile);	

	// confirm same size
	if (buffer_infile.st_size != buffer_outfile.st_size) {
		std::cout << "File sizes not equal." << std::endl;
		std::cout << infilehost << " file size = " << buffer_infile.st_size << std::endl; 
		std::cout << outfile << " file size = " << buffer_outfile.st_size << std::endl;
		return 1; 		
	} else {
		std::cout << "File Sizes are Equal!" << std::endl;
	}
	return 0;
}

int run_client(std::string client, std::string file){
	std::stringstream stream;
	stream << client << " " << file;
	std::cout << client << std::endl;
	std::cout << file << std::endl;
	system(stream.str().c_str());
	return 0;
}

int test_file(std::string filename){
	std::string directory = "//home//john//Documents//MP1//";
	std::string host = "http:://localhost:8888/Documents/MP1/";
	std::string client = "http_client";	
	std::string infilename = filename;
	std::string outfilename = "output";	
	std::string clientpath = directory + client;
	std::string infilepath = directory + infilename;	
	std::string infilehost = host + infilename;	
	std::string outfilepath = directory + outfilename;
	run_client(clientpath, infilehost);
	
	if (int rc = (check_filesize(infilehost, infilepath, outfilepath)) != 0){
		return rc;
	}
	if (int rc = (check_contents(infilehost, infilepath, buffer_infile, 
								outfilepath, buffer_outfile)) != 0) {		
		return rc;
	}
}

int main(int argc, char *argv[])
{
	std::string filenames[] = {"some.txt", "another.pdf", "another.zip", "av3mb.avi",
								"some.pdf", "100MB.zip", "some.deb", "some.pptx", "some.jpeg"};	
	std::cout << sizeof(filenames) << std::endl;
	std::cout << sizeof(filenames[0]) << std::endl;

	
	for (int i = 0; i < sizeof(filenames)/sizeof(filenames[0]); ++i){
		test_file(filenames[i]);		
		}			
}

