#pragma once

#include <iostream>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

std::string requestToStr(std::string &request);

std::pair<std::string, std::string> RequestCity(std::string &name);

std::string RequestWeather(std::pair<std::string, std::string> coords, int &cntDays);
