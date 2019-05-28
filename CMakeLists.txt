cmake_minimum_required (VERSION 2.8.3)
project(ST)

add_subdirectory(ext ext_build)

include_directories(
  # Include files
  # ${CMAKE_CURRENT_SOURCE_DIR}/src/analyzer

  # Eigen linear algebra library
  #${EIGEN_INCLUDE_DIR}
  # OpenEXR high dynamic range bitmap library
  ${OPENEXR_INCLUDE_DIRS}
  # Intel Thread Building Blocks
  #${TBB_INCLUDE_DIR}
  # Boost C++ libraries
  #${Boost_INCLUDE_DIR}
)

add_definitions(-O2 -Wno-deprecated)

find_package(OpenMP)  
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

file ( GLOB ST_SOURCE
  src/*.cpp
)

file ( GLOB ST_HEADER
  src/*.h
)

set(ST_DEPS_LIBRARIES
  #tbb_static
  IlmImf
  #${Boost_LIBRARIES}
)

add_library ( st_static STATIC
        ${ST_HEADER}
        ${ST_SOURCE}
        )
        

# Main exec
add_executable( st_exe
      src/main.cxx
)

set_target_properties ( st_exe
        PROPERTIES
        OUTPUT_NAME st
        )

target_link_libraries( st_exe st_static ${ST_DEPS_LIBRARIES})


# vim: set et ts=2 sw=2 ft=cmake nospell:
