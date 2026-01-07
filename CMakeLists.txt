cmake_minimum_required(VERSION 3.28)
project(SporeSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window  
    SFML::System
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC include
    PRIVATE src
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -DNDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

add_custom_command(
    TARGET SporeSimulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:SporeSimulation>/assets
    COMMENT "Copie des assets vers le dossier de build..."
)