cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(OpenGLGameEngine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCE_FILES
        source/main.cpp
        source/Game.h
        source/Game.cpp
        source/StbImage.cpp
        source/BaseObjects/TestObject.h
        source/BaseObjects/TestObject.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glew/include
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/glm-1.0.1
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/thirdparty/stb_image
)

# Add Engine library
add_subdirectory(engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")

# Link engine library
target_link_libraries(${PROJECT_NAME}
        Engine
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
