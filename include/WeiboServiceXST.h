#ifndef WEIBO_SERVICE_XST_H
#define WEIBO_SERVICE_XST_H

#include "ServiceXST.h"

// 微博服务：账号与 QQ 共享 ID
class WeiboServiceXST : public ServiceXST {
public:
    ServiceTypeXST type() const override;
    std::unique_ptr<AccountXST> createAccount(
        const std::string& id, const UserXST* owner, const std::string& nickname,
        const DateXST& registerDate, const std::string& extra = "") const override;
    std::unique_ptr<GroupPolicyXST> createDefaultGroupPolicy() const override;
};

#endif
