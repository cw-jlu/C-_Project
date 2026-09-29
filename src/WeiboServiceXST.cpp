#include "WeiboServiceXST.h"

#include "WeiboAccountXST.h"
#include "WeiboGroupPolicyXST.h"

ServiceTypeXST WeiboServiceXST::type() const { return ServiceTypeXST::Weibo; }

AccountIdRuleXST WeiboServiceXST::idRule() const { return AccountIdRuleXST::SharedQQNumber; }

std::unique_ptr<AccountXST> WeiboServiceXST::createAccount(
    const std::string& id, const UserXST* owner, const std::string& nickname,
    const DateXST& registerDate, const std::string& /*extra*/) const {
    return std::make_unique<WeiboAccountXST>(id, owner, nickname, registerDate);
}

std::unique_ptr<GroupPolicyXST> WeiboServiceXST::createDefaultGroupPolicy() const {
    return std::make_unique<WeiboGroupPolicyXST>();
}
