#ifndef WECHAT_GROUP_POLICY_XST_H
#define WECHAT_GROUP_POLICY_XST_H

#include "GroupPolicyXST.h"

// 微信群模式：只能由群成员推荐（邀请）加入；仅群主为特权账号；不允许子群
class WeChatGroupPolicyXST : public GroupPolicyXST {
public:
    std::string code() const override;
    std::string name() const override;
    std::vector<std::string> features() const override;

    bool allowApply() const override;
    bool supportsAdmins() const override;
    bool supportsSubGroups() const override;
};

#endif
