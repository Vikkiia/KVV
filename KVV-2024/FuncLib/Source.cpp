
#include <iostream>
#include <ctime>

extern "C"
{

	unsigned int toPow(unsigned int a, unsigned int b)
	{

		return pow(b, a);
	}
	unsigned int random(unsigned int a)
	{
		
		if (a == 0)
			return 0;
		srand(time(NULL));
		int k =  (rand() % (a * 2));
		return k;
	}
	
	int  writenum(unsigned int value)
	{
		std::cout << value;
		return 0;
	}
	int  writestr(char* ptr)
	{
		setlocale(0,"");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		return 0;
	}
	unsigned int  writenumline(unsigned int value)
	{
		
		std::cout << value << std::endl;
		return 0;
	}
	unsigned int  writestrline(char* ptr)
	{
		setlocale(0, "");
		
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		std::cout << std::endl;
		return 0;
	}
	int  system_pause()
	{
		system("pause");
		return 0;
	}
}