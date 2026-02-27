cmake_minimum_required(VERSION 3.20)
project(PongGame)

# Use C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Fetch SFML 3.0 automatically
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

# 2. Tell CMake where your code is
add_executable(PongGame src/main.cpp)

# 3. Link SFML
target_link_libraries(PongGame sfml-graphics sfml-window sfml-audio sfml-system)

# 4. Handle Assets (Copies them to where the EXE is so the game can find them)
if(WIN32)
    add_custom_command(TARGET PongGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:PongGame>/Assets")
endif()