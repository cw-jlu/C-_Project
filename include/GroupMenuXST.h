#ifndef GROUP_MENU_XST_H
#define GROUP_MENU_XST_H

#include <string>
#include <vector>

#include "GroupOpResultXST.h"
#include "ServiceMenuXST.h"

class GroupPolicyXST;
class GroupXST;

// 群管理菜单：加群、退群、邀请、踢人、管理员、临时讨论组、群特色展示与管理模式切换
class GroupMenuXST : public ServiceMenuXST {
public:
    GroupMenuXST(PlatformXST& platform, LoginManagerXST& session);

protected:
    std::string title() const override;
    std::vector<std::string> items() const override;
    void handle(int index) override;

private:
    void listMyGroups() const;
    void showGroupInfo() const;
    void applyJoin();
    void inviteFriend();
    void quitGroup();
    void kickMember();
    void toggleAdmin();
    void createSubGroup();
    void dissolveSubGroup();
    void showFeatures() const;
    void changePolicy();

    const GroupXST* chooseGroup() const;                  // 输入群号，0 取消，不存在返回 nullptr
    void printGroupInfo(const GroupXST& group) const;
    static void printFeatures(const GroupPolicyXST& policy);
    void report(GroupOpResultXST result, const GroupXST& group) const;
    std::string memberText(const std::string& accountId) const;   // 如 "10002(小李飞刀)"
    static std::vector<std::string> splitIds(const std::string& text);
};

#endif
