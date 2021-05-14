#include "utils.h"

std::string classname_ = "utils.cpp";

//---------------------------------------------------------------------------//
//                               INT TO STRING                               //
//---------------------------------------------------------------------------//
std::string intToString(short n) {
    std::ostringstream oss;
    oss << n;
    return oss.str();
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
//                                PRINT SHORT                                //
//---------------------------------------------------------------------------//
void print_short(std::string name, short& value) {    
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
bool invec(short n, std::vector<short> v) {
    
    for (short i=0; i<v.size(); ++i) {
        if (n == v[i]) {
            return true;
        }
    }
    return false;
}


short getNewNum(short min, short max, std::vector<short> v) {
    
    short num = min + (rand() % static_cast<short>(max-min+1));
    while(invec(num, v)) {
        num = min + (rand() % static_cast<short>(max-min+1));
    }
    return num;
}

short getRandnum(short min, short max) {
    return min + (rand() % static_cast<short>(max-min+1));
}
std::vector<short> getRandom(short n_network, short min, short max) {
    short num = 0;
    short idx = 0;
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

short getOneNum(std::vector<short> v) {
    return v[getRandnum(0,v.size())];
}


short aNotInB(std::vector<short> a, std::vector<short> b, std::string func) {

    short idx;
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

bool inArray(short n,const int* a) {
    short size = sizeof(a)/sizeof(a[0]);
    for (short i=0; i<size; ++i) {
        if (n == a[i]) return true;
    }
    return false;
}

short aInB(std::vector<short> a, std::vector<short> b, std::string func) {

    short idx;
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

short chooseOne(std::vector<short> a) {
    short idx = getRandnum(0, a.size());
    return a[idx];
}

std::string chooseOneS(std::vector<std::string> a) {
    iam(classname_, __FUNCTION__);
    short idx = getRandnum(0, a.size());
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

short btoi(char* buffer, short len) {
    char temp[1000];
    memcpy(temp, &buffer, len);
    return atoi(temp);
}

std::vector<short> getRandomVec(short n, short min, short max, std::vector<short> v) {
    iam(classname_, __FUNCTION__);
    short num = 0;
    short idx = 0;
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


std::vector<short> makePath(short len, short factor) {
    std::vector<short> p;
    for (short i=0; i<len; ++i) {
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
    short a_size = sizeof(a) / sizeof(a[0]);
    short b_size = sizeof(a) / sizeof(b[0]);
    if (a_size != b_size) return false;
    for (short i=0;i<a_size;++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool vectors_equal(std::vector<short> a, std::vector<short> b) {
    if (a.size() != b.size()) return false;
    for (short i=0;i<a.size();++i) {
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


char* createBuffer(short dest_id, std::string message) {
		short msgLen = 4+sizeof(short int)+strlen(message.c_str());
        char* sendBuf = new char[msgLen];

		strcpy(sendBuf, "send");
		memcpy(sendBuf+4, &dest_id, sizeof(short int));
		memcpy(sendBuf+4+sizeof(short int), message.c_str(), strlen(message.c_str()));   
        return sendBuf;
}
void printSendBuffer(char* buffer) {

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
    memcpy(temp_buffer,buffer + offset,MAX_MESSAGE_LENGTH);
    message = std::string(temp_buffer);
    std::cout << packet_type << " " << dest_id << " " << message << std::endl;        

}
void printRouteBuffer(char* buffer) {

    size_t offset = 0;
    char temp_buffer[128];
    short temp_short;
    std::string packet_type, message;
    short router_id, neighbor_id, dest_id;    

    // packet type
    memcpy(temp_buffer,buffer,4);
    packet_type = std::string(temp_buffer);
    offset += 4;       

    //  Router id
    memcpy(&temp_short, buffer + offset, sizeof(short));
    router_id = ntohs(temp_short);
    offset += sizeof(short);

    // Neighbor id
    memcpy(&temp_short, buffer + offset, sizeof(short));
    neighbor_id = ntohs(temp_short);
    offset += sizeof(short);    

    // Dest id
    memcpy(&temp_short, buffer + offset, sizeof(short));
    dest_id = ntohs(temp_short);
    offset += sizeof(short);    

    std::cout << "Serialized route from " << router_id << " to " << dest_id << " through neighbor " << neighbor_id << std::endl; 

}
void printBuffer(char* buffer) {

    if(!strncmp(buffer, "send", 4) ||!strncmp(buffer, "fwdp",4)) {
        printSendBuffer(buffer);
    } else {
        printRouteBuffer(buffer);
    }
}

void printLink(Link link) {
    std::cout << "Link: Router id: " << link.router_id_ << \
                 " Neighbor id: " << link.neighbor_id_ << \
                 " Version: " << link.version_ << std::endl;
}
void printLinks(std::vector<Link> links) {
    std::vector<Link>::iterator it;
    for (it=links.begin();it!=links.end();++it) {
        printLink(*it);
    }
}