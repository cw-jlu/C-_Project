#ifndef BINDABLE_ACCOUNT_XST_H
#define BINDABLE_ACCOUNT_XST_H

#include "AccountXST.h"

// 使用独立 ID 的账号（抽象），可与一个 QQ 号绑定对应：如微信
// 新的“独立 ID”类微X产品只需继承本类并实现 serviceType()、idKindName()
class BindableAccountXST : public AccountXST {
public:
    BindableAccountXST(const std::string& id, const UserXST* owner,
                       const std::string& nickname, const DateXST& registerDate,
                       const std::string& boundQQ = "");

    // 独立 ID 格式：6~20 位，字母开头，只含字母、数字、下划线、减号
    static bool isValidIndependentId(const std::string& id);

    bool isValidAccountId(const std::string& id) const override;
    std::string linkedQQ() const override;        // 即绑定的 QQ 号

    // QQ 绑定管理
    bool bindQQ(const std::string& qq);           // QQ 号格式错误返回 false；已绑定则改绑
    bool unbindQQ();                              // 未绑定返回 false
    bool isBound() const;
    const std::string& boundQQ() const;

protected:
    void printExtraInfo(std::ostream& os) const override;

private:
    std::string m_boundQQ;
};

#endif
