#include "ShowcaseMenuXST.h"

#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ConsoleXST.h"
#include "DemoDataXST.h"
#include "DisplayXST.h"
#include "FriendManagerXST.h"
#include "LoginManagerXST.h"
#include "PlatformXST.h"

namespace {
const ServiceTypeXST QQ = ServiceTypeXST::QQ;
const ServiceTypeXST WX = ServiceTypeXST::WeChat;
const ServiceTypeXST WB = ServiceTypeXST::Weibo;

void printLinks(const std::vector<AccountLinkXST>& links, const char* arrow) {
    if (links.empty()) std::cout << "  （无）\n";
    for (const AccountLinkXST& link : links) {
        std::cout << "  " << DisplayXST::describe(link.source()) << arrow
                  << DisplayXST::describe(link.target()) << "\n";
    }
}
}  // namespace

void ShowcaseMenuXST::run() {
    using Demo = void (ShowcaseMenuXST::*)();
    const std::vector<std::pair<std::string, Demo>> demos = {
        {"用户基本信息与 ID 体系（需求1）", &ShowcaseMenuXST::demoUserInfo},
        {"好友管理：添加、修改、删除、查询（需求2(1)）", &ShowcaseMenuXST::demoFriendBasics},
        {"共同好友与依据其他服务的好友添加好友（需求2(2)、6(3)）", &ShowcaseMenuXST::demoFriendAcross},
        {"群管理：加群、退群、踢人、查成员及各服务群的差异（需求3）", &ShowcaseMenuXST::demoGroups},
        {"开通管理：选择开通 N 个微X服务（需求4）", &ShowcaseMenuXST::demoOpening},
        {"登录管理：一处登录，其他服务确认后自动登录（需求5、6(2)）", &ShowcaseMenuXST::demoLogin},
        {"群特色展示与管理模式动态切换（需求6(4)）", &ShowcaseMenuXST::demoGroupPolicy},
    };
    while (true) {
        std::cout << "\n===== 功能展示（每项均在全新的演示数据上运行） =====\n";
        for (size_t i = 0; i < demos.size(); ++i) {
            std::cout << i + 1 << ". " << demos[i].first << "\n";
        }
        const int all = static_cast<int>(demos.size()) + 1;
        std::cout << all << ". 依次演示全部\n0. 返回\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, all);
        if (choice == 0) return;
        if (choice == all) {
            for (const auto& demo : demos) {
                (this->*demo.second)();
                ConsoleXST::pause();
            }
        } else {
            (this->*demos[choice - 1].second)();
            ConsoleXST::pause();
        }
    }
}

// ------------------------------------------------------------------ 需求1
void ShowcaseMenuXST::demoUserInfo() {
    PlatformXST p;
    DemoDataXST::seed(p);
    heading("需求1  用户基本信息与 ID 体系");
    note("用户基本信息：号码ID、昵称、出生时间、T龄（号码申请时间）、所在地、好友列表、群列表");
    note("微博与 QQ 共享 ID；微信采用独立 ID，可以与 QQ 号码绑定对应");

    const UserXST& zhang = *p.findUser("P001");
    step("张三（P001）开通了 " + zhang.servicesText() + "，各账号信息:");
    for (ServiceTypeXST type : zhang.services()) {
        p.accountOf("P001", type)->printInfo(std::cout);
    }
    step("张三 QQ 的好友列表:");
    const AccountXST& zhangQQ = *p.accountOf("P001", QQ);
    DisplayXST::printFriendTable(std::cout, p.service(QQ), zhangQQ.friends().items());

    step("各服务的 ID 产生方式（多态：ServiceXST::idRule）:");
    for (ServiceTypeXST type : p.serviceTypes()) {
        std::cout << "  " << serviceDisplayName(type) << ": "
                  << DisplayXST::idRuleText(p.service(type).idRule()) << "\n";
    }
    step("对比：周八的微信没有绑定 QQ");
    p.accountOf("P006", WX)->printInfo(std::cout);
}

