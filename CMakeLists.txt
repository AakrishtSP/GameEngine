cmake_minimum_required(VERSION 3.11)

project(GameEngine LANGUAGES CXX)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

# Dependencies
# raylib
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
        # Build raylib with -fPIC
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# raygui
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

# nlohmann_json
set(NLOHMMANN_VERSION 3.11.3)
find_package(nlohmann_json ${NLOHMMANN_VERSION} QUIET) # QUIET or REQUIRED
if (NOT nlohmann_json_FOUND) # If there's none, fetch and build nlohmann_json
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

# Source files
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/Engine/*.cpp)
file(GLOB_RECURSE COMPONENT_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/Engine/Components/*.cpp)
file(GLOB_RECURSE SCRIPT_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/assets/Scripts/*.cpp)

# Create library for engine
add_library(Engine STATIC ${ENGINE_SOURCES})
target_link_libraries(Engine PRIVATE raylib nlohmann_json::nlohmann_json raygui)
target_include_directories(Engine PRIVATE ${nlohmann_json_SOURCE_DIR}/include) # Ensure Engine can find nlohmann_json

# Create library for components
add_library(Components STATIC ${COMPONENT_SOURCES})
target_link_libraries(Components PRIVATE Engine)
target_include_directories(Components PRIVATE ${raylib_SOURCE_DIR}/src ${raygui_SOURCE_DIR}/src ${nlohmann_json_SOURCE_DIR}/include) # Ensure Components can find raylib and nlohmann_json

# Create shared libraries for each script
foreach(script_cpp IN LISTS SCRIPT_SOURCES)
    get_filename_component(script_name ${script_cpp} NAME_WE)
    add_library(${script_name} SHARED ${script_cpp})
    target_link_libraries(${script_name} PRIVATE Engine Components)
    set_target_properties(${script_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/assets/Build)
endforeach()

# Create the main executable
add_executable(GameEngine ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(GameEngine PRIVATE Engine Components raylib nlohmann_json::nlohmann_json raygui)

# Ensure correct RPATH for runtime linking
set_target_properties(GameEngine PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_RPATH ${CMAKE_SOURCE_DIR}/assets/Build
)

# Web Configurations
if (PLATFORM STREQUAL "Web")
    set_target_properties(GameEngine PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(GameEngine
        PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()
