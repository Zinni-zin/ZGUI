#include <iostream>

#include "Application.h"

int main()
{
	Application& app = Application::GetInstance();

	if (app.Init() != 0)
	{
		std::cout << "Error Iniitalzing Application!\n";
		return -1;
	}
	
	app.Run();
	return 0;
}