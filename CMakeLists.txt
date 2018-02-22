cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
#link_directories(${CMAKE_PROJECT_NAME}/libraries/)
#set(${CMAKE_PROJECT_NAME}_LIBS TimerOne)
set(PROJECT_NAME FreePEMF)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH freePEMF.ino)
set(${CMAKE_PROJECT_NAME}_BOARD nano)
set(nano.build.mcu atmega328p)
set(nano.upload.protocol arduino)
set(nano.upload.speed 57600)

set(${CMAKE_PROJECT_NAME}_PORT COM3)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
