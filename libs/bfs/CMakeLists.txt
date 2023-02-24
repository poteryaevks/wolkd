set(TARGET_NAME bfs)


file(GLOB _SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file(GLOB _HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_library(${TARGET_NAME} STATIC
    ${_SOURCE_FILES}
    ${_HEADER_FILES}
    )

target_include_directories(${TARGET_NAME} PUBLIC
    ${Sgraphics_INCLUDE_DIRS}
    )


target_link_libraries(${TARGET_NAME} PRIVATE
    Sgraphics::geometry
    )

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )


