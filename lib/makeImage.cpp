#include "makeImage.h"
#include "Requests.h"

#include <thread>

const auto buttonIsOn = 0x8000;

const int checkFrequency = 100;
const int minFrequency = 50;

const int minDays = 1;
const int maxDays = 16;

const int hourStep = 5;

namespace pictures {
    const std::string dayParts[4]{"Morning", "Afternoon", "Evening", "Night"};

    const int picWidth = 27;
    const int picHeight = 8;

    const int secondColumnX = 12;
    const int secondColumnY = 4;

    std::vector<std::string> clearSky = {"  \\.|./     Clear sky     |",
                                         "__/   \\__                 |",
                                         "  \\   /                   |",
                                         "  /'|'\\                   |"};

    std::vector<std::string> cloud = {"_\\/_        Cloudly       |",
                                      " /\\  .-.                  |",
                                      "  .-(   ).                |",
                                      " (__.__)_)                |"};

    std::vector<std::string> overcast = {"    .-.     Overcast      |",
                                         " .-(   ).                 |",
                                         "(__.__)__)                |",
                                         "                          |"};

    std::vector<std::string> fog = {"- _  - -    Fog           |",
                                    " _ - _ -                  |",
                                    "- - _ -                   |",
                                    "  - _  -                  |"};

    std::vector<std::string> rain = {"  / .-. /   Rain          |",
                                     "/.-( / ).                 |",
                                     "(__./_)__)                |",
                                     " /   /  /                 |"};

    std::vector<std::string> freezingRain = {"*   .-. /   Freezing rain |",
                                             " ./( * ).                 |",
                                             "(__.__)/_)                |",
                                             " *  /   *                 |"};

    std::vector<std::string> snow = {"  * .-. *   Snow          |",
                                     " .-(*  ).                 |",
                                     "(*_._*)__)                |",
                                     "   *     *                |"};

    std::vector<std::string> determinePicture(int weatherCode) {
        if (weatherCode <= 0) {
            return clearSky;
        }
        if (weatherCode <= 1) {
            return cloud;
        }
        if (weatherCode <= 3) {
            return overcast;
        }
        if (weatherCode <= 45 or weatherCode > 71) {
            return fog;
        }
        if (weatherCode <= 51) {
            return rain;
        }
        if (weatherCode <= 66) {
            return freezingRain;
        }
        if (weatherCode <= 71) {
            return snow;
        }
    }

    void line(std::string &s) {
        for (int i = 0; i < picWidth - 1; ++i) {
            s.push_back('-');
        }
        s.push_back('|');
        s.push_back('\n');
    }
}

