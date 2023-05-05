#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <sstream>
#include "CryptoClass.h"
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
#pragma once
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
	static void CheckUser(string username)
	{
	}
	static std::vector<User> getUserArray(std::string filename)
	{// Не трогаем - он правильно берет данные из текстового файла
		std::ifstream file(filename);
		const int MAX_RECORDS = 20; // максимальное количество записей
		User recordsArray[MAX_RECORDS]; // массив структур
		if (file.is_open()) {
			std::string line;
			int index = 0;
			while (std::getline(file, line) && index < MAX_RECORDS) {
				User record;
				std::istringstream iss(line);
				iss >> record.login >> record.role >>
					record.salt >> record.saltedHashPassword >>
					record.subscriberName >> record.subscriberNumber;
				recordsArray[index] = record;
				++index;
			}
			file.close();
			std::vector<User> records(recordsArray, recordsArray + index);
			return records;
		}
		else {
			std::cout << "Не удалось открыть файл для чтения.\n";
		}
	}
	static void UpdateUsersFile(vector<User> users, const std::string& filename, string Operation)
	{ //редактирует адекватно
		//setlocale(LC_CTYPE, "rus");
		//std::locale::global(std::locale("C"));
		std::ofstream outFile(filename, std::ios_base::out | std::ios_base::binary);
		if (!outFile.is_open())
		{
			std::cout << "Не удалось открыть файл для записи.\n";
			return;
		}
		for (const auto& user : users)
		{
			outFile << user.login << " " << user.role << " " << user.salt << " " << user.saltedHashPassword << " " << user.subscriberName << " " << user.subscriberNumber << std::endl;
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
	static void CreateUser(const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::app | std::ios_base::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file " << filename << std::endl;
			return;
		}
		bool isLoginCorrect = false;
		string login;
		vector<User> records = FileWorker::getUserArray(filename);
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
		string FIO = GetFullFIO();
		int phoneNumber = 0;
		std::srand(std::time(nullptr)); // вызов относительно времени
		int subscriber_numer = rand() % 1000000000;
		//file._setmode(_fileno(stdout), _O_WTEXT);
		file << login << " "
			<< role << " "
			<< salt << " "
			<< hashPassword << " "
			<< FIO << " "
			<< to_string(subscriber_numer) << std::endl;
		file.close();
		if (role == 1)
		{
			cout << "Администратор добавлен\n";
		}
		else
			cout << "Пользователь добавлен\n";
	}
	static string ChangeParametr(string parametr)
	{
		string newData;
		std::cout << "Введите новое значение для " + parametr;
		std::cin >> newData;
		//Проверка ввода фамилии
		return newData;
	}
	static string GetFullFIO() // ВВод полного ФИО
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
	static void UpdateUser(string filename) // Редактирования файла с пользователями
	{
		string login;
		std::cout << "Введите логин пользователя для поиска:\n";
		std::cin >> login;
		vector<User> records = FileWorker::getUserArray(filename);
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
					string newParam;
					bool IsCorrect = true;
					switch (choice) {
					case 1:
						do {
							newParam = ChangeParametr("логина:\n");
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
						newParam = ChangeParametr("пароля:\n");
						item.saltedHashPassword = CryptoClass::hashPasswordWithSaltMethod(newParam, item.salt);
						choice = 5;
						break;
					case 3:
						do {
							newParam = ChangeParametr("номера телефона:\n");
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
						} while (IsCorrect == false);
						item.subscriberNumber = stoi(newParam);
						choice = 5;
						break;
					case 4:
						item.subscriberName = GetFullFIO();
						choice = 5;
						break;
					case 5:
						break;
					default:
						std::cout << "Неправильный выбор.\n";
						break;
					}
				} while (choice != 5);
				UpdateUsersFile(records, filename, "upd");
				break;
				//запись в найш файл нового массива структур
			}
		}
		if (!isFounded)
		{
			std::cout << "Нет такого логина в система:\n";
		}
	}
	static void DeleteUser(string filename)
	{
		string login;
		std::cout << "Введите логин пользователя для удаления:\n";
		std::cin >> login;
		vector<User> records = FileWorker::getUserArray(filename);
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
			UpdateUsersFile(records, filename, "del");
		}
	}
	static void viewAllUsers(string filename) // Просмотр всех пользователей
	{
		setlocale(LC_CTYPE, "rus");
		vector<User> records = FileWorker::getUserArray(filename);
		std::cout << std::setw(15) << std::left << "| Логин"
			<< std::setw(30) << std::left << "| ФИО"
			<< std::setw(20) << std::left << "| Номер телефона" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
		//SetConsoleOutputCP(CP_UTF8);
		for (auto& item : records) {
			std::cout << std::setw(15) << std::left << "| " + item.login
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
		vector<User> records = FileWorker::getUserArray(filename);
		bool isFounded = false;
		for (auto& item : records) {
			if (item.login == login)
			{
				std::cout << "Такой пользователь найден\n";
				std::cout << std::setw(15) << std::left << "| Логин"
					<< std::setw(30) << std::left << "| ФИО"
					<< std::setw(20) << std::left << "| Номер телефона" << std::endl;
				std::cout << "---------------------------------------------------------------" << std::endl;
				std::cout << std::setw(15) << std::left << "| " + item.login
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
