cmake_minimum_required(VERSION 3.14)
project(nes_emulator)

set(CMAKE_PREFIX_PATH "c:\\Qt\\5.15.2\\mingw81_64\\")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB Sources "src/*")
add_executable(nes_emulator ${Sources})

target_link_libraries(nes_emulator Qt5::Widgets)
target_link_libraries(nes_emulator -pthread)