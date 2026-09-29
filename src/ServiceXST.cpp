#include "ServiceXST.h"

#include <stdexcept>

#include "GroupPolicyFactoryXST.h"
#include "UserXST.h"

std::string ServiceXST::name() const {
    return serviceDisplayName(type());
}

AccountXST& ServiceXST::addAccount(std::unique_ptr<AccountXST> account) {
    if (!account) throw std::invalid_argument("账号为空");
    if (account->serviceType() != type()) {
        throw std::invalid_argument("账号类型与服务不符: " + account->id());
    }
    if (m_accounts.count(account->id())) {
        throw std::invalid_argument(name() + " 账号已存在: " + account->id());
    }
    if (findAccountByOwner(account->owner().id())) {
        throw std::invalid_argument(account->owner().name() + " 已开通" + name());
    }
    AccountXST& ref = *account;
    m_accounts[account->id()] = std::move(account);
    return ref;
}

AccountXST* ServiceXST::findAccount(const std::string& id) {
    auto it = m_accounts.find(id);
    return it == m_accounts.end() ? nullptr : it->second.get();
}

const AccountXST* ServiceXST::findAccount(const std::string& id) const {
    auto it = m_accounts.find(id);
    return it == m_accounts.end() ? nullptr : it->second.get();
}

AccountXST* ServiceXST::findAccountByOwner(const std::string& personId) {
    for (auto& entry : m_accounts) {
        if (entry.second->owner().id() == personId) return entry.second.get();
    }
    return nullptr;
}

const AccountXST* ServiceXST::findAccountByOwner(const std::string& personId) const {
    for (const auto& entry : m_accounts) {
        if (entry.second->owner().id() == personId) return entry.second.get();
    }
    return nullptr;
}

const AccountXST* ServiceXST::findAccountByQQ(const std::string& qq) const {
    if (qq.empty()) return nullptr;
    for (const auto& entry : m_accounts) {
        if (entry.second->linkedQQ() == qq) return entry.second.get();
    }
    return nullptr;
}

std::vector<const AccountXST*> ServiceXST::accounts() const {
    std::vector<const AccountXST*> result;
    result.reserve(m_accounts.size());
    for (const auto& entry : m_accounts) result.push_back(entry.second.get());
    return result;
}

size_t ServiceXST::accountCount() const { return m_accounts.size(); }

FriendOpResultXST ServiceXST::addFriendship(const std::string& a, const std::string& b,
                                            const std::string& remarkByA,
                                            const std::string& remarkByB) {
    AccountXST* first = findAccount(a);
    if (!first) return FriendOpResultXST::AccountNotFound;
    if (a == b) return FriendOpResultXST::SelfNotAllowed;
    if (!first->isValidAccountId(b)) return FriendOpResultXST::InvalidId;
    AccountXST* second = findAccount(b);
    if (!second) return FriendOpResultXST::FriendNotFound;
    if (first->hasFriend(b) && second->hasFriend(a)) return FriendOpResultXST::AlreadyFriends;

    // 任意一侧缺失都补齐，保证关系双向一致
    if (!first->hasFriend(b)) first->addFriend(b, remarkByA);
    if (!second->hasFriend(a)) second->addFriend(a, remarkByB);
    return FriendOpResultXST::Ok;
}

FriendOpResultXST ServiceXST::removeFriendship(const std::string& a, const std::string& b) {
    AccountXST* first = findAccount(a);
    if (!first) return FriendOpResultXST::AccountNotFound;
    if (!first->removeFriend(b)) return FriendOpResultXST::NotFriends;
    if (AccountXST* second = findAccount(b)) second->removeFriend(a);
    return FriendOpResultXST::Ok;
}

FriendOpResultXST ServiceXST::setFriendRemark(const std::string& owner, const std::string& friendId,
                                              const std::string& remark) {
    AccountXST* account = findAccount(owner);
    if (!account) return FriendOpResultXST::AccountNotFound;
    return account->setFriendRemark(friendId, remark) ? FriendOpResultXST::Ok
                                                      : FriendOpResultXST::NotFriends;
}

std::vector<const AccountXST*> ServiceXST::commonFriends(const std::string& a,
                                                         const std::string& b) const {
    std::vector<const AccountXST*> result;
    const AccountXST* first = findAccount(a);
    const AccountXST* second = findAccount(b);
    if (!first || !second) return result;
    for (const FriendXST& f : first->friends().items()) {
        if (second->hasFriend(f.id())) {
            if (const AccountXST* common = findAccount(f.id())) result.push_back(common);
        }
    }
    return result;
}

// ============================== 群 ==============================

GroupXST& ServiceXST::requireGroup(int groupId) {
    GroupXST* group = findGroup(groupId);
    if (!group) throw std::invalid_argument(name() + " 群不存在: " + std::to_string(groupId));
    return *group;
}

AccountXST& ServiceXST::requireAccount(const std::string& id) {
    AccountXST* account = findAccount(id);
    if (!account) throw std::invalid_argument(name() + " 账号不存在: " + id);
    return *account;
}

