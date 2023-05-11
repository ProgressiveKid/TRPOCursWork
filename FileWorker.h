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
#include "Header.h"
//#pragma once

class FileWorker {
public:
	static void checkFile(string filename);
	static std::vector<User> getUserArray(std::string filename);
	static void updateUsersFile(vector<User> users, const std::string& filename, string Operation);
	static void findForLoginViewUser(string filename);
	static void updateUser(string filename);
	static void deleteUser(string filename);
	static void viewAllUsers(string filename);

};
