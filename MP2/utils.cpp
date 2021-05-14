#include "utils.h"

std::string classname_ = "utils.cpp";

//---------------------------------------------------------------------------//
//                               INT TO STRING                               //
//---------------------------------------------------------------------------//
std::string intToString(int n) {
    std::ostringstream oss;
    oss << n;
    return oss.str();
}
//---------------------------------------------------------------------------//
//                             SHORT TO STRING                               //
//---------------------------------------------------------------------------//
std::string shortToString(short n) {
    std::ostringstream oss;
    oss << n;
    return oss.str();
}
//---------------------------------------------------------------------------//
//                             SIZE_T TO STRING                              //
//---------------------------------------------------------------------------//
std::string sizeToString(size_t n) {
    std::ostringstream oss;
    oss << n;
    return oss.str();
}
//---------------------------------------------------------------------------//
//                              VEC TO STRING                                //
//---------------------------------------------------------------------------//
std::string vecToString(std::vector<short> v) {
    std::ostringstream oss;
    if (!v.empty()) {
        // Convert all but last element to string to avoid trailing delimiter
        std::copy(v.begin(),v.end()-1, std::ostream_iterator<short>(oss, ",-"));
        // Now add the last element
        oss << v.back();
    }

    std::string result = oss.str();
    
    return result;
}

//---------------------------------------------------------------------------//
//                            PRINT ARRAY OF CHARS                           //
//---------------------------------------------------------------------------//
void print_array(char* a, int len) {
    std::cout << "______________________________________________" << std::endl;
    
    for (int i=0;i<len;++i) {
        std::cout << a[i] << "-";        
    }
    std::cout << std::endl;    
    std::cout << "______________________________________________\n" << std::endl;
}
//---------------------------------------------------------------------------//
//                            PRINT VEC OF SHORTS                            //
//---------------------------------------------------------------------------//
void print_vec(std::vector<short> v) {
    std::vector<short>::iterator it;
    for (it=v.begin(); it != v.end(); ++it) {
        std::cout << intToString(*it) << "-";
    }
    std::cout << std::endl;    
}
//---------------------------------------------------------------------------//
//                            PRINT SET OF SHORTS                            //
//---------------------------------------------------------------------------//
void print_set(std::set<short> v) {
    std::set<short>::iterator it;
    for (it=v.begin(); it != v.end(); ++it) {
        std::cout << intToString(*it) << "-";
    }
    std::cout << std::endl;    
}
//---------------------------------------------------------------------------//
//                       CONVERT VECTOR TO STRING                            //
//---------------------------------------------------------------------------//
std::string string_vec(std::vector<short> v) {
    std::vector<short>::iterator it;
    std::string s;
    for (it=v.begin(); it != v.end(); ++it) {
        s += static_cast<char>(*it);
        s += "-";
    }
    return s;
}
//---------------------------------------------------------------------------//
//                             PRINT C STRING                                //
//---------------------------------------------------------------------------//
void print_cstr(std::string name, char* value) {    
    std::cout << "\n____________________________________________________________" << std::endl;
    std::cout << name << " at: " << (void *) &value << " is: " << *value << std::endl;
    std::cout << "____________________________________________________________" << std::endl;
}

//---------------------------------------------------------------------------//
//                                PRINT STR                                  //
//---------------------------------------------------------------------------//
void print_str(std::string name, std::string& value) {    
    std::cout << "\n____________________________________________________________" << std::endl;
    std::cout << name << " at: " << (void *) &value << " is: " << value << std::endl;
    std::cout << "____________________________________________________________" << std::endl;
}

//---------------------------------------------------------------------------//
//                                PRINT INT                                  //
//---------------------------------------------------------------------------//
void print_int(std::string name, int& value) {    
    std::cout << "\n____________________________________________________________" << std::endl;
    std::cout << name << " at: " << (void *) &value << " is: " << value << std::endl;
    std::cout << "____________________________________________________________" << std::endl;
}


//---------------------------------------------------------------------------//
//                                PRINT INT                                  //
//---------------------------------------------------------------------------//
void print_buf(std::string name, char* value) {    
    std::cout << "\n____________________________________________________________" << std::endl;
    std::cout << name << " at: " << (void *) &value << " is: " << value << std::endl;
    std::cout << "____________________________________________________________" << std::endl;
}


