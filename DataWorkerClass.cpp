#include "DataWorkerClass.h"

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
		sscanf(callDate.c_str(), "%d-%d-%d", &year, &month, &day);
		sscanf(callTime.c_str(), "%d:%d", &hour, &minute);

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
		vector<User> recordsUsers = FileWorker::getUserArray(UsersFile);
		User user; // Пользователь для которого создаём запись
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
		cout << "Звонок пользователя исходящий (0) или входящий ()? ";







		//получение текущего времени
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d.%m.%Y %H:%M:%S");
		std::string current_datetime = oss.str();
		double tariffPerMinute = getCallTariff(current_datetime); // определение тарифа
		std::string callDate = current_datetime.substr(0, 10); // Дата вызова
		std::string callTime = current_datetime.substr(11, 8); // Время вызова

		int phoneNumber = 0;
		std::srand(std::time(nullptr)); // вызов относительно времени
		int subscriber_numer = rand() % 1000000000;
		//file._setmode(_fileno(stdout), _O_WTEXT);\
		// Ввод данных 
	/*	file << login << " "
			<< role << " "
			<< salt << " "
			<< hashPassword << " "
			<< FIO << " "
			<< to_string(subscriber_numer) << std::endl;*/
		file.close();
		
	}
};
