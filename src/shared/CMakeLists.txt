add_library(aether_shared_lib STATIC
  lexer/Lexer.cpp
  lexer/Lexer.hpp
  lexer/Token.hpp
  parser/Parser.cpp
  parser/Parser.hpp
  parser/AstNodes.hpp
  common/Errors.hpp
  builtins/Builtins.cpp
  builtins/Builtins.hpp
)

target_include_directories(aether_shared_lib
  PUBLIC ${CMAKE_SOURCE_DIR}/src
)