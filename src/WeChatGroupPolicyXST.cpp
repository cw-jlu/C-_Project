#include "WeChatGroupPolicyXST.h"

std::string WeChatGroupPolicyXST::code() const { return "WX"; }
std::string WeChatGroupPolicyXST::name() const { return "微信群"; }

std::vector<std::string> WeChatGroupPolicyXST::features() const {
    return {
        "不能主动申请加入，只能由群成员推荐（邀请）好友加入",
        "没有管理员，仅群主为特权账号，只有群主可以踢人",
        "不允许创建临时讨论组（子群）"
    };
}

bool WeChatGroupPolicyXST::allowApply() const { return false; }
bool WeChatGroupPolicyXST::supportsAdmins() const { return false; }
bool WeChatGroupPolicyXST::supportsSubGroups() const { return false; }
