#include "FriendMenuXST.h"

#include <iomanip>
#include <iostream>

#include "ConsoleXST.h"
#include "PlatformXST.h"

FriendMenuXST::FriendMenuXST(PlatformXST& platform, LoginManagerXST& session)
    : ServiceMenuXST(platform, session), m_manager(platform) {}

std::string FriendMenuXST::title() const { return "好友管理"; }

std::vector<std::string> FriendMenuXST::items() const {
    return {
        "查看好友列表",
        "添加好友",
        "修改好友备注",
        "删除好友",
        "查询好友（ID/备注/昵称）",
        "与某人的共同好友（本服务内）",
        "跨服务共同好友（两个服务中都是好友的人）",
        "从其他服务添加好友（如：微信添加QQ推荐好友）"
    };
}

void FriendMenuXST::handle(int index) {
    switch (index) {
        case 0: listFriends(); break;
        case 1: addFriend(); break;
        case 2: editRemark(); break;
        case 3: removeFriend(); break;
        case 4: searchFriends(); break;
        case 5: showCommonWithin(); break;
        case 6: showCommonAcross(); break;
        case 7: addFromOtherService(); break;
        default: break;
    }
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
    std::vector<AccountLinkXST> links =
        m_manager.commonFriendsAcross(user().id(), currentType(), other);
    std::cout << serviceDisplayName(currentType()) << " 与 " << serviceDisplayName(other)
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
        std::vector<AccountLinkXST> links = m_manager.recommend(user().id(), currentType(), source);
        if (links.empty()) {
            std::cout << "没有可推荐的好友（对方需在" << serviceDisplayName(currentType())
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
        FriendOpResultXST result =
            m_manager.acceptRecommendation(user().id(), currentType(), links[choice - 1]);
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