//---------------------------------------------------------------------------//
//                              FORMAT ROUTE ID                              //
//---------------------------------------------------------------------------//
std::string formatRouteId(short node_id, short dest_id) {
    std::string n = intToString(node_id);
    std::string d = intToString(dest_id);
    return n + "-" + d;
}


//---------------------------------------------------------------------------//
//                              ERROR HANDLING                               //
//---------------------------------------------------------------------------//
void print_detail(std::string classname, std::string methodname, std::string message) {
    std::cout << "              Class : " << classname << std::endl;
    std::cout << "             Method : " << methodname << std::endl;
    std::cout << message << std::endl;
    std::cout << "---------------------------------------" << std::endl;    

}

void warn(std::string classname, std::string methodname, std::string message) {
    if (VERBOSE) {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "                 WARNING" << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        print_detail(classname, methodname, message);    
    }
}

void error(std::string classname, std::string methodname, std::string message) {
    if (VERBOSE) {
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "                 ERROR" << std::endl;
        std::cout << "---------------------------------------" << std::endl;    
        print_detail(classname, methodname, message);    
    }
}

void iam(std::string classname, std::string funcname) {
    if (TRACE) {
        std::cout << "I am Class: " << classname << "    Method: " << funcname << std::endl;
    } else if (DEBUGGING) {
        int n_classes = sizeof(CLASSES) / sizeof(CLASSES[0]);
        for (int i=0;i<n_classes;++i) {
            if (CLASSES[i]== classname) {
                std::cout << "I am Class: " << classname << "    Method: " << funcname << std::endl;
            }
        }
    }
}
void leaving(std::string classname, std::string funcname) {
    if (TRACE) {
        std::cout << "Leaving Class: " << classname << "        Method: " << funcname << std::endl;
    }
}
void note(std::string classname, std::string funcname, std::string message) {
    if (DEBUGGING) {
        std::cout << "Class: " << classname << " Method: " << funcname << " " << message << std::endl;
    }
}
bool invec(int n, std::vector<short> v) {
    
    for (int i=0; i<v.size(); ++i) {
        if (n == v[i]) {
            return true;
        }
    }
    return false;
}


int getNewNum(int min, int max, std::vector<short> v) {
    
    int num = min + (rand() % static_cast<short>(max-min+1));
    while(invec(num, v)) {
        num = min + (rand() % static_cast<short>(max-min+1));
    }
    return num;
}

int getRandnum(int min, int max) {
    return min + (rand() % static_cast<short>(max-min+1));
}
std::vector<short> getRandom(int n_network, int min, int max) {
    int num = 0;
    int idx = 0;
    std::vector<short> output;
    while(idx < n_network) {
        num = min + (rand() % static_cast<short>(max-min+1));
        if (!invec(num, output)) {
            output.push_back(num);
            idx += 1;
        }
    }
    return output;
}

int getOneNum(std::vector<short> v) {
    return v[getRandnum(0,v.size())];
}


int aNotInB(std::vector<short> a, std::vector<short> b, std::string func) {

    int idx;
    std::vector<short> results;
    std::vector<short>::iterator ai;
    std::vector<short>::iterator bi;

    for (ai=a.begin(); ai!=a.end(); ++ai) {
        bi = std::find(b.begin(), b.end(), *ai);
        if (bi==b.end()) results.push_back(*ai);
    }

    if (results.size() == 0) {
        std::string classname = "testRouteInfoBase";
        std::string methodname = func;
        std::string message = "There are no a's not in b.";
        error(classname, methodname, message);
        exit(1);    
    }
    idx = getRandnum(0, results.size());
    return results[idx];
}

bool inArray(int n,const int* a) {
    int size = sizeof(a)/sizeof(a[0]);
    for (int i=0; i<size; ++i) {
        if (n == a[i]) return true;
    }
    return false;
}

