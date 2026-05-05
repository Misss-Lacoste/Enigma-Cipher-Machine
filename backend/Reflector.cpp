#include <string>
#include "Reflector.hpp"

void Reflector::reflectorConfiguration(char &alphabetChar) {
    if(alphabetChar >= 'A' && alphabetChar <= 'Z') {
        int index = alphabetChar -'A';
        alphabetChar = reflector[index];
    }
}
