cmake_minimum_required(VERSION 3.0.0)

project(AnisoVMS VERSION 1.0.0 LANGUAGES CXX)

include(bob.cmake)
bob_begin_package()

set(AnisoVMS_USE_Omega_h_DEFAULT ON)
bob_public_dep(Omega_h)

set(CMAKE_CXX_FLAGS "${Omega_h_CXX_FLAGS}")
message("-- CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

add_executable(ex1 ex1.cpp)
target_link_libraries(ex1 PUBLIC Omega_h::omega_h)

bob_end_package()
