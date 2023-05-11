#!/bin/bash

# 定义颜色
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
RED="\033[1;31m"
RESET="\033[0m"

# 首先检测Eigen和GTest库是否存在
eigen_found=$(pkg-config --exists eigen3 && echo "yes" || echo "no")
gtest_found=$(pkg-config --exists gtest && echo "yes" || echo "no")

# 安装缺失的库
if [ "$eigen_found" == "no" ] || [ "$gtest_found" == "no" ]; then
  echo -e "${YELLOW}Installing missing libraries...${RESET}"
  sudo apt-get update

  if [ "$eigen_found" == "no" ]; then
    echo -e "${RED}Eigen library not found, installing...${RESET}"
    sudo apt-get install -y libeigen3-dev
  fi

  if [ "$gtest_found" == "no" ]; then
    echo -e "${RED}GTest library not found, installing...${RESET}"
    sudo apt-get install -y libgtest-dev
  fi
else
  echo -e "${GREEN}Eigen and GTest libraries are already installed.${RESET}"
fi

# 删除先前的构建目录（如果存在）
rm -rf build

# 构建项目
echo -e "${YELLOW}Building the project...${RESET}"
cmake -B build && cmake --build build

echo -e "${GREEN}Build complete. Check the 'build' directory for the generated executable.${RESET}"
echo -e "${GREEN}Congradulation! Let's run the executable with './build/cluster <dataset>'${RESET}"