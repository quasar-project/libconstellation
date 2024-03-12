cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(constellation
  VERSION 0.1.0
  DESCRIPTION "Library for working with QuaSAR remote protocols and hardware"
  HOMEPAGE_URL "io.github.whs31"
  LANGUAGES C CXX
)

option(DEV_BINARY "Enable building of dev binary" OFF)

include(dependencies/cmake/all.cmake)
enable_cross_compile_options()
set_language_standard(23)

find_package(leaf REQUIRED CONFIG)
find_package(leafconf REQUIRED CONFIG)
find_package(Boost REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

set(CMAKE_AUTOMOC ON)

add_library(${PROJECT_NAME} STATIC
  include/constellation/global.h
  src/c++/ip/netif.h)
add_library(${PROJECT_NAME}::leaf ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/constellation/global.h> $<INSTALL_INTERFACE:include/constellation/global.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/constellation/network/api.h> $<INSTALL_INTERFACE:include/constellation/network/api.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/constellation/ip/ipv4.h> $<INSTALL_INTERFACE:include/constellation/ip/ipv4.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/constellation/network/modules/powerswitch.h> $<INSTALL_INTERFACE:include/constellation/network/modules/powerswitch.h>

  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/ip/netif.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/ip/ipv4.c++
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/network/api.c++
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/network/modules/powerswitch.c++
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    leaf::leaf
    leaf::leafconf
    Boost::boost
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  src/c++
)

target_add_metadata_definitions(${PROJECT_NAME} COMPANY "whs31")
enable_static_crt()

target_install_library(${PROJECT_NAME} NAME constellation NAMESPACE quasar)

if(DEV_BINARY)
    add_subdirectory(bin)
endif()

unset(DEV_BINARY CACHE)