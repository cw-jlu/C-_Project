#ifndef QQ_SERVICE_XST_H
#define QQ_SERVICE_XST_H

#include "ServiceXST.h"

// QQ ·þÎñ
class QQServiceXST : public ServiceXST {
public:
    ServiceTypeXST type() const override;
    AccountIdRuleXST idRule() const override;
    std::unique_ptr<AccountXST> createAccount(
        const std::string& id, const UserXST* owner, const std::string& nickname,
        const DateXST& registerDate, const std::string& extra = "") const override;
    std::unique_ptr<GroupPolicyXST> createDefaultGroupPolicy() const override;
};

#endif
