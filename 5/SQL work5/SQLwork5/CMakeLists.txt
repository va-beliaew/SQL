cmake_minimum_required (VERSION 3.20)
project (SQLwork5)
add_executable (SQLwork5 main.cpp)
add_subdirectory (C:\\libpqxx-7.7.4 libpxx-build)
target_compile_features (SQLwork5 PRIVATE cxx_std_17)
target_link_libraries (SQLwork5 pqxx)