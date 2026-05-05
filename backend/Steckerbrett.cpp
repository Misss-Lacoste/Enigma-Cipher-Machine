#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include "Steckerbrett.hpp"

bool Steckerbrett::SteckerbrettsearchCopies() {
    std::map<char, int> SeenLetter;
    for (auto i = 0; i < SteckerbrettLetters.length(); i++) { //if (SteckerbrettLetters[firstCharacter] == SteckerbrettLetters[secondCharacter])
        char c = SteckerbrettLetters[i];
        SeenLetter[c]++;
        if (SeenLetter[c] > 1) {
            std::cout << "Warning. You have to enter different symbols, but entered similar ones in different digraphs (ML and MR, for example)." << std::endl;
            return true;
        }
    }
    return false;
}

bool Steckerbrett::lengthCheck() {
    auto pairCount = SteckerbrettLetters.length() / 2; 
    if (pairCount > 13) {    //26 latin letters -> 13 pairs of them
        std::cout << "Warning. Error! You entered too many pairs. Essential to enter less or equal to 13 pairs.\n";
        return true;
    }
    return false;
}

bool Steckerbrett::OkInput() {
    for (auto i = 0; i < SteckerbrettLetters.length(); i++) {
        char c = SteckerbrettLetters[i];
        if (!std::isupper(c)) {
          std::cout <<"Warning. Error! Not all of your characters are alphabetical ones. Allowed only chars from 'A' to 'Z'\n.";
          return true;
        }
    }
    return false;
}

bool Steckerbrett::SteckerbrettConfiguration() {
    std::cout << "Please, enter digraph pairs (pairs of letters) as 'ML PI TA' etc., for example." << std::endl;
    //std::cin.ignore();

    std::getline(std::cin, SteckerbrettLetters);

    std::transform(SteckerbrettLetters.begin(), SteckerbrettLetters.end(), SteckerbrettLetters.begin(), ::toupper);

    SteckerbrettLetters.erase(std::remove(SteckerbrettLetters.begin(), SteckerbrettLetters.end(), ' '), SteckerbrettLetters.end());

    if (SteckerbrettsearchCopies() || lengthCheck() || OkInput()) {
        return true;
    }

    Steckerbrett.clear();
    for (auto i = 0; i + 1 < SteckerbrettLetters.length(); i = i+2) {
        char first = SteckerbrettLetters[i];
        char second = SteckerbrettLetters[i + 1];
        Steckerbrett[first] = second;
        Steckerbrett[second] = first;
    }
    return false;
}

/*void Steckerbrett::substitute(char &symbol) {
    for (auto mapIterator = Steckerbrett.begin(); mapIterator != Steckerbrett.end(); mapIterator++) {
    auto const &pair = *mapIterator;
    if (pair.first == symbol) {
        symbol = pair.second;
        break;
    }
    if (pair.second == symbol) {
        symbol = pair.first;
        break;
    }
    }
}*/
void Steckerbrett::substitute(char &symbol) {
    auto it = Steckerbrett.find(symbol);
    if (it != Steckerbrett.end()) {
        symbol = it->second;
    }
}