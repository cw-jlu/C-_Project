#include "OpenServiceMenuXST.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "ConsoleXST.h"
#include "LoginManagerXST.h"
#include "PlatformXST.h"

OpenServiceMenuXST::OpenServiceMenuXST(PlatformXST& platform, LoginManagerXST& session)
    : m_platform(platform), m_session(session) {}

void OpenServiceMenuXST::run() {
    const UserXST& user = *m_session.currentUser();
    while (true) {
        std::cout << "\n===== 开通管理  用户: " << user.name() << " =====\n";
        std::vector<ServiceTypeXST> types = m_platform.serviceTypes();
        for (size_t i = 0; i < types.size(); ++i) {
            const AccountXST* account = m_platform.accountOf(user.id(), types[i]);
            std::cout << i + 1 << ". " << serviceDisplayName(types[i]) << "  ";
            if (account) {
                std::cout << "已开通 " << account->id() << "(" << account->nickname() << ")"
                          << (m_session.isLoggedIn(types[i]) ? "  [已登录]" : "  [未登录]");
            } else {
                std::cout << "未开通";
            }
            std::cout << "\n";
        }
        std::cout << "0. 返回\n";
        int choice = ConsoleXST::readInt("选择要开通的服务: ", 0, static_cast<int>(types.size()));
        if (choice == 0) return;

        ServiceTypeXST type = types[choice - 1];
        if (m_platform.accountOf(user.id(), type)) {
            std::cout << "你已开通" << serviceDisplayName(type) << "。\n";
            continue;
        }
        if (!openInteractive(m_platform, user.id(), type)) continue;
        // 已有服务处于登录状态，新开通的服务简单确认即可登录
        if (ConsoleXST::confirm("是否立即登录" + serviceDisplayName(type) + "（无需再输入密码）")) {
            std::cout << loginMessage(m_session.confirm(type)) << "\n";
        }
    }
}

const AccountXST* OpenServiceMenuXST::openInteractive(PlatformXST& platform,
                                                      const std::string& personId,
                                                      ServiceTypeXST type) {
    const UserXST* user = platform.findUser(personId);
    if (!user) return nullptr;
    const ServiceXST& service = platform.service(type);
    const AccountXST* qq = platform.accountOf(personId, ServiceTypeXST::QQ);

    std::string id;
    std::string extra;
    switch (service.idRule()) {
        case AccountIdRuleXST::NewQQNumber:
            id = platform.nextQQNumber();
            std::cout << "系统为你分配的" << service.name() << "号: " << id << "\n";
            break;
        case AccountIdRuleXST::SharedQQNumber:
            if (!qq) {
                std::cout << service.name() << "与 QQ 共用号码，请先开通 QQ。\n";
                return nullptr;
            }
            id = qq->id();
            std::cout << service.name() << "将使用你的 QQ 号: " << id << "\n";
            break;
        case AccountIdRuleXST::IndependentId:
            id = ConsoleXST::readLine("请设置" + service.name() +
                                      "号（6~20 位，字母开头，可含字母/数字/_/-）: ");
            if (id.empty()) return nullptr;
            if (qq && ConsoleXST::confirm("是否与你的 QQ " + qq->id() + " 绑定")) extra = qq->id();
            break;
    }

    std::string nickname = ConsoleXST::readLine("昵称（留空则使用姓名）: ");
    if (nickname.empty()) nickname = user->name();
    std::string password = ConsoleXST::readLine("设置密码（6~16 位）: ");
    if (ConsoleXST::readLine("确认密码: ") != password) {
        std::cout << "两次输入的密码不一致，开通取消。\n";
        return nullptr;
    }

    try {
        const AccountXST& account =
            platform.openService(personId, type, id, nickname, password, DateXST::today(), extra);
        std::cout << "开通成功: " << account.serviceName() << " " << account.id() << "("
                  << account.nickname() << ")\n";
        return &account;
    } catch (const std::exception& e) {
        std::cout << "开通失败: " << e.what() << "\n";
        return nullptr;
    }
}
