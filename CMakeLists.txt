cmake_minimum_required(VERSION 3.10)
project(OpenGLGameEngine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCE_FILES
        source/main.cpp
        source/Game.h
        source/Game.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

# Add Engine library
add_subdirectory(engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")

# Link engine library
target_link_libraries(${PROJECT_NAME}
        Engine
)