void outputImage(std::vector<std::string> &cities, int &frequency, int &daysCnt) {
    frequency = std::max(frequency, minFrequency);

    daysCnt = std::max(daysCnt, minDays);
    daysCnt = std::min(daysCnt, maxDays);

    int curCity = 0;
    int cntCycle = 0;

    bool needChange = true;
    while (!(GetKeyState(VK_ESCAPE) & buttonIsOn)) {
        if (!cntCycle) {
            cntCycle = frequency / checkFrequency;
            if (needChange) {
                buildImage(cities, curCity, daysCnt);
                needChange = false;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(checkFrequency));

        if (GetKeyState('N') & buttonIsOn) {
            (++curCity) %= cities.size();
            needChange = true;
        }

        if (GetKeyState('P') & buttonIsOn) {
            --curCity;
            if (curCity < 0) {
                curCity += cities.size();
            }
            needChange = true;
        }

        if ((GetKeyState(VK_ADD) & buttonIsOn) && (daysCnt < maxDays)) {
            ++daysCnt;
            needChange = true;
        }

        if ((GetKeyState(VK_OEM_MINUS) & buttonIsOn) && (daysCnt > 1)) {
            --daysCnt;
            needChange = true;
        }


        --cntCycle;
    }
}

void buildImage(std::vector<std::string> &cities, int &index, int &daysCnt) {
    pictureInfo picture;
    requestParse(RequestWeather(RequestCity(cities[index]), daysCnt), picture);

    std::string result;
    pictures::line(result);

    result += cities[index];
    for (int i = cities[index].size(); i < pictures::picWidth - 1; ++i) {
        result.push_back(' ');
    }
    result.push_back('|');
    result.push_back('\n');

    pictures::line(result);

    for (int i = 7; i < picture.hours.size(); i += 24) {
        result.push_back('\n');
        pictures::line(result);
        for (int j = 0; j < pictures::picWidth - 1; ++j) {
            if (j < 10) {
                result.push_back(picture.hours[i][j]);
            } else {
                result.push_back(' ');
            }
        }
        result.push_back('|');
        result.push_back('\n');

        std::vector<std::string> s = getPicture(i, picture);

        for (auto &j: s) {
            result += j;
            result.push_back('\n');
        }
    }

    system("cls");
    std::cout << result;
}

void writeInfo(std::vector<std::string> &vec, char split, std::string &s, int &u, int &size, const std::string &add) {
    bool writeOn = true;
    std::string cur;

    while (u < size and s[u] != ']') {
        if (writeOn and s[u] != split) {
            cur.push_back(s[u]);
        }

        if (s[u] == split or s[u + 1] == ']') {
            if (writeOn) {
                vec.push_back(cur + add);
                cur.clear();
            }
            if (split == '"')
                writeOn ^= true;
        }


        ++u;
    }

    while (u < size and s[u] != '[') {
        ++u;
    }
    ++u;
}

void requestParse(std::string s, pictureInfo &image) {
    int u = 0;
    int size = s.size();

    while (u - 4 < size and (s[u] != '2' or s[u + 1] != '0' or s[u + 2] != '2' or s[u + 3] != '4')) {
        ++u;
    }

    writeInfo(image.hours, '"', s, u, size, "");
    writeInfo(image.app_temp, ',', s, u, size, "");
    writeInfo(image.temp, ',', s, u, size, "");
    writeInfo(image.weatherCode, ',', s, u, size, "");
    writeInfo(image.wind, ',', s, u, size, " km/h");
    writeInfo(image.humidity, ',', s, u, size, "%");

    return;
}

std::vector<std::string> getPicture(int &index, pictureInfo &picture) {
    std::vector<std::string> result(pictures::picHeight, std::string(pictures::picWidth * 4, ' '));

    for (int i = 0; i < 4; ++i) {
        int ind = index + i * hourStep;
        int shift = i * pictures::picWidth;

        for (int j = shift; j < shift + pictures::picWidth; ++j) {
            result[0][j] = '-';
        }
        result[0][shift + pictures::picWidth - 1] = '|';

        for (int j = shift; j < shift + pictures::dayParts[i].size(); ++j) {
            result[1][j + 8] = pictures::dayParts[i][j - shift];
        }
        result[1][shift + pictures::picWidth - 1] = '|';

        for (int j = shift; j < shift + pictures::picWidth - 1; ++j) {
            result[2][j] = '-';
        }
        result[2][shift + pictures::picWidth - 1] = '|';

        std::vector<std::string> cell = pictures::determinePicture(std::stoi(picture.weatherCode[ind]));

        for (int y = 3; y < 3 + cell.size(); ++y) {
            for (int x = shift; x < shift + cell[0].size(); ++x) {
                result[y][x] = cell[y - 3][x - shift];
            }
        }

        for (int j = shift; j < shift + pictures::picWidth - 1; ++j) {
            result[7][j] = '-';
        }
        result[7][shift + pictures::picWidth - 1] = '|';

        // Add info
        for (int j = 0; j < picture.temp[ind].size(); ++j) {
            result[pictures::secondColumnY][pictures::secondColumnX + shift + j] = picture.temp[ind][j];
        }

        int u = pictures::secondColumnX + shift + picture.temp[ind].size();
        result[pictures::secondColumnY][u] = '(';
        ++u;

        for (int j = 0; j < picture.app_temp[ind].size(); ++j) {
            result[pictures::secondColumnY][u + j] = picture.app_temp[ind][j];
        }

        result[pictures::secondColumnY][u + picture.app_temp[ind].size()] = ')';
        result[pictures::secondColumnY][u + picture.app_temp[ind].size() + 1] = 'C';

        for (int j = 0; j < picture.wind[ind].size(); ++j) {
            result[pictures::secondColumnY + 1][pictures::secondColumnX + shift + j] = picture.wind[ind][j];
        }

        for (int j = 0; j < picture.humidity[ind].size(); ++j) {
            result[pictures::secondColumnY + 2][pictures::secondColumnX + shift + j] = picture.humidity[ind][j];
        }
    }
    return result;
}
