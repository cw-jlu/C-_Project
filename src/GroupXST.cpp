#include "GroupXST.h"

#include <algorithm>
#include <stdexcept>

GroupXST::GroupXST(int id, const std::string& name, const std::string& ownerId,
                   std::unique_ptr<GroupPolicyXST> policy)
    : m_id(id), m_name(name), m_ownerId(ownerId), m_policy(std::move(policy)) {
    if (m_id <= 0) throw std::invalid_argument("群号必须为正整数");
    if (m_name.empty()) throw std::invalid_argument("群名不能为空");
    if (m_ownerId.empty()) throw std::invalid_argument("群主不能为空");
    if (!m_policy) throw std::invalid_argument("群管理模式不能为空");
    m_members.push_back(m_ownerId);
}

int GroupXST::id() const { return m_id; }
const std::string& GroupXST::name() const { return m_name; }
const std::string& GroupXST::ownerId() const { return m_ownerId; }
const GroupPolicyXST& GroupXST::policy() const { return *m_policy; }

bool GroupXST::isMember(const std::string& id) const {
    return std::find(m_members.begin(), m_members.end(), id) != m_members.end();
}

bool GroupXST::isOwner(const std::string& id) const { return id == m_ownerId; }

bool GroupXST::isAdmin(const std::string& id) const {
    return m_policy->supportsAdmins() && m_admins.count(id) > 0;
}

GroupRoleXST GroupXST::roleOf(const std::string& id) const {
    if (isOwner(id)) return GroupRoleXST::Owner;
    if (isAdmin(id)) return GroupRoleXST::Admin;
    if (isMember(id)) return GroupRoleXST::Member;
    return GroupRoleXST::None;
}

const std::vector<std::string>& GroupXST::members() const { return m_members; }
const std::set<std::string>& GroupXST::adminRecords() const { return m_admins; }
const std::vector<SubGroupXST>& GroupXST::subGroupRecords() const { return m_subGroups; }

GroupOpResultXST GroupXST::apply(const std::string& applicant) {
    if (isMember(applicant)) return GroupOpResultXST::AlreadyMember;
    if (!m_policy->allowApply()) return GroupOpResultXST::NotSupported;
    m_members.push_back(applicant);
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::invite(const std::string& inviter, const std::string& invitee) {
    if (!isMember(inviter)) return GroupOpResultXST::NotMember;
    if (isMember(invitee)) return GroupOpResultXST::AlreadyMember;
    if (!m_policy->canInvite(*this, inviter)) return GroupOpResultXST::PermissionDenied;
    m_members.push_back(invitee);
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::quit(const std::string& member) {
    if (!isMember(member)) return GroupOpResultXST::NotMember;
    if (isOwner(member)) return GroupOpResultXST::OwnerCannotQuit;
    removeMemberEverywhere(member);
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::kick(const std::string& op, const std::string& target) {
    if (!isMember(op)) return GroupOpResultXST::NotMember;
    if (op == target) return GroupOpResultXST::InvalidTarget;
    if (!isMember(target)) return GroupOpResultXST::TargetNotMember;
    if (!m_policy->canKick(*this, op, target)) return GroupOpResultXST::PermissionDenied;
    removeMemberEverywhere(target);
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::setAdmin(const std::string& op, const std::string& target, bool grant) {
    if (!m_policy->supportsAdmins()) return GroupOpResultXST::NotSupported;
    if (!isMember(op)) return GroupOpResultXST::NotMember;
    if (!isMember(target)) return GroupOpResultXST::TargetNotMember;
    if (isOwner(target)) return GroupOpResultXST::InvalidTarget;
    if (!m_policy->canManageAdmins(*this, op)) return GroupOpResultXST::PermissionDenied;
    if (grant) {
        m_admins.insert(target);
    } else {
        m_admins.erase(target);
    }
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::createSubGroup(const std::string& op, const std::string& name,
                                          const std::vector<std::string>& members) {
    if (!m_policy->supportsSubGroups()) return GroupOpResultXST::NotSupported;
    if (!isMember(op)) return GroupOpResultXST::NotMember;
    if (!m_policy->canCreateSubGroup(*this, op)) return GroupOpResultXST::PermissionDenied;
    if (name.empty()) return GroupOpResultXST::InvalidSubGroup;
    if (findSubGroup(name)) return GroupOpResultXST::SubGroupExists;

    std::vector<std::string> chosen{op};
    for (const std::string& id : members) {
        if (!isMember(id)) return GroupOpResultXST::TargetNotMember;
        if (std::find(chosen.begin(), chosen.end(), id) == chosen.end()) chosen.push_back(id);
    }
    if (chosen.size() < 2) return GroupOpResultXST::InvalidSubGroup;
    m_subGroups.emplace_back(name, op, chosen);
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::dissolveSubGroup(const std::string& op, const std::string& name) {
    if (!m_policy->supportsSubGroups()) return GroupOpResultXST::NotSupported;
    if (!isMember(op)) return GroupOpResultXST::NotMember;
    auto it = std::find_if(m_subGroups.begin(), m_subGroups.end(),
                           [&name](const SubGroupXST& s) { return s.name() == name; });
    if (it == m_subGroups.end()) return GroupOpResultXST::SubGroupNotFound;
    // 发起人、群主或管理员可以解散
    if (it->creatorId() != op && roleOf(op) < GroupRoleXST::Admin) {
        return GroupOpResultXST::PermissionDenied;
    }
    m_subGroups.erase(it);
    return GroupOpResultXST::Ok;
}

GroupOpResultXST GroupXST::changePolicy(const std::string& op,
                                        std::unique_ptr<GroupPolicyXST> policy) {
    if (!policy) return GroupOpResultXST::InvalidPolicy;
    if (!isOwner(op)) return GroupOpResultXST::PermissionDenied;
    m_policy = std::move(policy);
    return GroupOpResultXST::Ok;
}

void GroupXST::restoreMember(const std::string& id) {
    if (id.empty()) throw std::invalid_argument("群成员 ID 不能为空");
    if (!isMember(id)) m_members.push_back(id);
}

void GroupXST::restoreAdmin(const std::string& id) {
    if (!isMember(id) || isOwner(id)) {
        throw std::invalid_argument("群 " + std::to_string(m_id) + " 的管理员必须是非群主成员: " + id);
    }
    m_admins.insert(id);
}

void GroupXST::restoreSubGroup(const SubGroupXST& subGroup) {
    if (findSubGroup(subGroup.name())) {
        throw std::invalid_argument("讨论组重名: " + subGroup.name());
    }
    for (const std::string& id : subGroup.members()) {
        if (!isMember(id)) {
            throw std::invalid_argument("讨论组 " + subGroup.name() + " 的成员不在群内: " + id);
        }
    }
    m_subGroups.push_back(subGroup);
}

const SubGroupXST* GroupXST::findSubGroup(const std::string& name) const {
    for (const SubGroupXST& s : m_subGroups) {
        if (s.name() == name) return &s;
    }
    return nullptr;
}

void GroupXST::removeMemberEverywhere(const std::string& id) {
    m_members.erase(std::remove(m_members.begin(), m_members.end(), id), m_members.end());
    m_admins.erase(id);
    for (SubGroupXST& s : m_subGroups) s.removeMember(id);
    // 讨论组发起人离开或只剩一人时，讨论组自动解散
    m_subGroups.erase(std::remove_if(m_subGroups.begin(), m_subGroups.end(),
                                     [&id](const SubGroupXST& s) {
                                         return s.creatorId() == id || s.members().size() < 2;
                                     }),
                      m_subGroups.end());
}
