#include "WeChatServiceXST.h"

#include "WeChatAccountXST.h"

ServiceTypeXST WeChatServiceXST::type() const { return ServiceTypeXST::WeChat; }

std::unique_ptr<AccountXST> WeChatServiceXST::createAccount(
    const std::string& id, const UserXST* owner, const std::string& nickname,
    const DateXST& registerDate, const std::string& extra) const {
    return std::make_unique<WeChatAccountXST>(id, owner, nickname, registerDate, extra);
}
