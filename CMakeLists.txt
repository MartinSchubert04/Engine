cmake_minimum_required(VERSION 3.22)

set(APP openGL) # .exe name 

project(${APP} LANGUAGES C CXX)

cmake_policy(SET CMP0072 NEW)

# ===============================
# General
# ===============================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")

# Binarios en la raíz (como ya usás)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ===============================
# OpenGL / GLFW
# ===============================
find_package(OpenGL REQUIRED)
if(WIN32)
    # En Windows seguimos como antes
    find_package(glfw3 CONFIG REQUIRED)
else()
    # En Linux, forzamos la descarga de GLFW 3.4 para que ImGui no falle
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

add_compile_options(-fdiagnostics-color=always -w)


# ===============================
# Sources
# ===============================
set(SOURCE_FILE
    Demos/Sim/src/App.cpp
)

set(IMGUI_SOURCES
    Engine/src/Engine/ImGui/ImGuiBuild.cpp
)

set(SOURCES
    Engine/include/glad/glad.c
    Engine/src/pch.cpp
    

    # Renderer ( API agnostic) ------------
    Engine/src/Engine/Renderer/Buffer.cpp
    Engine/src/Engine/Renderer/FrameBuffer.cpp
    Engine/src/Engine/Renderer/Renderer.cpp
    Engine/src/Engine/Renderer/VertexArray.cpp
    Engine/src/Engine/Renderer/RendererAPI.cpp
    Engine/src/Engine/Renderer/RenderCommand.cpp
    Engine/src/Engine/Renderer/Shader.cpp


    # Core --------------------------------
    Engine/src/Engine/Core/Application.cpp
    Engine/src/Engine/Core/Window.cpp
    Engine/src/Engine/Core/Log.cpp
    Engine/src/Engine/Core/LayerStack.cpp
    Engine/src/Engine/Core/Layer.cpp
    Engine/src/Engine/Core/Mesh.cpp
    Engine/src/Engine/Core/Texture.cpp
    Engine/src/Engine/Core/CameraFPS.cpp
    Engine/src/Engine/Core/Camera.cpp
    Engine/src/Engine/Core/CameraController.cpp
    # Engine/src/Engine/Core/Model.cpp
    
    Engine/src/Engine/Debug/Instrumentor.cpp
    
    Engine/src/Engine/ImGui/ImGuiLayer.cpp
    
    # Engine/src/Engine/ui/Scene.cpp
    # Engine/src/Engine/ui/Panel.cpp
    
    # Platforms & APIs --------------------------
    Engine/src/Platform/OpenGL/OpenGLcontext.cpp
    Engine/src/Platform/OpenGL/OpenGLrendererAPI.cpp
    Engine/src/Platform/OpenGL/OpenGLvertexArray.cpp
    Engine/src/Platform/OpenGL/OpenGLbuffer.cpp
    Engine/src/Platform/OpenGL/OpenGLframeBuffer.cpp
    Engine/src/Platform/Windows/WindowsWindow.cpp
    Engine/src/Platform/Windows/WindowsInput.cpp

    # Demos ----------------------------------
    Demos/Sim/src/ApplicationLayer.cpp
    Demos/Sim/src/Sphere.cpp
    # Demos/Sim/src/System.cpp
    # Demos/Sim/src/Solver.cpp
    # Demos/Sim/src/Grid.cpp

    Engine/include/stb/stb_image.cpp
    ${IMGUI_SOURCES}
)

# ===============================
# Executable
# ===============================
add_executable(${APP}
    ${SOURCE_FILE}
    ${SOURCES} 
)

# ===============================
# Included directories
# ===============================
target_include_directories(${APP} PRIVATE
    ${PROJECT_SOURCE_DIR}
    Engine/include/
    Engine/include/imgui
    Engine/src
    Engine/src/Engine/
    Sim/Assets/
    Demos/
)

# ===============================
# ImGui loader
# ===============================
target_compile_definitions(${APP} PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
    ROOT_DIR="${CMAKE_SOURCE_DIR}/"
    GLFW_EXPOSE_NATIVE_WIN32
    ASSIMP_STATIC_LIB
)

# ===============================
# Link libraries
# ===============================
if(WIN32)
    target_link_libraries(${APP} PRIVATE
        OpenGL::GL
        glfw
        "${CMAKE_SOURCE_DIR}/Engine/lib/assimp/libassimp.a"
        z  # <---(dependencia de Assimp)
    )

    if(MINGW OR GNUCXX)
        target_link_options(${APP} PRIVATE -mconsole)
    endif()
else()
    find_package(assimp REQUIRED)
    find_package(ZLIB REQUIRED) # Linux prefiere buscarla formalmente

    target_link_libraries(${APP} PRIVATE
        OpenGL::GL
        glfw
        assimp::assimp
        ZLIB::ZLIB
    )
endif()
# ===============================
# Run + Clean target
# ===============================


# 1. Aseguramos que el ejecutable sea tipo CONSOLA
if(MINGW OR GNUCXX)
    # Quitamos -mwindows si existiera y forzamos -mconsole
    target_link_options(${APP} PRIVATE -mconsole)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Fuerza el uso de la librería estándar de GNU que MSYS2 tiene bien instalada
    add_link_options("-static-libstdc++")
    add_link_options("-lstdc++")
endif()

# 2. Corregimos el Target de Ejecución y Limpieza
add_custom_target(run_and_clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${APP}
    # Ejecutamos directamente (sin 'start') para ver los errores en TU consola
    COMMAND "$<TARGET_FILE:${APP}>"
    # El borrado se ejecuta SOLO después de que el .exe se cierra
    COMMAND ${CMAKE_COMMAND} -E rm -f "$<TARGET_FILE:${APP}>"
    DEPENDS ${APP}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compilando, Ejecutando y Limpiando..."
    USES_TERMINAL
)