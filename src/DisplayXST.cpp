#include "DisplayXST.h"

#include <iomanip>
#include <ostream>

#include "PlatformXST.h"
#include "WeChatAccountXST.h"

std::string DisplayXST::describe(const AccountXST& account) {
    return account.serviceName() + " " + account.id() + "(" + account.nickname() + ")";
}

std::string DisplayXST::joinServiceNames(const std::vector<ServiceTypeXST>& types) {
    std::string text;
    for (ServiceTypeXST type : types) {
        if (!text.empty()) text += "、";
        text += serviceDisplayName(type);
    }
    return text.empty() ? "无" : text;
}

std::string DisplayXST::idRuleText(AccountIdRuleXST rule) {
    switch (rule) {
        case AccountIdRuleXST::NewQQNumber:    return "由平台分配新的 QQ 号";
        case AccountIdRuleXST::SharedQQNumber: return "与 QQ 共享号码，需先开通 QQ";
        case AccountIdRuleXST::IndependentId:  return "独立 ID，可与本人 QQ 绑定";
    }
    return "";
}

void DisplayXST::printFriendTable(std::ostream& os, const ServiceXST& service,
                                  const std::vector<FriendXST>& items) {
    if (items.empty()) {
        os << "  （无）\n";
        return;
    }
    os << "  " << std::left << std::setw(16) << "好友ID" << std::setw(18) << "备注" << "昵称\n";
    for (const FriendXST& f : items) {
        const AccountXST* account = service.findAccount(f.id());
        os << "  " << std::setw(16) << f.id() << std::setw(18)
           << (f.remark().empty() ? "-" : f.remark())
           << (account ? account->nickname() : "（账号不存在）") << "\n";
    }
    os << std::right;
}

void DisplayXST::printGroupInfo(std::ostream& os, const ServiceXST& service,
                                const GroupXST& group) {
    const GroupPolicyXST& policy = group.policy();
    os << "群 " << group.id() << "「" << group.name() << "」  管理模式: " << policy.name()
       << "  成员 " << group.members().size() << " 人\n";
    os << "  " << std::left << std::setw(16) << "成员ID" << std::setw(18) << "昵称" << "身份\n";
    for (const std::string& id : group.members()) {
        const AccountXST* account = service.findAccount(id);
        std::string role = groupRoleName(group.roleOf(id));
        if (!policy.supportsAdmins() && group.adminRecords().count(id)) {
            role += "（管理员身份已保留）";
        }
        os << "  " << std::setw(16) << id << std::setw(18) << (account ? account->nickname() : "?")
           << role << "\n";
    }
    os << std::right;

    if (!policy.supportsAdmins() && !group.adminRecords().empty()) {
        os << "  * 当前模式没有管理员制度，已保留 " << group.adminRecords().size()
           << " 条管理员记录，切换回支持的模式后恢复\n";
    }
    const std::vector<SubGroupXST>& subs = group.subGroupRecords();
    if (policy.supportsSubGroups()) {
        os << "  临时讨论组（" << subs.size() << " 个）:\n";
        for (const SubGroupXST& s : subs) {
            os << "    [" << s.name() << "] 发起人 " << s.creatorId() << "  成员: "
               << s.membersText() << "\n";
        }
    } else if (!subs.empty()) {
        os << "  * 当前模式不支持临时讨论组，已保留 " << subs.size()
           << " 个讨论组数据，切换回支持的模式后恢复\n";
    }
}

void DisplayXST::printFeatures(std::ostream& os, const GroupPolicyXST& policy) {
    os << "「" << policy.name() << "」模式特色:\n";
    for (const std::string& line : policy.features()) {
        os << "  - " << line << "\n";
    }
}

void DisplayXST::printPlatformOverview(std::ostream& os, const PlatformXST& platform) {
    os << "\n---------- 用户（" << platform.users().size() << " 人） ----------\n";
    os << "  " << std::left << std::setw(8) << "编号" << std::setw(10) << "姓名" << std::setw(8)
       << "年龄" << std::setw(12) << "所在地" << "已开通\n";
    for (const UserXST* u : platform.users()) {
        os << "  " << std::setw(8) << u->id() << std::setw(10) << u->name() << std::setw(8)
           << (std::to_string(u->age()) + "岁") << std::setw(12) << u->location()
           << u->servicesText() << "\n";
    }

    for (ServiceTypeXST type : platform.serviceTypes()) {
        const ServiceXST& service = platform.service(type);
        os << "\n---------- " << service.name() << "（" << idRuleText(service.idRule())
           << "） ----------\n";
        os << "  " << std::setw(16) << "账号" << std::setw(16) << "昵称" << std::setw(10)
           << "所属" << std::setw(8) << "T龄" << std::setw(8) << "好友" << "群\n";
        for (const AccountXST* a : service.accounts()) {
            std::string owner = a->owner().name();
            if (const auto* wx = dynamic_cast<const WeChatAccountXST*>(a)) {
                owner += wx->isBound() ? "" : "*";
            }
            os << "  " << std::setw(16) << a->id() << std::setw(16) << a->nickname()
               << std::setw(10) << owner << std::setw(8) << (std::to_string(a->tAge()) + "年")
               << std::setw(8) << a->friends().size() << a->groups().size() << "\n";
        }
        if (type == ServiceTypeXST::WeChat) os << "  （* 表示未绑定 QQ）\n";
        os << "  " << std::setw(8) << "群号" << std::setw(20) << "群名" << std::setw(10) << "模式"
           << std::setw(16) << "群主" << "人数\n";
        for (const GroupXST* g : service.groups()) {
            os << "  " << std::setw(8) << g->id() << std::setw(20) << g->name() << std::setw(10)
               << g->policy().name() << std::setw(16) << g->ownerId() << g->members().size()
               << "\n";
        }
    }
    os << std::right;
}
