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

set(sonia_common_SRC_DIR "src")

#===============================================================================
# C A T K I N   C O N F I G U R A T I O N

find_package(catkin REQUIRED COMPONENTS
    std_msgs
    geometry_msgs
    sensor_msgs
    nav_msgs
    std_srvs
    message_generation
    image_transport
    cv_bridge
    )

find_package(catkin REQUIRED cmake_modules)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

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
    INCLUDE_DIRS src
    LIBRARIES sonia_common
    CATKIN_DEPENDS
    roscpp
    std_msgs
    sensor_msgs
    geometry_msgs
    nav_msgs
    message_runtime
    image_transport
      cv_bridge
)

#===============================================================================
# I N C L U D E   F I L E S

# Get all the files of the projects.
file(GLOB_RECURSE sonia_common_SRC_FILES "${sonia_common_SRC_DIR}/*.h")

# Include all the directories where source can be found.
include_directories(
    ${catkin_INCLUDE_DIRS}
    ${Eigen_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${sonia_common_SRC_DIR}
)

#==========================================================================
# C R E A T E   E X E C U T A B L E

add_library(sonia_common ${sonia_common_SRC_FILES})
target_link_libraries(sonia_common ${catkin_LIBRARIES} ${Eigen_LIBRARIES} ${OpenCV_LIBRARIES})
set_target_properties(sonia_common PROPERTIES LINKER_LANGUAGE CXX)

install(
    TARGETS ${TARGET_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(
    DIRECTORY ${sonia_common_SRC_DIR}/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    PATTERN ".git" EXCLUDE
)

#==========================================================================
# U N I T   T E S T S

if (CATKIN_ENABLE_TESTING)
  add_subdirectory(test)
endif ()