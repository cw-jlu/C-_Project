#include "FriendMenuXST.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "ConsoleXST.h"
#include "PlatformXST.h"

FriendMenuXST::FriendMenuXST(PlatformXST& platform, const std::string& personId)
    : m_platform(platform), m_manager(platform), m_user(platform.findUser(personId)),
      m_current(ServiceTypeXST::QQ) {
    if (!m_user) throw std::invalid_argument("用户不存在: " + personId);
    if (!m_user->services().empty()) m_current = *m_user->services().begin();
}

AccountXST& FriendMenuXST::currentAccount() const {
    return *m_platform.accountOf(m_user->id(), m_current);
}

ServiceXST& FriendMenuXST::currentService() const {
    return m_platform.service(m_current);
}

void FriendMenuXST::run() {
    if (m_user->services().empty()) {
        std::cout << m_user->name() << " 尚未开通任何服务。\n";
        return;
    }
    while (true) {
        std::cout << "\n===== 好友管理  用户: " << m_user->name()
                  << "  当前: " << describe(currentAccount()) << " =====\n"
                  << "1. 切换服务\n"
                  << "2. 查看好友列表\n"
                  << "3. 添加好友\n"
                  << "4. 修改好友备注\n"
                  << "5. 删除好友\n"
                  << "6. 查询好友（ID/备注/昵称）\n"
                  << "7. 与某人的共同好友（本服务内）\n"
                  << "8. 跨服务共同好友（两个服务中都是好友的人）\n"
                  << "9. 从其他服务添加好友（如：微信添加QQ推荐好友）\n"
                  << "0. 返回\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, 9);
        switch (choice) {
            case 1: switchService(); break;
            case 2: listFriends(); break;
            case 3: addFriend(); break;
            case 4: editRemark(); break;
            case 5: removeFriend(); break;
            case 6: searchFriends(); break;
            case 7: showCommonWithin(); break;
            case 8: showCommonAcross(); break;
            case 9: addFromOtherService(); break;
            default: return;
        }
    }
}

