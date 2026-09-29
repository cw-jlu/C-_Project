#ifndef DISPLAY_XST_H
#define DISPLAY_XST_H

#include <iosfwd>
#include <string>
#include <vector>

#include "AccountIdRuleXST.h"
#include "FriendXST.h"
#include "ServiceTypeXST.h"

class AccountXST;
class GroupPolicyXST;
class GroupXST;
class PlatformXST;
class ServiceXST;

// 统一的信息展示（纯静态）：各菜单与功能展示共用，保证输出格式一致
class DisplayXST {
public:
    DisplayXST() = delete;

    // 如 "QQ 10002(小李飞刀)"
    static std::string describe(const AccountXST& account);
    // 如 "QQ、微信"，空列表为 "无"
    static std::string joinServiceNames(const std::vector<ServiceTypeXST>& types);
    static std::string idRuleText(AccountIdRuleXST rule);

    // 好友表格：ID / 备注 / 昵称（昵称从 service 中查询）
    static void printFriendTable(std::ostream& os, const ServiceXST& service,
                                 const std::vector<FriendXST>& items);
    // 群信息：模式、成员及身份、管理员与讨论组（含当前模式下未生效但已保留的数据）
    static void printGroupInfo(std::ostream& os, const ServiceXST& service, const GroupXST& group);
    // 群管理模式特色
    static void printFeatures(std::ostream& os, const GroupPolicyXST& policy);
    // 平台概览：全部用户、各服务账号与群
    static void printPlatformOverview(std::ostream& os, const PlatformXST& platform);
};

#endif
