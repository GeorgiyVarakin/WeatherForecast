#pragma once

#include <iostream>
#include <vector>

struct pictureInfo {
    std::vector<std::string> hours,
                             temp,
                             app_temp,
                             wind,
                             humidity,
                             weatherCode;

    ~pictureInfo(){};
};

void outputImage(std::vector<std::string> &cities, int &frequency, int &daysCnt);

void buildImage(std::vector<std::string> &cities, int &index, int &daysCnt);

void writeInfo(std::vector<std::string> &vec, char split, std::string &s, int &u, int &size, const std::string& add);

void requestParse(std::string s, pictureInfo &image);

std::vector<std::string> getPicture(int &index, pictureInfo &picture);
