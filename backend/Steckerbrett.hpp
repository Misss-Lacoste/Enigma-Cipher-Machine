#ifndef Steckerbrett_HPP
#define Steckerbrett_HPP

#include <map>
#include <string>

class Steckerbrett
{
private:    
    std::map<char, char> Steckerbrett;
    std::string SteckerbrettLetters;

public:
    bool SteckerbrettsearchCopies();
    bool lengthCheck();
    bool OkInput();
    bool SteckerbrettConfiguration();
    void substitute(char &);
};

#endif
