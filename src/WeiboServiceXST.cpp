#include "WeiboServiceXST.h"

#include "WeiboAccountXST.h"

ServiceTypeXST WeiboServiceXST::type() const { return ServiceTypeXST::Weibo; }

std::unique_ptr<AccountXST> WeiboServiceXST::createAccount(
    const std::string& id, const UserXST* owner, const std::string& nickname,
    const DateXST& registerDate, const std::string& /*extra*/) const {
    return std::make_unique<WeiboAccountXST>(id, owner, nickname, registerDate);
}
