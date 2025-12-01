cmake_minimum_required(VERSION 3.10)

project(HW6_VAR13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Строгие флаги
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Включаем include/ для заголовков
include_directories(
    ${PROJECT_SOURCE_DIR}/objects/npc
    ${PROJECT_SOURCE_DIR}/objects/dragon
    ${PROJECT_SOURCE_DIR}/objects/frog
    ${PROJECT_SOURCE_DIR}/objects/knight
)

# Главная программа
add_executable(HW6_VAR13
    main.cpp
    objects/npc/npc.cpp
    objects/dragon/dragon.cpp
    objects/frog/frog.cpp
    objects/knight/knight.cpp
)

# ---- GoogleTest ----
find_package(GTest)

if(GTEST_FOUND)
    message(STATUS "GoogleTest найден. Тесты включены.")
    enable_testing()

    add_executable(gtests
        tests.cpp
        objects/npc/npc.cpp
        objects/dragon/dragon.cpp
        objects/frog/frog.cpp
        objects/knight/knight.cpp
    )

    target_link_libraries(gtests GTest::GTest GTest::Main)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_link_libraries(gtests pthread)
    endif()

    add_test(NAME unit_tests COMMAND gtests)

else()
    message(WARNING "GoogleTest НЕ найден — тесты отключены!")
endif()
