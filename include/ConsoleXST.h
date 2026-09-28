#ifndef CONSOLE_XST_H
#define CONSOLE_XST_H

#include <string>

// 控制台输入输出工具类（纯静态）
// - 程序内部字符串统一为 GBK，启动时把控制台固定为代码页 936，
//   不受终端默认编码（如 VS Code 终端的 UTF-8）影响
// - 封装菜单常用的整行读取、整数校验、是/否确认
class ConsoleXST {
public:
    ConsoleXST() = delete;

    // 程序启动时调用一次
    static void init();

    // 读取一整行（不含换行符）；输入结束时返回空串并置 eof
    static std::string readLine(const std::string& prompt = "");

    // 读取 [minValue, maxValue] 范围内的整数，非法输入会重新提示；
    // 输入结束时返回 minValue（菜单约定 0 为“返回/退出”）
    static int readInt(const std::string& prompt, int minValue, int maxValue);

    // 询问是/否，输入 y/Y 返回 true
    static bool confirm(const std::string& prompt);

    static void pause();
    static void clear();
    static bool eof();

private:
    static bool s_eof;
};

#endif
