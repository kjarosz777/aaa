cmake_minimum_required(VERSION 3.12)
project(aaa CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_SHARED_LIBS TRUE)
add_subdirectory(fmt)
find_package(fmt)

add_executable(${PROJECT_NAME}
  main.cc
  file_descriptor.cc
  )

target_link_libraries(${PROJECT_NAME} fmt::fmt)

install(TARGETS ${PROJECT_NAME})
