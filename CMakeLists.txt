cmake_minimum_required(VERSION 3.0.0)

project(AnisoVMS VERSION 1.0.0 LANGUAGES CXX)

include(bob.cmake)
bob_begin_package()
bob_begin_cxx_flags()
bob_cxx11_flags()
bob_end_cxx_flags()

set(AnisoVMS_USE_SCOREC_DEFAULT ON)
bob_public_dep(SCOREC)

set(SOURCES
disc.cpp
lhs.cpp
rhs.cpp)

set(HEADERS
disc.hpp
lhs.hpp
rhs.cpp)

add_library(AnisoVMS ${SOURCES} ${HEADERS})
target_include_directories(AnisoVMS PUBLIC ${PETSC_INCLUDE_DIR})
target_link_libraries(AnisoVMS PUBLIC SCOREC::core ${PETSC_LIBRARY})

add_executable(ex1 ex1.cpp)
target_link_libraries(ex1 PUBLIC AnisoVMS)

bob_end_package()
