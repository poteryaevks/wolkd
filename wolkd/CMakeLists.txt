cmake_minimum_required(VERSION 3.11.0)

set(TARGET_NAME wolkd)

file(GLOB _SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/*.cpp
    )

# file (GLOB _HEADER_FILES
    # ${CMAKE_CURRENT_SOURCE_DIR}/hpp/*.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/hpp/*.hpp
    # )

add_executable(${TARGET_NAME}
    ${_SOURCE_FILES}
    # ${_HEADER_FILES}
    )

target_include_directories(${TARGET_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${Alm_INCLUDE_DIRS}
    ${SG_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/libs"
    ${RapidJSON_INCLUDE_DIR}
    )




target_link_libraries(${PROJECT_NAME} PUBLIC
    SG::convertion
    SG::geometry
    SG::engine
    Qt5::Core
    bfs
    )

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )

target_compile_definitions(${TARGET_NAME} PRIVATE
    -D_USE_MATH_DEFINES
    -DNDEBUG
    )

target_compile_options(${TARGET_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-Wall;-D_DEBUG>"
    "$<$<CONFIG:Release>:-O2;-DNDEBUG>"
    )



