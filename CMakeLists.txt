cmake_minimum_required(VERSION 3.20)
project(IMSystemXST LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # 供 VS Code IntelliSense 使用

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 一个类一对文件：include/*.h + src/*.cpp，新增文件后重新配置即可自动收录
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(IMSystemXST ${SOURCES})
target_include_directories(IMSystemXST PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 编码约定：.cpp/.h 源码与 data/*.txt 均以 GBK 保存，与中文 Windows 控制台（代码页 936）一致，
# std::cin / std::cout 可直接处理中文，任何编译器无需额外的字符集参数
target_compile_options(IMSystemXST PRIVATE -Wall -Wextra)

# 静态链接运行库，exe 拷到别的电脑（验收机）也能直接运行
target_link_options(IMSystemXST PRIVATE -static-libgcc -static-libstdc++ -static)

if(WIN32)
    target_link_libraries(IMSystemXST PRIVATE ws2_32)   # TCP 通信（选做）
endif()
