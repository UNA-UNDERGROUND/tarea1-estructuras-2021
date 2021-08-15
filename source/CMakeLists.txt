cmake_minimum_required(VERSION 3.0.0)
project(archivos VERSION 0.1.0)

set(include_src
    include/vector.hpp
)
set(sources_src
    ${include_src}

)

add_executable(archivos ${sources_src} src/source.cpp)
target_include_directories(archivos PRIVATE include)
