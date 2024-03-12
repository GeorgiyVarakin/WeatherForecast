#include <iostream>
#include <vector>

#include "lib/ArgParser.h"
#include "lib/makeImage.h"

int main() {
    std::vector<std::string> cities;
    int frequency;
    int daysCount;

    Parse(cities, frequency, daysCount);

    if (cities.size() == 0) {
        std::cout << "WRONG_DATA\n";
        exit(1);
    }
    outputImage(cities, frequency, daysCount);

    return 0;
}
