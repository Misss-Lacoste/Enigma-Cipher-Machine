#include "Crypto.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cctype>

void CryptoAnalyzer::performFrequencyAnalysis(const std::string &ciphertext) {
    std::map<char, int> freq;
    int totalChars = 0;

    for (char c : ciphertext) {
        if (std::isalpha(c)) {
            char upperC = std::toupper(c);
            freq[upperC]++;
            totalChars++;
        }
    }

    if (totalChars == 0) {
        std::cout << "No symbol for analysis." << std::endl;
        return;
    }

    std::cout << "\nFrequency analysis:" << std::endl;
    std::cout << "Letter | Frequency (%)" << std::endl;
    std::cout << "----------------------" << std::endl;

    std::vector<std::pair<char, int>> sortedFreq(freq.begin(), freq.end());
    std::sort(sortedFreq.begin(), sortedFreq.end(), [](const auto  &a, const auto &b) {
        return a.second > b.second;
    });

    for (const auto &pair : sortedFreq) {
        double percentage = (static_cast<double>(pair.second) / totalChars) * 100.0;
        int barLength = static_cast<int>(percentage * 2);

        std::cout << "  " << pair.first 
                  << "   | " << std::fixed << std::setprecision(2) << percentage << " % ";
        
        //for (int i = 0; i < barLength; i++) std::cout << "*";
        std::cout << std::endl;
    }
}

bool CryptoAnalyzer::checkNoSelfMapping(const std::string &plaintext, const std::string &ciphertext) {
    if (plaintext.length() != ciphertext.length()) {
        return false;
    }
    for (size_t i = 0; i < plaintext.length(); i++) {
        char p = std::toupper(plaintext[i]);
        char c = std::toupper(ciphertext[i]);
        
        if (std::isalpha(p) && std::isalpha(c)) {
            if (p == c) {
                return false; //property broken
            }
        }
    }
    return true;
}

std::vector<int> CryptoAnalyzer::findCribPositions(const std::string &ciphertext, const std::string &crib) {
    std::vector<int> validPositions;
    size_t ctLen = ciphertext.length();
    size_t cribLen = crib.length();

    if (cribLen > ctLen) {
        return validPositions;
    }

    std::cout << "\nSearch pos for Crib: '" << crib << "' " << std::endl;

    for (size_t i = 0; i <= ctLen - cribLen; i++) {
        bool isValid = true;
        for (size_t j = 0; j < cribLen; j++) {
            char ctChar = std::toupper(ciphertext[i + j]);
            char cribChar = std::toupper(crib[j]);
            
            if (std::isalpha(ctChar) && std::isalpha(cribChar)) {
                if (ctChar == cribChar) {
                    isValid = false; //letter cant be herself
                    break;
                }
            }
        }
        if (isValid) {
            validPositions.push_back(static_cast<int>(i));
        }
    }

    if (validPositions.empty()) {
        std::cout << "Positions not found." << std::endl;
    } else {
        std::cout << "Found possible positions: " << validPositions.size() << std::endl;
        std::cout << "Indexes: ";
        for (int pos : validPositions) std::cout << pos << " ";
        std::cout << std::endl;
    }

    return validPositions;
}