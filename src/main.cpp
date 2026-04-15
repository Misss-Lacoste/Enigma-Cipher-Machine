#include <iostream>
#include <stdlib.h>
//#include <chrono>
//#include <thread>
#include <windows.h>
#include "Enigma.hpp"
#include <fstream>

void showEnigmaInfo() {
    std::ifstream file("enigma_info_eng.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
	file.close();

    std::cout << "Press 'Enter' to go back to the main menu...";
    //std::cin.ignore();
    std::cin.get();
}

void showAttackMethods() {
	std::ifstream file("crib_attack.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
    file.close();

    std::cout << "Press 'Enter' to go back to the main menu...";
    //std::cin.ignore();
    std::cin.get();
}

void Help() {
	std::ifstream file("help.txt");
	std::string line;

	while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
	file.close();

    std::cout << "Press 'Enter' to go back to the main menu...";
    //std::cin.ignore();
    std::cin.get();
}

void showMenu() {
	std::cout << "\n*********************************************\n";
	std::cout << "Enigma Options.\n";                           
	std::cout << "1. Encrypt the message.\n";
	std::cout << "2. Decrypt the message.\n";
	std::cout << "3. Look at the Enigma Historical Information.\n";
	std::cout << "4. Study attack methods.\n";
	std::cout << "5. Help.\n";
	std::cout << "6. Exit.\n";
	std::cout << "*********************************************\n";
}

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
	amazingEffect("Welcome to Enigma Cipher machine!");
	amazingEffect("Enigma configured successfully!");

	Enigma enigma;
	int choice;

	do {
		showMenu();
		std::cin >> choice;
		std::cin.ignore();

		switch (choice) {
			case 1: {
			char UserInput;
			do {
				enigma.start();
				std::cout << "To continue encrypting press 'Y', to stop press 'N': ";
				std::cin >> UserInput; //
				std::cin.ignore();
				std::cout << std::endl;
			} while (UserInput == 'Y' || UserInput == 'y');
				break;
		}
		case 2: {
			std::cout << "Decryption is still coding.\n";
			std::cout << "Press 'Enter' to go back to the main menu...";
			std::cin.get();
			break;
		}
		case 3: {
			showEnigmaInfo();
			break;
		}
		case 4: {
			showAttackMethods();
			break;
		}
		case 5: {
			Help();
			break;
		}
		case 6: {
			amazingEffect("See you later!");
			break;
		}
		default: {
			std::cout << "Unfortunately, you misspelled. Enter option point again.\n";
			break;
		}
	}
} while (choice != 6);

amazingEffect("Press 'Enter' to stop the Machine.");
std::cin.get();

	return EXIT_SUCCESS;
}
