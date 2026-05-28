cmake_minimum_required(VERSION 4.0)
project(sound_processor)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.4.2
)
FetchContent_MakeAvailable(CLI11)

add_executable(sound_processor main.cpp
        src/ArgsParser.cpp
        src/ArgsParser.h
        src/Waveform.cpp
        src/Waveform.h
        src/Datastructs.h
        src/WavStream.cpp
        src/WavStream.h)
target_link_libraries(sound_processor PRIVATE CLI11::CLI11)