// ------------------------------------------------------------------ 需求2(1)
void ShowcaseMenuXST::demoFriendBasics() {
    PlatformXST p;
    DemoDataXST::seed(p);
    ServiceXST& qq = p.service(QQ);
    const AccountXST& li = *qq.findAccount("10002");
    heading("需求2(1)  好友信息的添加、修改、删除、查询");
    note("以李四的 QQ（10002）为例；好友关系是双向的，备注只影响自己这一侧");

    step("李四当前的好友:");
    DisplayXST::printFriendTable(std::cout, qq, li.friends().items());
    step("添加 10003（王五），备注“王五同学”");
    result(friendOpMessage(qq.addFriendship("10002", "10003", "王五同学")));
    step("再次添加 10003");
    result(friendOpMessage(qq.addFriendship("10002", "10003")));
    step("添加自己 10002");
    result(friendOpMessage(qq.addFriendship("10002", "10002")));
    step("添加微信号 wx_lisi（与 QQ 的 ID 体系不符）");
    result(friendOpMessage(qq.addFriendship("10002", "wx_lisi")));
    step("添加不存在的 QQ 号 99999");
    result(friendOpMessage(qq.addFriendship("10002", "99999")));
    step("把 10003 的备注改为“五五”");
    result(friendOpMessage(qq.setFriendRemark("10002", "10003", "五五")));
    step("按关键字“老”查询好友（匹配 ID 或备注）:");
    DisplayXST::printFriendTable(std::cout, qq, li.friends().search("老"));
    step("删除好友 10004（赵六）");
    result(friendOpMessage(qq.removeFriendship("10002", "10004")));
    step("赵六的好友列表中也不再有李四:");
    DisplayXST::printFriendTable(std::cout, qq, qq.findAccount("10004")->friends().items());
    step("李四最新的好友列表:");
    DisplayXST::printFriendTable(std::cout, qq, li.friends().items());
}

// ------------------------------------------------------------------ 需求2(2)、6(3)
void ShowcaseMenuXST::demoFriendAcross() {
    PlatformXST p;
    DemoDataXST::seed(p);
    FriendManagerXST manager(p);
    heading("需求2(2)、6(3)  共同好友，以及依据其他服务的好友添加好友");
    note("不同服务之间以 QQ 号作为纽带：QQ、微博即本号，微信为绑定的 QQ（未绑定则无法对应）");

    step("QQ 内：张三(10001) 与 李四(10002) 的共同好友:");
    for (const AccountXST* a : p.service(QQ).commonFriends("10001", "10002")) {
        std::cout << "  " << DisplayXST::describe(*a) << "\n";
    }
    step("跨服务：在张三的 QQ 与微信中都是好友的人:");
    printLinks(manager.commonFriendsAcross("P001", QQ, WX), "  <->  ");

    step("微信添加 QQ 推荐好友：张三的 QQ 好友中，在微信有对应账号、且尚未成为微信好友的人:");
    std::vector<AccountLinkXST> links = manager.recommend("P001", WX, QQ);
    printLinks(links, "  ->  ");
    note("李四已是微信好友；周八的微信未绑定 QQ，无法对应；王五没有开通微信");
    if (!links.empty()) {
        step("接受推荐，添加 " + DisplayXST::describe(links[0].target()));
        result(friendOpMessage(manager.acceptRecommendation("P001", WX, links[0])));
    }
    step("张三的微信好友（备注沿用了 QQ 中的备注）:");
    DisplayXST::printFriendTable(std::cout, p.service(WX), p.accountOf("P001", WX)->friends().items());

    step("同理，微博也可以依据 QQ 好友添加:");
    links = manager.recommend("P001", WB, QQ);
    printLinks(links, "  ->  ");
    if (!links.empty()) {
        result(friendOpMessage(manager.acceptRecommendation("P001", WB, links[0])));
    }
    DisplayXST::printFriendTable(std::cout, p.service(WB), p.accountOf("P001", WB)->friends().items());
}

