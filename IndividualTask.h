#pragma once

#include <vector>
#include "Structures.h"
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include "ReadFromFileClass.h"
#include "GlobalVariablesClass.h"

using namespace std;

class IndividualTask
{ 
public:
	static void Surt(string filename);
	static void IndividualTasksInterface();
};

