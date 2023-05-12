#include "IndividualTask.h"
void ViewInfoForTask(vector<CallRecord> records) // вывод информации по фильтрации
{
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
			<< std::setw(20) << std::left << "| " + (item.isOutgoingCall ? std::string("Да") : std::string("Нет"))
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
		std::cout << "Выберите параметр для поиска:\n";
		std::cout << "1. Номер записи\n";
		std::cout << "2. ФИО абонента\n";
		std::cout << "3. Номер абонента\n";
		std::cout << "4. Номер звонка\n";
		std::cout << "5. Назад\n";
		int key; string PhoneNumber; int IntCallPhone; bool IsCorrect = false; string FIO;
		std::cin >> choice;
		switch (choice) {
		case 1:
			std::cout << "Введите номер записи: ";
			std::cin >> key;
			// Поиск по номеру записи
			for (const auto& record : records) {
				if (record.key == key) {
					recordsForTask.push_back(record);
				}
			}
			break;
		case 2:
			std::cout << "Введите ФИО абонента: ";
			FIO = FileWorker::getFullFIO();
			for (const auto& record : records) {
				if (record.subscriberName == FIO) {
					recordsForTask.push_back(record);
				}
			}
			break;
		case 3:
			std::cout << "Введите номер абонента: ";
			cin >> PhoneNumber;
			IntCallPhone = DataWorkerClass::ConvertToInt(PhoneNumber);
			for (const auto& item : records)
			{
				if (item.subscriberNumber == IntCallPhone)
				{// нельзя звонить самому себе
					recordsForTask.push_back(item);
					IsCorrect = true;
					break;
				}
			}
			break;
		case 4:
				cout << "Введите номер звонка: ";
				cin >> PhoneNumber;
				IntCallPhone = DataWorkerClass::ConvertToInt(PhoneNumber);
				for (auto& item : records)
				{
					if (item.callNumber == IntCallPhone)
					{// нельзя звонить самому себе
						recordsForTask.push_back(item);
						IsCorrect = true;
						break;
					}
				}
			break;
		case 5:
			break;
		default:
			std::cout << "Неправильный выбор.\n";
			break;
		}
		if (recordsForTask.size() > 0)
		{
			ViewInfoForTask(recordsForTask);
		}
		else
		{
			std::cout << "По заданному параметру не найдено ни одного совпадения\n";
		}
	} while (choice != 5);
}
void IndividualTask::IndividualTasksInterface()
{
	vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::DataCallFile);
	vector<CallRecord> recordsForTask;
	std::cout << "Введите начальное время для фильтрации:\n";
	string startTime = DataWorkerClass::inputDate(); string endTime;
	if (!startTime.empty())
	{

		std::cout << "Введите конечное время для фильтрации:\n";
		endTime = DataWorkerClass::inputDate();
		if (!endTime.empty())
		{
			std::tm tm1 = {};
			std::tm tm2 = {};
			std::tm tm3 = {};

			std::istringstream ss1(startTime);
			std::istringstream ss2(endTime);

			// Парсинг строковых дат в структуру std::tm
			ss1 >> std::get_time(&tm1, "%d.%m.%Y");
			ss2 >> std::get_time(&tm2, "%d.%m.%Y");

			for (auto& item : records) {
				std::istringstream ss3(item.callDate);
				ss3 >> std::get_time(&tm3, "%d.%m.%Y");

				if (std::mktime(&tm1) >= std::mktime(&tm3) && std::mktime(&tm3) <= std::mktime(&tm1))
				{
					recordsForTask.push_back(item);
				}

			}
		}
	}
	if (recordsForTask.size() > 0)
	{
		//	ViewInfoForTask(recordsForTask);
		int choice = 0;
		do {
			std::cout << "Выберите опцию:\n";
			std::cout << "1. Перечень входящих и исходящих вызовов\n";
			std::cout << "2. Общее время входящих вызовов\n";
			std::cout << "3. Общее время исходящих вызовов\n";
			std::cout << "4. Общую сумму на исходящие вызовы\n";
			std::cout << "5. Назад\n";
			std::cin >> choice;
			int sum = 0; int sumTime = 0;
			vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::UsersFile);
			switch (choice) {
			case 1:
				std::cout << "Список звонков между " + startTime + " и " + endTime + "\n";
				ViewInfoForTask(recordsForTask);
				break;
			case 2:
				for (auto& item : recordsForTask) {
					if (item.isOutgoingCall)
					{
						sumTime += item.callDuration/60;
					}
				}
				std::cout << "Общее время входящих вызовов = " << sumTime << "мин" << '\n';
				break;
			case 3:
				for (auto& item : recordsForTask) {
					if (!item.isOutgoingCall)
					{
						sumTime += item.callDuration/60;
					}
				}
				std::cout << "Общее время исходящих вызовов = " << sumTime << "мин" << '\n';
				break;
			case 4:
				for (auto& item : recordsForTask) {
					sum += (item.callDuration/60) * item.tariffPerMinute;
				}
				std::cout << "Общая сумма на исходящие вызовы = " << sum << "руб" << '\n';				//	choice = 5;
				break;
			case 5:
				break;
			default:
				std::cout << "Неправильный выбор.\n";
				break;
			}
		} while (choice != 5);
	}
	else
	{
		std::cout << "По заданной выборке не найдено ни одного совпадения\n";
	}
};

bool compareByDuration(const CallRecord& record1, const CallRecord& record2) {
	return record1.callDuration < record2.callDuration;
}
bool compareByFIO(const CallRecord& record1, const CallRecord& record2) {
	return record1.subscriberName < record2.subscriberName;
}
bool compareByDate(const CallRecord& record1, const CallRecord& record2) {



	return record1.callDate < record2.callDate;
}



void IndividualTask::Sort()
{
	int choice = 0;
	do {
		vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::DataCallFile);
		//vector<CallRecord> recordsForTask;
		std::cout << "Выберите параметр для сортировки:\n";
		std::cout << "1. ФИО абонента\n";
		std::cout << "2. Дата звонка\n";
		std::cout << "3. Продолжительность\n";
		std::cout << "4. Назад\n";
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
			std::cout << "Неправильный выбор.\n";
			break;
		}


	} while (choice != 4);
}