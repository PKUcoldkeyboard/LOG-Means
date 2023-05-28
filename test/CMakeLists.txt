# 添加测试源文件
set(TEST_SOURCES
    main_test.cpp
    utils_test.cpp
)

# 定义测试用例
add_executable(runTests ${TEST_SOURCES})

# 链接gtest
target_link_libraries(runTests gtest)