#ifndef CRYPTOANALYSIS_HPP
#define CRYPTOANALYSIS_HPP

#include <string>
#include <vector>
#include <map>

struct AnalysisResult {
    bool success;
    std::string message;
    double confidence;
};

class CryptoAnalyzer {
public:
    static void performFrequencyAnalysis(const std::string &ciphertext);
    
    static bool checkNoSelfMapping(const std::string &plaintext, const std::string &ciphertext);
    
    static std::vector<int> findCribPositions(const std::string &ciphertext, const std::string &crib);
};

#endif