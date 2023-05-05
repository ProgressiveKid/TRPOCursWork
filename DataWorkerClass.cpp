#include "DataWorkerClass.h"
#include <__msvc_chrono.hpp>
#define _CRT_SECURE_NO_WARNINGS
#pragma once

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
	static std::vector<CallRecord> getDataArray(std::string filename)
	{// Не трогаем - он правильно берет данные из текстового файла
		std::ifstream file(filename);
		const int MAX_RECORDS = 150; // максимальное количество записей
		CallRecord recordsArray[MAX_RECORDS]; // массив структур
		if (file.is_open()) {
			std::string line;
			int index = 0;
			while (std::getline(file, line) && index < MAX_RECORDS) {
				CallRecord record;
				std::istringstream iss(line);
				iss >> record.subscriberNumber >> record.subscriberName >>
					record.isOutgoingCall >> record.callNumber >>
					record.callDate >> record.callTime >>
					record.callDuration >> record.tariffPerMinute;
				recordsArray[index] = record;
				++index;
			}
			file.close();
			std::vector<CallRecord> records(recordsArray, recordsArray + index);
			return records;
		}
		else {
			std::cout << "Не удалось открыть файл для чтения.\n";
		}
	}
	static double getCallTariff(const std::string dateTimeString) {
		// Разбиваем дату и время звонка на составляющие
		// Идея логики определения тарифа: в зависимости от времни создания записи и даты менять тариф
	//	std::string dateTimeString = "01.05.2023 15:30:00"; // пример строки с датой и временем
		std::string callDate = dateTimeString.substr(0, 10); // выделение подстроки с датой "dd.mm.yyyy"
		std::string callTime = dateTimeString.substr(11, 8);

		int year, month, day, hour, minute;
		sscanf_s(callDate.c_str(), "%d-%d-%d", &year, &month, &day);
		sscanf_s(callTime.c_str(), "%d:%d", &hour, &minute);

		// Вычисляем день недели (0 - воскресенье, 6 - суббота)
		std::tm timeinfo = { 0 };
		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		std::time_t t = std::mktime(&timeinfo);
		int weekday = std::localtime(&t)->tm_wday;

		

		double tariff = 0.5; // Тариф по умолчанию

		// Проверяем день недели и добавляем соответствующий коэффициент
		if (weekday == 0 || weekday == 6) {
			tariff += 0.4;
		}
		else {
			tariff += 0.2;
		}

		// Проверяем время звонка и добавляем соответствующий коэффициент
		if (hour >= 0 && hour < 6) {
			tariff *= 0.8;
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
		bool isLoginCorrect = false;
		string login;
		vector<CallRecord> records = DataWorkerClass::getDataArray(filename);
		vector<User> recordsUsers = FileWorker::getUserArray("usersSystem.txt");
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
					cout << "\n Аббонент с таким логином не найден:\n ";
				else
					break;
		}
		bool typeCall; // флаг для звонка <----------
		do
		{
			cout << "Звонок пользователя исходящий (0) или входящий (1)?:  ";
			cin >> typeCall;
			//0 false, а 1 true
			if (typeCall != 1 && typeCall != 0)
				cout << "\n выберите 0 или 1\n";
			else
				break;
		} while (true);

		int CallPhone;
		bool IsCorrect;
		User CallUser; // тот кому звоним или тот кто нам звонит
		//Определение
		//Проверка
		do {
			cout << "Введите номер вызова или логин абонента:  ";
			cin >> CallPhone;

			for (char c : to_string(CallPhone)) {
				if (!isdigit(c)) {
					IsCorrect = false;
					std::cout << "\nВ номере телефона должны быть только цифры:\n";
					break;
				}
				else
				{
					IsCorrect = true;
				}
			}
			if (IsCorrect) //определили что это цифры
			{
				for (auto& item : recordsUsers)
				{
					if (item.subscriberNumber == CallPhone)
					{
						CallUser = item;
						IsCorrect = true;
						break;
					}
				}
			}
			else // ищем по логину
			{
				for (auto& item : recordsUsers)
				{
					if (item.subscriberName == to_string(CallPhone))
					{
						CallUser = item;
						IsCorrect = true;
						break;
					}
				}
			}

		} while (IsCorrect == false);
		IsCorrect = false;
		int CallDuration; // <---------
		do {
		cout << "Введите продолжительность вызова (укажите в минутах):  ";
		cin >> CallDuration;
		for (char c : to_string(CallDuration)) {
			if (!isdigit(c)) {
				IsCorrect = false;
				std::cout << "\nВ номере телефона должны быть только цифры:\n";
				break;
			}
			else
			{
				IsCorrect = true;
			}
		}
		} while (IsCorrect == false);

		//получение текущего времени

		//time_t now = time(nullptr);
		//struct tm timeinfo;
		//auto time = localtime_s(&timeinfo, &now);


		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
		std::string current_datetime = oss.str();
		double tariffPerMinute = getCallTariff(current_datetime); // определение тарифа
		std::string callDate = current_datetime.substr(0, 10); // Дата вызова
		std::string callTime = current_datetime.substr(11, 8); // Время вызова

		file << user.subscriberNumber << " "
			<< user.subscriberName << " "
			<< typeCall << " "
			<< CallUser.subscriberNumber << " "
			<< callDate << " "
			<< callTime << " "
			<< CallDuration << " "
			<< tariffPerMinute << std::endl;
		file.close();
		cout << "Запись добавлена\n";

	}
};
