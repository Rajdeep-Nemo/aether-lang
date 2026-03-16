cmake_minimum_required(VERSION 3.31)
project(aether-lang VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
  -Wall -Wextra -Wpedantic
  $<$<CONFIG:Debug>:-g3>
  $<$<CONFIG:Release>:-O2>
)

# ── Subdirectories ────────────────────────────────────────────────────────────
add_subdirectory(src/shared)
add_subdirectory(src/interpreter)
add_subdirectory(src/vm)
add_subdirectory(src/compiler)

# ── Tests ─────────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()