#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <cctype>
#include "Enigma.hpp"
#include "Reflector.hpp"
#include "Rotor.hpp"
#include "Plugboard.hpp"

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

bool Enigma::searchCopies(const std::array<int, 3>& rotorsID) //link to avoid copying
{
	if (rotorsID[0] == rotorsID[1] || rotorsID[1] == rotorsID[2] ||	rotorsID[0] == rotorsID[2])
	{
		std::cout << "Warning. Error! Has been found similar rotors. Have to put different ones.\n";
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
        std::cout << "Warning. Error! No ring settings. Essential to set up rings." << std::endl;
        return true;
    }
	if (ringSettings.length() != ringNumber) {
		std::cout << "Warning. Error! Expected " << ringNumber<< " ring settings, but entered " << ringSettings.length() << "." << std::endl;
		return true;
	}
	for (size_t i = 0; i < ringSettings.length(); ++i) {
        char c = ringSettings[i];
        if (!std::isalpha(c) || !std::isupper(c)) {
            std::cout << "Warning. Error! Character at position " << (i + 1) << " ('" << c << "') is not a valid letter (A-Z)." << std::endl;
            return true;
        }
    }
    return false;
}

void Enigma::encipher(std::array<Rotor, 3> &rotors, Reflector &reflector, Plugboard &plugboard, char &eachCharacter, int &MovingsCount)
{
    plugboard.substitute(eachCharacter); //step1: substitute through plugboard

    advanceRotors(rotors, MovingsCount); //step2: rotation of rotors

    applyRingSettings(rotors); //step3

    forwardPassThroughRotors(rotors, eachCharacter); //step4: right to left pass ('s' aka straight)

    reflector.reflectorSubstitute(eachCharacter); //step5

    reversePassThroughRotors(rotors, eachCharacter); //step6: reversed pass left to right  ('r' aka reversed)

    plugboard.substitute(eachCharacter); //step7: final substitution through plugboard
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

int Enigma::start()
{
	std::array<Rotor, 3> rotors;
	std::array<int, 3> rotorsID = {0, 0, 0};
	char buffer = '0'; 

	std::cout << "Set up 3 rotors. Choose from 1 to 5. Repeats restricted" << std::endl;

	std::cout << "First rotor: ";
	std::cin >> buffer;
	rotorsID[0] = char_to_int(buffer);
	if (input(rotorsID[0]))
	{
		return EXIT_FAILURE;
	}
	std::cout << "Second rotor: ";
	std::cin >> buffer;
	rotorsID[1] = char_to_int(buffer);
	if (searchCopies(rotorsID) || input(rotorsID[1]))
	{
		return EXIT_FAILURE;
	}
	std::cout << "Third rotor: ";
	std::cin >> buffer;
	rotorsID[2] = char_to_int(buffer);
	if (searchCopies(rotorsID) || input(rotorsID[2]))
	{
		return EXIT_FAILURE;
	}
	for (auto i = 0; i < 3; i++)
	{	
		rotors[i].setRotor(rotorsID[i]);
	}

	std::cout << "Set up rings. Enter 3 characters from A to Z. Not case sensitive" << std::endl;
	if (Rings(*ringSettings))
	{
		return EXIT_FAILURE;
	}
	for (auto i = 0; i < 3; i++)
	{
		rotors[i].ring = ringSettings[0][i];
	}

	Plugboard plugboard;
	char plugboardOption;
	std::cout << "Would you like to set up plugboard? Y/y to set up or any other character to skip: ";
	std::cin >> plugboardOption;
	if (plugboardOption == 'y' || plugboardOption == 'Y')
	{
		if (plugboard.setPlugboard())
		{
			return EXIT_FAILURE;
		}
	}
 
	Reflector reflector;
	std::string message;
	int MovingsCount = 0;
	std::cout << "Type your message: ";
	if (!(plugboardOption == 'y' || plugboardOption == 'Y')) std::cin.ignore();
	std::getline(std::cin, message);
	std::transform(message.begin(), message.end(), message.begin(), ::toupper);
	for (auto &eachCharacter : message)
	{
		if (eachCharacter > 64 && eachCharacter < 91)	// 65 == 'A', 90 == 'Z'
		{
			encipher(rotors, reflector, plugboard, eachCharacter, MovingsCount);
		}
	}
	std::cout << "Ciphered message:  " << message << std::endl;
	std::ofstream result;
	result.open("result.txt");
	result << message;
	result.close();

	return EXIT_SUCCESS;
}
