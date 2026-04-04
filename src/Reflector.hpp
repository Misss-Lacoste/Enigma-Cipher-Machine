#ifndef REFLECTOR_HPP
#define REFLECTOR_HPP
#include <string>
//initial Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//https://www.codesandciphers.org.uk/enigma/example1.htm
class Reflector {
private:
    std::string reflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT"; //standard reflector configuration, A->Y etc.

public:
    std::string Alphabet =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    void reflectorConfiguration(char &);
};
#endif
