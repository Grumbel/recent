cmake_minimum_required(VERSION 3.16)

project(recent)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

set(CMAKE_CXX_STANDARD 20)
add_executable(recent src/recent.cpp)
target_link_libraries(recent PRIVATE
  ${GTKMM_LIBRARIES}
  fmt::fmt)

install(TARGETS recent
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# EOF #
