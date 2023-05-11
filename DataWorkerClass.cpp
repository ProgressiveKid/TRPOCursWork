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
using namespace std;


class DataWorkerClass {
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
	static int ConvertToInt(const std::string& str)
	{
		try {
			size_t pos;
			int number = std::stoi(str, &pos);
			// Проверяем, что преобразование прошло успешно и все символы были использованы
			if (pos == str.length()) {
				return number;
			}
		}
		catch (const std::exception&) {
			return 0;
			// Обработка ошибки преобразования
		}
		// Если преобразование не удалось или остались недопустимые символы, возвращаем значение по умолчанию
		return 0; // Или любое другое значение по вашему выбору
	}
	static bool IsDouble(const std::string& str) {
		try {
			size_t pos;
			double value = std::stod(str, &pos);
			// Проверяем, что преобразование прошло успешно и все символы были использованы
			if (pos == str.length()) {
				return true;
			}
		}
		catch (const std::exception&) {
		}
		return false;
	}
	static double getCallTariff(const std::string dateTimeString) {
		// Разбиваем дату и время звонка на составляющие
		std::tm timeinfo = {};
		std::istringstream iss(dateTimeString);
		// Читаем дату из строки
		iss >> std::get_time(&timeinfo, "%d.%m.%Y %H:%M:%S");
		// Преобразуем std::tm в std::time_t
		std::time_t t = std::mktime(&timeinfo);
		// Преобразуем std::time_t в std::tm с использованием localtime_s
		std::tm localTime = {};
		localtime_s(&localTime, &t);
		// Получаем день недели (0 - воскресенье, 1 - понедельник, и т.д.)
		int weekday = localTime.tm_wday;
		//return weekdayNumber;
		double tariff = 0.5; // Тариф по умолчанию
		// Проверяем день недели и добавляем соответствующий коэффициент
		if (weekday >= 6) {
			tariff += 0.4;
		}
		else {
			tariff += 0.2;
		}
		// Проверяем время звонка и добавляем соответствующий коэффициент
		//int hour = timeinfo.tm_hour;
		int hour = localTime.tm_hour;
		if (hour >= 0 && hour < 8) {
			tariff *= 0.5;
		}
		return tariff;
	}
	static void createData(const std::string& filename) {
		cout << "---Введите информацию о звонке:--- \n";
		std::ofstream file;
		file.open(filename, std::ios::app | std::ios_base::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file " << filename << std::endl;
			return;
		}
		bool isLoginCorrect = false; string login;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		vector<User> recordsUsers = ReadFromFileClass::getUserArray(GlobalVariablesClass::UsersFile);
		User user; // Пользователь для которого создаём запись <-----------
		while (!isLoginCorrect) {
			cout << "Введите логин аббонента: ";
			cin >> login;
			for (auto& item : recordsUsers) {
				if (item.login == login) {
					user = item;
					isLoginCorrect = true;
				}
			}
			if (!isLoginCorrect)
				cout << "\n Аббонент с таким логином не найден: ";
			else
				break;
		}
		bool typeCall; // флаг для звонка <----------
		cout << "Звонок пользователя исходящий(0) или входящий(1) ? :";
		cin >> typeCall;
		if (typeCall == 1)
			typeCall = true;
		else if (typeCall == 0)
			typeCall = false;
		else
			true;
		std::cin.ignore();
		//0 false, а 1 true
		string CallPhone; bool IsCorrect = false;
		User CallUser; // тот кому звоним или тот кто нам звонит
		do {
			cout << "Введите номер вызова или логин абонента:  ";
			cin >> CallPhone;
			int IntCallPhone = ConvertToInt(CallPhone);
			for (auto& item : recordsUsers)
			{
				if ((item.subscriberNumber == IntCallPhone || item.login == CallPhone) && item.login != login)
				{
					CallUser = item;
					IsCorrect = true;
					break;
				}
			}
		} while (IsCorrect == false);
		IsCorrect = false;
		std::string CallDuration;
		do {
			std::cin.ignore();
			std::cout << "Введите продолжительность вызова (в минутах): " << endl;
			std::getline(std::cin, CallDuration);
			if (IsDouble(CallDuration)) {
				//std::cout << "Число введено правильно." << std::endl;
				break;
			}
			else {
				std::cout << "Число введено неправильно." << std::endl;
			}
		} while (true);
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
		std::string current_datetime = oss.str();
		double tariffPerMinute = getCallTariff(current_datetime); // определение тарифа
		std::string callDate = current_datetime.substr(0, 10); // Дата вызова
		std::string callTime = current_datetime.substr(11, 8); // Время вызова
		file << records.size() + 1 << " "
			<< user.subscriberNumber << " "
			<< user.subscriberName << " "
			<< typeCall << " "
			<< CallUser.subscriberNumber << " "
			<< callDate << " "
			<< callTime << " "
			<< stod(CallDuration) * 60 << " "
			<< tariffPerMinute << std::endl;
		file.close();
		cout << " Запись добавлена \n";
	}
	static void viewData(const std::string& filename) {
		setlocale(LC_CTYPE, "rus");
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		std::cout << std::setw(4) << std::left << "| №"
			<< std::setw(17) << std::left << "| Номер абонента"
			<< std::setw(30) << std::left << "| ФИО абонента"
			<< std::setw(20) << std::left << "| Исходящий вызов"
			<< std::setw(15) << std::left << "| Номер вызова"
			<< std::setw(15) << std::left << "| Дата звонка"
			<< std::setw(15) << std::left << "| Время звонка"
			<< std::setw(30) << std::left << "| Продолжительность (мин)"
			<< std::setw(20) << std::left << "| Тариф на минуту"
			<< std::endl;
		for (auto& item : records) {
			//	string callduration = to_string();
			ostringstream oss;
			oss << std::fixed << std::setprecision(1) << item.callDuration / 60;
			string formattedDuration = "| " + oss.str(); oss.str("");
			oss << std::fixed << std::setprecision(1) << item.tariffPerMinute;
			string formattedTariff = "| " + oss.str();
			std::cout << std::setw(4) << std::left << "| " + std::to_string(item.key)
				<< std::setw(17) << std::left << "| " + std::to_string(item.subscriberNumber)
				<< std::setw(30) << std::left << "| " + item.subscriberName
				<< std::setw(20) << std::left << "| " + (item.isOutgoingCall ? std::string("Да") : std::string("Нет"))
				<< std::setw(15) << std::left << "| " + std::to_string(item.callNumber)
				<< std::setw(15) << std::left << "| " + item.callDate
				<< std::setw(15) << std::left << "| " + item.callTime
				<< std::setw(30) << std::left << formattedDuration
				<< std::setw(30) << std::left << formattedTariff << std::endl;
		}
	}
	static string changeParametr(string parametr)
	{
		string newData;
		std::cout << "Введите новое значение для " + parametr;
		std::cin >> newData;
		return newData;
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
			//outFile << newIndex << " " << record.login << " " << record.role << " " << record.salt << " " << record.saltedHashPassword << " " << record.subscriberName << " " << record.subscriberNumber << std::endl;
		}
		if (Operation == "del")
		{
			std::cout << "Запись удалена\n";
		}
		else if (Operation == "upd")
		{
			std::cout << "Запись изменена\n";
		}
		else if (Operation == " ")
		{
		}
		outFile.close();
	}
	static bool isValidDate(int day, int month, int year)
	{
		if (year < 1900 || year > 2100 || month < 1 || month > 12)
			return false;
		int maxDay = 31;
		if (month == 4 || month == 6 || month == 9 || month == 11)
			maxDay = 30;
		else if (month == 2) {
			if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
				maxDay = 29;
			else
				maxDay = 28;
		}
		return day >= 1 && day <= maxDay;
	}
	static int inputComponent(const std::string& componentName, int minValue, int maxValue) {
		int component;
		while (true) {
			std::cout << "Введите " << componentName << ": ";
			std::cin >> component;
			if (std::cin.fail() || component < minValue || component > maxValue) {
				std::cin.clear();
				//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Некорректный ввод. Попробуйте снова." << std::endl;
				continue;
			}
			break;
		}
		return component;
	}
	static string inputDate()
	{
		std::string date;
		//std::cout << "\nВведите  ";
		int day = inputComponent("День", 1, 31);
		int month = inputComponent("Месяц", 1, 12);
		int year = inputComponent("Год", 2000, 2024);
		if (isValidDate(day, month, year)) {
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << day;
			string Day = oss.str();
			oss.str("");
			oss << std::setfill('0') << std::setw(2) << month;
			string Month = oss.str();
			//std::cout << "Введенная дата корректна: " << day << "." << month << "." << year << std::endl;
			date = Day + "." + Month + "." + to_string(year);
		}
		else {
			std::cout << "Некорректная дата." << std::endl;
		}
		return date;
	}
	static string inputTime()
	{
		std::string time;
		int hour = inputComponent("Время", 1, 60);
		int minute = inputComponent("Минуты", 1, 60);
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << hour;
		string Hour = oss.str();
		oss.str("");
		oss << std::setfill('0') << std::setw(2) << minute;
		string Minute = oss.str();
		time = Hour + ":" + Minute;
		return time;
	}
	static void updateCallRecord(string filename)
	{
		int callKey;
		std::cout << "Введите номер записи для поиска:\n";
		std::cin >> callKey;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		bool isFound = false;
		for (auto& item : records) {
			if (item.key == callKey) {
				isFound = true;
				int choice = 0;
				do {
					std::cout << "Что вы хотите редактировать?:\n";
					std::cout << "*Редактирование ФИО и номера аббонента происходит при редактировании пользователя\n";
					std::cout << "1. Номер исходящего вызова\n";
					std::cout << "2. Дату звонка\n";
					std::cout << "3. Время звонка\n";
					std::cout << "4. Продолжительность\n";
					std::cout << "5. Назад\n";
					std::cin >> choice;
					std::string newParam;
					bool isCorrect = true;
					int newtariff;
					switch (choice) {
					case 1:
						do {
							newParam = changeParametr("исходящего вызова (Да/Нет):\n");
							if (newParam == "Да" || newParam == "да" || newParam == "Нет" || newParam == "нет") {
								item.isOutgoingCall = (newParam == "Да" || newParam == "да");
								isCorrect = true;
							}
							else {
								std::cout << "Некорректный ввод. Введите 'Да' или 'Нет'.\n";
								isCorrect = false;
							}
						} while (!isCorrect);
						choice = 5;
						break;
					case 2:
						newParam = inputDate();
						item.tariffPerMinute = getCallTariff(newParam + " " + item.callTime);
						item.callDate = newParam;
						choice = 5;
						break;
					case 3:
						newParam = inputTime();
						item.callTime = newParam;
						item.tariffPerMinute = getCallTariff(item.callDate + " " + newParam);
						choice = 5;
						break;
					case 4:
						do {
							newParam = changeParametr("продолжительности звонка:\n");
							if (IsDouble(newParam)) {
								//std::cout << "Число введено правильно." << std::endl;
								isCorrect = true;
							}
							else
							{
								isCorrect = false;
								std::cout << "В продолжительности звонка должны быть только цифры:\n";
							}
						} while (!isCorrect);
						item.callDuration = std::stod(newParam) * 60;
						choice = 5;
						break;
					case 5:
						break;
					default:
						std::cout << "Неправильный выбор.\n";
						break;
					}
				} while (choice != 5);
				updateDataFile(records, filename, "upd");
				break;
			}
		}
		if (!isFound) {
			std::cout << "Нет такого номера вызова в системе:\n";
		}
	}
	static void deleteCallRecord(string filename)
	{
		int callKey;
		std::cout << "Введите номер записи для удаления:\n";
		std::cin >> callKey;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		bool isFounded = false;
		for (int i = 0; i < records.size(); i++) {
			if (records[i].key == callKey) {
				isFounded = true;
				//std::remove_if(records.begin(), records.end(), records[i]), records.end();
				//records.erase(records.begin() + i);
				std::string answer;
				while (true) {
					std::cout << "Вы действительно хотите удалить запись? (да/нет): ";
					std::cin >> answer;
					if (answer == "да" || answer == "Да" || answer == "ДА") {
						// Выполнение действий удаления файла или записи
						records.erase(records.begin() + i);
						updateDataFile(records, filename, "del");
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
				break;
			}
		}
		if (!isFounded) {
			std::cout << "Нет такого номера вызова в системе:\n";
		}
		//std::cout << "Вы действительно хотите удалить данного пользователя?:\n";
	}


};
