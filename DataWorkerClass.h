#pragma once
#include <string>
#include <fstream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Structures.h"
#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip> // для std::setw()
#include <windows.h>
#include <algorithm>
#include <io.h>
#include <ctime>
#include <stdio.h>
#include <ctime>
#include <limits>
#include <regex>
#include <chrono>
#include <cwctype> // Для использования std::iswdigit()
#include <__msvc_chrono.hpp>
#include "ReadFromFileClass.h"
#include "GlobalVariablesClass.h"
//#pragma once
using namespace std;
class DataWorkerClass {
public:
	static void checkFile(string filename);
	static int ConvertToInt(const std::string& str);
	static bool IsDouble(const std::string& str);
	static double getCallTariff(const std::string dateTimeString);
	static void createData(const std::string& filename);
	static void viewData(const std::string& filename);
	static string changeParametr(string parametr);
	static void updateDataFile(vector<CallRecord> data, const std::string& filename, string Operation);
	static bool isValidDate(int day, int month, int year);
	static int inputComponent(const std::string& componentName, int minValue, int maxValue);
	static string inputDate();
	static string inputTime();
	static void updateCallRecord(string filename);
	static void deleteCallRecord(string filename);
	//static void updateDataFile(vector<CallRecord> data, const std::string& filename, string Operation);


};
