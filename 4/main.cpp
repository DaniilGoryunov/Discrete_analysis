#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<int> calculateZArray(const std::string& s) {
    int n = s.length();

    std::vector<int> z(n);
    int lt = 0, rt = 0;

    for (int i = 1; i < n; i++) {
        if (i <= rt) {
            z[i] = std::min(rt - i + 1, z[i - lt]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] - 1 > rt) {
            lt = i;
            rt = i + z[i] - 1;
        }
    }
    return z;
}

std::vector<int> searchPattern(const std::string& txt, const std::string& pat) {
    std::string concat = pat + "$" + txt;
    std::vector<int> z = calculateZArray(concat);
    std::vector<int> positions;

    for (int i = 0; i < z.size(); i++) {
        if (z[i] == pat.length()) {
            positions.push_back(i - pat.length() - 1);
        }
    }
    return positions;
}

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::string txt, pat;

    std::getline(inputFile, txt);
    std::getline(inputFile, pat);

    std::vector<int> positions = searchPattern(txt, pat);

    for (int pos : positions) {
        outputFile << pos << std::endl;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}