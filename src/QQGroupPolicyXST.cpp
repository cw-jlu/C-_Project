#include "QQGroupPolicyXST.h"

std::string QQGroupPolicyXST::code() const { return "QQ"; }
std::string QQGroupPolicyXST::name() const { return "QQ群"; }

std::vector<std::string> QQGroupPolicyXST::features() const {
    return {
        "可主动申请加入，群成员也可邀请好友加入",
        "以群主为核心的管理员制度：群主任免管理员，管理员可踢出普通成员",
        "允许群成员创建临时讨论组（子群）"
    };
}

bool QQGroupPolicyXST::allowApply() const { return true; }
bool QQGroupPolicyXST::supportsAdmins() const { return true; }
bool QQGroupPolicyXST::supportsSubGroups() const { return true; }
