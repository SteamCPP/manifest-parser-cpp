cmake_minimum_required(VERSION 3.21)

project(ManifestParser VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/sources.cmake)

add_library(ManifestParser
    ${MANIFESTPARSER_SOURCES}
    external/miniz/miniz.c
)

target_include_directories(ManifestParser
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# == PACKAGING == #
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ManifestParserConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    cmake/ManifestParserConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ManifestParserConfig.cmake
    INSTALL_DESTINATION lib/cmake/ManifestParser
)

install(TARGETS ManifestParser
    EXPORT ManifestParserTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT ManifestParserTargets
    FILE ManifestParserTargets.cmake
    NAMESPACE ManifestParser::
    DESTINATION lib/cmake/ManifestParser
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ManifestParserConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ManifestParserConfigVersion.cmake
    DESTINATION lib/cmake/ManifestParser
)

add_library(ManifestParser::ManifestParser ALIAS ManifestParser)