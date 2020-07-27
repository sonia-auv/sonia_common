#===============================================================================
# G L O B A L   P R O J E C T   C O N F I G U R A T I O N

cmake_minimum_required(VERSION 2.8.3)

project(sonia_common)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

#===============================================================================
# C A T K I N   C O N F I G U R A T I O N

find_package(catkin REQUIRED COMPONENTS
    std_msgs
    geometry_msgs
    sensor_msgs
    nav_msgs
    std_srvs
    message_generation
    )

# CMake does not detect new file when globbing.
# Please, regenerate your CMake solution if you add a file.
file(GLOB srv_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/srv/"
    "${CMAKE_CURRENT_SOURCE_DIR}/srv/*.srv")
add_service_files(DIRECTORY srv FILES ${srv_files})

file(GLOB msg_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/srv/"
    "${CMAKE_CURRENT_SOURCE_DIR}/msg/*.msg")
add_message_files(DIRECTORY msg FILES ${msg_files})

generate_messages(DEPENDENCIES std_msgs geometry_msgs sensor_msgs )

#===============================================================================
# C R E A T E   C A T K I N   P A C K A G E

catkin_package(
    CATKIN_DEPENDS
    std_msgs
    sensor_msgs
    geometry_msgs
    nav_msgs
    message_runtime
)
