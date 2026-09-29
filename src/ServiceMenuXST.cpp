#include "ServiceMenuXST.h"

#include <iostream>
#include <stdexcept>

#include "ConsoleXST.h"
#include "PlatformXST.h"

ServiceMenuXST::ServiceMenuXST(PlatformXST& platform, const std::string& personId)
    : m_platform(platform), m_user(platform.findUser(personId)), m_current(ServiceTypeXST::QQ) {
    if (!m_user) throw std::invalid_argument("用户不存在: " + personId);
    if (!m_user->services().empty()) m_current = *m_user->services().begin();
}

void ServiceMenuXST::run() {
    if (m_user->services().empty()) {
        std::cout << m_user->name() << " 尚未开通任何服务。\n";
        return;
    }
    const std::vector<std::string> options = items();
    while (true) {
        std::cout << "\n===== " << title() << "  用户: " << m_user->name()
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
const UserXST& ServiceMenuXST::user() const { return *m_user; }
ServiceTypeXST ServiceMenuXST::currentType() const { return m_current; }
ServiceXST& ServiceMenuXST::currentService() const { return m_platform.service(m_current); }

AccountXST& ServiceMenuXST::currentAccount() const {
    return *m_platform.accountOf(m_user->id(), m_current);
}

bool ServiceMenuXST::chooseService(const std::string& prompt, bool excludeCurrent,
                                   ServiceTypeXST& out) const {
    std::vector<ServiceTypeXST> options;
    for (ServiceTypeXST type : m_user->services()) {
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
                  << describe(*m_platform.accountOf(m_user->id(), options[i])) << "\n";
    }
    std::cout << "  0. 取消\n";
    int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(options.size()));
    if (choice == 0) return false;
    out = options[choice - 1];
    return true;
}

void ServiceMenuXST::switchService() {
    ServiceTypeXST type;
    if (chooseService("选择要切换到的服务:", false, type)) m_current = type;
}

std::string ServiceMenuXST::describe(const AccountXST& account) {
    return account.serviceName() + " " + account.id() + "(" + account.nickname() + ")";
}
