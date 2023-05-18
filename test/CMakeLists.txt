# 引入gtest库
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 添加测试源文件
set(TEST_SOURCES
    main_test.cpp
    utils_test.cpp
    data_preprocessing_test.cpp
    gain_calculation_test.cpp
    cluster_analysis_test.cpp
)

# 定义测试用例
add_executable(runTests ${TEST_SOURCES})

# 链接gtest
target_link_libraries(runTests ${GTEST_BOTH_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

# 添加测试
gtest_discover_tests(runTests)