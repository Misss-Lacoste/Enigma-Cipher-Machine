#ifndef ROTOR_HPP
#define ROTOR_HPP
//#include <iostream>
#include <string>
#include <string_view>
#include <array>

//Initial alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//https://www.codesandciphers.org.uk/enigma/index.htm
//https://www.codesandciphers.org.uk/enigma/rotorspec.htm

constexpr std::array<std::string_view, 5> rotors = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ",  //official configurations
    "AJDKSIRUXBLHWTMCQGZNPYFVOE",
    "BDFHJLCPRTXVZNYEIWGAKMUSQO",
    "ESOVPZJAYQUIRHXLNFTGKDCMWB",
    "VZBRGITYUPSDNHLXAWMJQOFECK"  
};

constexpr std::array<std::string_view, 5> inversedRotors = {
    "UWYGADFPVZBECKMTHXSLRINQOJ", //also official configurations
    "AJPCZWRLFBDKOTYUQGENHXMIVS",
    "TAGBPCSDQEUFVNZHYIXJWLRKOM",
    "HZWVARTNLGUPXQCEJMBSKDYOIF",
    "QCYLXWENFTZOSMVJUDKGIARPHB"
};

class Rotor {
private:
    std::string rotor;
    std::string inversedRotor;

public:
    std::string Alphabet =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string inversedAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char ring;
    void ConfigureRotor(int);
    void substitute(char &, char);
    void offset();
    void RingConfig(char);
};
#endif