#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <ctime>
#include <iostream>
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
#include "PacketFactory.h"

class Packet;
struct  Route;
struct Link;

std::string intToString(short n);
std::string formatRouteId(short node_id, short dest_id);

void print_vec(std::vector<short> v);
void print_set(std::set<short> v);
std::string string_vec(std::vector<short> v);
void print_array(char* a, int len);
void print_cstr(std::string name, char* value);
void print_str(std::string name, std::string& value);
void print_short(std::string name, short& value);
void print_int(std::string name, int& value);
void print_buf(std::string name, char* value);

void warn(std::string classname, std::string methodname, std::string message);
void error(std::string classname, std::string methodname, std::string message);
void iam(std::string classname, std::string funcname);
void leaving(std::string classname, std::string funcname);
bool invec(short n, std::vector<short> v);
short getNewNum(short min, short max, std::vector<short> v);
short getRandnum(short min, short max);
std::vector<short> getRandom(short n_network, short min, short max);
short getOneNum(std::vector<short> v);
short aNotInB(std::vector<short> a, std::vector<short> b, std::string func);
short aInB(std::vector<short> a, std::vector<short> b, std::string func);
bool inArray(short n, const int* a);
short chooseOne(std::vector<short> a);
std::string chooseOneS(std::vector<std::string> a);
std::vector<short> setToVec(std::set<short> s);
std::vector<short> getRandomVec(short n, short min, short max, std::vector<short> v);
const char* stoc(std::string s);
std::string vtos(std::vector<short> v);
short btoi(char* buffer, short len);
std::vector<short> makePath(short len, short factor);
bool arrays_equal(char* a, char* b);
bool vectors_equal(std::vector<short> a, std::vector<short> b);

double difftime_ms(timeval end, timeval start);
std::string dtString();

void holla(std::string classname, std::string addr);
void heard(char* buffer, int nbytes, std::string ipaddr);
void showPacket(std::string classname, std::string funcname, std::string message, Packet* packet);
void showRoute(std::string classname, std::string funcname, std::string message, Route route);
void note(std::string classname, std::string funcname, std::string message);

char* createBuffer(short dest_id, std::string message);
void printBuffer(char* buffer);
void printSendBuffer(char* buffer);
void printRouteBuffer(char* buffer);
void printLink(Link link);
void printLinks(std::vector<Link> links);
#endif