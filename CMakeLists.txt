cmake_minimum_required(VERSION 3.10)
project(OpenGLGameEngine)
set(PROJECT_SOURCE_FILES
        source/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${PROJECT_NAME}
    PRIVATE
        source
        thirdparty/glfw-3.4/include
        thirdparty/glew/include
)

# Add GLFW library
add_subdirectory(thirdparty/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")

# Add Glew library
set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(thirdparty/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")
set_property(TARGET glew_s PROPERTY FOLDER GLEW)
set_property(TARGET glew   PROPERTY FOLDER GLEW)

# Link all thirdparty libraries
target_link_libraries(${PROJECT_NAME} 
    glfw 
    glew_s 
    opengl32
)
