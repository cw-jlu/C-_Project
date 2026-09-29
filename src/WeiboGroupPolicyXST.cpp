#include "WeiboGroupPolicyXST.h"

#include "GroupXST.h"

std::string WeiboGroupPolicyXST::code() const { return "WB"; }
std::string WeiboGroupPolicyXST::name() const { return "微博群"; }

std::vector<std::string> WeiboGroupPolicyXST::features() const {
    return {
        "粉丝可主动申请加入，但只有群主和管理员能邀请他人",
        "有管理员制度：群主任免管理员，管理员可踢出普通成员",
        "不允许创建临时讨论组（子群）"
    };
}

bool WeiboGroupPolicyXST::allowApply() const { return true; }
bool WeiboGroupPolicyXST::supportsAdmins() const { return true; }
bool WeiboGroupPolicyXST::supportsSubGroups() const { return false; }

bool WeiboGroupPolicyXST::canInvite(const GroupXST& group, const std::string& inviter) const {
    return group.roleOf(inviter) >= GroupRoleXST::Admin;
}
