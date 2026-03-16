# VM logic as a static lib so the compiler can reuse Chunk + Opcodes
add_library(aether_vm_lib STATIC
  Chunk.hpp
  Chunk.cpp
  Opcodes.hpp
  CallFrame.hpp
  VM.hpp
  VM.cpp
)

target_link_libraries(aether_vm_lib PUBLIC aether_shared_lib)
target_include_directories(aether_vm_lib
  PUBLIC ${CMAKE_SOURCE_DIR}/src
)

# Standalone VM binary
add_executable(aether_vm main.cpp)
target_link_libraries(aether_vm PRIVATE aether_vm_lib)
