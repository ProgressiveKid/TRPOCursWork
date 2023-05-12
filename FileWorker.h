#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <sstream>
#include "CryptoClass.cpp"
#include "Structures.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iomanip> // для std::setw()
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include "ReadFromFileClass.h"
#include "GlobalVariablesClass.h"
//#pragma once
using namespace std;
class FileWorker {
public:
	static void checkFile(string filename);
	static void updateUsersFile(vector<User> users, const std::string& filename, string Operation);
	static int generateMobileNumber();
	static void createUser(const std::string& filename);
	static string changeParametr(string parametr);
	static string getFullFIO();
	static void ChangeFIO(string filename, string oldFIO, string newFIO);
	static void ChangeNumber(string filename, string FIO, int newPhone, int oldPhone);
	static void updateDataFile(vector<CallRecord> data, const std::string& filename, string Operation);
	static void updateUser(string filename);
	static void deleteUser(string filename, string username);
	static void viewAllUsers(string filename);
	static void findForLoginViewUser(string filename);

};
