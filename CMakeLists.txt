cmake_minimum_required(VERSION 3.11)

project(GameEngine LANGUAGES CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# raygui integration
set(RAYGUI_VERSION 4.0)
find_package(raygui ${RAYGUI_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raygui_FOUND) # If there's none, fetch and build raygui
    include(FetchContent)
    FetchContent_Declare(
        raygui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raygui)
    if (NOT raygui_POPULATED) # Have we downloaded raygui yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raygui)

        # Add raygui as an interface library
        add_library(raygui INTERFACE)
        target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR}/src)
    endif()
endif()

#nllohmann_json integration
set(NLOHMMANN_VERSION 3.11.3)
find_package(nlohmann_json ${NLOHMMANN_VERSION} QUIET) # QUIET or REQUIRED
if (NOT nlohmann_json_FOUND) # If there's none, fetch and build nlohmann
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/nlohmann/json/archive/refs/tags/v${NLOHMMANN_VERSION}.tar.gz
    )
    FetchContent_GetProperties(nlohmann_json)
    if (NOT nlohmann_json_POPULATED) # Have we downloaded nlohmann_json yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(nlohmann_json)
        add_subdirectory(${nlohmann_json_SOURCE_DIR} ${nlohmann_json_BINARY_DIR})
    endif()
endif()

# Automatically add all .cpp files in src and subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} raylib nlohmann_json::nlohmann_json raygui)

# Web Configurations
if (PLATFORM STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()