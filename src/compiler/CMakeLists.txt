cmake_minimum_required(VERSION 3.31)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

llvm_map_components_to_libnames(LLVM_LIBS
  core support irreader passes
  native orcjit
)

add_executable(aetherc
  main.cpp
  Compiler.hpp
  Compiler.cpp
)

target_include_directories(aetherc PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(aetherc PRIVATE
  ${LLVM_DEFINITIONS}
)

target_link_libraries(aetherc PRIVATE
  aether_shared_lib
  aether_vm_lib        # reuses Chunk + Opcodes for IR lowering
  ${LLVM_LIBS}
)