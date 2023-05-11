#include "IndividualTask.h"

void IndividualTask::Surt(string filename)
{
	

}


void IndividualTask::IndividualTasksInterface()
{
	int choice = 0;
	do {
		std::cout << "Выберите опцию:\n";
		std::cout << "1. Вывести информацию по каждому абоненту за требуемый период времени\n";
		std::cout << "2. Перечень входящих и исходящих вызовов\n";
		std::cout << "3. Общее время входящих вызовов\n";
		std::cout << "4. Общее время исходящих вызовов\n";
		std::cout << "5. Общую сумму на исходящие вызовы\n";
		std::cout << "6. Назад\n";

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
			std::cout << "Неправильный выбор.\n";
			break;
		}
	} while (choice != 4);


}
