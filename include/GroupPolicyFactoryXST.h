#ifndef GROUP_POLICY_FACTORY_XST_H
#define GROUP_POLICY_FACTORY_XST_H

#include <memory>
#include <string>
#include <vector>

#include "GroupPolicyXST.h"

// 群管理策略工厂：按模式代码（QQ / WX / WB）创建策略对象
// 用于运行时切换群模式与从数据文件恢复群模式；新增模式只需在此登记
class GroupPolicyFactoryXST {
public:
    GroupPolicyFactoryXST() = delete;

    static std::unique_ptr<GroupPolicyXST> create(const std::string& code);   // 未知代码返回 nullptr
    static const std::vector<std::string>& codes();
};

#endif
