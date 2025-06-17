cmake_minimum_required(VERSION 3.5)
project(WebCrawler CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)          # forcing C++20
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main executable
add_executable(${PROJECT_NAME} main.cc)

# Find Drogon
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# Gumbo: system-installed
find_library(GUMBO_LIB gumbo REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GUMBO_LIB})

# Set C++ version check message
if (CMAKE_CXX_STANDARD LESS 20)         # coroutines used
    message(FATAL_ERROR "c++20 or higher is required")
else ()
    message(STATUS "use c++20")
endif ()

# Source directories
aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(utills UTILS_SRC)

# View templates
drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/utills
)

# Add all sources
target_sources(${PROJECT_NAME}
    PRIVATE
    ${CTL_SRC}
    ${FILTER_SRC}
    ${PLUGIN_SRC}
    ${MODEL_SRC}
    ${UTILS_SRC}
)

# Add test folder if needed
add_subdirectory(test)
