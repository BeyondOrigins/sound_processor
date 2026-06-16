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
        src/WavIStream.cpp
        src/WavIStream.h
        src/WavBaseStream.cpp
        src/WavBaseStream.h
        src/WavOStream.cpp
        src/WavOStream.h
        src/IFilter.h
        src/Pipeline.cpp
        src/Pipeline.h
        src/CmdLineArgs2PipelineConverter.cpp
        src/CmdLineArgs2PipelineConverter.h
        src/ModifyingFilters.cpp
        src/ModifyingFilters.h
        src/GeneratorFilters.cpp
        src/GeneratorFilters.h
)
target_link_libraries(sound_processor PRIVATE CLI11::CLI11)