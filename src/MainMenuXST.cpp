#include "MainMenuXST.h"

#include <iostream>
#include <vector>

#include "ConsoleXST.h"
#include "DisplayXST.h"
#include "HomeMenuXST.h"
#include "OpenServiceMenuXST.h"
#include "PlatformXST.h"

MainMenuXST::MainMenuXST(PlatformXST& platform)
    : m_platform(platform), m_session(platform) {}

void MainMenuXST::run() {
    // 通过服务基类访问具体服务，入口不重复实现三套登录逻辑。
    const std::vector<ServiceTypeXST> types = m_platform.serviceTypes();
    while (true) {
        std::cout << "\n==============================\n"
                  << "   腾*立体社交平台  (XST)\n"
                  << "==============================\n"
                  << "请先选择服务，再登录对应账号:\n";
        for (size_t i = 0; i < types.size(); ++i) {
            std::cout << i + 1 << ". " << m_platform.service(types[i]).name() << "\n";
        }
        std::cout << "0. 退出\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(types.size()));
        if (choice == 0) return;
        runService(types[choice - 1]);
    }
}

void MainMenuXST::runService(ServiceTypeXST type) {
    const ServiceXST& service = m_platform.service(type);
    while (true) {
        std::cout << "\n===== " << service.name() << " 服务入口 =====\n"
                  << "1. 登录" << service.name() << "\n"
                  << "2. 注册新用户并开通" << service.name() << "\n"
                  << "3. 平台概览（查看全部用户、账号与群）\n"
                  << "0. 返回服务选择\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, 3);
        switch (choice) {
            case 1: login(type); break;
            case 2: registerUser(type); break;
            case 3: showOverview(); break;
            default: return;
        }
    }
}

void MainMenuXST::showOverview() const {
    DisplayXST::printPlatformOverview(std::cout, m_platform);
    std::cout << "\n提示: 预置账号的密码均为 123456，可用任意账号登录\n";
    ConsoleXST::pause();
}

void MainMenuXST::login(ServiceTypeXST type) {
    const ServiceXST& service = m_platform.service(type);
    std::cout << "\n===== 登录" << service.name() << "（账号留空返回） =====\n";
    std::string id = ConsoleXST::readLine(service.name() + "号: ");
    if (id.empty()) return;
    std::string password = ConsoleXST::readLine("密码: ");
    if (ConsoleXST::eof()) return;

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

void MainMenuXST::registerUser(ServiceTypeXST type) {
    const ServiceXST& service = m_platform.service(type);
    // 沿用服务子类提供的号码规则，微博等共享QQ号码的服务需先建立QQ账号。
    const bool needsQQ = service.idRule() == AccountIdRuleXST::SharedQQNumber;
    std::cout << "\n===== 注册新用户并开通" << service.name() << " =====\n";
    if (needsQQ && !ConsoleXST::confirm(service.name() + "与 QQ 共用号码，是否先开通 QQ 再继续")) {
        return;
    }
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

    const std::string personId = m_platform.nextUserId();
    m_platform.addUser(personId, name, birthday, location);
    if (needsQQ && !OpenServiceMenuXST::openInteractive(m_platform, personId, ServiceTypeXST::QQ)) {
        m_platform.removeUser(personId);
        std::cout << "QQ 未开通，注册未完成。\n";
        return;
    }
    const AccountXST* account = OpenServiceMenuXST::openInteractive(m_platform, personId, type);
    if (!account) {
        if (m_platform.removeUser(personId)) {
            std::cout << "注册未完成。\n";
        } else {
            const AccountXST* qq = m_platform.accountOf(personId, ServiceTypeXST::QQ);
            std::cout << service.name() << "未开通，已开通的 QQ " << qq->id()
                      << " 已保留。可登录 QQ 后在开通管理中继续开通。\n";
        }
        return;
    }
    std::cout << "注册成功！用户编号 " << personId << "，请使用" << account->serviceName()
              << "号 " << account->id() << " 登录。\n";
}
