//#include <iostream>
#include "Rotor.hpp"

int toZeroBased(int oneBasedIndex) { //rotors in a prog memory start from 0, we have to operate with rotors from 1
    return oneBasedIndex - 1;
}
void Rotor::ConfigureRotor(int choice) {
    int index = toZeroBased(choice);
    rotor = rotors[index];
    inversedRotor = inversedRotors[index];
}

//replaces an alphabet letter with the corresponding letter from the rotor
char findMatchingChar(char target, const std::string  &alphabet, const std::string &rotorPosition) {
    for (auto i = 0; i < alphabet.length(); i++) {
        if (target == alphabet[i]) {
            return rotorPosition[i];
        }
    }
    return target;
}
void Rotor::substitute(char &ch, char option) {
    if (option == 's') {
        ch = findMatchingChar(ch, Alphabet, rotor); //if (ch == *alphabetIterator) -> ch = *rotorIterator
    }
    else if (option == 'r') {
        ch = findMatchingChar(ch, iAlphabet, inversedRotor); //if (ch == *alphabetIterator) -> ch = *inverseRotorIterator
    }
}

void Rotor::offset() {
    for (int i = 0; i < (int)rotor.size(); i++) {
        if (rotor[i] == 'Z') {
            rotor[i] = 'A';
        }
        else {
            rotor[i] = rotor[i] + 1; //rotor[i]++
        }
    }
    //reverse (rotate) offset
    char lastChar = inversedRotor.back();
    for (int i = inversedRotor.size() - 1; i > 0; i--) {
        inversedRotor[i] = inversedRotor[i - 1];
    }
    inversedRotor[0] = lastChar;
}

//an additional shift that is applied before the main encryption
void Rotor::ringApply(char ring) {
    int steps = ring - 'A'; //how many times do we spin
    
    for (int step = 0; step < steps; step++) { //move alphabet right
        char lastLetter = Alphabet[Alphabet.size() - 1];
        for (int i = Alphabet.size() - 1; i > 0; i--) {
            Alphabet[i] = Alphabet[i - 1]; 
        }
        Alphabet[0] = lastLetter;
        
        for (int i = 0; i < inversedRotor.size(); i++) {
            if (inversedRotor[i] == 'A') {
                inversedRotor[i] = 'Z';
            }
            else {
                inversedRotor[i] = inversedRotor[i] - 1; //inversedRotor--
            }
        }
    }
}