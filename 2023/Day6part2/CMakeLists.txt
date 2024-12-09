cmake_minimum_required(VERSION 3.13.0)

set(FullPath ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(ProjectName ${FullPath} NAME)


project(${ProjectName} VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
