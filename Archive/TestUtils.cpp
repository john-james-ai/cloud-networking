#include "assert.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

struct Link;
//===========================================================================//
//                                TEST LINKS                                 // 
//===========================================================================//
class TestUtils {
    public:
        TestUtils() {}

        char* createBuffer(short dest_id, std::string message) {
                short msgLen = 4+sizeof(short int)+strlen(message.c_str());
                char* sendBuf = new char[msgLen];

                strcpy(sendBuf, "send");
                memcpy(sendBuf+4, &dest_id, sizeof(short int));
                memcpy(sendBuf+4+sizeof(short int), message.c_str(), strlen(message.c_str()));   
                return sendBuf;
        }

        void printBuffer(char* buffer) {

            size_t offset = 0;
            char temp_buffer[128];
            short temp_short;
            std::string packet_type, message;
            short dest_id;    

            // packet type
            memcpy(temp_buffer,buffer,4);
            packet_type = std::string(temp_buffer);
            offset += 4;    

            // Dest id
            memcpy(&dest_id, buffer + offset, sizeof(short));    
            offset += sizeof(short); 

            // message
            memcpy(temp_buffer,buffer + offset,100);
            message = std::string(temp_buffer);
            std::cout << packet_type << " " << dest_id << " " << message << std::endl;    
        }        

        void testBuffer() {
            std::cout <<  "TestUtils: Buffer" << std::endl; 
            short dest_id = 5;
            char* buffer;
            std::string message = "What you DON'T do, more than what you do, is a predictor of success";
            buffer = createBuffer(dest_id,message);
            printBuffer(buffer);
            std::cout <<  "TestUtils: Buffer Complete!" << std::endl; 
        };
        void main() {
            std::cout << "\nTestUtils" << std::endl;  
            testBuffer();
            std::cout << "\nTestUtils Complete" << std::endl;  
        };
};

int main() {
    TestUtils().main();
}
 