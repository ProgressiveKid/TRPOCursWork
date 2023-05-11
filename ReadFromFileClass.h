#pragma once
#include <vector>
#include "Structures.h"
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <sstream>
#include "Structures.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <string>
using namespace std;
class ReadFromFileClass
{
public:
	static std::vector<User> getUserArray(std::string filename);
    static std::vector<CallRecord> getDataArray(std::string filename);

};