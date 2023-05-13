#include "ReadFromFileClass.h"




	std::vector<User> ReadFromFileClass::getUserArray(std::string filename)
	{
		std::ifstream file(filename);
		const int MAX_RECORDS = 100; // ������������ ���������� �������
		User recordsArray[MAX_RECORDS]; // ������ ��������
		if (file.is_open()) {
			std::string line;
			int index = 0;
			while (std::getline(file, line) && index < MAX_RECORDS) {
				User record;
				std::istringstream iss(line);
				iss >> record.key >> record.login >> record.role >>
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
			std::cout << "�� ������� ������� ���� ��� ������.\n";
		}
	}
	std::vector<CallRecord> ReadFromFileClass::getDataArray(std::string filename)
	{
		std::ifstream file(filename);
		const int MAX_RECORDS = 150; // ������������ ���������� �������
		CallRecord recordsArray[MAX_RECORDS]; // ������ ��������
		if (file.is_open()) {
			std::string line;
			int index = 0;
			while (std::getline(file, line) && index < MAX_RECORDS) {
				CallRecord record;
				std::istringstream iss(line);
				iss >> record.key >> record.subscriberNumber >> record.subscriberName >>
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


