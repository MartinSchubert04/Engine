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

# Binarios en la raíz (como ya usás)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ===============================
# OpenGL / GLFW
# ===============================
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

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
)

set(SOURCES
    src/glad.c
    src/Application.cpp
    src/window/GLwindow.cpp

    src/renderer/GLrenderer.cpp
    src/renderer/UIcontext.cpp
    src/renderer/Buffers/VertexBuffer.cpp
    src/renderer/Buffers/FrameBuffer.cpp
    src/renderer/Buffers/VertexArray.cpp
    src/renderer/Buffers/IndexBuffer.cpp

    src/elements/Camera.cpp
    src/elements/Mesh.cpp
    src/elements/Model.cpp
    src/elements/Shader.cpp
    src/elements/Texture.cpp
    
    src/ui/Scene.cpp
    src/ui/Panel.cpp

    src/common.cpp

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
    src
    src/renderer
)

# ===============================
# ImGui loader
# ===============================
target_compile_definitions(${APP} PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
    ROOT_DIR="${CMAKE_SOURCE_DIR}/"
)

set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/external/assimp)


add_custom_command(TARGET openGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/libassimp-6.dll
    $<TARGET_FILE_DIR:openGL>
)

# ===============================
# Link libraries
# ===============================
target_link_libraries(${APP} PRIVATE
    OpenGL::GL
    glfw
    "${CMAKE_SOURCE_DIR}/lib/assimp/libassimp.dll.a"
)
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