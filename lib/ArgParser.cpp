#include <fstream>

#include "ArgParser.h"

void Parse(std::vector<std::string> &cities, int &frequency, int &daysCount) {
    std::fstream file("cfg.json");

    std::string s;
    file >> s;
    file >> s;

    while(true) {
        file >> s;

        bool is_Name = false;
        std::string curString;

        for(char &i : s) {
            if (i == '"') {
                if (is_Name) {
                    cities.push_back(curString);
                    curString = "";
                }

                is_Name ^= true;
            }
            else if (is_Name) {
                curString.push_back(i);
            }
        }

        if (s[s.size() - 2] == ']')
            break;
    }

    file >> s;
    file >> frequency;
    file >> s >> s;
    file >> daysCount;

    return;
}