int aInB(std::vector<short> a, std::vector<short> b, std::string func) {

    int idx;
    std::vector<short> results;
    std::vector<short>::iterator ai;
    std::vector<short>::iterator bi;

    for (ai=a.begin(); ai!=a.end(); ++ai) {
        bi = std::find(b.begin(), b.end(), *ai);
        if (bi!=b.end()) results.push_back(*ai);
    }

    if (results.size() == 0) {
        std::string classname = "testRouteInfoBase";
        std::string methodname = func;
        std::string message = "There are no a's not in b.";
        error(classname, methodname, message);
        exit(1);    
    }
    idx = getRandnum(0, results.size());
    return results[idx];
}

int chooseOne(std::vector<short> a) {
    int idx = getRandnum(0, a.size());
    return a[idx];
}

std::string chooseOneS(std::vector<std::string> a) {
    iam(classname_, __FUNCTION__);
    int idx = getRandnum(0, a.size());
    return a[idx];
}

std::vector<short> setToVec(std::set<short> s) {
    iam(classname_, __FUNCTION__);
    std::vector<short> v;
    v.assign(s.begin(), s.end());
    return v;
}

std::string vtos(std::vector<short> v) {
    std::string s;
    std::vector<short>::iterator it;   
    it = v.begin();
    s += intToString(*it);    
    while(it != v.end()) {
        it++;
        s += "-";
        s+= intToString(*it);        
    }
    return s;
}

int btoi(char* buffer, int len) {
    char temp[1000];
    memcpy(temp, &buffer, len);
    return atoi(temp);
}

std::vector<short> getRandomVec(int n, int min, int max, std::vector<short> v) {
    iam(classname_, __FUNCTION__);
    int num = 0;
    int idx = 0;
    std::vector<short> output;
    while(idx < n) {
        num = min + (rand() % static_cast<short>(max-min+1));
        if (!invec(num, output)) {
            output.push_back(num);
            idx += 1;
        }
    }
    return output;
}


std::vector<short> makePath(int len, int factor) {
    std::vector<short> p;
    for (int i=0; i<len; ++i) {
        p.push_back(i*factor);
    }
    return p;
}

double difftime_ms(timeval end, timeval start) {
    double elapsed_us;
    double elapsed_ms; 
    elapsed_us = (end.tv_sec-start.tv_sec) * 1e6;
    elapsed_ms = (elapsed_us+(end.tv_usec-start.tv_usec)) /1e3;
    return elapsed_ms;
}

std::string dtString() {
    std::string dts;
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    dts += now->tm_wday;
    dts + " ";
    dts += now->tm_mon;
    dts += "/";
    dts += now->tm_mday;
    dts += now->tm_year + 1900;
    dts += " ";
    dts += now->tm_hour;
    dts += ":";
    dts += now->tm_min;
    return dts;
}

