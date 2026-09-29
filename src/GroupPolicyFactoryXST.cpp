#include "GroupPolicyFactoryXST.h"

#include "QQGroupPolicyXST.h"
#include "WeChatGroupPolicyXST.h"
#include "WeiboGroupPolicyXST.h"

std::unique_ptr<GroupPolicyXST> GroupPolicyFactoryXST::create(const std::string& code) {
    if (code == "QQ") return std::make_unique<QQGroupPolicyXST>();
    if (code == "WX") return std::make_unique<WeChatGroupPolicyXST>();
    if (code == "WB") return std::make_unique<WeiboGroupPolicyXST>();
    return nullptr;
}

const std::vector<std::string>& GroupPolicyFactoryXST::codes() {
    static const std::vector<std::string> all = {"QQ", "WX", "WB"};
    return all;
}
