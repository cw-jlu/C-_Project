#ifndef WEIBO_GROUP_POLICY_XST_H
#define WEIBO_GROUP_POLICY_XST_H

#include "GroupPolicyXST.h"

// 微博群（粉丝群）模式：可申请加入，但只有群主和管理员能邀请；有管理员制度；不允许子群
class WeiboGroupPolicyXST : public GroupPolicyXST {
public:
    std::string code() const override;
    std::string name() const override;
    std::vector<std::string> features() const override;

    bool allowApply() const override;
    bool supportsAdmins() const override;
    bool supportsSubGroups() const override;

    bool canInvite(const GroupXST& group, const std::string& inviter) const override;
};

#endif
