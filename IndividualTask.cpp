#include "IndividualTask.h"

void IndividualTask::Surt(string filename)
{
	

}

void ShowInformationOnTime(string choose)
{
	vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::UsersFile);
	if (choose == "time")
	{






	}
	else if (choose == "date")
	{





	}
}

void ChooseTime() // 
{
	int choice = 0;
	do {
		std::cout << "�������� ��������:\n";
		std::cout << "1. ����� ���������� �� �������\n";
		std::cout << "2. ����� ���������� �� ����\n";
		std::cout << "3. �����\n";
		std::cin >> choice;
		switch (choice) {
		case 1:
			ShowInformationOnTime("time");
			break;
		case 2:
			ShowInformationOnTime("date");
			break;
		case 3:

			break;
		default:
			std::cout << "������������ �����.\n";
			break;
		}
	} while (choice != 3);
}

void IndividualTask::IndividualTasksInterface()
{
	int choice = 0;
	do {
		std::cout << "�������� �����:\n";
		std::cout << "1. ������� ���������� �� ������� �������� �� ��������� ������ �������\n";
		std::cout << "2. �������� �������� � ��������� �������\n";
		std::cout << "3. ����� ����� �������� �������\n";
		std::cout << "4. ����� ����� ��������� �������\n";
		std::cout << "5. ����� ����� �� ��������� ������\n";
		std::cout << "6. �����\n";

		std::cin >> choice;
		vector<CallRecord> records = ReadFromFileClass::getDataArray(GlobalVariablesClass::UsersFile);

		switch (choice) {
		case 1:
			
			break;
		case 2:
			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		default:
			std::cout << "������������ �����.\n";
			break;
		}
	} while (choice != 4);


};

