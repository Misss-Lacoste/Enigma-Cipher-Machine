#include <iostream>
#include <stdlib.h>
//#include <chrono>
//#include <thread>
#include <windows.h>
#include "Enigma.hpp"

void amazingEffect(const std::string &text, int sleepingTime = 50) {
    for (auto i = 0; i < text.length(); i++) {
        std::cout << text[i] << "|";
		Sleep(sleepingTime / 2);  //cursor wink
        Sleep(sleepingTime);
		std::cout << "\b ";  //space washes the cursor
        Sleep(sleepingTime / 2);
    }
    std::cout << std::endl;
}

int main() {
	//system("Color E4");
	//добавить логгирование потом
	//цвет ?
	amazingEffect("Welcome to Enigma Cipher machine!");
	amazingEffect("Enigma configured successfully!");

	Enigma enigma;
	char UserInput;

	do
	{
		enigma.start();
		std::cout << "To continue encrypting press 'Y', to stop press 'N': ";
		std::cin >> UserInput;
		//std::cin.ignore();
		std::cout << std::endl;
	} while (UserInput == 'Y' || UserInput == 'y');
	
	amazingEffect("Press 'Enter' to stop the Machine.");
	std::cin.ignore();
	std::cin.get();

	return EXIT_SUCCESS;
}
