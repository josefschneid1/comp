add_executable(Compiler src/main.cpp)

target_compile_features(Compiler
    PUBLIC
        cxx_std_23
)

target_link_libraries(Compiler
    Parser
)