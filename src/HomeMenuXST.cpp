#include "HomeMenuXST.h"

#include <iostream>
#include <vector>

#include "ConsoleXST.h"
#include "DisplayXST.h"
#include "FriendMenuXST.h"
#include "GroupMenuXST.h"
#include "LoginManagerXST.h"
#include "OpenServiceMenuXST.h"
#include "PlatformXST.h"

namespace {
std::string joinNames(const std::vector<ServiceTypeXST>& types) {
    return DisplayXST::joinServiceNames(types);
}
}

HomeMenuXST::HomeMenuXST(PlatformXST& platform, LoginManagerXST& session)
    : m_platform(platform), m_session(session) {}

void HomeMenuXST::run() {
    while (true) {
        const UserXST& user = *m_session.currentUser();
        std::cout << "\n===== " << user.name() << " (" << user.id() << ")  " << statusLine()
                  << " =====\n"
                  << "1. 好友管理\n"
                  << "2. 群管理\n"
                  << "3. 开通管理（开通新的微X服务）\n"
                  << "4. 登录状态（确认登录其他服务）\n"
                  << "5. 我的资料\n"
                  << "0. 注销\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, 5);
        switch (choice) {
            case 1: FriendMenuXST(m_platform, m_session).run(); break;
            case 2: GroupMenuXST(m_platform, m_session).run(); break;
            case 3: OpenServiceMenuXST(m_platform, m_session).run(); break;
            case 4: manageLogin(); break;
            case 5: showProfile(); break;
            default: return;
        }
    }
}

void HomeMenuXST::manageLogin() {
    while (true) {
        const UserXST& user = *m_session.currentUser();
        std::cout << "\n各服务登录状态（" << serviceDisplayName(m_session.primaryService())
                  << " 为密码登录）:\n";
        for (ServiceTypeXST type : user.services()) {
            const AccountXST* account = m_platform.accountOf(user.id(), type);
            std::cout << "  " << serviceDisplayName(type) << " " << account->id() << "  "
                      << (m_session.isLoggedIn(type) ? "已登录" : "待确认") << "\n";
        }
        std::vector<ServiceTypeXST> pending = m_session.pendingServices();
        if (pending.empty()) {
            std::cout << "所有已开通的服务均已登录。\n";
            return;
        }
        std::cout << "确认登录（无需密码）:\n";
        for (size_t i = 0; i < pending.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << serviceDisplayName(pending[i]) << "\n";
        }
        const int all = static_cast<int>(pending.size()) + 1;
        std::cout << "  " << all << ". 全部确认\n  0. 返回\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, all);
        if (choice == 0) return;
        if (choice == all) {
            std::cout << "已自动登录: " << joinNames(m_session.confirmAll()) << "\n";
        } else {
            std::cout << loginMessage(m_session.confirm(pending[choice - 1])) << "\n";
        }
    }
}

void HomeMenuXST::showProfile() const {
    const UserXST& user = *m_session.currentUser();
    std::cout << "\n姓名: " << user.name() << "  编号: " << user.id() << "  " << user.age()
              << " 岁  出生: " << user.birthday() << "  所在地: " << user.location() << "\n"
              << "已开通: " << user.servicesText() << "\n\n";
    for (ServiceTypeXST type : user.services()) {
        m_platform.accountOf(user.id(), type)->printInfo(std::cout);
        std::cout << "\n";
    }
}

std::string HomeMenuXST::statusLine() const {
    return "已登录: " + joinNames(m_session.loggedInServices()) +
           "  待确认: " + joinNames(m_session.pendingServices());
}
