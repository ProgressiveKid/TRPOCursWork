//#include "FileWorker.h"
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
//#include "DataWorkerClass.cpp"
using namespace std;
class FileWorker {
public:
	static void checkFile(string filename) {
		ifstream iff(filename, ios::out);
		if (iff.good() == true)
		{
			//Предусмотри чтобы файл создавался в нужной кодировке
			std::ofstream file(filename);
			cout << "Новый файл создан \n";
		}
	}
	static void updateUsersFile(vector<User> users, const std::string& filename, string Operation)
	{ //редактирует адекватно
		std::ofstream outFile(filename, std::ios_base::out | std::ios_base::binary);
		if (!outFile.is_open())
		{
			std::cout << "Не удалось открыть файл для записи.\n";
			return;
		}
		int newIndex = 0;
		for (const auto& user : users)
		{
			newIndex++;
			outFile << newIndex << " " << user.login << " " << user.role << " " << user.salt << " " << user.saltedHashPassword << " " << user.subscriberName << " " << user.subscriberNumber << std::endl;
		}
		if (Operation == "del")
		{
			std::cout << "Запись удалена\n";
		}
		else if (Operation == "upd")
		{
			std::cout << "Запись изменена\n";
		}
		outFile.close();
	}
	static int generateMobileNumber()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> digitDist(0, 9);
		string mobileNumber;
		for (int i = 0; i < 8; ++i) {
			int digit = digitDist(gen);
			mobileNumber += digit;
		}
		return stoi(mobileNumber);
	}
	static void createUser(const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::app | std::ios_base::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file " << filename << std::endl;
			return;
		}
		bool isLoginCorrect = false;
		string login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		while (!isLoginCorrect) {
			cout << "Введите логин: ";
			cin >> login;
			if (records.size() > 0)
			{
				for (auto& item : records) {
					if (item.login == login) {
						cout << "Такой логин уже существует." << endl;
						isLoginCorrect = false;
						break;
					}
					else
						isLoginCorrect = true;
				}
			}
			else
				break;
		}
		string password;
		cout << "Введите пароль:\n";
		cin >> password;
		string salt = CryptoClass::generate_salt(16);
		string passwordA = password + salt;
		auto hashPassword = CryptoClass::hashPasswordWithSaltMethod(password, salt);
		int role;
		do
		{
			cout << "Укажите роль(0 - пользователь, 1 - администратор) \n";
			cin >> role;
			if (role != 1 && role != 0)
				cout << "выберите 0 или 1\n";
			else
				break;
		} while (true);
		string FIO = getFullFIO();
		int phoneNumber = 0;
		std::srand(std::time(nullptr)); // вызов относительно времени
		int subscriber_numer = generateMobileNumber();
		//file._setmode(_fileno(stdout), _O_WTEXT);
		file << records.size() + 1 << " " << login << " " << role << " "
			<< salt << " "<< hashPassword << " " << FIO << " "
			<< to_string(subscriber_numer) << std::endl;
		file.close();
		if (role == 1)
			cout << "Администратор добавлен\n";
		else
			cout << "Пользователь добавлен\n";
	}
	static string changeParametr(string parametr)
	{
		string newData;
		std::cout << "Введите новое значение для " + parametr;
		std::cin >> newData;
		return newData;
	}
	static string getFullFIO() // ВВод полного ФИО
	{
		std::string firstName, lastName, middleName;
		std::cout << "Введите Фамилию: ";
		std::cin >> lastName;
		std::cout << "Введите Имя: ";
		std::cin >> firstName;
		std::cout << "Введите Отчество: ";
		std::cin >> middleName;
		std::string fullName;
		if (middleName.empty()) {
			fullName = lastName + "_" + firstName;
		}
		else {
			fullName = lastName + "_" + firstName + "_" + middleName;
		}
		if (fullName[0] == '\n')
			fullName.erase(0, 1);
		if (fullName[fullName.size()] == '\n')
			fullName.erase(fullName.size() - 1, 1);
		std::cout << "Полное имя: " << fullName << std::endl;
		return fullName;
	}
	static void ChangeFIO(string filename, string oldFIO, string newFIO)
	{
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		for (auto& item : records) {
			if (item.subscriberName == oldFIO)
			{
				item.subscriberName = newFIO;
			}
		}
		updateDataFile(records, filename, " ");
	}
	static void updateDataFile(vector<CallRecord> data, const std::string& filename, string Operation)
	{ //редактирует адекватно
		std::ofstream outFile(filename, std::ios_base::out | std::ios_base::binary);
		if (!outFile.is_open())
		{
			std::cout << "Не удалось открыть файл для записи.\n";
			return;
		}
		int newIndex = 0;
		for (const auto& record : data)
		{
			newIndex++;
			outFile << " " << newIndex << " " << record.subscriberNumber << " " << record.subscriberName << " " <<
				record.isOutgoingCall << " " << record.callNumber << " " <<
				record.callDate << " " << record.callTime << " " <<
				record.callDuration << " " << record.tariffPerMinute << std::endl;
		}
		outFile.close();
	}
	static void updateUser(string filename) // Редактирования файла с пользователями
	{
		string login;
		std::cout << "Введите логин пользователя для поиска:\n";
		std::cin >> login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		bool isFounded = false;
		for (auto& item : records) {
			if (item.login == login)
			{
				isFounded = true;
				int choice = 0;
				do {
					std::cout << "Что вы хотите редактировать?:\n";
					std::cout << "1. Логин\n";
					std::cout << "2. Пароль\n";
					std::cout << "3. Номер телефона\n";
					std::cout << "4. ФИО\n";
					std::cout << "5. Назад\n";
					std::cin >> choice;
					std::string firstName, lastName, middleName;
					std::stringstream fullNameStream;
					string OldFIO;
					string newParam;
					bool IsCorrect = true;
					switch (choice) {
					case 1:
						do {
							newParam = changeParametr("логина:\n");
							for (auto& item : records) {
								if (item.login == newParam)
								{
									std::cout << "Такой логин уже существует, придумайте другой:\n";
									IsCorrect = false; break;
								}
								else
									IsCorrect = true;
							}
							//item.login = newParam;
						} while (IsCorrect == false);
						item.login = newParam;
						choice = 5;
						break;
					case 2:
						newParam = changeParametr("пароля:\n");
						item.saltedHashPassword = CryptoClass::hashPasswordWithSaltMethod(newParam, item.salt);
						choice = 5;
						break;
					case 3:
						do {
							newParam = changeParametr("номера телефона (8 символов):\n");
							for (char c : newParam) {
								if (!isdigit(c)) {
									IsCorrect = false;
									std::cout << "В номере телефона должны быть только цифры:\n";
									break;
								}
								else
								{
									IsCorrect = true;
								}
							}
							//проверка корректности длины телефона
							if (newParam.size() != 8)
							{
								IsCorrect = false;
								std::cout << "В номере телефона должно быть 8 цифр:\n";
							}
							else
							{
								for (auto& item : records) {
									if (item.subscriberNumber == stoi(newParam))
									{
										IsCorrect = false;
										std::cout << "Такой номер телефона уже существует:\n";
										break;
									}
								}
							}
						} while (IsCorrect == false);
						item.subscriberNumber = stoi(newParam); // перевели в секунды
						choice = 5;
						break;
					case 4:
						OldFIO = item.subscriberName;
						item.subscriberName = getFullFIO();
						// Также меняем фио в файле с записями звонков
						ChangeFIO("infoUsers.txt", OldFIO, item.subscriberName);
						choice = 5;
						break;
					case 5:
						break;
					default:
						std::cout << "Неправильный выбор.\n";
						break;
					}
				} while (choice != 5);
				updateUsersFile(records, filename, "upd");
				break;
				//запись в найш файл нового массива структур
			}
		}
		if (!isFounded)
		{
			std::cout << "Нет такого логина в система:\n";
		}
	}
	static void deleteUser(string filename, string username)
	{
		string login;
		std::cout << "Введите логин пользователя для удаления:\n";
		std::cin >> login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		bool isFounded = false;
		for (int i = 0; i < records.size(); i++) {
			if (records[i].login == login) {
				
				isFounded = true;
				//std::remove_if(records.begin(), records.end(), records[i]), records.end();
				records.erase(records.begin() + i);
				break;
			}
		}
		if (!isFounded)
		{
			std::cout << "Нет такого логина в система:\n";
		}
		else
		{
			if (login == username)
			{
				std::cout << "Невозможно удалить собственную учётную запись\n";
			}
			else
			{
				std::string answer;
				while (true) {
					std::cout << "Вы действительно хотите удалить данного пользователя? (да/нет): ";
					std::cin >> answer;
					if (answer == "да" || answer == "Да" || answer == "ДА") {
						// Выполнение действий удаления файла или записи
						updateUsersFile(records, filename, "del");
						break;
					}
					else if (answer == "нет" || answer == "Нет" || answer == "НЕТ") {
						std::cout << "Удаление файла (записи) отменено.\n";
						break;
					}
					else {
						std::cout << "Некорректный ввод. Пожалуйста, введите 'да' или 'нет'.\n";
						// Очистка буфера ввода
						std::cin.clear();
					}
				}
			}
				
		}
	}
	static void viewAllUsers(string filename) // Просмотр всех пользователей
	{
		setlocale(LC_CTYPE, "rus");
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		std::cout << std::setw(4) << std::left << "| №"
			<< std::setw(15) << std::left << "| Логин"
			<< std::setw(30) << std::left << "| ФИО"
			<< std::setw(20) << std::left << "| Номер телефона" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
		//SetConsoleOutputCP(CP_UTF8);
		for (auto& item : records) {
			std::cout << std::setw(4) << std::left << "| " + std::to_string(item.key)
				<< setw(15) << std::left << "| " + item.login
				<< std::setw(30) << std::left << "| " + item.subscriberName
				<< std::setw(20) << std::left << "| " + to_string(item.subscriberNumber) + "" << std::endl;
			std::cout << "---------------------------------------------------------------" << std::endl;
		}
	}
	static void findForLoginViewUser(string filename) // Вывод информации по пользователю через его Логин
	{
		string login;
		std::cout << "Введите логин пользователя для поиска:\n";
		std::cin >> login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		bool isFounded = false;
		for (auto& item : records) {
			if (item.login == login)
			{
				std::cout << std::setw(4) << std::left << "| №"
					<< std::setw(15) << std::left << "| Логин"
					<< std::setw(30) << std::left << "| ФИО"
					<< std::setw(20) << std::left << "| Номер телефона" << std::endl;
				std::cout << "---------------------------------------------------------------" << std::endl;
				//SetConsoleOutputCP(CP_UTF8);
					std::cout << std::setw(4) << std::left << "| " + std::to_string(item.key)
						<< setw(15) << std::left << "| " + item.login
						<< std::setw(30) << std::left << "| " + item.subscriberName
						<< std::setw(20) << std::left << "| " + to_string(item.subscriberNumber) + "" << std::endl;
					std::cout << "---------------------------------------------------------------" << std::endl;
				isFounded = true;
			}
		}
		if (!isFounded)
			std::cout << "В системе нет пользователя с таким логином\n";
	}
};