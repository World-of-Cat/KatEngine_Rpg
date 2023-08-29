
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(eventpp)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb/)

add_library(stb::stb ALIAS stb)

