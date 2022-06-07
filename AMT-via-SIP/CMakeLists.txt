cmake_minimum_required (VERSION 3.8)

project (AMT-via-SIP LANGUAGES CXX)
SET(CMAKE_CXX_FLAGS "-O3")

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

# GMP
find_path(GMP_INCLUDE_DIR NAMES gmp.h)
find_library(GMP_LIBRARIES NAMES gmp libgmp)
find_library(GMPXX_LIBRARIES NAMES gmpxx libgmpxx)

add_executable (AMT-via-SIP src/main.cpp src/global.cpp src/matrix.cpp src/mulgate.cpp src/network.cpp src/share.cpp src/triples.cpp)

target_link_libraries(AMT-via-SIP gmp gmpxx)
