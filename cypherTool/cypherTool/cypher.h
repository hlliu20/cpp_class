#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include <iomanip>
#include<unordered_map>
using namespace std;

//char * caesarEncode(char* s, int n=-1);
void strShiftOne(std::string & s);
std::string caesar(std::string strIn);
std::string morseEncode(std::string strIn);
std::string morseDecode(std::string strIn);
std::string fence(std::string strIn);