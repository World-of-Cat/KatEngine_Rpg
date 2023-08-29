cmake_minimum_required(VERSION 3.24)
project(TriggerHappy VERSION 0.0.1)

add_executable(TriggerHappy src/th/triggerhappy.cpp src/th/triggerhappy.hpp)
target_include_directories(TriggerHappy PRIVATE src/)
target_link_libraries(TriggerHappy KatEngine::KatEngine)
