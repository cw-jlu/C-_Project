#include "QQServiceXST.h"

#include "QQAccountXST.h"
#include "QQGroupPolicyXST.h"

ServiceTypeXST QQServiceXST::type() const { return ServiceTypeXST::QQ; }

std::unique_ptr<AccountXST> QQServiceXST::createAccount(
    const std::string& id, const UserXST* owner, const std::string& nickname,
    const DateXST& registerDate, const std::string& /*extra*/) const {
    return std::make_unique<QQAccountXST>(id, owner, nickname, registerDate);
}

std::unique_ptr<GroupPolicyXST> QQServiceXST::createDefaultGroupPolicy() const {
    return std::make_unique<QQGroupPolicyXST>();
}
