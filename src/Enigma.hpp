#ifndef ENIGMA_HPP
#define ENIGMA_HPP

#include <array>
#include <string>
#include "Steckerbrett.hpp"
#include "Reflector.hpp"
#include "Rotor.hpp"

class Enigma
{
private:
    void advanceRotors(std::array<Rotor, 3>& rotors, int& MovingsCount);
    void applyRingSettings(std::array<Rotor, 3>& rotors);
    void forwardPassThroughRotors(std::array<Rotor, 3>& rotors, char& character);
    void reversePassThroughRotors(std::array<Rotor, 3>& rotors, char& character);
    bool selectRotors(std::array<int, 3>& rotorsID);
public:
    std::string ringSettings[3];
    bool defaultSettings();
    bool searchCopies(const std::array<int, 3>&);
    int char_to_int(char);
    bool input(int);
    bool Rings(std::string &);
    void encipher(std::array<Rotor, 3> &, Reflector &, Steckerbrett &, char &, int &);
    bool decrypt(std::array<Rotor, 3> &rotors, Reflector &reflector, Steckerbrett &steckerbrett, std::string &message, int &movingsCount);
    int start();
};

#endif
