cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Engine)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

configure_file(
        ${PROJECT_SOURCE_DIR}/config.h.in
        ${PROJECT_BINARY_DIR}/config.h
)

set(PROJECT_SOURCE_FILES
        source/Engine.h
        source/Engine.cpp
        source/Common.h
        source/GLTF.cpp
        source/Application.h
        source/Application.cpp
        source/input/InputManager.h
        source/input/InputManager.cpp
        source/graphics/ShaderProgram.h
        source/graphics/ShaderProgram.cpp
        source/graphics/GraphicsAPI.h
        source/graphics/GraphicsAPI.cpp
        source/graphics/VertexLayout.h
        source/graphics/Texture.h
        source/graphics/Texture.cpp
        source/render/Material.h
        source/render/Material.cpp
        source/render/Mesh.h
        source/render/Mesh.cpp
        source/render/RenderQueue.h
        source/render/RenderQueue.cpp
        source/scene/GameObject.h
        source/scene/GameObject.cpp
        source/scene/Scene.h
        source/scene/Scene.cpp
        source/scene/Component.h
        source/scene/Component.cpp
        source/scene/components/MeshComponent.h
        source/scene/components/MeshComponent.cpp
        source/scene/components/CameraComponent.h
        source/scene/components/CameraComponent.cpp
        source/scene/components/PlayerControllerComponent.h
        source/scene/components/PlayerControllerComponent.cpp
        source/scene/components/LightComponent.h
        source/scene/components/LightComponent.cpp
        source/io/FileSystem.h
        source/io/FileSystem.cpp
        #source/eng.h
)

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glew/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw-3.4/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm-1.0.1
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cgltf
)

# Add GLFW library
add_subdirectory(thirdparty/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")

# Add Glew library
set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(thirdparty/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")
set_property(TARGET glew_s PROPERTY FOLDER GLEW)
set_property(TARGET glew   PROPERTY FOLDER GLEW)

# Add GLM library
add_subdirectory(thirdparty/glm-1.0.1/glm "${CMAKE_CURRENT_BINARY_DIR}/glm_build")

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

# Link all thirdparty libraries
target_link_libraries(${PROJECT_NAME} 
    glfw 
    glew_s 
    opengl32
    glm
)
