#!/bin/bash
set -e


rm -rf build
echo "🧹 删除 build 目录成功 ✔️"

#conan install . -of build --build=missing -s build_type=Release
echo "📦 开始执行 conan install ..."
conan install . --build=missing -s build_type=Debug
echo "📦 conan install 完成 ✅"

echo "⚙️ 开始执行 cmake 配置 (preset=conan-debug) ..."
cmake --preset conan-debug
echo "⚙️ cmake 配置完成 ✅"


cmake --build build/Debug
echo "🛠️ 编译完成 ✅"

echo "📂 开始安装到系统目录 (/usr/local) ..."
sudo cmake --install build/Debug
echo "📂 安装完成 ✅"

echo "🎉 ✅ 构建 + 编译 + 安装 全部完成！ 🚀"
