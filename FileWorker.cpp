#include "FileWorker.h"
using namespace std;
class FileWorker {
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
	static std::vector<User> getUserArray(std::string filename)
	{// �� ������� - �� ��������� ����� ������ �� ���������� �����
		std::ifstream file(filename);
		const int MAX_RECORDS = 100; // ������������ ���������� �������
		User recordsArray[MAX_RECORDS]; // ������ ��������
		if (file.is_open()) {
			std::string line;
			int index = 0;
			while (std::getline(file, line) && index < MAX_RECORDS) {
				User record;
				std::istringstream iss(line);
				iss >> record.login >> record.role >>
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
	static void updateUsersFile(vector<User> users, const std::string& filename, string Operation)
	{ //����������� ���������
		std::ofstream outFile(filename, std::ios_base::out | std::ios_base::binary);
		if (!outFile.is_open())
		{
			std::cout << "�� ������� ������� ���� ��� ������.\n";
			return;
		}
		for (const auto& user : users)
		{
			outFile << user.login << " " << user.role << " " << user.salt << " " << user.saltedHashPassword << " " << user.subscriberName << " " << user.subscriberNumber << std::endl;
		}
		if (Operation == "del")
		{
			std::cout << "������ �������\n";
		}
		else if (Operation == "upd")
		{
			std::cout << "������ ��������\n";
		}
		outFile.close();
	}
	static void createUser(const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::app | std::ios_base::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file " << filename << std::endl;
			return;
		}
		bool isLoginCorrect = false;
		string login;
		vector<User> records = FileWorker::getUserArray(filename);
		while (!isLoginCorrect) {
			cout << "������� �����: ";
			cin >> login;
			if (records.size() > 0)
			{
				for (auto& item : records) {
					if (item.login == login) {
						cout << "����� ����� ��� ����������." << endl;
						isLoginCorrect = false;
						break;
					}
					else
						isLoginCorrect = true;
				}
			}
			else
				break;
		}
		string password;
		cout << "������� ������:\n";
		cin >> password;
		string salt = CryptoClass::generate_salt(16);
		string passwordA = password + salt;
		auto hashPassword = CryptoClass::hashPasswordWithSaltMethod(password, salt);
		int role;
		do
		{
			cout << "������� ����(0 - ������������, 1 - �������������) \n";
			cin >> role;
			if (role != 1 && role != 0)
				cout << "�������� 0 ��� 1\n";
			else
				break;
		} while (true);
		string FIO = getFullFIO();
		int phoneNumber = 0;
		std::srand(std::time(nullptr)); // ����� ������������ �������
		int subscriber_numer = rand() % 1000000000;
		//file._setmode(_fileno(stdout), _O_WTEXT);
		file << login << " "
			<< role << " "
			<< salt << " "
			<< hashPassword << " "
			<< FIO << " "
			<< to_string(subscriber_numer) << std::endl;
		file.close();
		if (role == 1)
		{
			cout << "������������� ��������\n";
		}
		else
			cout << "������������ ��������\n";
	}
	static string changeParametr(string parametr)
	{
		string newData;
		std::cout << "������� ����� �������� ��� " + parametr;
		std::cin >> newData;
		//�������� ����� �������
		return newData;
	}
	static string getFullFIO() // ���� ������� ���
	{
		std::string firstName, lastName, middleName;
		std::cout << "������� �������: ";
		std::cin >> lastName;
		std::cout << "������� ���: ";
		std::cin >> firstName;
		std::cout << "������� ��������: ";
		std::cin >> middleName;
		std::string fullName;
		if (middleName.empty()) {
			fullName = lastName + "_" + firstName;
		}
		else {
			fullName = lastName + "_" + firstName + "_" + middleName;
		}
		if (fullName[0] == '\n')
			fullName.erase(0, 1);
		if (fullName[fullName.size()] == '\n')
			fullName.erase(fullName.size() - 1, 1);
		std::cout << "������ ���: " << fullName << std::endl;
		return fullName;
	}
	static void updateUser(string filename) // �������������� ����� � ��������������
	{
		string login;
		std::cout << "������� ����� ������������ ��� ������:\n";
		std::cin >> login;
		vector<User> records = FileWorker::getUserArray(filename);
		bool isFounded = false;
		for (auto& item : records) {
			if (item.login == login)
			{
				isFounded = true;
				int choice = 0;
				do {
					std::cout << "��� �� ������ �������������?:\n";
					std::cout << "1. �����\n";
					std::cout << "2. ������\n";
					std::cout << "3. ����� ��������\n";
					std::cout << "4. ���\n";
					std::cout << "5. �����\n";
					std::cin >> choice;
					std::string firstName, lastName, middleName;
					std::stringstream fullNameStream;
					string newParam;
					bool IsCorrect = true;
					switch (choice) {
					case 1:
						do {
							newParam = changeParametr("������:\n");
							for (auto& item : records) {
								if (item.login == newParam)
								{
									std::cout << "����� ����� ��� ����������, ���������� ������:\n";
									IsCorrect = false; break;
								}
								else
									IsCorrect = true;
							}
							//item.login = newParam;
						} while (IsCorrect == false);
						item.login = newParam;
						choice = 5;
						break;
					case 2:
						newParam = changeParametr("������:\n");
						item.saltedHashPassword = CryptoClass::hashPasswordWithSaltMethod(newParam, item.salt);
						choice = 5;
						break;
					case 3:
						do {
							newParam = changeParametr("������ ��������:\n");
							for (char c : newParam) {
								if (!isdigit(c)) {
									IsCorrect = false;
									std::cout << "� ������ �������� ������ ���� ������ �����:\n";
									break;
								}
								else
								{
									IsCorrect = true;
								}
							}
						} while (IsCorrect == false);
						item.subscriberNumber = stoi(newParam);
						choice = 5;
						break;
					case 4:
						item.subscriberName = getFullFIO();
						choice = 5;
						break;
					case 5:
						break;
					default:
						std::cout << "������������ �����.\n";
						break;
					}
				} while (choice != 5);
				updateUsersFile(records, filename, "upd");
				break;
				//������ � ���� ���� ������ ������� ��������
			}
		}
		if (!isFounded)
		{
			std::cout << "��� ������ ������ � �������:\n";
		}
	}
	static void deleteUser(string filename)
	{
		string login;
		std::cout << "������� ����� ������������ ��� ��������:\n";
		std::cin >> login;
		vector<User> records = FileWorker::getUserArray(filename);
		bool isFounded = false;
		for (int i = 0; i < records.size(); i++) {
			if (records[i].login == login) {
				isFounded = true;
				//std::remove_if(records.begin(), records.end(), records[i]), records.end();
				records.erase(records.begin() + i);
				break;
			}
		}
		if (!isFounded)
		{
			std::cout << "��� ������ ������ � �������:\n";
		}
		else
		{
			updateUsersFile(records, filename, "del");
		}
	}
	static void viewAllUsers(string filename) // �������� ���� �������������
	{
		setlocale(LC_CTYPE, "rus");
		vector<User> records = FileWorker::getUserArray(filename);
		std::cout << std::setw(15) << std::left << "| �����"
			<< std::setw(30) << std::left << "| ���"
			<< std::setw(20) << std::left << "| ����� ��������" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
		//SetConsoleOutputCP(CP_UTF8);
		for (auto& item : records) {
			std::cout << std::setw(15) << std::left << "| " + item.login
				<< std::setw(30) << std::left << "| " + item.subscriberName
				<< std::setw(20) << std::left << "| " + to_string(item.subscriberNumber) + "" << std::endl;
			std::cout << "---------------------------------------------------------------" << std::endl;
		}
	}
	static void findForLoginViewUser(string filename) // ����� ���������� �� ������������ ����� ��� �����
	{
		string login;
		std::cout << "������� ����� ������������ ��� ������:\n";
		std::cin >> login;
		vector<User> records = FileWorker::getUserArray(filename);
		bool isFounded = false;
		for (auto& item : records) {
			if (item.login == login)
			{
				std::cout << "����� ������������ ������\n";
				std::cout << std::setw(15) << std::left << "| �����"
					<< std::setw(30) << std::left << "| ���"
					<< std::setw(20) << std::left << "| ����� ��������" << std::endl;
				std::cout << "---------------------------------------------------------------" << std::endl;
				std::cout << std::setw(15) << std::left << "| " + item.login
					<< std::setw(30) << std::left << "| " + item.subscriberName
					<< std::setw(20) << std::left << "| " + to_string(item.subscriberNumber) + "" << std::endl;
				std::cout << "---------------------------------------------------------------" << std::endl;
				isFounded = true;
			}
		}
		if (!isFounded)
			std::cout << "� ������� ��� ������������ � ����� �������\n";
	}
};