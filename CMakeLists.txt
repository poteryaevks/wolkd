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

# Seek Almaz

if(NOT Almaz_FOUND)

    if(EXISTS ${ALMAZ_ROOT})
        list(APPEND CMAKE_PREFIX_PATH "${ALMAZ_ROOT}/lib/cmake/almaz")
    endif()

    find_package(Almaz CONFIG
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

find_package(Qt5 COMPONENTS Core REQUIRED)

# message("Qt5Core_INCLUDE_DIRS: ${Qt5Core_INCLUDE_DIRS}")
# message("Qt5Core_LIBRARIES: ${Qt5Core_LIBRARIES}")

# Seek BOOST

find_package(Boost 1.62
   COMPONENTS
   program_options
   system
   filesystem
   REQUIRED
   )

#if(Boost_FOUND)
#    message(STATUS "Boost_LIBRARIES: " "${Boost_LIBRARIES}")
#else()
#    message(FATAL_ERROR " Boost has not been found")
#endif()


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
