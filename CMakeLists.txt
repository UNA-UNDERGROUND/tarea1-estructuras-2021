cmake_minimum_required(VERSION 3.0.0)
project(archivos VERSION 0.1.0)

add_subdirectory(source)
file(COPY assets/ DESTINATION ${archivos_BINARY_DIR}/)