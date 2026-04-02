#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <cctype>
#include "Enigma.hpp"
#include "Reflector.hpp"
#include "Rotor.hpp"
#include "Steckerbrett.hpp"

int Enigma::start() {
    std::array<Rotor, 3> rotors;
    std::array<int, 3> rotorsID = {0, 0, 0};
    std::cout << "Please, configure 3 rotors from '1' to '5'. Have to put different ones." << std::endl;

    if (!selectRotors(rotorsID)) {
        return EXIT_FAILURE;
    }
    for (auto i = 0; i < 3; i++) {   
        rotors[i].setRotor(rotorsID[i]);
    }
    std::cout << "Please, configure rings. Enter 3 alphabetic chars from 'A' to 'Z'. Upper or lowercase accepted." << std::endl;

    if (Rings(*ringSettings)) {
        return EXIT_FAILURE;
    }
    for (auto i = 0; i < 3; i++) {
        rotors[i].ring = ringSettings[0][i];
    }
    
    Steckerbrett Steckerbrett;
	char userSteckInput;
	//bool correctInput = false;
	while(true) {
		std::cout << "Configuration of the steckerbrett is optional. Press 'Y' or 'y' to adjust it. Press 'N' or 'n' to skip: ";
		std::cin >> userSteckInput;
		if (userSteckInput == 'Y' || userSteckInput == 'y') {
        	if (Steckerbrett.SteckerbrettConfiguration()) {
            	return EXIT_FAILURE;
        	}
			//correctInput = true;
			break;
		} else if (userSteckInput == 'n' || userSteckInput == 'N') {
			std::cout << "Continue without steckerbrett configuration." << std::endl;
			//correctInput = true;
			break;
		} else {
			std::cerr << "Misspelled. ";
		}
	}

	Reflector reflector;
    std::string message;
    int MovingsCount = 0;
    std::cout << "Please, enter the secret message: ";

	if (!(userSteckInput == 'y' || userSteckInput == 'Y')) {
		std::cin.ignore();
	}
	std::getline(std::cin, message);
    std::transform(message.begin(), message.end(), message.begin(), ::toupper);

	for (size_t i = 0; i < message.length(); ++i) {
    char& eachCharacter = message[i];  //ссылкa на символ по индексу i

    if (eachCharacter >= 'A' && eachCharacter <= 'Z') {
        encipher(rotors, reflector, Steckerbrett, eachCharacter, MovingsCount);
    	}
	}

    std::cout << "Your ciphered message is:  " << message << std::endl;
    std::ofstream result;
    result.open("SecretMessage.txt");
    result << message;
    result.close();
    return EXIT_SUCCESS;
}

bool Enigma::input(int choice) {
    switch(choice) {
		case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return false;  //ok
		default:
            std::cout << "Warning. Error! Incorrect input. You are allowed to enter only 3 rotors.\n";
			//std::cerr << "Error: incorrect input. You are allowed to enter only 3 rotors.\n";
            return true;   //error
    }
}

bool Enigma::searchCopies(const std::array<int, 3> &rotorsID)  //link to avoid copying
{
	if (rotorsID[0] == rotorsID[1] || rotorsID[1] == rotorsID[2] ||	rotorsID[0] == rotorsID[2]) {
		std::cout << "Warning. Error! Have been found similar rotors. Have to put different ones.\n";
		//throw std::invalid_argument("Error: there have been foun duplicates.");
		return true;
	}
		return false;
}

int Enigma::char_to_int(char c) { //ASCII code conversion
	return c - '0';
}

bool Enigma::Rings(std::string &ringSettings) {
	std::cin >> ringSettings;

	std::transform(ringSettings.begin(), ringSettings.end(), ringSettings.begin(), ::toupper); //to make not case sensitive

	const size_t ringNumber = 3;

	if (ringSettings.empty()) {
        std::cout << "Warning. Error! No ring settings. Essential to configure rings." << std::endl;
        return true;
    }
	if (ringSettings.length() != ringNumber) {
		std::cout << "Warning. Error! Expected " << ringNumber<< " ring settings, but entered " << ringSettings.length() << "." << std::endl;
		return true;
	}
	for (size_t i = 0; i < ringSettings.length(); ++i) {
        char c = ringSettings[i];
        if (!std::isalpha(c) || !std::isupper(c)) {
            std::cout << "Warning. Error! Character at position " << (i + 1) << " ('" << c << "') is not correct alphabetic character (A-Z)." << std::endl;
            return true;
        }
    }
    return false;
}

void Enigma::encipher(std::array<Rotor, 3> &rotors, Reflector &reflector, Steckerbrett &Steckerbrett, char &eachCharacter, int &MovingsCount)
{
    Steckerbrett.substitute(eachCharacter); //step1: substitute through Steckerbrett

    advanceRotors(rotors, MovingsCount); //step2: rotation of rotors

    applyRingSettings(rotors); //step3

    forwardPassThroughRotors(rotors, eachCharacter); //step4: right to left pass ('s' aka straight)

    reflector.reflectorSubstitute(eachCharacter); //step5

    reversePassThroughRotors(rotors, eachCharacter); //step6: reversed pass left to right  ('r' aka reversed)

    Steckerbrett.substitute(eachCharacter); //step7: final substitution through Steckerbrett
}

void Enigma::advanceRotors(std::array<Rotor, 3> &rotors, int &MovingsCount)
{
    constexpr int ROTOR_2_STEP = 26;
    constexpr int ROTOR_1_STEP = 676; //26*26

    rotors[2].offset();
    if (MovingsCount % ROTOR_2_STEP == 0 && MovingsCount > 0) {
        rotors[1].offset();
    }

    if (MovingsCount % ROTOR_1_STEP == 0 && MovingsCount > 0) {
        rotors[0].offset();
    }
    ++MovingsCount;
}

void Enigma::applyRingSettings(std::array<Rotor, 3> &rotors) {
    /*for (auto i = 0; i < 3; i++) {
		rotors[i].ringApply(rotors[i].ring);
	}*/
	for (auto& rotor : rotors) {
        rotor.ringApply(rotor.ring);
    }
}

void Enigma::forwardPassThroughRotors(std::array<Rotor, 3> &rotors, char &character) {
    for (int i = 2; i >= 0; --i) {
        rotors[i].substitute(character, 's');
    }
}

void Enigma::reversePassThroughRotors(std::array<Rotor, 3> &rotors, char &character) //signal reverse through the rotor, after reflection in the reflector
{
	/*for (int i = 0; i < 3; ++i) {
		rotors[i].substitute(character, 'r');
	}*/
    for (auto& rotor : rotors) {
        rotor.substitute(character, 'r');
    }
}

//bool Enigma::isValidRingSetting(const std::string& settings)

/*std::cout << "Third rotor: ";
	std::cin >> buffer;
	rotorsID[2] = char_to_int(buffer);
	if (searchCopies(rotorsID) || input(rotorsID[2]))
	{
		return EXIT_FAILURE;
	}*/

bool Enigma::selectRotors(std::array<int, 3> &rotorsID) {
    const char* rotorName[3] = {"First", "Second", "Third"};
    char buffer;
    
    for (int i = 0; i < 3; i++) {
        std::cout << rotorName[i] << " rotor: ";
        std::cin >> buffer;
        rotorsID[i] = char_to_int(buffer);
        
        if (i == 0) { //if it is the first rotor
            if (input(rotorsID[i])) {
                return false; //error
            }
        }
        else { //if it is the 2nd and 3rd rotrs
            if (searchCopies(rotorsID) || input(rotorsID[i])) {
                return false; //error
            }
        }
    }
    return true; //ok
}