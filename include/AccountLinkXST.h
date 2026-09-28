#ifndef ACCOUNT_LINK_XST_H
#define ACCOUNT_LINK_XST_H

class AccountXST;

// 跨服务账号对应关系：source 服务中的账号 <-> target 服务中对应同一 QQ 号的账号
class AccountLinkXST {
public:
    AccountLinkXST(const AccountXST& source, const AccountXST& target);

    const AccountXST& source() const;
    const AccountXST& target() const;

private:
    const AccountXST* m_source;
    const AccountXST* m_target;
};

#endif
