cmake_minimum_required(VERSION 3.10.0)

project(mesh-cluster-gen)
set(CMAKE_CXX_STANDARD 20)

set(LINK_LIBS pmp pmp_vis)

# configure the cwd ===================
configure_file( ${CMAKE_SOURCE_DIR}/impl/inc/cwd/cwd.h.in
                ${CMAKE_SOURCE_DIR}/impl/inc/cwd/cwd.h)

# build pmp and pmp_vis ===============
add_subdirectory(third-party/pmp-library)

# executable ==========================
add_executable( ${PROJECT_NAME} main.cpp
        impl/src/mesh-parameterisation/distortion-checker/distortion-checker.cpp
        impl/src/mesh-parameterisation/seam-generator/seam-generator.cpp)

# Link the main exec ==================
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

# Include directories =================
target_include_directories(${PROJECT_NAME} PUBLIC impl/inc)
