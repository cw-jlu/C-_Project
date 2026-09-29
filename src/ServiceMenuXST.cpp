#include "ServiceMenuXST.h"

#include <iostream>
#include <stdexcept>

#include "ConsoleXST.h"
#include "LoginManagerXST.h"
#include "PlatformXST.h"

ServiceMenuXST::ServiceMenuXST(PlatformXST& platform, LoginManagerXST& session)
    : m_platform(platform), m_session(session), m_current(session.primaryService()) {
    if (!m_session.isActive()) throw std::logic_error("尚未登录");
}

void ServiceMenuXST::run() {
    const std::vector<std::string> options = items();
    while (true) {
        std::cout << "\n===== " << title() << "  用户: " << user().name()
                  << "  当前: " << describe(currentAccount()) << " =====\n"
                  << "1. 切换服务\n";
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << i + 2 << ". " << options[i] << "\n";
        }
        std::cout << "0. 返回\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(options.size()) + 1);
        if (choice == 0) return;
        if (choice == 1) {
            switchService();
        } else {
            handle(choice - 2);
        }
    }
}

PlatformXST& ServiceMenuXST::platform() const { return m_platform; }
LoginManagerXST& ServiceMenuXST::session() const { return m_session; }
const UserXST& ServiceMenuXST::user() const { return *m_session.currentUser(); }
ServiceTypeXST ServiceMenuXST::currentType() const { return m_current; }
ServiceXST& ServiceMenuXST::currentService() const { return m_platform.service(m_current); }

AccountXST& ServiceMenuXST::currentAccount() const {
    return *m_platform.accountOf(user().id(), m_current);
}

bool ServiceMenuXST::chooseService(const std::string& prompt, bool excludeCurrent,
                                   ServiceTypeXST& out) const {
    std::vector<ServiceTypeXST> options;
    for (ServiceTypeXST type : user().services()) {
        if (excludeCurrent && type == m_current) continue;
        options.push_back(type);
    }
    if (options.empty()) {
        std::cout << "没有可选择的其他服务（请先开通更多服务）。\n";
        return false;
    }
    std::cout << prompt << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << "  " << i + 1 << ". "
                  << describe(*m_platform.accountOf(user().id(), options[i]))
                  << (m_session.isLoggedIn(options[i]) ? "  [已登录]" : "  [未登录]") << "\n";
    }
    std::cout << "  0. 取消\n";
    int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(options.size()));
    if (choice == 0) return false;
    out = options[choice - 1];
    return true;
}

void ServiceMenuXST::switchService() {
    ServiceTypeXST type;
    if (!chooseService("选择要切换到的服务:", false, type)) return;
    if (!m_session.isLoggedIn(type)) {
        // 已通过其他服务登录，简单确认即可登录，无需密码
        if (!ConsoleXST::confirm("尚未登录" + serviceDisplayName(type) + "，确认登录")) return;
        std::cout << loginMessage(m_session.confirm(type)) << "\n";
        if (!m_session.isLoggedIn(type)) return;
    }
    m_current = type;
}

std::string ServiceMenuXST::describe(const AccountXST& account) {
    return account.serviceName() + " " + account.id() + "(" + account.nickname() + ")";
}
