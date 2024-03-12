add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

include_directories(../lib)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

target_link_libraries(${PROJECT_NAME} CURL::libcurl)
