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
			//����������� ����� ���� ���������� � ������ ���������
			std::ofstream file(filename);
			cout << "����� ���� ������ \n";
		}
	}
	static std::vector<CallRecord> getDataArray(std::string filename)
	{// �� ������� - �� ��������� ����� ������ �� ���������� �����
		std::ifstream file(filename);
		const int MAX_RECORDS = 150; // ������������ ���������� �������
		CallRecord recordsArray[MAX_RECORDS]; // ������ ��������
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
			std::cout << "�� ������� ������� ���� ��� ������.\n";
		}
	}
	static double getCallTariff(const std::string dateTimeString) {
		// ��������� ���� � ����� ������ �� ������������
		// ���� ������ ����������� ������: � ����������� �� ������ �������� ������ � ���� ������ �����
	//	std::string dateTimeString = "01.05.2023 15:30:00"; // ������ ������ � ����� � ��������
		std::string callDate = dateTimeString.substr(0, 10); // ��������� ��������� � ����� "dd.mm.yyyy"
		std::string callTime = dateTimeString.substr(11, 8);

		int year, month, day, hour, minute;
		sscanf_s(callDate.c_str(), "%d-%d-%d", &year, &month, &day);
		sscanf_s(callTime.c_str(), "%d:%d", &hour, &minute);

		// ��������� ���� ������ (0 - �����������, 6 - �������)
		std::tm timeinfo = { 0 };
		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		std::time_t t = std::mktime(&timeinfo);
		int weekday = std::localtime(&t)->tm_wday;

		

		double tariff = 0.5; // ����� �� ���������

		// ��������� ���� ������ � ��������� ��������������� �����������
		if (weekday == 0 || weekday == 6) {
			tariff += 0.4;
		}
		else {
			tariff += 0.2;
		}

		// ��������� ����� ������ � ��������� ��������������� �����������
		if (hour >= 0 && hour < 6) {
			tariff *= 0.8;
		}

		return tariff;
	}

	static void createData(const std::string& filename) {
		cout << "---������� ���������� � ������:--- \n";
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
		User user; // ������������ ��� �������� ������ ������ <-----------
		while (!isLoginCorrect) {
			cout << "������� ����� ���������: ";
			cin >> login;			
				for (auto& item : recordsUsers) {
					if (item.login == login) {
						user = item;
						isLoginCorrect = true;
					}
				}
				if (!isLoginCorrect)
					cout << "\n �������� � ����� ������� �� ������:\n ";
				else
					break;
		}
		bool typeCall; // ���� ��� ������ <----------
		do
		{
			cout << "������ ������������ ��������� (0) ��� �������� (1)?:  ";
			cin >> typeCall;
			//0 false, � 1 true
			if (typeCall != 1 && typeCall != 0)
				cout << "\n �������� 0 ��� 1\n";
			else
				break;
		} while (true);

		int CallPhone;
		bool IsCorrect;
		User CallUser; // ��� ���� ������ ��� ��� ��� ��� ������
		//�����������
		//��������
		do {
			cout << "������� ����� ������ ��� ����� ��������:  ";
			cin >> CallPhone;

			for (char c : to_string(CallPhone)) {
				if (!isdigit(c)) {
					IsCorrect = false;
					std::cout << "\n� ������ �������� ������ ���� ������ �����:\n";
					break;
				}
				else
				{
					IsCorrect = true;
				}
			}
			if (IsCorrect) //���������� ��� ��� �����
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
			else // ���� �� ������
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
		cout << "������� ����������������� ������ (������� � �������):  ";
		cin >> CallDuration;
		for (char c : to_string(CallDuration)) {
			if (!isdigit(c)) {
				IsCorrect = false;
				std::cout << "\n� ������ �������� ������ ���� ������ �����:\n";
				break;
			}
			else
			{
				IsCorrect = true;
			}
		}
		} while (IsCorrect == false);

		//��������� �������� �������

		//time_t now = time(nullptr);
		//struct tm timeinfo;
		//auto time = localtime_s(&timeinfo, &now);


		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
		std::string current_datetime = oss.str();
		double tariffPerMinute = getCallTariff(current_datetime); // ����������� ������
		std::string callDate = current_datetime.substr(0, 10); // ���� ������
		std::string callTime = current_datetime.substr(11, 8); // ����� ������

		file << user.subscriberNumber << " "
			<< user.subscriberName << " "
			<< typeCall << " "
			<< CallUser.subscriberNumber << " "
			<< callDate << " "
			<< callTime << " "
			<< CallDuration << " "
			<< tariffPerMinute << std::endl;
		file.close();
		cout << "������ ���������\n";

	}
};
