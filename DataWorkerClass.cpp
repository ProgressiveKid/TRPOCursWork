#include "DataWorkerClass.h"
	 void DataWorkerClass::checkFile(string filename) {
		ifstream iff(filename, ios::out);
		if (iff.good() == true)
		{
			//����������� ����� ���� ���������� � ������ ���������
			std::ofstream file(filename);
			cout << "����� ���� ������ \n";
		}
	}
	 int DataWorkerClass::ConvertToInt(const std::string& str)
	{
		try {
			size_t pos;
			int number = std::stoi(str, &pos);
			// ���������, ��� �������������� ������ ������� � ��� ������� ���� ������������
			if (pos == str.length()) {
				return number;
			}
		}
		catch (const std::exception&) {
			return 0;
			// ��������� ������ ��������������
		}
		// ���� �������������� �� ������� ��� �������� ������������ �������, ���������� �������� �� ���������
		return 0; // ��� ����� ������ �������� �� ������ ������
	}
	 bool DataWorkerClass::IsDouble(const std::string& str) {
		try {
			size_t pos;
			double value = std::stod(str, &pos);
			// ���������, ��� �������������� ������ ������� � ��� ������� ���� ������������
			if (pos == str.length()) {
				return true;
			}
		}
		catch (const std::exception&) {
		}
		return false;
	}
	 double DataWorkerClass::getCallTariff(const std::string dateTimeString) {
		// ��������� ���� � ����� ������ �� ������������
		std::tm timeinfo = {};
		std::istringstream iss(dateTimeString);
		// ������ ���� �� ������
		iss >> std::get_time(&timeinfo, "%d.%m.%Y %H:%M:%S");
		// ����������� std::tm � std::time_t
		std::time_t t = std::mktime(&timeinfo);
		// ����������� std::time_t � std::tm � �������������� localtime_s
		std::tm localTime = {};
		localtime_s(&localTime, &t);
		// �������� ���� ������ (0 - �����������, 1 - �����������, � �.�.)
		int weekday = localTime.tm_wday;
		//return weekdayNumber;
		double tariff = 0.5; // ����� �� ���������
		// ��������� ���� ������ � ��������� ��������������� �����������
		if (weekday >= 6) {
			tariff += 0.4;
		}
		else {
			tariff += 0.2;
		}
		// ��������� ����� ������ � ��������� ��������������� �����������
		//int hour = timeinfo.tm_hour;
		int hour = localTime.tm_hour;
		if (hour >= 0 && hour < 8) {
			tariff *= 0.5;
		}
		return tariff;
	}
	 void DataWorkerClass::createData(const std::string& filename) {
		cout << "---������� ���������� � ������:--- \n";
		std::ofstream file;
		file.open(filename, std::ios::app | std::ios_base::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file " << filename << std::endl;
			return;
		}
		bool isLoginCorrect = false; string login;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		vector<User> recordsUsers = ReadFromFileClass::getUserArray(GlobalVariablesClass::UsersFile);
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
				cout << "\n �������� � ����� ������� �� ������: ";
			else
				break;
		}
		bool typeCall; // ���� ��� ������ <----------
		cout << "������ ������������ ���������(0) ��� ��������(1) ? :";
		cin >> typeCall;
		if (typeCall == 1)
			typeCall = true;
		else if (typeCall == 0)
			typeCall = false;
		else
			true;
		std::cin.ignore();
		string CallPhone; bool IsCorrect = false;
		User CallUser; // ��� ���� ������ ��� ��� ��� ��� ������
		do {
			cout << "������� ����� ������ ��� ����� ��������:  ";
			cin >> CallPhone;
			int IntCallPhone = ConvertToInt(CallPhone);
			for (auto& item : recordsUsers)
			{
				if ((item.subscriberNumber == IntCallPhone || item.login == CallPhone) && item.login != login)
				{// ������ ������� ������ ����
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
			std::cout << "������� ����������������� ������ (� �������): " << endl;
			std::getline(std::cin, CallDuration);
			if (IsDouble(CallDuration)) {
				break;
			}
			else {
				std::cout << "����� ������� �����������." << std::endl;
			}
		} while (true);
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
		std::string current_datetime = oss.str();
		double tariffPerMinute = getCallTariff(current_datetime); // ����������� ������
		std::string callDate = current_datetime.substr(0, 10); // ���� ������
		std::string callTime = current_datetime.substr(11, 8); // ����� ������
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
		cout << " ������ ��������� \n";
	}
	 void DataWorkerClass::viewData(const std::string& filename) {
		setlocale(LC_CTYPE, "rus");
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		std::cout << std::setw(4) << std::left << "| �"
			<< std::setw(17) << std::left << "| ����� ��������"
			<< std::setw(30) << std::left << "| ��� ��������"
			<< std::setw(20) << std::left << "| ��������� �����"
			<< std::setw(15) << std::left << "| ����� ������"
			<< std::setw(15) << std::left << "| ���� ������"
			<< std::setw(15) << std::left << "| ����� ������"
			<< std::setw(30) << std::left << "| ����������������� (���)"
			<< std::setw(20) << std::left << "| ����� �� ������"
			<< std::endl;
		for (auto& item : records) {
			ostringstream oss;
			oss << std::fixed << std::setprecision(1) << item.callDuration / 60;
			string formattedDuration = "| " + oss.str(); oss.str("");
			oss << std::fixed << std::setprecision(1) << item.tariffPerMinute;
			string formattedTariff = "| " + oss.str();
			std::cout << std::setw(4) << std::left << "| " + std::to_string(item.key)
				<< std::setw(17) << std::left << "| " + std::to_string(item.subscriberNumber)
				<< std::setw(30) << std::left << "| " + item.subscriberName
				<< std::setw(20) << std::left << "| " + (item.isOutgoingCall ? std::string("��") : std::string("���"))
				<< std::setw(15) << std::left << "| " + std::to_string(item.callNumber)
				<< std::setw(15) << std::left << "| " + item.callDate
				<< std::setw(15) << std::left << "| " + item.callTime
				<< std::setw(30) << std::left << formattedDuration
				<< std::setw(30) << std::left << formattedTariff << std::endl;
		}
	}
	 string DataWorkerClass::changeParametr(string parametr)
	{
		string newData;
		std::cout << "������� ����� �������� ��� " + parametr;
		std::cin >> newData;
		return newData;
	}
	 void DataWorkerClass::updateDataFile(vector<CallRecord> data, const std::string& filename, string Operation)
	{ //����������� ���������
		std::ofstream outFile(filename, std::ios_base::out | std::ios_base::binary);
		if (!outFile.is_open())
		{
			std::cout << "�� ������� ������� ���� ��� ������.\n";
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
		if (Operation == "del")
		{
			std::cout << "������ �������\n";
		}
		else if (Operation == "upd")
		{
			std::cout << "������ ��������\n";
		}
		else if (Operation == " ")
		{
		}
		outFile.close();
	}
	 bool DataWorkerClass::isValidDate(int day, int month, int year)
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
	 int DataWorkerClass::inputComponent(const std::string& componentName, int minValue, int maxValue) {
		int component;
		while (true) {
			std::cout << "������� " << componentName << ": ";
			std::cin >> component;
			if (std::cin.fail() || component < minValue || component > maxValue) {
				std::cin.clear();
				//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "������������ ����. ���������� �����." << std::endl;
				continue;
			}
			break;
		}
		return component;
	}
	 string DataWorkerClass::inputDate()
	{
		std::string date;
		//std::cout << "\n�������  ";
		int day = inputComponent("����", 1, 31);
		int month = inputComponent("�����", 1, 12);
		int year = inputComponent("���", 2000, 2024);
		if (isValidDate(day, month, year)) {
			std::ostringstream oss;
			oss << std::setfill('0') << std::setw(2) << day;
			string Day = oss.str();
			oss.str("");
			oss << std::setfill('0') << std::setw(2) << month;
			string Month = oss.str();
			//std::cout << "��������� ���� ���������: " << day << "." << month << "." << year << std::endl;
			date = Day + "." + Month + "." + to_string(year);
		}
		else {
			std::cout << "������������ ����." << std::endl;
		}
		return date;
	}
	 string DataWorkerClass::inputTime()
	{
		std::string time;
		int hour = inputComponent("�����", 1, 60);
		int minute = inputComponent("������", 1, 60);
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << hour;
		string Hour = oss.str();
		oss.str("");
		oss << std::setfill('0') << std::setw(2) << minute;
		string Minute = oss.str();
		time = Hour + ":" + Minute;
		return time;
	}
	 void DataWorkerClass::updateCallRecord(string filename)
	{
		int callKey;
		std::cout << "������� ����� ������ ��� ������:\n";
		std::cin >> callKey;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		bool isFound = false;
		for (auto& item : records) {
			if (item.key == callKey) {
				isFound = true;
				int choice = 0;
				do {
					std::cout << "��� �� ������ �������������?:\n";
					std::cout << "*�������������� ��� � ������ ��������� ���������� ��� �������������� ������������\n";
					std::cout << "1. ��� ������ (��������/���������)\n";
					std::cout << "2. ���� ������\n";
					std::cout << "3. ����� ������\n";
					std::cout << "4. �����������������\n";
					std::cout << "5. �����\n";
					std::cin >> choice;
					std::string newParam;
					bool isCorrect = true;
					int newtariff;
					switch (choice) {
					case 1:
						do {
							newParam = changeParametr("���������� ������ (��/���):\n");
							if (newParam == "��" || newParam == "��" || newParam == "���" || newParam == "���") {
								if (newParam == "��" || newParam == "��")
								{
									item.isOutgoingCall = false;
								}
								else
									item.isOutgoingCall = true;
								isCorrect = true;
							}
							else {
								std::cout << "������������ ����. ������� '��' ��� '���'.\n";
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
							newParam = changeParametr("����������������� ������:\n");
							if (IsDouble(newParam)) {
								isCorrect = true;
							}
							else
							{
								isCorrect = false;
								std::cout << "� ����������������� ������ ������ ���� ������ �����:\n";
							}
						} while (!isCorrect);
						item.callDuration = std::stod(newParam) * 60;
						choice = 5;
						break;
					case 5:
						break;
					default:
						std::cout << "������������ �����.\n";
						break;
					}
				} while (choice != 5);
				updateDataFile(records, filename, "upd");
				break;
			}
		}
		if (!isFound) {
			std::cout << "��� ������ ������ ������ � �������:\n";
		}
	}
	 void DataWorkerClass::deleteCallRecord(string filename)
	{
		int callKey;
		std::cout << "������� ����� ������ ��� ��������:\n";
		std::cin >> callKey;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		bool isFounded = false;
		for (int i = 0; i < records.size(); i++) {
			if (records[i].key == callKey) {
				isFounded = true;
				std::string answer;
				while (true) {
					std::cout << "�� ������������� ������ ������� ������? (��/���): ";
					std::cin >> answer;
					if (answer == "��" || answer == "��" || answer == "��") {
						// ���������� �������� �������� ����� ��� ������
						records.erase(records.begin() + i);
						updateDataFile(records, filename, "del");
						break;
					}
					else if (answer == "���" || answer == "���" || answer == "���") {
						std::cout << "�������� ����� (������) ��������.\n";
						break;
					}
					else {
						std::cout << "������������ ����. ����������, ������� '��' ��� '���'.\n";
						// ������� ������ �����
						std::cin.clear();
					}
				}
				break;
			}
		}
		if (!isFounded) {
			std::cout << "��� ������ ������ ������ � �������:\n";
		}
	}
