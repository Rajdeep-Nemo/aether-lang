include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(aether_tests
  LexerTest.cpp
  ParserTest.cpp
  InterpreterTest.cpp
  VMTest.cpp
)

target_link_libraries(aether_tests PRIVATE
  aether_shared_lib
  aether_vm_lib
  GTest::gtest_main
)

target_include_directories(aether_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(aether_tests)