bool FriendMenuXST::chooseService(const std::string& title, bool excludeCurrent,
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
    std::cout << title << "\n";
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

void FriendMenuXST::switchService() {
    ServiceTypeXST type;
    if (chooseService("选择要切换到的服务:", false, type)) m_current = type;
}

void FriendMenuXST::listFriends() const {
    const AccountXST& me = currentAccount();
    std::cout << describe(me) << " 的好友（共 " << me.friends().size() << " 人）:\n";
    printFriendTable(me.friends().items());
}

void FriendMenuXST::addFriend() {
    const AccountXST& me = currentAccount();
    std::string id = ConsoleXST::readLine("请输入对方" + me.idKindName() + ": ");
    if (id.empty()) return;
    const AccountXST* other = currentService().findAccount(id);
    if (other) {
        std::cout << "找到账号: " << describe(*other) << "\n";
        if (!ConsoleXST::confirm("确认添加为好友")) return;
    }
    std::string remark = ConsoleXST::readLine("备注（可留空）: ");
    FriendOpResultXST result = currentService().addFriendship(me.id(), id, remark);
    std::cout << friendOpMessage(result) << "\n";
}

void FriendMenuXST::editRemark() {
    const AccountXST& me = currentAccount();
    std::string id = ConsoleXST::readLine("请输入好友" + me.idKindName() + ": ");
    if (id.empty()) return;
    if (!me.hasFriend(id)) {
        std::cout << friendOpMessage(FriendOpResultXST::NotFriends) << "\n";
        return;
    }
    std::cout << "当前备注: " << me.findFriend(id)->displayName() << "\n";
    std::string remark = ConsoleXST::readLine("新备注（留空则清除备注）: ");
    std::cout << friendOpMessage(currentService().setFriendRemark(me.id(), id, remark)) << "\n";
}

void FriendMenuXST::removeFriend() {
    const AccountXST& me = currentAccount();
    std::string id = ConsoleXST::readLine("请输入要删除的好友" + me.idKindName() + ": ");
    if (id.empty()) return;
    if (!me.hasFriend(id)) {
        std::cout << friendOpMessage(FriendOpResultXST::NotFriends) << "\n";
        return;
    }
    if (!ConsoleXST::confirm("确认删除好友 " + me.findFriend(id)->displayName() + "（双方均解除）")) {
        return;
    }
    std::cout << friendOpMessage(currentService().removeFriendship(me.id(), id)) << "\n";
}

void FriendMenuXST::searchFriends() const {
    std::string keyword = ConsoleXST::readLine("请输入关键字: ");
    if (keyword.empty()) return;
    std::vector<FriendXST> result;
    for (const FriendXST& f : currentAccount().friends().items()) {
        const AccountXST* account = currentService().findAccount(f.id());
        bool nicknameHit = account && account->nickname().find(keyword) != std::string::npos;
        if (f.id().find(keyword) != std::string::npos ||
            f.remark().find(keyword) != std::string::npos || nicknameHit) {
            result.push_back(f);
        }
    }
    std::cout << "找到 " << result.size() << " 位好友:\n";
    printFriendTable(result);
}

void FriendMenuXST::showCommonWithin() const {
    const AccountXST& me = currentAccount();
    std::string id = ConsoleXST::readLine("请输入对方" + me.idKindName() + ": ");
    if (id.empty()) return;
    const AccountXST* other = currentService().findAccount(id);
    if (!other) {
        std::cout << friendOpMessage(FriendOpResultXST::FriendNotFound) << "\n";
        return;
    }
    std::vector<const AccountXST*> common = currentService().commonFriends(me.id(), id);
    std::cout << "你与 " << describe(*other) << " 的共同好友（共 " << common.size() << " 人）:\n";
    for (const AccountXST* account : common) {
        std::cout << "  " << describe(*account) << "\n";
    }
}

void FriendMenuXST::showCommonAcross() const {
    ServiceTypeXST other;
    if (!chooseService("与哪个服务比较:", true, other)) return;
    std::vector<AccountLinkXST> links = m_manager.commonFriendsAcross(m_user->id(), m_current, other);
    std::cout << serviceDisplayName(m_current) << " 与 " << serviceDisplayName(other)
              << " 中都是你好友的人（共 " << links.size() << " 人）:\n";
    for (const AccountLinkXST& link : links) {
        std::cout << "  " << describe(link.source()) << "  <->  " << describe(link.target())
                  << "  [" << link.source().owner().name() << "]\n";
    }
}

void FriendMenuXST::addFromOtherService() {
    ServiceTypeXST source;
    if (!chooseService("从哪个服务的好友中推荐:", true, source)) return;
    while (true) {
        std::vector<AccountLinkXST> links = m_manager.recommend(m_user->id(), m_current, source);
        if (links.empty()) {
            std::cout << "没有可推荐的好友（对方需在" << serviceDisplayName(m_current)
                      << "中有对应 QQ 号的账号，且尚未成为好友）。\n";
            return;
        }
        std::cout << "来自" << serviceDisplayName(source) << "好友的推荐:\n";
        for (size_t i = 0; i < links.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << describe(links[i].source())
                      << "  ->  " << describe(links[i].target()) << "\n";
        }
        std::cout << "  0. 返回\n";
        int choice = ConsoleXST::readInt("选择要添加的好友: ", 0, static_cast<int>(links.size()));
        if (choice == 0) return;
        FriendOpResultXST result = m_manager.acceptRecommendation(m_user->id(), m_current,
                                                                  links[choice - 1]);
        std::cout << friendOpMessage(result) << "\n";
    }
}

void FriendMenuXST::printFriendTable(const std::vector<FriendXST>& items) const {
    if (items.empty()) {
        std::cout << "  （无）\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(16) << "好友ID" << std::setw(18) << "备注"
              << "昵称\n";
    for (const FriendXST& f : items) {
        const AccountXST* account = currentService().findAccount(f.id());
        std::cout << "  " << std::setw(16) << f.id()
                  << std::setw(18) << (f.remark().empty() ? "-" : f.remark())
                  << (account ? account->nickname() : "（账号不存在）") << "\n";
    }
    std::cout << std::right;
}

std::string FriendMenuXST::describe(const AccountXST& account) {
    return account.serviceName() + " " + account.id() + "(" + account.nickname() + ")";
}
