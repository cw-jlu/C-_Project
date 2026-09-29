#ifndef WECHAT_SERVICE_XST_H
#define WECHAT_SERVICE_XST_H

#include "ServiceXST.h"

// 微信服务：createAccount 的 extra 参数为绑定的 QQ 号（可为空）
class WeChatServiceXST : public ServiceXST {
public:
    ServiceTypeXST type() const override;
    std::unique_ptr<AccountXST> createAccount(
        const std::string& id, const UserXST* owner, const std::string& nickname,
        const DateXST& registerDate, const std::string& extra = "") const override;
    std::unique_ptr<GroupPolicyXST> createDefaultGroupPolicy() const override;
};

#endif
