cmake_minimum_required(VERSION 3.27)


project(backend)
set(CMAKE_CXX_STANDARD 17)

#set(DCMAKE_TOOLCHAIN_FILE "D:/Facultate/3nd Year Sem2/SD/FileSearchEngineSystem/vcpkg/scripts/buildsystems/vcpkg.cmake")


include_directories(include)

add_executable(backend
        src/main.cpp
        src/database/database.cpp
        src/indexer/file-crawler.cpp
        src/indexer/file-ignorer.cpp
        src/indexer/index-builder.cpp
        src/service/search-service.cpp
        src/config/config.cpp
        src/service/insert-service.cpp
        src/model/file.cpp
        src/logger/logger.cpp
        src/utils/string-processor.cpp
)
find_package(libpqxx REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem algorithm)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(backend PRIVATE
        libpqxx::pqxx
        Boost::filesystem
        Boost::algorithm
        nlohmann_json::nlohmann_json
)
