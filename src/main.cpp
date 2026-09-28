// 模拟即时通信系统 —— 程序入口
// P0：环境自检（中文输出 / 中文输入 / 数字菜单）

#include <iostream>
#include <string>

#include "ConsoleXST.h"

int main() {
    ConsoleXST::init();

    std::cout << "==============================\n";
    std::cout << "   腾*立体社交平台  (XST)\n";
    std::cout << "==============================\n";

    std::string nickname = ConsoleXST::readLine("请输入昵称（可输入中文）: ");
    std::cout << "你好，" << nickname << "！\n";

    while (true) {
        std::cout << "\n1. 再打个招呼\n0. 退出\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, 1);
        if (choice == 0) break;
        std::cout << "你好，" << nickname << "～\n";
    }

    std::cout << "再见！\n";
    return 0;
}
