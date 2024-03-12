cmake_minimum_required()

project(Weather)

add_executable(Weather ArgParser.cpp
                    ArgParser.h
                    makeImage.cpp
                    makeImage.h
                    Requests.cpp
                    Requests.h)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

target_link_libraries(${PROJECT_NAME} CURL::libcurl)
