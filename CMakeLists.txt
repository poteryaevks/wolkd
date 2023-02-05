cmake_minimum_required(VERSION 3.11.0)

project(wolkd LANGUAGES C CXX VERSION 2.0)

set(CMAKE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(${CMAKE_MODULES_DIR}/functions.cmake)

# Config

if(EXISTS "${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
    include("${CMAKE_SOURCE_DIR}/LocalConfig.cmake")
else()
    message(FATAL_ERROR "LocalConfig.cmake has not been found. Please, put it into project root directory.")
endif()

# RapidJSON

find_package(RapidJSON CONFIG REQUIRED)
message("RapidJSON_INCLUDE_DIR: ${RapidJSON_INCLUDE_DIR}")

# Seek Glew

find_package(GLEW REQUIRED)

if(NOT Alm_FOUND)

    if(EXISTS ${ALM_ROOT})
        list(APPEND CMAKE_PREFIX_PATH "${ALM_ROOT}/lib/cmake/alm")
    endif()

    find_package(alm CONFIG
        COMPONENTS
        vimpel
        misc
        config
        math
        prot_decl
        router
        logger
        REQUIRED
        )

endif()

# Seek Qt

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
add_compile_options(-fPIC)

find_package(Boost 1.62
   COMPONENTS
   program_options
   system
   filesystem
   REQUIRED
   )

find_package(SG
    COMPONENTS
    convertion
    geometry
    engine
    REQUIRED
    )

if(SG_FOUND)
    message("SG_INCLUDE_DIRS: ${SG_INCLUDE_DIRS}")
endif()

add_subdirectory(wolkd)
add_subdirectory(libs)
#add_subdirectory(bfs)
