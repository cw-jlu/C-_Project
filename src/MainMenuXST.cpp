#include "MainMenuXST.h"

#include <iostream>
#include <vector>

#include "ConsoleXST.h"
#include "DisplayXST.h"
#include "HomeMenuXST.h"
#include "OpenServiceMenuXST.h"
#include "PlatformXST.h"
#include "ShowcaseMenuXST.h"

namespace {
// 从平台已注册的服务中选择一个，0 取消
bool chooseServiceType(const PlatformXST& platform, const std::string& prompt,
                       ServiceTypeXST& out) {
    std::vector<ServiceTypeXST> types = platform.serviceTypes();
    std::cout << prompt << "\n";
    for (size_t i = 0; i < types.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << serviceDisplayName(types[i]) << "\n";
    }
    std::cout << "  0. 取消\n";
    int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(types.size()));
    if (choice == 0) return false;
    out = types[choice - 1];
    return true;
}
}

MainMenuXST::MainMenuXST(PlatformXST& platform)
    : m_platform(platform), m_session(platform) {}

void MainMenuXST::run() {
    while (true) {
        std::cout << "\n==============================\n"
                  << "   腾*立体社交平台  (XST)\n"
                  << "==============================\n"
                  << "1. 登录\n"
                  << "2. 注册新用户\n"
                  << "3. 功能展示（按题目要求逐项演示）\n"
                  << "4. 平台概览（查看全部用户、账号与群）\n"
                  << "0. 退出\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, 4);
        switch (choice) {
            case 1: login(); break;
            case 2: registerUser(); break;
            case 3: ShowcaseMenuXST().run(); break;
            case 4: showOverview(); break;
            default: return;
        }
    }
}

void MainMenuXST::showOverview() const {
    DisplayXST::printPlatformOverview(std::cout, m_platform);
    std::cout << "\n提示: 预置账号的密码均为 123456，可用任意账号登录\n";
    ConsoleXST::pause();
}

void MainMenuXST::login() {
    ServiceTypeXST type;
    if (!chooseServiceType(m_platform, "选择要登录的服务:", type)) return;
    std::string id = ConsoleXST::readLine(serviceDisplayName(type) + "号: ");
    std::string password = ConsoleXST::readLine("密码: ");

    LoginResultXST result = m_session.login(type, id, password);
    std::cout << loginMessage(result) << "\n";
    if (result != LoginResultXST::Ok) return;

    const UserXST& user = *m_session.currentUser();
    std::cout << "欢迎你，" << user.name() << "！\n";
    std::vector<ServiceTypeXST> pending = m_session.pendingServices();
    if (!pending.empty()) {
        std::cout << "你还开通了:";
        for (ServiceTypeXST t : pending) {
            std::cout << "  " << serviceDisplayName(t) << " " << m_platform.accountOf(user.id(), t)->id();
        }
        std::cout << "\n";
        if (ConsoleXST::confirm("已登录" + serviceDisplayName(type) + "，是否确认自动登录以上服务")) {
            m_session.confirmAll();
            std::cout << "已全部登录。\n";
        } else {
            std::cout << "可稍后在“登录状态”中确认登录。\n";
        }
    }

    HomeMenuXST(m_platform, m_session).run();
    m_session.logout();
    std::cout << user.name() << " 已注销。\n";
}

void MainMenuXST::registerUser() {
    std::cout << "\n===== 注册新用户 =====\n";
    std::string name = ConsoleXST::readLine("姓名: ");
    if (name.empty()) return;

    DateXST birthday;
    while (true) {
        std::string text = ConsoleXST::readLine("出生日期（YYYY-MM-DD）: ");
        if (text.empty()) return;
        if (DateXST::tryParse(text, birthday) && birthday <= DateXST::today()) break;
        std::cout << "日期无效，请重新输入。\n";
    }
    std::string location = ConsoleXST::readLine("所在地: ");

    ServiceTypeXST type;
    if (!chooseServiceType(m_platform, "选择要开通的第一个服务（微博与 QQ 共用号码，需先开通 QQ）:", type)) {
        return;
    }

    const std::string personId = m_platform.nextUserId();
    m_platform.addUser(personId, name, birthday, location);
    const AccountXST* account = OpenServiceMenuXST::openInteractive(m_platform, personId, type);
    if (!account) {
        m_platform.removeUser(personId);   // 未能开通任何服务，撤销注册
        std::cout << "注册未完成。\n";
        return;
    }
    std::cout << "注册成功！用户编号 " << personId << "，请使用" << account->serviceName()
              << "号 " << account->id() << " 登录。\n";
}
