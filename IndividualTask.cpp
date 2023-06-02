#include "IndividualTask.h"

void ViewInfoForTask(vector<CallRecord> records) // ����� ���������� �� ����������
{
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
	int index = 0;
	for (auto& item : records) {
		//	string callduration = to_string();
		index++;
		ostringstream oss;
		oss << std::fixed << std::setprecision(1) << item.callDuration / 60;
		string formattedDuration = "| " + oss.str(); oss.str("");
		oss << std::fixed << std::setprecision(1) << item.tariffPerMinute;
		string formattedTariff = "| " + oss.str();
		std::cout << std::setw(4) << std::left << "| " + std::to_string(index)
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
void IndividualTask::Search()
{
	int choice = 0;
	do {
		vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::DataCallFile);
		vector<CallRecord> recordsForTask;
		std::cout << "�������� �������� ��� ������:\n";
		std::cout << "1. ����� ������\n";
		std::cout << "2. ��� ��������\n";
		std::cout << "3. ����� ��������\n";
		std::cout << "4. ����� ������\n";
		std::cout << "5. �����\n";
		int key; string PhoneNumber; int IntCallPhone; bool IsCorrect = false; string FIO;
		std::cin >> choice;
		switch (choice) {
		case 1:
			std::cout << "������� ����� ������: ";
			std::cin >> key;
			// ����� �� ������ ������
			for (const auto& record : records) {
				if (record.key == key) {
					recordsForTask.push_back(record);
				}
			}
			break;
		case 2:
			std::cout << "������� ��� ��������: \n";
			FIO = FileWorker::getFullFIO();
			for (const auto& record : records) {
				if (record.subscriberName == FIO) {
					recordsForTask.push_back(record);
				}
			}
			break;
		case 3:
			std::cout << "������� ����� ��������: ";
			cin >> PhoneNumber;
			IntCallPhone = DataWorkerClass::ConvertToInt(PhoneNumber);
			for (const auto& item : records)
			{
				if (item.subscriberNumber == IntCallPhone)
				{// ������ ������� ������ ����
					recordsForTask.push_back(item);
					IsCorrect = true;
					//break;
				}
			}
			break;
		case 4:
				cout << "������� ����� ������: ";
				cin >> PhoneNumber;
				IntCallPhone = DataWorkerClass::ConvertToInt(PhoneNumber);
				for (auto& item : records)
				{
					if (item.callNumber == IntCallPhone)
					{// ������ ������� ������ ����
						recordsForTask.push_back(item);
						IsCorrect = true;
						//break;
					}
				}
			break;
		case 5:
			break;
		default:
			std::cout << "������������ �����.\n";
			break;
		}
		if (recordsForTask.size() > 0)
		{
			ViewInfoForTask(recordsForTask);
		}
		else
		{
			std::cout << "�� ��������� ��������� �� ������� �� ������ ����������\n";
		}
	} while (choice != 5);
}
bool compareByDate(const CallRecord& record1, const CallRecord& record2) {
	std::tm tm1 = {};
	std::tm tm2 = {};
	std::istringstream ss1(record1.callDate);
	std::istringstream ss2(record2.callDate);
	ss1 >> std::get_time(&tm1, "%d.%m.%Y");
	ss2 >> std::get_time(&tm2, "%d.%m.%Y");
	return std::mktime(&tm1) < std::mktime(&tm2);
}
void IndividualTask::IndividualTasksInterface()
{
	vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::DataCallFile);
	vector<CallRecord> recordsForTask;
	std::cout << "������� ��������� ����� ��� ����������:\n";
	string startTime = DataWorkerClass::inputDate(); string endTime;
	if (!startTime.empty())
	{
		std::cout << "������� �������� ����� ��� ����������:\n";
		endTime = DataWorkerClass::inputDate();
		if (!endTime.empty())
		{
			std::tm tm1 = {};
			std::tm tm2 = {};
			std::tm tm3 = {};
			std::istringstream ss1(startTime);
			std::istringstream ss2(endTime);
			// ������� ��������� ��� � ��������� std::tm
			ss1 >> std::get_time(&tm1, "%d.%m.%Y");
			ss2 >> std::get_time(&tm2, "%d.%m.%Y");
			for (auto& item : records) {
				std::istringstream ss3(item.callDate);
				ss3 >> std::get_time(&tm3, "%d.%m.%Y");
				if (std::mktime(&tm1) <= std::mktime(&tm3) && std::mktime(&tm3) <= std::mktime(&tm2))
				{
					recordsForTask.push_back(item);
				}
			}
		}
	}
	if (recordsForTask.size() > 0)
	{
		int choice = 0;
		do {
			std::cout << "�������� �����:\n";
			std::cout << "1. �������� �������� � ��������� �������\n";
			std::cout << "2. ����� ����� �������� �������\n";
			std::cout << "3. ����� ����� ��������� �������\n";
			std::cout << "4. ����� ����� �� ��������� ������\n";
			std::cout << "5. �����\n";
			std::cin >> choice;
			int sum = 0; int sumTime = 0;
			vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::UsersFile);
			switch (choice) {
			case 1:
				std::cout << "������ ������� ����� " + startTime + " � " + endTime + "\n";
				std::sort(recordsForTask.begin(), recordsForTask.end(), compareByDate);
				ViewInfoForTask(recordsForTask);
				break;
			case 2:
				for (auto& item : recordsForTask) {
					if (item.isOutgoingCall)
					{
						sumTime += item.callDuration/60;
					}
				}
				std::cout << "����� ����� �������� ������� = " << sumTime << "���" << '\n';
				break;
			case 3:
				for (auto& item : recordsForTask) {
					if (!item.isOutgoingCall)
					{
						sumTime += item.callDuration/60;
					}
				}
				std::cout << "����� ����� ��������� ������� = " << sumTime << "���" << '\n';
				break;
			case 4:
				for (auto& item : recordsForTask) {
					sum += (item.callDuration/60) * item.tariffPerMinute;
				}
				std::cout << "����� ����� �� ��������� ������ = " << sum << "���" << '\n';				//	choice = 5;
				break;
			case 5:
				break;
			default:
				std::cout << "������������ �����.\n";
				break;
			}
		} while (choice != 5);
	}
	else
	{
		std::cout << "�� �������� ������� �� ������� �� ������ ����������\n";
	}
};
bool compareByDuration(const CallRecord& record1, const CallRecord& record2) {
	return record1.callDuration < record2.callDuration;
}
bool compareByFIO(const CallRecord& record1, const CallRecord& record2) {
	return record1.subscriberName < record2.subscriberName;
}
void IndividualTask::Sort()
{
	int choice = 0;
	do {
		vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::DataCallFile);
		std::cout << "�������� �������� ��� ����������:\n";
		std::cout << "1. ��� ��������\n";
		std::cout << "2. ���� ������\n";
		std::cout << "3. �����������������\n";
		std::cout << "4. �����\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			std::sort(records.begin(), records.end(), compareByFIO);
			ViewInfoForTask(records);
			break;
		case 2:
			std::sort(records.begin(), records.end(), compareByDate);
			ViewInfoForTask(records);
			break;
		case 3:
			std::sort(records.begin(), records.end(), compareByDuration);
			ViewInfoForTask(records);
			break;
		default:
			std::cout << "������������ �����.\n";
			break;
		}
	} while (choice != 4);
}