add_executable(aether
    main.cpp
    Value.hpp
    Environment.hpp
    Environment.cpp
    Interpreter.hpp
    Interpreter.cpp
)

target_link_libraries(aether PRIVATE aether_shared_lib)
target_include_directories(aether PRIVATE ${CMAKE_SOURCE_DIR}/src)