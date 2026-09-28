// 模拟即时通信系统 —— 程序入口
// P1：基础类演示（用户 / 各服务账号 / 好友 / 群 / T龄），P6 将替换为正式菜单

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "ConsoleXST.h"
#include "QQAccountXST.h"
#include "UserXST.h"
#include "WeChatAccountXST.h"
#include "WeiboAccountXST.h"

int main() {
    ConsoleXST::init();

    std::cout << "==============================\n";
    std::cout << "   腾*立体社交平台 \n";
    std::cout << "==============================\n\n";

    UserXST zhang("P001", "张三", DateXST(2005, 3, 12), "吉林长春");
    UserXST li("P002", "李四", DateXST(2004, 11, 2), "北京");
    zhang.openService(ServiceTypeXST::QQ);
    zhang.openService(ServiceTypeXST::WeChat);
    zhang.openService(ServiceTypeXST::Weibo);
    li.openService(ServiceTypeXST::QQ);
    li.openService(ServiceTypeXST::WeChat);

    // 用基类指针统一管理不同服务的账号
    std::vector<std::unique_ptr<AccountXST>> accounts;
    accounts.push_back(std::make_unique<QQAccountXST>("10001", &zhang, "三哥", DateXST(2015, 6, 1)));
    accounts.push_back(std::make_unique<WeChatAccountXST>("wx_zhangsan", &zhang, "张三", DateXST(2019, 5, 5), "10001"));
    accounts.push_back(std::make_unique<WeiboAccountXST>("10001", &zhang, "张三的微博", DateXST(2017, 2, 2)));
    accounts.push_back(std::make_unique<QQAccountXST>("10002", &li, "小李飞刀", DateXST(2014, 3, 15)));
    accounts.push_back(std::make_unique<WeChatAccountXST>("wx_lisi", &li, "李四", DateXST(2019, 8, 8)));

    AccountXST& zhangQQ = *accounts[0];
    zhangQQ.addFriend("10002", "室友李四");
    zhangQQ.addFriend("10003", "高中同学");
    zhangQQ.joinGroup(1001);
    zhangQQ.joinGroup(1004);
    accounts[1]->addFriend("wx_lisi", "室友李四");
    accounts[3]->addFriend("10001", "三哥");
    accounts[3]->joinGroup(1001);

    std::cout << "---------- 1. 用户信息 ----------\n";
    for (const UserXST* u : {&zhang, &li}) {
        std::cout << u->name() << " (" << u->id() << ")  " << u->age() << " 岁  "
                  << u->location() << "  已开通: " << u->servicesText() << "\n";
    }

    std::cout << "\n---------- 2. 账号信息 ----------\n";
    for (const auto& account : accounts) {
        account->printInfo(std::cout);
        std::cout << "\n";
    }

    std::cout << "---------- 3. 各账号对应的 QQ 号 ----------\n";
    for (const auto& account : accounts) {
        std::string qq = account->linkedQQ();
        std::cout << "  [" << account->serviceName() << "] " << account->id()
                  << " -> " << (qq.empty() ? "无" : qq) << "\n";
    }

    std::cout << "\n---------- 4. 好友维护 ----------\n";
    std::cout << "  添加自己为好友: " << (zhangQQ.addFriend("10001") ? "成功" : "拒绝") << "\n";
    std::cout << "  重复添加 10002: " << (zhangQQ.addFriend("10002") ? "成功" : "拒绝") << "\n";
    std::cout << "  QQ 添加微信号:  " << (zhangQQ.addFriend("wx_lisi") ? "成功" : "拒绝（ID 体系不符）") << "\n";
    zhangQQ.setFriendRemark("10003", "老同学王五");
    zhangQQ.removeFriend("10002");
    std::cout << "  修改 10003 备注、删除 10002 后，好友列表:\n";
    for (const FriendXST& f : zhangQQ.friends().items()) {
        std::cout << "    " << f.id() << "  " << f.displayName() << "\n";
    }

    std::cout << "\n---------- 5. 微信绑定 QQ ----------\n";
    auto* liWeChat = dynamic_cast<WeChatAccountXST*>(accounts[4].get());
    std::cout << "  绑定非法 QQ 号 abc: " << (liWeChat->bindQQ("abc") ? "成功" : "失败") << "\n";
    std::cout << "  绑定 QQ 10002:      " << (liWeChat->bindQQ("10002") ? "成功" : "失败") << "\n";
    liWeChat->printInfo(std::cout);

    std::cout << "\n---------- 6. 非法数据校验 ----------\n";
    try {
        QQAccountXST bad("01234", &li, "坏号码", DateXST(2020, 1, 1));
    } catch (const std::invalid_argument& e) {
        std::cout << "  " << e.what() << "\n";
    }
    try {
        DateXST::parse("2023-02-29");
    } catch (const std::invalid_argument& e) {
        std::cout << "  " << e.what() << "\n";
    }

    std::cout << "\n";
    ConsoleXST::pause();
    return 0;
}
