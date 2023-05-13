#include "FileWorker.h"

//#include "DataWorkerClass.cpp"

	 void FileWorker::checkFile(string fileUser, string fileInfo) {
		ifstream iff(fileUser, ios::out);
		if (iff.good() == false)
		{
			//����������� ����� ���� ���������� � ������ ���������
			std::ofstream file(fileUser);
			file << 1 << " " << "admin" << " " << 1 << " "
				<< "AHgXRGU55gKOnKHw" << " "
				<< "bfknefpkckfjbcpoilinaeeblmkfggbpcnccakmdcilcfeboijmehigigooecbap" << " " << "�����_���������_���������" << " "
				<< "59741426" << std::endl;
		}
		ifstream iffs(fileInfo, ios::out);
		if (iffs.good() == false)
		{

			std::ofstream file(fileInfo);

		}

	}
	 void FileWorker::updateUsersFile(vector<User> users, const std::string& filename, string Operation)
	{ //����������� ���������
		std::ofstream outFile(filename, std::ios_base::out | std::ios_base::binary);
		if (!outFile.is_open())
		{
			std::cout << "�� ������� ������� ���� ��� ������.\n";
			return;
		}
		int newIndex = 0;
		for (const auto& user : users)
		{
			newIndex++;
			outFile << newIndex << " " << user.login << " " << user.role << " " << user.salt << " " << user.saltedHashPassword << " " << user.subscriberName << " " << user.subscriberNumber << std::endl;
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
	 int FileWorker::generateMobileNumber()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> digitDist(0, 9);
		string mobileNumber;
		for (int i = 0; i < 8; ++i) {
			int digit = digitDist(gen);
			mobileNumber += to_string(digit);
		}
		return stoi(mobileNumber);
	}
	 void FileWorker::createUser(const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::app | std::ios_base::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening file " << filename << std::endl;
			return;
		}
		bool isLoginCorrect = false;
		string login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
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
		int subscriber_numer = generateMobileNumber();
		//file._setmode(_fileno(stdout), _O_WTEXT);
		file << records.size() + 1 << " " << login << " " << role << " "
			<< salt << " "<< hashPassword << " " << FIO << " "
			<< to_string(subscriber_numer) << std::endl;
		file.close();
		if (role == 1)
			cout << "������������� ��������\n";
		else
			cout << "������������ ��������\n";
	}
	 string FileWorker::changeParametr(string parametr)
	{
		string newData;
		std::cout << "������� ����� �������� ��� " + parametr;
		std::cin >> newData;
		return newData;
	}
	 string FileWorker::getFullFIO() // ���� ������� ���
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
	 void FileWorker::ChangeFIO(string filename, string oldFIO, string newFIO)
	{
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		for (auto& item : records) {
			if (item.subscriberName == oldFIO)
			{
				item.subscriberName = newFIO;
			}
		}
		updateDataFile(records, filename, " ");
	}	
	 void FileWorker::ChangeNumber(string filename, string FIO, int newPhone, int oldPhone)
	{
		vector<CallRecord> records = ReadFromFileClass::getDataArray(filename);
		for (auto& item : records) {
			if (item.subscriberName == FIO) // ������ ��������� ������
			{
				item.subscriberNumber = newPhone;
			}
			if (item.callNumber == oldPhone) // ������ �������� ������
			{
				item.callNumber == newPhone;
			}
		}
		updateDataFile(records, filename, " ");
	}
	 void FileWorker::updateDataFile(vector<CallRecord> data, const std::string& filename, string Operation)
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
		outFile.close();
	}
	 void FileWorker::updateUser(string filename) // �������������� ����� � ��������������
	{
		string login;
		std::cout << "������� ����� ������������ ��� ������:\n";
		std::cin >> login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
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
					string OldFIO;	string newParam;
					int oldPhone;
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
							newParam = changeParametr("������ �������� (8 ��������):\n");
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
							//�������� ������������ ����� ��������
							if (newParam.size() != 8)
							{
								IsCorrect = false;
								std::cout << "� ������ �������� ������ ���� 8 ����:\n";
							}
							else
							{
								for (auto& item : records) {
									if (item.subscriberNumber == stoi(newParam))
									{
										IsCorrect = false;
										std::cout << "����� ����� �������� ��� ����������:\n";
										break;
									}
								}
							}
						} while (IsCorrect == false);
						oldPhone = item.subscriberNumber;
						item.subscriberNumber = stoi(newParam); 
						//�������� ���� ����� �������� � ����� �� ��������
						ChangeNumber(GlobalVariablesClass::DataCallFile, item.subscriberName, item.subscriberNumber, oldPhone);

						choice = 5;
						break;
					case 4:
						OldFIO = item.subscriberName;
						item.subscriberName = getFullFIO();
						// ����� ������ ��� � ����� � �������� �������
						ChangeFIO(GlobalVariablesClass::DataCallFile, OldFIO, item.subscriberName);
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
	 void FileWorker::deleteUser(string filename, string username)
	{
		string login;
		std::cout << "������� ����� ������������ ��� ��������:\n";
		std::cin >> login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
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
			if (login == username)
			{
				std::cout << "���������� ������� ����������� ������� ������\n";
			}
			else
			{
				std::string answer;
				while (true) {
					std::cout << "�� ������������� ������ ������� ������� ������������? (��/���): ";
					std::cin >> answer;
					if (answer == "��" || answer == "��" || answer == "��") {
						// ���������� �������� �������� ����� ��� ������
						updateUsersFile(records, filename, "del");
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
			}
				
		}
	}
	 void FileWorker::viewAllUsers(string filename) // �������� ���� �������������
	{
		setlocale(LC_CTYPE, "rus");
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		std::cout << std::setw(4) << std::left << "| �"
			<< std::setw(15) << std::left << "| �����"
			<< std::setw(30) << std::left << "| ���"
			<< std::setw(20) << std::left << "| ����� ��������" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
		//SetConsoleOutputCP(CP_UTF8);
		for (auto& item : records) {
			std::cout << std::setw(4) << std::left << "| " + std::to_string(item.key)
				<< setw(15) << std::left << "| " + item.login
				<< std::setw(30) << std::left << "| " + item.subscriberName
				<< std::setw(20) << std::left << "| " + to_string(item.subscriberNumber) + "" << std::endl;
			std::cout << "---------------------------------------------------------------" << std::endl;
		}
	}
	 void FileWorker::findForLoginViewUser(string filename) // ����� ���������� �� ������������ ����� ��� �����
	{
		string login;
		std::cout << "������� ����� ������������ ��� ������:\n";
		std::cin >> login;
		vector<User> records = ReadFromFileClass::getUserArray(filename);
		bool isFounded = false;
		for (auto& item : records) {
			if (item.login == login)
			{
				std::cout << std::setw(4) << std::left << "| �"
					<< std::setw(15) << std::left << "| �����"
					<< std::setw(30) << std::left << "| ���"
					<< std::setw(20) << std::left << "| ����� ��������" << std::endl;
				std::cout << "---------------------------------------------------------------" << std::endl;
				//SetConsoleOutputCP(CP_UTF8);
					std::cout << std::setw(4) << std::left << "| " + std::to_string(item.key)
						<< setw(15) << std::left << "| " + item.login
						<< std::setw(30) << std::left << "| " + item.subscriberName
						<< std::setw(20) << std::left << "| " + to_string(item.subscriberNumber) + "" << std::endl;
					std::cout << "---------------------------------------------------------------" << std::endl;
				isFounded = true;
			}
		}
		if (!isFounded)
			std::cout << "� ������� ��� ������������ � ����� �������\n";
	}
