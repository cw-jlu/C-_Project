#include "GroupMenuXST.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "ConsoleXST.h"
#include "DisplayXST.h"
#include "GroupPolicyFactoryXST.h"
#include "PlatformXST.h"

GroupMenuXST::GroupMenuXST(PlatformXST& platform, LoginManagerXST& session)
    : ServiceMenuXST(platform, session) {}

std::string GroupMenuXST::title() const { return "群管理"; }

std::vector<std::string> GroupMenuXST::items() const {
    return {
        "我的群",
        "查看群信息与成员",
        "申请加入群",
        "邀请好友入群",
        "退出群",
        "踢出成员",
        "设置/取消管理员",
        "创建临时讨论组（子群）",
        "解散临时讨论组",
        "查看群管理特色",
        "切换群管理模式（群主；成员数据保持不变）"
    };
}

void GroupMenuXST::handle(int index) {
    switch (index) {
        case 0: listMyGroups(); break;
        case 1: showGroupInfo(); break;
        case 2: applyJoin(); break;
        case 3: inviteFriend(); break;
        case 4: quitGroup(); break;
        case 5: kickMember(); break;
        case 6: toggleAdmin(); break;
        case 7: createSubGroup(); break;
        case 8: dissolveSubGroup(); break;
        case 9: showFeatures(); break;
        case 10: changePolicy(); break;
        default: break;
    }
}

void GroupMenuXST::listMyGroups() const {
    const AccountXST& me = currentAccount();
    std::vector<const GroupXST*> mine = currentService().groupsOf(me.id());
    std::cout << describe(me) << " 加入的群（共 " << mine.size() << " 个）:\n";
    for (const GroupXST* g : mine) {
        std::cout << "  " << g->id() << "  " << std::left << std::setw(20) << g->name()
                  << std::right << "[" << g->policy().name() << "]  身份: "
                  << groupRoleName(g->roleOf(me.id())) << "  成员 " << g->members().size()
                  << " 人\n";
    }
    std::cout << "本服务所有群: ";
    for (const GroupXST* g : currentService().groups()) std::cout << g->id() << " ";
    std::cout << "\n";
}

void GroupMenuXST::showGroupInfo() const {
    if (const GroupXST* group = chooseGroup()) printGroupInfo(*group);
}

void GroupMenuXST::applyJoin() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    report(currentService().applyJoin(group->id(), currentAccount().id()), *group);
}

void GroupMenuXST::inviteFriend() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    const AccountXST& me = currentAccount();
    std::cout << "可邀请的好友（尚未入群）:";
    bool any = false;
    for (const FriendXST& f : me.friends().items()) {
        if (!group->isMember(f.id())) {
            std::cout << "  " << memberText(f.id());
            any = true;
        }
    }
    std::cout << (any ? "\n" : "  （无）\n");
    std::string id = ConsoleXST::readLine("请输入要邀请的好友" + me.idKindName() + ": ");
    if (id.empty()) return;
    report(currentService().invite(group->id(), me.id(), id), *group);
}

void GroupMenuXST::quitGroup() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    if (!ConsoleXST::confirm("确认退出群 " + group->name())) return;
    report(currentService().quitGroup(group->id(), currentAccount().id()), *group);
}

void GroupMenuXST::kickMember() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    printGroupInfo(*group);
    std::string id = ConsoleXST::readLine("请输入要踢出的成员ID: ");
    if (id.empty()) return;
    report(currentService().kick(group->id(), currentAccount().id(), id), *group);
}

void GroupMenuXST::toggleAdmin() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    printGroupInfo(*group);
    std::string id = ConsoleXST::readLine("请输入成员ID（是管理员则取消，否则设为管理员）: ");
    if (id.empty()) return;
    bool grant = group->adminRecords().count(id) == 0;
    std::cout << (grant ? "将设为管理员: " : "将取消管理员: ") << memberText(id) << "\n";
    report(currentService().setGroupAdmin(group->id(), currentAccount().id(), id, grant), *group);
}

void GroupMenuXST::createSubGroup() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    std::string name = ConsoleXST::readLine("讨论组名称: ");
    std::string ids = ConsoleXST::readLine("成员ID（空格或逗号分隔，自己会自动加入）: ");
    report(currentService().createSubGroup(group->id(), currentAccount().id(), name, splitIds(ids)),
           *group);
}

void GroupMenuXST::dissolveSubGroup() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    std::string name = ConsoleXST::readLine("要解散的讨论组名称: ");
    report(currentService().dissolveSubGroup(group->id(), currentAccount().id(), name), *group);
}

void GroupMenuXST::showFeatures() const {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    std::cout << "群 " << group->id() << "「" << group->name() << "」当前管理模式: "
              << group->policy().name() << "\n";
    printFeatures(group->policy());
}

void GroupMenuXST::changePolicy() {
    const GroupXST* group = chooseGroup();
    if (!group) return;
    std::cout << "切换前:\n";
    printGroupInfo(*group);

    const std::vector<std::string>& codes = GroupPolicyFactoryXST::codes();
    std::cout << "选择新的管理模式:\n";
    for (size_t i = 0; i < codes.size(); ++i) {
        std::unique_ptr<GroupPolicyXST> sample = GroupPolicyFactoryXST::create(codes[i]);
        std::cout << "  " << i + 1 << ". " << sample->name()
                  << (codes[i] == group->policy().code() ? "（当前）" : "") << "\n";
    }
    std::cout << "  0. 取消\n";
    int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(codes.size()));
    if (choice == 0) return;

    GroupOpResultXST result =
        currentService().changeGroupPolicy(group->id(), currentAccount().id(), codes[choice - 1]);
    report(result, *group);
    if (result == GroupOpResultXST::Ok) {
        std::cout << "切换后（群成员数据保持不变）:\n";
        printGroupInfo(*group);
        printFeatures(group->policy());
    }
}

const GroupXST* GroupMenuXST::chooseGroup() const {
    int id = ConsoleXST::readInt("请输入群号（0 取消）: ", 0, 99999999);
    if (id == 0) return nullptr;
    const GroupXST* group = currentService().findGroup(id);
    if (!group) std::cout << groupOpMessage(GroupOpResultXST::GroupNotFound) << "\n";
    return group;
}

void GroupMenuXST::printGroupInfo(const GroupXST& group) const {
    DisplayXST::printGroupInfo(std::cout, currentService(), group);
}

void GroupMenuXST::printFeatures(const GroupPolicyXST& policy) {
    DisplayXST::printFeatures(std::cout, policy);
}

void GroupMenuXST::report(GroupOpResultXST result, const GroupXST& group) const {
    std::cout << groupOpMessage(result);
    if (result == GroupOpResultXST::NotSupported || result == GroupOpResultXST::PermissionDenied) {
        std::cout << "（群 " << group.id() << " 当前为「" << group.policy().name() << "」模式，你的身份: "
                  << groupRoleName(group.roleOf(currentAccount().id())) << "）";
    }
    std::cout << "\n";
}

std::string GroupMenuXST::memberText(const std::string& accountId) const {
    const AccountXST* account = currentService().findAccount(accountId);
    return accountId + "(" + (account ? account->nickname() : "?") + ")";
}

std::vector<std::string> GroupMenuXST::splitIds(const std::string& text) {
    std::string normalized = text;
    for (char& c : normalized) {
        if (c == ',' || c == ';') c = ' ';
    }
    std::istringstream in(normalized);
    std::vector<std::string> ids;
    std::string id;
    while (in >> id) ids.push_back(id);
    return ids;
}
