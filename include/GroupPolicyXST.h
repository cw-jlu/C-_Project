#ifndef GROUP_POLICY_XST_H
#define GROUP_POLICY_XST_H

#include <string>
#include <vector>

class GroupXST;

// 群管理策略（抽象）：决定一个群“能做什么、谁能做”
// 群对象只保存数据，所有权限判断都委托给当前策略；更换策略即可动态切换群的管理模式，
// 而群成员、管理员、子群等数据保持不变（策略模式）
class GroupPolicyXST {
public:
    virtual ~GroupPolicyXST() = default;

    virtual std::string code() const = 0;             // 数据文件中的模式代码：QQ / WX / WB
    virtual std::string name() const = 0;             // 如 "QQ群"
    virtual std::vector<std::string> features() const = 0;   // 该模式的特色说明

    // ---------- 功能开关：不支持时操作返回 NotSupported ----------
    virtual bool allowApply() const = 0;              // 能否主动申请加入
    virtual bool supportsAdmins() const = 0;          // 是否有管理员制度
    virtual bool supportsSubGroups() const = 0;       // 是否允许临时讨论组

    // ---------- 权限判断：不满足时操作返回 PermissionDenied ----------
    // 默认规则见 GroupPolicyXST.cpp，子类可按需覆盖
    virtual bool canInvite(const GroupXST& group, const std::string& inviter) const;
    virtual bool canKick(const GroupXST& group, const std::string& op,
                         const std::string& target) const;
    virtual bool canManageAdmins(const GroupXST& group, const std::string& op) const;
    virtual bool canCreateSubGroup(const GroupXST& group, const std::string& op) const;
};

#endif
