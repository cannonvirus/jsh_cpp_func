cmake_minimum_required(VERSION 3.5.1)
project("optical_flow" VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O3")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Try to find OpenCV =============================================================================
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Could not locate OpenCV")
endif()
# Try to find OpenCV =============================================================================

include_directories(${PROJECT_SOURCE_DIR}/include)


file(GLOB SOURCE_FILES src/*.cpp)

set(OUTPUT_EXEC "${CMAKE_PROJECT_NAME}_app")
add_executable(${OUTPUT_EXEC} ${SOURCE_FILES})
target_link_libraries (
        ${OUTPUT_EXEC}
        ${OpenCV_LIBS}
)