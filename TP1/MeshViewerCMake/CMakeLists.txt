cmake_minimum_required(VERSION 3.10)
project(MeshViewer)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

set(GLEW_INCLUDE_DIR "/opt/homebrew/include")
set(GLEW_LIBRARY "/opt/homebrew/lib/libGLEW.dylib")

include_directories(
    ${GLEW_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES 
    main.cpp
    myHalfedge.cpp
    myVector3D.cpp
    myPoint3D.cpp
    myFace.cpp
    myMesh.cpp
    myVertex.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# macOS: GLUT natif (Cocoa) — pas de X11 / XQuartz. Linux/Windows: FreeGLUT.
if(APPLE)
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARY}
        "-framework GLUT"
        glm::glm
    )
else()
    find_library(FREEGLUT_LIB glut PATHS "/opt/homebrew/lib" "/usr/local/lib" "/usr/lib" "C:/freeglut/lib")
    if(NOT FREEGLUT_LIB)
        message(FATAL_ERROR "FreeGLUT introuvable (ex: brew install freeglut)")
    endif()
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARY}
        ${FREEGLUT_LIB}
        glm::glm
    )
endif()