cmake_minimum_required(VERSION 3.13)
project(usbtest-host)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)
add_executable(usbtest-host main.cpp)

target_link_libraries(usbtest-host usb-1.0 ${CMAKE_THREAD_LIBS_INIT})
