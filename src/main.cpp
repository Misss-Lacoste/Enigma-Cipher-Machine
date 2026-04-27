#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include "Enigma.hpp"
#include <fstream>
#include "Crypto.hpp"
#include <limits>

void showEnigmaInfo() {
    std::ifstream file("../data/enigma_info_eng.txt");
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
	std::ifstream file("../data/crib_attack_eng.txt");
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
	std::ifstream file("../data/help.txt");
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
	std::cout << "\n--------------------------------------------\n";
	std::cout << "Enigma Options.\n";                           
	std::cout << "1. Ciphering.\n";
	std::cout << "2. Look at the Enigma Historical Information.\n";
	std::cout << "3. Study attack methods.\n";
	std::cout << "4. Help.\n";
	std::cout << "5. Cryptoanalysis Module.\n";
	std::cout << "6. Exit.\n";
	std::cout << "--------------------------------------------\n";
}
//cursor winks
void sleep_ms(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void amazingEffect(const std::string &text) {
    for (char c : text) {
        std::cout << c;
        std::cout.flush(); //out symbol
        sleep_ms(30); //cursor winks
    }
    std::cout << std::endl;
}

void runCryptoAnalysisModule() {
    std::cout << "Cryptoanalysis Module.\n";
    int subChoice;
    std::cout << "1. Frequency Analysis.\n";
    std::cout << "2. Check No-Self-Mapping Property.\n";
    std::cout << "3. Find Crib Positions.\n";
    std::cout << "Enter choice: ";
    std::cin >> subChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string ciphertext, crib, plaintext;

    switch (subChoice) {
        case 1:
            std::cout << "Enter ciphertext: ";
            std::getline(std::cin, ciphertext);
            CryptoAnalyzer::performFrequencyAnalysis(ciphertext);
            break;
        case 2:
            std::cout << "Enter plaintext: ";
            std::getline(std::cin, plaintext);
            std::cout << "Enter ciphertext: ";
            std::getline(std::cin, ciphertext);
            if (CryptoAnalyzer::checkNoSelfMapping(plaintext, ciphertext)) {
                std::cout << "Property hold: valid Enigma output.\n";
            } else {
                std::cout << "Property violated: not a correct crib.\n";
            }
            break;
        case 3:
            std::cout << "Enter ciphertext: ";
            std::getline(std::cin, ciphertext);
            std::cout << "Enter crib: ";
            std::getline(std::cin, crib);
            CryptoAnalyzer::findCribPositions(ciphertext, crib);
            break;
        default:
            std::cout << "Invalid choice.\n";
    }
    
    std::cout << "\nPress 'Enter' to go back to the main menu...";
    std::cin.get();
}

int main() {
	amazingEffect("Welcome to Enigma Cipher Machine!");
	amazingEffect("Enigma configured successfully!");

	Enigma enigma;
	int choice = 0;

	do {
		showMenu();
		if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //deletes wrong symbol
            std::cout << "\nError: please, enter a valid number 1 - 6.\n";
            continue; //go to new iteration(menu again)
        }
        //clear stream until the right number will be entered
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		switch (choice) {
			case 1: {
			char UserInput;
			do {
				enigma.start();
				std::cout << "To continue encrypting press 'Y', to stop press 'N': ";
				std::cin >> UserInput;
				std::cout << std::endl;
			} while (UserInput == 'Y' || UserInput == 'y');
				break;
		}
		case 2: {
			showEnigmaInfo();
			break;
		}
		case 3: {
			showAttackMethods();
			break;
		}
		case 4: {
			Help();
			break;
		}
		case 5: {
			runCryptoAnalysisModule();
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
