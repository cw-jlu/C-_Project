#ifndef QQ_GROUP_POLICY_XST_H
#define QQ_GROUP_POLICY_XST_H

#include "GroupPolicyXST.h"

// QQ 群模式：可申请加入；以群主为核心的管理员制度；允许临时讨论组
class QQGroupPolicyXST : public GroupPolicyXST {
public:
    std::string code() const override;
    std::string name() const override;
    std::vector<std::string> features() const override;

    bool allowApply() const override;
    bool supportsAdmins() const override;
    bool supportsSubGroups() const override;
};

#endif
