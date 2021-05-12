cmake_minimum_required(VERSION 3.5)

project(Metopt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Metopt
    main.cpp
    matrix.h
    matrix.cpp
    profile.cpp
    profile.h
    lu.cpp
    lu.h
    vector_matrix.cpp
    vector_matrix.h
    test_generator.cpp
    test_generator.h)
