cmake_minimum_required(VERSION 2.8.12)
project(CppHttpInterceptor CXX)

set (CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(chi src/Main.cpp src/HttpRecorder.cpp src/Metric.cpp)
target_link_libraries(chi ${CONAN_LIBS})