bool arrays_equal(char* a, char* b) {
    int a_size = sizeof(a) / sizeof(a[0]);
    int b_size = sizeof(a) / sizeof(b[0]);
    if (a_size != b_size) return false;
    for (int i=0;i<a_size;++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool vectors_equal(std::vector<short> a, std::vector<short> b) {
    if (a.size() != b.size()) return false;
    for (int i=0;i<a.size();++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

void holla(std::string classname, std::string addr) {
    if (TRACE) {
        //printf("Hey! I'm %s at %s", classname.c_str(), addr.c_str());
        std::cout << "Hey! I'm " << classname << " at " << addr << std::endl;    
    }
}

void heard(char* buffer, int nbytes, std::string ipaddr) {
    char send_addr[100];
    if (TRACE) {
        std::cout << "\n\nSender: " << ipaddr << " Message Size: " << nbytes << " bytes. Message: "\
        << buffer << "\n" << std::endl;
    }
}

void showPacket(std::string classname, std::string funcname, std::string message, Packet* packet) {
    if (VERBOSE) {
        std::cout << "Class: " << classname << " Method: " << funcname << std::endl;
        std::cout << message << std::endl;
        Route* route = packet->route_;
        route->print();
    }
}
void showRoute(std::string classname, std::string funcname, std::string message, Route* route) {
    if (VERBOSE) {
        std::cout << message << std::endl;
        route->print();
    }
}

void printPacket(std::string classname, std::string funcname, std::string message, Packet* packet) {
    if (VERBOSE) {
        std::cout << "__________________________________________________" << std::endl;
        std::cout << "\nClass: " << classname << " Method: " << funcname << std::endl;
        std::cout << "__________________________________________________" << std::endl;
        std::cout << message << std::endl;        
        std::cout << "__________________________________________________" << std::endl;
        packet->print();
    }
}

char* createBuffer(short dest_id, std::string message) {
		int msgLen = 4+sizeof(int)+strlen(message.c_str());
        char* sendBuf = new char[msgLen];

		strcpy(sendBuf, "send");
		memcpy(sendBuf+4, &dest_id, sizeof(int));
		memcpy(sendBuf+4+sizeof(int), message.c_str(), strlen(message.c_str()));   
        return sendBuf;
}
void printSendBuffer(char* buffer) {

    size_t offset = 0;
    char temp_buffer[128];
    int temp_int;
    std::string packet_type, message;
    short dest_id;    

    // packet type
    memcpy(temp_buffer,buffer,4);
    packet_type = std::string(temp_buffer);
    offset += 4;    

    // Dest id
    memcpy(&dest_id, buffer + offset, sizeof(int));    
    offset += sizeof(int); 

    // message
    memcpy(temp_buffer,buffer + offset,MAX_MESSAGE_LENGTH);
    message = std::string(temp_buffer);
    std::cout << packet_type << " " << dest_id << " " << message << std::endl;        

}
void printRouteBuffer(char* buffer) {

    size_t offset = 0;
    char temp_buffer[128];
    int temp_int;
    std::string packet_type, message;
    short router_id, neighbor_id, dest_id;    

    // packet type
    memcpy(temp_buffer,buffer,4);
    packet_type = std::string(temp_buffer);
    offset += 4;       

    //  Router id
    memcpy(&temp_int, buffer + offset, sizeof(int));
    router_id = ntohl(temp_int);
    offset += sizeof(int);

    // Neighbor id
    memcpy(&temp_int, buffer + offset, sizeof(int));
    neighbor_id = ntohl(temp_int);
    offset += sizeof(int);    

    // Dest id
    memcpy(&temp_int, buffer + offset, sizeof(int));
    dest_id = ntohl(temp_int);
    offset += sizeof(int);    

    std::cout << "Serialized route from " << router_id << " to " << dest_id << " through neighbor " << neighbor_id << std::endl; 

}
void printBuffer(char* buffer, size_t blen) {
    for (size_t i = 0; i<blen; i++) {
        printf("%#x ", buffer[i]);
    }
}

void prints(std::string classname, std::string methodname, std::string varname, short v, short* p) {
    if (TRACE) {
        std::string var = shortToString(v);
        const void* address = static_cast<const void*>(p);
        std::stringstream ss;
        ss << address;
        std::string addr = ss.str();
        std::cout << "Class: "<< classname << " Method: " << varname << ": " << var << " at " << addr << std::endl;
    }
}

void printc(std::string classname, std::string methodname, std::string varname, std::string v, char* p) {
    if (TRACE) {
        std::string var = v;
        const void* address = static_cast<const void*>(p);
        std::stringstream ss;
        ss << address;
        std::string addr = ss.str();
        std::cout << "Class: "<< classname << " Method: " << varname << ": " << var << " at " << addr << std::endl;
    }
}
void printi(std::string classname, std::string methodname, std::string varname, int v, int* p) {
    if (TRACE) {
        std::string var = intToString(v);
        const void* address = static_cast<const void*>(p);
        std::stringstream ss;
        ss << address;
        std::string addr = ss.str();
        std::cout << "Class: "<< classname << " Method: " << varname << ": " << var << " at " << addr << std::endl;
    }
}
void printt(std::string classname, std::string methodname, std::string varname, size_t v, size_t* p) {
    if (TRACE) {
        std::string var = sizeToString(v);
        const void* address = static_cast<const void*>(p);
        std::stringstream ss;
        ss << address;
        std::string addr = ss.str();
        std::cout << "Class: "<< classname << " Method: " << varname << ": " << var << " at " << addr << std::endl;
    }
}
void printa(std::string classname, std::string methodname, std::string varname, size_t s, int* p) {
    if (TRACE) {
        std::string var = sizeToString(s);
        const void* address = static_cast<const void*>(p);
        std::stringstream ss;
        ss << address;
        std::string addr = ss.str();
        std::cout << "Class: "<< classname << " Method: " << varname << ": " << var << " at " << addr << std::endl;
    }
}