// ------------------------------------------------------------------ 需求3
void ShowcaseMenuXST::demoGroups() {
    PlatformXST p;
    DemoDataXST::seed(p);
    ServiceXST& qq = p.service(QQ);
    ServiceXST& wx = p.service(WX);
    ServiceXST& wb = p.service(WB);
    auto report = [](GroupOpResultXST r) { result(groupOpMessage(r)); };
    heading("需求3  群管理：加入群、退出群、踢人、查询群成员，以及各服务群的差异");
    note("每个服务都预置了 1001~1006 号群");
    note("QQ群可申请加入、有以群主为核心的管理员、允许临时讨论组；微信群只能邀请加入、仅群主有特权、不允许子群");

    step("【查询群成员】QQ 群 1001:");
    DisplayXST::printGroupInfo(std::cout, qq, *qq.findGroup(1001));

    step("【加入群】周八(10006) 申请加入 QQ 群 1003");
    report(qq.applyJoin(1003, "10006"));
    step("【加入群】周八(wx_zhouba) 申请加入微信群 1001");
    report(wx.applyJoin(1001, "wx_zhouba"));
    step("【加入群】张三(wx_zhangsan) 邀请还不是好友的周八加入微信群 1001");
    report(wx.invite(1001, "wx_zhangsan", "wx_zhouba"));
    step("【加入群】张三与周八互加微信好友后再邀请");
    wx.addFriendship("wx_zhangsan", "wx_zhouba");
    report(wx.invite(1001, "wx_zhangsan", "wx_zhouba"));
    step("【加入群】微博群 1001 的普通成员王五(10003) 邀请赵六(10004)");
    report(wb.invite(1001, "10003", "10004"));
    note("微博群仅群主和管理员可以邀请他人");

    step("【踢人】QQ 群 1001：管理员李四(10002) 踢出普通成员王五(10003)");
    report(qq.kick(1001, "10002", "10003"));
    step("【踢人】QQ 群 1001：管理员李四(10002) 踢出群主张三(10001)");
    report(qq.kick(1001, "10002", "10001"));
    step("【踢人】微信群 1004：普通成员李四踢出赵六");
    report(wx.kick(1004, "wx_lisi", "wx_zhaoliu"));
    step("【踢人】微信群 1004：群主张三踢出赵六");
    report(wx.kick(1004, "wx_zhangsan", "wx_zhaoliu"));

    step("【管理员】QQ 群 1001：群主张三将赵六(10004) 设为管理员");
    report(qq.setGroupAdmin(1001, "10001", "10004", true));
    step("【管理员】微信群 1004：群主张三将李四设为管理员");
    report(wx.setGroupAdmin(1004, "wx_zhangsan", "wx_lisi", true));

    step("【临时讨论组】QQ 群 1001：李四发起讨论组“篮球小分队”（成员 10004）");
    report(qq.createSubGroup(1001, "10002", "篮球小分队", {"10004"}));
    step("【临时讨论组】微信群 1004：张三发起讨论组");
    report(wx.createSubGroup(1004, "wx_zhangsan", "讨论组", {"wx_lisi"}));

    step("【退出群】张三退出 QQ 群 1002");
    report(qq.quitGroup(1002, "10001"));
    step("【退出群】群主张三退出自己的 QQ 群 1006");
    report(qq.quitGroup(1006, "10001"));

    step("操作后的 QQ 群 1001:");
    DisplayXST::printGroupInfo(std::cout, qq, *qq.findGroup(1001));
}

// ------------------------------------------------------------------ 需求4
void ShowcaseMenuXST::demoOpening() {
    PlatformXST p;
    DemoDataXST::seed(p);
    heading("需求4  开通管理：用户可以选择开通平台上的 N 个微X服务");
    for (ServiceTypeXST type : p.serviceTypes()) {
        note(serviceDisplayName(type) + "：" + DisplayXST::idRuleText(p.service(type).idRule()));
    }

    const UserXST& sun = *p.findUser("P005");
    step("孙七(P005) 目前只开通了: " + sun.servicesText());

    const std::string qqNumber = p.nextQQNumber();
    step("孙七尝试直接开通微博（微博与 QQ 共享号码，但孙七还没有 QQ）");
    try {
        p.openService("P005", WB, qqNumber, "孙七的微博", "123456", DateXST::today());
        result("开通成功");
    } catch (const std::exception& e) {
        result(std::string("开通失败: ") + e.what());
    }
    step("孙七开通 QQ，平台分配新号码 " + qqNumber);
    p.openService("P005", QQ, qqNumber, "小孙", "123456", DateXST::today());
    result("开通成功");
    step("孙七再开通微博，沿用 QQ 号 " + qqNumber);
    p.openService("P005", WB, qqNumber, "孙七的微博", "123456", DateXST::today());
    result("开通成功");

    step("孙七现在开通了: " + sun.servicesText());
    for (ServiceTypeXST type : sun.services()) {
        p.accountOf("P005", type)->printInfo(std::cout);
    }
}

