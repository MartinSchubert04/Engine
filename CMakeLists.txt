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

# ===============================
# Sources
# ===============================
set(SOURCE_FILE
    src/main.cpp
)

set(IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_impl_opengl3.cpp

    include/imguizmo/GraphEditor.cpp
    include/imguizmo/ImCurveEdit.cpp
    include/imguizmo/ImGradient.cpp
    include/imguizmo/ImGuizmo.cpp
    include/imguizmo/ImSequencer.cpp
)

set(SOURCES
    src/glad.c
    src/Application.cpp
    src/Core/window/GLwindow.cpp

    src/Core/renderer/GLrenderer.cpp
    src/Core/renderer/UIcontext.cpp
    src/Core/renderer/Buffers/VertexBuffer.cpp
    src/Core/renderer/Buffers/FrameBuffer.cpp
    src/Core/renderer/Buffers/VertexArray.cpp
    src/Core/renderer/Buffers/IndexBuffer.cpp

    src/Core/elements/CameraFPS.cpp
    src/Core/elements/Mesh.cpp
    src/Core/elements/Model.cpp
    src/Core/elements/Shader.cpp
    src/Core/elements/Texture.cpp
    src/Core/elements/Log.cpp
    
    src/Core/ui/Scene.cpp
    src/Core/ui/Panel.cpp

    src/Sim/Planet.cpp
    src/Sim/System.cpp
    src/Sim/Solver.cpp
    src/Sim/Grid.cpp

    include/stb/stb_image.cpp
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
# Includes (TARGET-BASED)
# ===============================
target_include_directories(${APP} PRIVATE
    include
    include/imgui
    src
    src/Core/
    src/Sim/
    src/Core/renderer
)

# ===============================
# ImGui loader
# ===============================
target_compile_definitions(${APP} PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
    ROOT_DIR="${CMAKE_SOURCE_DIR}/"
    ASSIMP_STATIC_LIB
)

# ===============================
# Link libraries
# ===============================
if(WIN32)
    target_link_libraries(${APP} PRIVATE
        OpenGL::GL
        glfw
        "${CMAKE_SOURCE_DIR}/lib/assimp/libassimp.a"
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