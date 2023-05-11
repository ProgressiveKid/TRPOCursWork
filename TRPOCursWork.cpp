#include <iostream>
#include <windows.h>
#include <wincrypt.h>
#include <string>
//#include "FileWorker.h"
#include "DataWorkerClass.cpp"
#include "FileWorker.cpp"
#include "Structures.h"
#include <vector>
//#include "ReadFromFileClass.h"
#include <conio.h> // для функции _getch()
//#include "CryptoClass.h"
//#include "DataWorkerClass.h"
using namespace std;

static string const UsersFile = "usersSystem.txt";
static string const DataCallFile = "infoUsers.txt";
void UserInterface()
{
}
void ChooseView()
{
	int choiceView = 0;
	std::cout << "Выберите опцию:\n";
	std::cout << "1. Вывод всех пользователей\n";
	std::cout << "2. Вывод информации о пользователе по его логину\n";
	std::cin >> choiceView;
	switch (choiceView) {
	case 1:
		FileWorker::viewAllUsers(UsersFile);
		break;
	case 2:
		FileWorker::findForLoginViewUser(UsersFile);
		break;
	default:
		std::cout << "Неправильный выбор.\n";
		break;
	}
}
void CRUDUserInterface()
{
	int choice = 0;
	do {
		std::cout << "Выберите опцию:\n";
		std::cout << "1. Добавление\n";
		std::cout << "2. Просмотр\n";
		std::cout << "3. Редактирование\n";
		std::cout << "4. Удаление\n";
		std::cout << "5. Назад\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			FileWorker::createUser(UsersFile);
			break;
		case 2:
			ChooseView();
			break;
		case 3:
			FileWorker::updateUser(UsersFile);
			break;
		case 4:
			FileWorker::deleteUser(UsersFile);
			break;
		case 5:
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 5);
}
void CRUDDataInterface()
{
	int choice = 0;
	do {
		std::cout << "Выберите опцию:\n";
		std::cout << "1. Добавление\n";
		std::cout << "2. Просмотр\n";
		std::cout << "3. Редактирование\n";
		std::cout << "4. Удаление\n";
		std::cout << "5. Назад\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			DataWorkerClass::createData(DataCallFile);
			break;
		case 2:
			DataWorkerClass::viewData(DataCallFile);
			break;
		case 3:
			DataWorkerClass::updateCallRecord(DataCallFile);

			break;
		case 4:
			DataWorkerClass::deleteCallRecord(DataCallFile);

			break;
		case 5:
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 5);


}

void DataInterface()
{
	int choice = 0;
	do {
		std::cout << "Выберите опцию:\n";
		std::cout << "1. Работа с данными\n";
		std::cout << "2. Индивидуальное задание\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			CRUDDataInterface();
			break;
		case 2:
			
			break;
		case 3:

			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 3);

}
void AdminInterface(string login)
{
	int choice = 0;
	do {
		std::cout << login + ", выберите опцию:\n";
		std::cout << "1. Работа с учётными записями\n";
		std::cout << "2. Работа с данными\n";
		std::cout << "3. Индивидуальное задание\n";
		std::cout << "4. Выход\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			CRUDUserInterface();
			break;
		case 2:
			//DataInterface();
			CRUDDataInterface();
			//UserInterface();
			break;
		case 3:
			break;
		case 4:
			std::cout << "Выход...\n";
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 4);
}
void Authorisation()
{
	cout << "Введите логин:\n";
	string username;
	string password;
	cin >> username;
	FileWorker::checkFile(UsersFile); //Проверка существует ли файл
	vector<User> records = ReadFromFileClass::getUserArray(UsersFile);
	for (auto& item : records)
	{
		if (item.login == username)
		{
			cout << "Пользователь найден\nВведите пароль:\n";
			//cin >> username;
			char ch;
			while ((ch = _getch()) != 13) { // 13 - код клавиши Enter
				if (ch == '\b') { // если нажата клавиша Backspace
					if (!password.empty()) {
						cout << "\b \b";
						password.pop_back(); // удаляем последний символ из строки
					}
				}
				else {
					password += ch;
					cout << "*";
				}
			}
			auto hashPassword = CryptoClass::hashPasswordWithSaltMethod(password, item.salt);
			if (hashPassword == item.saltedHashPassword)
			{
				cout << "Вы вошли в систему:\n";
				if (item.role == "0") // это пользователь
				{
					UserInterface();
				}
				else if (item.role == "1") //администратор
				{
					AdminInterface(item.login);
				}
				else
				{
					cout << "Неверная роль, обратитесь к администратору систему\n";
				}
			}
			else
			{
				cout << "ты долбаёб1:\n";
			}
		}
	}
}
//сначало - авторизация -> потом остальные меню
int main()
{
	//setlocale(LC_CTYPE, "rus");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	AdminInterface("vlad");
	//работаем над CRUDOM 
	//int choice = 0;
	//do {
	//	std::cout << "Выберите свою роль:\n";
	//	std::cout << "1. Авторизация\n";
	//	std::cout << "2. Выход\n";
	//	std::cin >> choice;
	//	switch (choice) {
	//	case 1:
	//		//Authorisation(choice);
	//		Authorisation();
	//		break;
	//	case 2:
	//		break;
	//	default:
	//		std::cout << "Неправильный выбор.\n";
	//		break;
	//	}
	//} while (choice != 2);
	//return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
