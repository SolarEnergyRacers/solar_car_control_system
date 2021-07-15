cmake_minimum_required(VERSION 3.16.0)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS  "${EXTRA_COMPONENT_DIRS} interfaces")

project(solar_car_control_system)

