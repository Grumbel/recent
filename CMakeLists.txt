cmake_minimum_required(VERSION 3.16)

project(recent)

include(mk/cmake/TinyCMMC.cmake)

find_package(PkgConfig REQUIRED)
find_package(fmt REQUIRED)
pkg_search_module(GTKMM REQUIRED gtkmm-3.0 IMPORTED_TARGET)

add_executable(recent src/recent.cpp)
target_compile_options(recent PRIVATE ${TINYCMMC_WARNINGS_CXX_FLAGS})
target_link_libraries(recent PRIVATE
  PkgConfig::GTKMM
  fmt::fmt)

install(TARGETS recent
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# EOF #
