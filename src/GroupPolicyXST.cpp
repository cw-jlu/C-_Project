#include "GroupPolicyXST.h"

#include "GroupXST.h"

// 默认：任何成员都可以邀请好友入群
bool GroupPolicyXST::canInvite(const GroupXST& group, const std::string& inviter) const {
    return group.isMember(inviter);
}

// 默认：至少是管理员，且只能踢身份比自己低的人
// 不支持管理员制度的模式下 roleOf() 不会返回 Admin，于是只剩群主能踢人
bool GroupPolicyXST::canKick(const GroupXST& group, const std::string& op,
                             const std::string& target) const {
    GroupRoleXST opRole = group.roleOf(op);
    return opRole >= GroupRoleXST::Admin && opRole > group.roleOf(target);
}

// 默认：只有群主可以任免管理员
bool GroupPolicyXST::canManageAdmins(const GroupXST& group, const std::string& op) const {
    return group.isOwner(op);
}

// 默认：任何成员都可以发起讨论组
bool GroupPolicyXST::canCreateSubGroup(const GroupXST& group, const std::string& op) const {
    return group.isMember(op);
}