GroupXST& ServiceXST::createGroup(int groupId, const std::string& groupName,
                                  const std::string& ownerId) {
    if (m_groups.count(groupId)) {
        throw std::invalid_argument(name() + " 群号已存在: " + std::to_string(groupId));
    }
    AccountXST& owner = requireAccount(ownerId);
    auto group = std::make_unique<GroupXST>(groupId, groupName, ownerId,
                                            createDefaultGroupPolicy());
    GroupXST& ref = *group;
    m_groups[groupId] = std::move(group);
    owner.joinGroup(groupId);
    return ref;
}

GroupXST* ServiceXST::findGroup(int groupId) {
    auto it = m_groups.find(groupId);
    return it == m_groups.end() ? nullptr : it->second.get();
}

const GroupXST* ServiceXST::findGroup(int groupId) const {
    auto it = m_groups.find(groupId);
    return it == m_groups.end() ? nullptr : it->second.get();
}

std::vector<const GroupXST*> ServiceXST::groups() const {
    std::vector<const GroupXST*> result;
    result.reserve(m_groups.size());
    for (const auto& entry : m_groups) result.push_back(entry.second.get());
    return result;
}

std::vector<const GroupXST*> ServiceXST::groupsOf(const std::string& accountId) const {
    std::vector<const GroupXST*> result;
    for (const auto& entry : m_groups) {
        if (entry.second->isMember(accountId)) result.push_back(entry.second.get());
    }
    return result;
}

void ServiceXST::restoreGroupMember(int groupId, const std::string& accountId) {
    GroupXST& group = requireGroup(groupId);
    AccountXST& account = requireAccount(accountId);
    group.restoreMember(accountId);
    account.joinGroup(groupId);
}

void ServiceXST::restoreGroupAdmin(int groupId, const std::string& accountId) {
    requireGroup(groupId).restoreAdmin(accountId);
}

void ServiceXST::restoreSubGroup(int groupId, const std::string& subName,
                                 const std::string& creatorId,
                                 const std::vector<std::string>& members) {
    requireGroup(groupId).restoreSubGroup(SubGroupXST(subName, creatorId, members));
}

GroupOpResultXST ServiceXST::applyJoin(int groupId, const std::string& applicant) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    AccountXST* account = findAccount(applicant);
    if (!account) return GroupOpResultXST::AccountNotFound;
    GroupOpResultXST result = group->apply(applicant);
    if (result == GroupOpResultXST::Ok) account->joinGroup(groupId);
    return result;
}

GroupOpResultXST ServiceXST::invite(int groupId, const std::string& inviter,
                                    const std::string& invitee) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    AccountXST* from = findAccount(inviter);
    AccountXST* to = findAccount(invitee);
    if (!from || !to) return GroupOpResultXST::AccountNotFound;
    if (!group->isMember(inviter)) return GroupOpResultXST::NotMember;
    if (group->isMember(invitee)) return GroupOpResultXST::AlreadyMember;
    if (!from->hasFriend(invitee)) return GroupOpResultXST::NotFriends;
    GroupOpResultXST result = group->invite(inviter, invitee);
    if (result == GroupOpResultXST::Ok) to->joinGroup(groupId);
    return result;
}

GroupOpResultXST ServiceXST::quitGroup(int groupId, const std::string& member) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    GroupOpResultXST result = group->quit(member);
    if (result == GroupOpResultXST::Ok) {
        if (AccountXST* account = findAccount(member)) account->leaveGroup(groupId);
    }
    return result;
}

GroupOpResultXST ServiceXST::kick(int groupId, const std::string& op, const std::string& target) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    GroupOpResultXST result = group->kick(op, target);
    if (result == GroupOpResultXST::Ok) {
        if (AccountXST* account = findAccount(target)) account->leaveGroup(groupId);
    }
    return result;
}

GroupOpResultXST ServiceXST::setGroupAdmin(int groupId, const std::string& op,
                                           const std::string& target, bool grant) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    return group->setAdmin(op, target, grant);
}

GroupOpResultXST ServiceXST::createSubGroup(int groupId, const std::string& op,
                                            const std::string& subName,
                                            const std::vector<std::string>& members) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    return group->createSubGroup(op, subName, members);
}

GroupOpResultXST ServiceXST::dissolveSubGroup(int groupId, const std::string& op,
                                              const std::string& subName) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    return group->dissolveSubGroup(op, subName);
}

GroupOpResultXST ServiceXST::changeGroupPolicy(int groupId, const std::string& op,
                                               const std::string& policyCode) {
    GroupXST* group = findGroup(groupId);
    if (!group) return GroupOpResultXST::GroupNotFound;
    std::unique_ptr<GroupPolicyXST> policy = GroupPolicyFactoryXST::create(policyCode);
    if (!policy) return GroupOpResultXST::InvalidPolicy;
    return group->changePolicy(op, std::move(policy));
}
