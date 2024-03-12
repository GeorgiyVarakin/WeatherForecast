#include "Requests.h"

#include <fstream>
#include <curl/curl.h>

const char* XApiKeyHeader = "X-Api-Key: UpjXfzUI5ruv+hOMlp1HOg==U24JrK7JE9ABvES9";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string requestToStr(std::string &request) {
    CURL *curl = curl_easy_init();

    curl_slist *headers = NULL;
    headers = curl_slist_append(headers, XApiKeyHeader);

    char req[request.size() + 1];
    for(int i = 0; i < request.size(); ++i) {
        req[i] = request[i];
    }

    req[request.size()] = '\0';

    std::string s;

    curl_easy_setopt(curl, CURLOPT_URL, req);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return s;
}

std::pair<std::string, std::string> RequestCity(std::string &name) {
    std::string request = "https://api.api-ninjas.com/v1/city?name=" + name;

    std::string s = requestToStr(request);

    int u = 0;
    for(int i = 0; i < 2; ++i) {
        while (s[u] != ':') {
            ++u;
        }
        ++u;
    }

    ++u;

    std::pair<std::string, std::string> res;

    std::string numb;
    while(s[u] != ',') {
        numb.push_back(s[u]);
        u++;
    }

    res.first = numb;
    while (s[u] != ':') {
        ++u;
    }

    ++u;
    ++u;

    numb = "";
    while(s[u] != ',') {
        numb.push_back(s[u]);
        u++;
    }

    res.second = numb;

    return res;
}

std::string RequestWeather(std::pair<std::string, std::string> coords, int &cntDays) {
    std::string request = "https://api.open-meteo.com/v1/forecast?&latitude=" + coords.first + "&longitude=" + coords.second +
                          "&forecast_days=" + std::to_string(cntDays) +
                          "&hourly=apparent_temperature&hourly=temperature_2m&hourly=weather_code&hourly=wind_speed_10m&hourly=relative_humidity_2m";

    return requestToStr(request);
}
