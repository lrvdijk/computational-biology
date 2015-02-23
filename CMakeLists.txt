cmake_minimum_required(VERSION 3.0)

project(Computational-Biology)

# Include boost for reversed_iteratoe
find_package(Boost REQUIRED) 

add_library(compbio
    libcompbio/alphabet.cpp
    libcompbio/alphabets/dna.cpp
    libcompbio/alignment/base.cpp
    libcompbio/alignment/hirschberg.cpp
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

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    target_link_libraries(compbio ${Boost_LIBRARIES})
endif()

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
