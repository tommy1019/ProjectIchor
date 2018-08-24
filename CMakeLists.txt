cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)

project(ichor)

file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE resources ${CMAKE_SOURCE_DIR}/res/*)

find_package(PkgConfig)

pkg_check_modules(GLFW REQUIRED glfw3)
find_library(GLFW_LIB glfw3)

pkg_check_modules(GLM REQUIRED glm)

find_package(VULKAN REQUIRED)
link_directories(ichor ${Vulkan_LIBRARY})

add_executable(ichor ${sources} ${headers})

target_include_directories(ichor PUBLIC ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIR})

target_link_libraries(ichor ${GLFW_LIB} vulkan)

add_custom_command(
    TARGET ichor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:ichor>/res
)