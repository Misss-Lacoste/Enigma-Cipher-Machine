#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include "Enigma.hpp"
#include "Plugboard.hpp"
#include "Reflector.hpp"
#include "Rotor.hpp"

bool Enigma::ChecksforDuplicates(std::array<int, 3> rotorIds)
{
	if (rotorIds[0] == rotorIds[1] || rotorIds[1] == rotorIds[2] ||	rotorIds[0] == rotorIds[2])
	{
        //throw std::invalid_argument("Error: there have been foun duplicates.");
        //throw RotorConfigurationError("Error: duplicate rotor indexes found");
        std::cout << "Error: duplicate rotor indexes have been found" << std::endl;
		return true;
	}
		return false;
}

bool Enigma::Input(int choice) {
    switch (choice) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return false;  //ok
        default:
            std::cerr << "Error: incorrect input. You are allowed to enter only 3 rotors.\n";
            return true;   //error
    }
}


int Enigma::atoi(char charToInteger)
{
	return static_cast<int>(charToInteger) - 48;
}

bool Enigma::settingRings(std::string &ringSettings)
{
	std::cin >> ringSettings;
	std::transform(ringSettings.begin(), ringSettings.end(), ringSettings.begin(), ::toupper);
	if (ringSettings.length() != 3)
	{
		std::cout << "Error: invalid number of rings. There should be 3 rings." << std::endl;
		return true;
	}
	else
	{
		for (auto &iterator : ringSettings)
		{
			if (!(iterator > 64 && iterator < 91)) //65 == 'A', 90 == 'Z'
			{
				std::cout << "Error: rings should be from 'A' to 'Z'." << std::endl;
				return true;
			}
		}
		return false;
	}
}

void Enigma::encipher(std::array<Rotor, 3> &rotors, Reflector &reflector, Plugboard &plugboard, char &eachCharacter, int &offsetCounter)
{
	plugboard.substitute(eachCharacter);
	
	rotors[2].offset();
	if (offsetCounter / 26 > 0)
	{
		for (int secondOffsetCounter = offsetCounter / 26; secondOffsetCounter > 0; --secondOffsetCounter)
		{
			rotors[1].offset();
		}
		if (offsetCounter / 676 > 0)
		{
			for (int thirdOffsetCounter = offsetCounter / 676; thirdOffsetCounter > 0; --thirdOffsetCounter)
			{
				rotors[0].offset();
			}
		}
	}
	++offsetCounter;

    for (auto i = 0; i < 3; i++)
	{
		rotors[i].ringApply(rotors[i].ring);
	}

	for (auto i = 2; i > -1; i--)
	{
		rotors[i].substitute(eachCharacter, 's');
	}

	reflector.reflectorSubstitute(eachCharacter);

	for (auto i = 0; i < 3; i++)
	{
		rotors[i].substitute(eachCharacter, 'r');
	}

	plugboard.substitute(eachCharacter);
}

int Enigma::start()
{
	std::array<Rotor, 3> rotors;
	std::array<int, 3> rotorIndexes = {0, 0, 0};
	char buffer = '0'; 

	std::cout << "Choose 3 rotors. Select from 1 to 5. Repeats are not allowed." << std::endl;

	std::cout << "Rotor 1: ";
	std::cin >> buffer;
	rotorIndexes[0] = atoi(buffer);
	if (Input(rotorIndexes[0]))
	{
		return EXIT_FAILURE;
	}
	std::cout << "Rotor 2: ";
	std::cin >> buffer;
	rotorIndexes[1] = atoi(buffer);
	if (ChecksforDuplicates(rotorIndexes) || Input(rotorIndexes[1]))
	{
		return EXIT_FAILURE;
	}
	std::cout << "Rotor 3: ";
	std::cin >> buffer;
	rotorIndexes[2] = atoi(buffer);
	if (ChecksforDuplicates(rotorIndexes) || Input(rotorIndexes[2]))
	{
		return EXIT_FAILURE;
	}
	for (auto i = 0; i < 3; i++)
	{	
		rotors[i].setRotor(rotorIndexes[i]);
	}

	std::cout << "Choose rings. Select 3 letters A-Z." << std::endl;
	if (settingRings(*ringSettings))
	{
		return EXIT_FAILURE;
	}
	for (auto i = 0; i < 3; i++)
	{
		rotors[i].ring = ringSettings[0][i];
	}

	Plugboard plugboard;
	char plugboardOption;
	std::cout << "Would you like to set up plugboard? type 'Y/y' in order to set up or type any other character to skip: ";
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
	int offsetCounter = 0;
	std::cout << "Enter the message you want to encrypt: ";
	if (!(plugboardOption == 'y' || plugboardOption == 'Y')) std::cin.ignore();
	std::getline(std::cin, message);
	std::transform(message.begin(), message.end(), message.begin(), ::toupper);
	for (auto &eachCharacter : message)
	{
		if (eachCharacter > 64 && eachCharacter < 91)	// 65 == 'A', 90 == 'Z'
		{
			encipher(rotors, reflector, plugboard, eachCharacter, offsetCounter);
		}
	}
	std::cout << "Ciphered message:  " << message << std::endl;
	std::ofstream result;
	result.open("result.txt");
	result << message;
	result.close();

	return EXIT_SUCCESS;
}