// ------------------------------------------------------------------ 需求5、6(2)
void ShowcaseMenuXST::demoLogin() {
    PlatformXST p;
    DemoDataXST::seed(p);
    LoginManagerXST session(p);
    auto status = [&session]() {
        if (!session.isActive()) {
            std::cout << "  状态: 未登录\n";
            return;
        }
        std::cout << "  状态: " << session.currentUser()->name() << "  已登录: "
                  << DisplayXST::joinServiceNames(session.loggedInServices()) << "  待确认: "
                  << DisplayXST::joinServiceNames(session.pendingServices()) << "\n";
    };
    heading("需求5、6(2)  登录管理：一个服务登录后，其他服务简单确认即视为自动登录");

    step("张三用 QQ 10001 登录，密码输错");
    result(loginMessage(session.login(QQ, "10001", "000000")));
    step("张三用 QQ 10001 和正确密码登录");
    result(loginMessage(session.login(QQ, "10001", "123456")));
    status();
    step("此时李四想登录微信（同一时间只允许一个会话）");
    result(loginMessage(session.login(WX, "wx_lisi", "123456")));
    step("张三确认登录微信（无需密码）");
    result(loginMessage(session.confirm(WX)));
    status();
    step("一键确认其余服务");
    result("已自动登录: " + DisplayXST::joinServiceNames(session.confirmAll()));
    status();
    step("注销");
    session.logout();
    status();
    step("张三改用微信 wx_zhangsan 登录，再一键确认");
    result(loginMessage(session.login(WX, "wx_zhangsan", "123456")));
    session.confirmAll();
    status();
    note("菜单中：登录后会列出本人开通的其他服务，输入 y 即全部登录；也可在“登录状态”中逐个确认");
}

// ------------------------------------------------------------------ 需求6(4)
void ShowcaseMenuXST::demoGroupPolicy() {
    PlatformXST p;
    DemoDataXST::seed(p);
    ServiceXST& qq = p.service(QQ);
    const GroupXST& group = *qq.findGroup(1001);
    auto report = [](GroupOpResultXST r) { result(groupOpMessage(r)); };
    auto show = [&]() {
        DisplayXST::printGroupInfo(std::cout, qq, group);
        DisplayXST::printFeatures(std::cout, group.policy());
    };
    heading("需求6(4)  展示群的特色功能，并在群成员数据不受伤害的前提下动态切换管理模式");
    note("策略模式：群只保存数据，权限判断交给“管理模式”对象；切换模式只替换这个对象");

    step("QQ 群 1001 当前情况:");
    show();
    step("群主张三把群 1001 切换为微信群模式");
    report(qq.changeGroupPolicy(1001, "10001", "WX"));
    show();
    step("微信群模式下：周八(10006) 申请加入");
    report(qq.applyJoin(1001, "10006"));
    step("微信群模式下：李四（管理员身份已保留但未生效）踢出赵六(10004)");
    report(qq.kick(1001, "10002", "10004"));
    step("微信群模式下：张三创建讨论组");
    report(qq.createSubGroup(1001, "10001", "新讨论组", {"10002"}));
    step("非群主李四尝试切换管理模式");
    report(qq.changeGroupPolicy(1001, "10002", "QQ"));

    step("张三把群切换为微博群模式（有管理员，但不支持讨论组）");
    report(qq.changeGroupPolicy(1001, "10001", "WB"));
    show();
    step("张三把群切回 QQ 群模式");
    report(qq.changeGroupPolicy(1001, "10001", "QQ"));
    show();
    step("QQ 群模式下：周八(10006) 申请加入");
    report(qq.applyJoin(1001, "10006"));
    note("整个过程中，群成员、管理员、讨论组数据都没有丢失");
}

// ------------------------------------------------------------------ 输出格式
void ShowcaseMenuXST::heading(const std::string& title) {
    std::cout << "\n==================================================================\n"
              << "  " << title
              << "\n==================================================================\n";
}

void ShowcaseMenuXST::note(const std::string& text) {
    std::cout << "  说明: " << text << "\n";
}

void ShowcaseMenuXST::step(const std::string& text) {
    std::cout << "\n> " << text << "\n";
}

void ShowcaseMenuXST::result(const std::string& text) {
    std::cout << "  结果: " << text << "\n";
}
