cmake_minimum_required(VERSION 3.22.1)
project(reggiebot_motors)

# Default to C11 and C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Compiler warning/error flags
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Werror
    -pedantic-errors # may be redundant with wpedantic
    -Weffc++
    -Wsign-conversion)
endif()

# FIND PACKAGE AND LIBRARIES ament build system
find_package(ament_cmake REQUIRED)
# libserial (not cmake ready) cmake finds lib<serial>.so in /usr/lib (a default
# lib dir)
find_library(LIBSERIAL serial REQUIRED)
# set(LIBSERIAL_INCLUDE_DIR /usr/include/libserial)

# ADD MAIN EXECUTABLE
add_executable(pi_comms src/pi_comms.cpp)
# link dependencies and add include dirs
target_link_libraries(pi_comms PUBLIC ${LIBSERIAL})
# not needed because it's in default /usr/include
# target_include_directories(pi_comms PUBLIC ${LIBSERIAL_INCLUDE_DIR}) add
target_include_directories(
  pi_comms PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)
# use below to link instead of target_link_libraries() if linking a package
# ament_target_dependencies(my_target my_package)

# copy targets and directories over so ros2 launch can see them
install(TARGETS pi_comms DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY config launch DESTINATION share/${PROJECT_NAME})

# enable linters/testers if testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
