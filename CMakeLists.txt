cmake_minimum_required(VERSION 3.10)
project(CPPTST)


# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加静态库目标
add_library(test_lib STATIC
    test/class_test.cpp
    test/class_test.h
    dbg.h
)

# 设置库的头文件目录
target_include_directories(test_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

# 添加可执行文件
add_executable(main_program
    main.cpp
)

# 链接静态库到可执行文件
target_link_libraries(main_program
    test_lib
)

# 设置可执行文件的头文件目录
target_include_directories(main_program PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)