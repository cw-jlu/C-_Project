#include "ConsoleXST.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

bool ConsoleXST::s_eof = false;

void ConsoleXST::init() {
#ifdef _WIN32
    SetConsoleCP(936);         // 输入：GBK
    SetConsoleOutputCP(936);   // 输出：GBK
#endif
}

std::string ConsoleXST::readLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string line;
    if (s_eof || !std::getline(std::cin, line)) {
        s_eof = true;
        return "";
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return line;
}

int ConsoleXST::readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::string text = readLine(prompt);
        if (s_eof) return minValue;
        try {
            size_t used = 0;
            int value = std::stoi(text, &used);
            if (used == text.size() && value >= minValue && value <= maxValue) {
                return value;
            }
        } catch (...) {
            // 非数字，落到下面的提示
        }
        std::cout << "输入无效，请输入 " << minValue << " ~ " << maxValue << " 之间的整数。\n";
    }
}

bool ConsoleXST::confirm(const std::string& prompt) {
    std::string answer = readLine(prompt + " (y/n): ");
    return answer == "y" || answer == "Y";
}

void ConsoleXST::pause() {
    readLine("按回车键继续...");
}

void ConsoleXST::clear() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

bool ConsoleXST::eof() {
    return s_eof;
}
