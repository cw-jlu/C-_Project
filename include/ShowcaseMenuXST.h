#ifndef SHOWCASE_MENU_XST_H
#define SHOWCASE_MENU_XST_H

#include <string>

// 功能展示：按题目要求逐项自动演示，便于检查与答辩
// 每一项都在一份全新的演示数据上运行，可反复演示，且不影响正常登录使用的数据
class ShowcaseMenuXST {
public:
    void run();

private:
    void demoUserInfo();        // 需求1：用户基本信息与 ID 体系
    void demoFriendBasics();    // 需求2(1)：好友增删改查
    void demoFriendAcross();    // 需求2(2)、6(3)：共同好友、依据其他服务的好友添加好友
    void demoGroups();          // 需求3：群管理及各服务群的差异
    void demoOpening();         // 需求4：开通管理
    void demoLogin();           // 需求5、6(2)：登录管理
    void demoGroupPolicy();     // 需求6(4)：群特色展示与管理模式动态切换

    static void heading(const std::string& title);
    static void note(const std::string& text);
    static void step(const std::string& text);
    static void result(const std::string& text);
};

#endif
