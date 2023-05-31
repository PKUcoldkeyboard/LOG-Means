cmake_minimum_required(VERSION 3.8)

project(LOG-Means)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
CHECK_CXX_COMPILER_FLAG("-mtune=generic" COMPILER_SUPPORTS_MTUNE_GENERIC)

if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
endif()

if(COMPILER_SUPPORTS_MTUNE_GENERIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mtune=generic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=generic")
endif()


# 添加项目源文件
set(SOURCES
    src/main.cpp
)

# 添加头文件目录
include_directories(include)

# 第三方头文件
include_directories(third-party)

set(Eigen3_DIR "third-party/eigen-3.4.0/cmake")
find_package(Eigen3 REQUIRED)

# Openmp引入
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

# json11引入
set(JSON11_CPP "third-party/json11.cpp")
set(JSON11_H "third-party/json11.hpp")
add_library(json11 ${JSON11_CPP} ${JSON11_H})

# 定义可执行文件
add_executable(cluster ${SOURCES})
target_link_libraries(cluster json11)

# 添加测试
# add_subdirectory(third-party/googletest-1.13.0)
# add_subdirectory(test)
