#include "IndividualTask.h"

void IndividualTask::Surt(string filename)
{
	

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


}
