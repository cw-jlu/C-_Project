// 模拟即时通信系统 —— 程序入口
// 启动时装入演示数据（与 data/ 下文件一致，预置账号密码均为 123456），进入平台入口菜单

#include <iostream>
#include <stdexcept>

#include "ConsoleXST.h"
#include "DemoDataXST.h"
#include "MainMenuXST.h"
#include "PlatformXST.h"

int main() {
    ConsoleXST::init();

    PlatformXST platform;
    try {
        DemoDataXST::seed(platform);
    } catch (const std::exception& e) {
        std::cout << "演示数据初始化失败: " << e.what() << "\n";
        return 1;
    }

    MainMenuXST(platform).run();

    std::cout << "再见！\n";
    return 0;
}
