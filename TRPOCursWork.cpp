#include <iostream>
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include "GlobalVariablesClass.h"
//#include "FileWorker.h"
#include "DataWorkerClass.h"
#include "FileWorker.h"
#include "Structures.h"
#include <vector>
#include "IndividualTask.h"
#include <conio.h> // для функции _getch()
using namespace std;
static string const USERS_FILE = GlobalVariablesClass::UsersFile;
static string const DATA_CALL_FILE = GlobalVariablesClass::DataCallFile;
string Username;
void UserInterface(string login)
{
	int choice = 0;
	do {
		std::cout << login + ", выберите опцию:\n";
		std::cout << "1. Просмотр\n";
		std::cout << "2. Индивидуальное задание\n";
		std::cout << "3. Поиск\n";
		std::cout << "4. Сортировка\n";
		std::cout << "5. Выход\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			DataWorkerClass::viewData(DATA_CALL_FILE);
			break;
		case 2:
			IndividualTask::individualTasksInterface();
			break;
		case 3:
			IndividualTask::Search();
			break;
		case 4:
			IndividualTask::sort();
			break;
		case 5:
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 5);
}
void ChooseView()
{
	int choiceView = 0;
	std::cout << "Выберите опцию:\n";
	std::cout << "1. Вывод всех пользователей\n";
	std::cout << "2. Поиск информации о пользователе по его логину\n";
	std::cin >> choiceView;
	switch (choiceView) {
	case 1:
		FileWorker::viewAllUsers(USERS_FILE);
		break;
	case 2:
		FileWorker::findForLoginViewUser(USERS_FILE);
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
			FileWorker::createUser(USERS_FILE);
			break;
		case 2:
			ChooseView();
			break;
		case 3:
			FileWorker::updateUser(USERS_FILE);
			break;
		case 4:
			FileWorker::deleteUser(USERS_FILE, Username);
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
			DataWorkerClass::createData(DATA_CALL_FILE);
			break;
		case 2:
			DataWorkerClass::viewData(DATA_CALL_FILE);
			break;
		case 3:
			DataWorkerClass::updateCallRecord(DATA_CALL_FILE);
			break;
		case 4:
			DataWorkerClass::deleteCallRecord(DATA_CALL_FILE);
			break;
		case 5:
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 5);
}
void IndividualProcessing()
{
	int choice = 0;
	do {
		std::cout << "Выберите опцию:\n";
		std::cout << "1. Индивидуальное задание\n";
		std::cout << "2. Поиск\n";
		std::cout << "3. Сортировка\n";
		std::cout << "4. Назад\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			IndividualTask::individualTasksInterface();
			break;
		case 2:
			IndividualTask::Search();
			break;
		case 3:
			IndividualTask::sort();
			break;
		case 4:
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 4);
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
		std::cout << "3. Обработка данных\n";
		std::cout << "4. Выход\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			CRUDUserInterface();
			break;
		case 2:
			CRUDDataInterface();
			break;
		case 3:
			IndividualProcessing();
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
	while (true)
	{//firstcommit
		cout << "Введите логин:\n";
		string username;
		string password;
		cin >> username;
		bool isFounded = false;
		vector<User> records = ReadFromFileClass::getUserArray(USERS_FILE);


		for (auto& item : records)
		{
			if (item.login == username)
			{
				isFounded = true;
				cout << "Пользователь найден\nВведите пароль:\n";
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
					Username = username;
					cout << "\nВы вошли в систему:\n";
					if (item.role == "0") // это пользователь
					{
						UserInterface(item.login);
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
					cout << "Неправильный пароль:\n";
				}
			}

		}
		if (!isFounded)
		{
			cout << "Такой пользователь не найден:\n";

		}
	};
}
int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	FileWorker::checkFile(USERS_FILE, DATA_CALL_FILE); //Проверка существует ли файл
	Authorisation();

}
