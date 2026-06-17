cmake_minimum_required(VERSION 3.20)
project(sound_processor CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sound_processor_lib STATIC
        src/ArgsParser.cpp
        src/Waveform.cpp
        src/WavIStream.cpp
        src/WavBaseStream.cpp
        src/WavOStream.cpp
        src/Pipeline.cpp
        src/CmdLineArgs2PipelineConverter.cpp
        src/ModifyingFilters.cpp
        src/GeneratorFilters.cpp
        src/FilterProducers.cpp
        src/Application.cpp
)
target_include_directories(sound_processor_lib PUBLIC src)

add_executable(sound_processor main.cpp)
target_link_libraries(sound_processor PRIVATE sound_processor_lib)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(sound_processor_tests tests/sound_processor_tests.cpp)
target_link_libraries(sound_processor_tests PRIVATE sound_processor_lib Catch2::Catch2WithMain)

enable_testing()
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(sound_processor_tests)