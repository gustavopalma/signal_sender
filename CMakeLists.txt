cmake_minimum_required(VERSION 3.10)

project (Daemon)

add_definitions("-std=c++11")

# add the executable
file(GLOB serial_signalSRC
     "*.h"
     "*.cpp"
)

file(COPY init DESTINATION ./)

add_executable(serial_signal ${serial_signalSRC})
