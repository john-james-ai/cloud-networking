#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <set>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#include "Constants.h"
#include "Packet.h"

class Packet;
struct  Route;

std::string intToString(int n);
std::string shortToString(short n);
std::string sizeToString(size_t n);
std::string vecToString(std::vector<short> v);
std::string formatRouteId(short node_id, short dest_id);

void print_vec(std::vector<short> v);
void print_set(std::set<short> v);
std::string string_vec(std::vector<short> v);
void print_array(char* a, int len);
void print_cstr(std::string name, char* value);
void print_str(std::string name, std::string& value);
void print_int(std::string name, int& value);
void print_buf(std::string name, char* value);

void warn(std::string classname, std::string methodname, std::string message);
void error(std::string classname, std::string methodname, std::string message);
void iam(std::string classname, std::string funcname);
void leaving(std::string classname, std::string funcname);
bool invec(int n, std::vector<short> v);
int getNewNum(int min, int max, std::vector<short> v);
int getRandnum(int min, int max);
std::vector<short> getRandom(int n_network, int min, int max);
int getOneNum(std::vector<short> v);
int aNotInB(std::vector<short> a, std::vector<short> b, std::string func);
int aInB(std::vector<short> a, std::vector<short> b, std::string func);
bool inArray(int n, const int* a);
int chooseOne(std::vector<short> a);
std::string chooseOneS(std::vector<std::string> a);
std::vector<short> setToVec(std::set<short> s);
std::vector<short> getRandomVec(int n, int min, int max, std::vector<short> v);
const char* stoc(std::string s);
std::string vtos(std::vector<short> v);
int btoi(char* buffer, int len);
std::vector<short> makePath(int len, int factor);
bool arrays_equal(char* a, char* b);
bool vectors_equal(std::vector<short> a, std::vector<short> b);

double difftime_ms(timeval end, timeval start);
std::string dtString();

void holla(std::string classname, std::string addr);
void heard(char* buffer, int nbytes, std::string ipaddr);
void showPacket(std::string classname, std::string funcname, std::string message, Packet* packet);
void showRoute(std::string classname, std::string funcname, std::string message, Route route);
void printPacket(std::string classname, std::string funcname, std::string message, Packet* packet);
void note(std::string classname, std::string funcname, std::string message);

char* createBuffer(short dest_id, std::string message);
void printBuffer(char* buffer, size_t blen);
void printSendBuffer(char* buffer);
void printRouteBuffer(char* buffer);
void prints(std::string classname, std::string methodname, std::string varname, short v, short* p);
void printc(std::string classname, std::string methodname, std::string varname, std::string v, char* p);
void printi(std::string classname, std::string methodname, std::string varname, int v, int* p);
void printt(std::string classname, std::string methodname, std::string varname, size_t v, size_t* p);
void printa(std::string classname, std::string methodname, std::string varname, size_t s, int* p);
#endif