cmake_minimum_required(VERSION 3.24)
project(KatEngine VERSION 0.1.0)

add_library(KatEngine src/kat/engine.cpp src/kat/engine.hpp)
target_include_directories(KatEngine PUBLIC src/)
target_link_libraries(KatEngine PUBLIC glfw glad::glad glm::glm stb::stb spdlog::spdlog)

add_library(KatEngine::KatEngine ALIAS KatEngine)
