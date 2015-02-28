cmake_minimum_required(VERSION 3.0)

project(Computational-Biology)

add_library(compbio
    libcompbio/alphabet.cpp
    libcompbio/alphabets/dna.cpp
    libcompbio/alignment/base.cpp
    libcompbio/alignment/hirschberg.cpp
    libcompbio/alignment/needleman-wunsch.cpp
    libcompbio/alignment/smith-waterman.cpp
    libcompbio/utils.cpp
    libcompbio/sequence.cpp
)
include_directories(.)

target_compile_features(compbio PRIVATE 
    cxx_lambdas 
    cxx_nullptr 
    cxx_rvalue_references
    cxx_range_for
)

add_executable(local-alignment
    src/local-alignment.cpp
)

target_link_libraries(local-alignment compbio)
target_compile_features(local-alignment PRIVATE 
    cxx_lambdas 
    cxx_nullptr 
    cxx_rvalue_references
    cxx_range_for
)

add_executable(hirschberg
    src/hirschberg.cpp
)

target_link_libraries(hirschberg compbio)
target_compile_features(hirschberg PRIVATE 
    cxx_lambdas 
    cxx_nullptr 
    cxx_rvalue_references
    cxx_range_for
)

add_executable(needleman-wunsch
    src/needleman-wunsch.cpp
)

target_link_libraries(needleman-wunsch compbio)
target_compile_features(needleman-wunsch PRIVATE 
    cxx_lambdas 
    cxx_nullptr 
    cxx_rvalue_references
    cxx_range_for
)
