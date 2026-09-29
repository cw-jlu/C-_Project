#include "DemoDataXST.h"

#include <initializer_list>
#include <string>

#include "PlatformXST.h"

namespace {

// 建群并恢复成员与管理员（群主自动入群）
void seedGroup(ServiceXST& service, int id, const std::string& name, const std::string& owner,
               std::initializer_list<const char*> members,
               std::initializer_list<const char*> admins = {}) {
    service.createGroup(id, name, owner);
    for (const char* m : members) service.restoreGroupMember(id, m);
    for (const char* a : admins) service.restoreGroupAdmin(id, a);
}

void seedAll(PlatformXST& p) {
    const ServiceTypeXST QQ = ServiceTypeXST::QQ;
    const ServiceTypeXST WX = ServiceTypeXST::WeChat;
    const ServiceTypeXST WB = ServiceTypeXST::Weibo;

    p.addUser("P001", "张三", DateXST(2005, 3, 12), "吉林长春");
    p.addUser("P002", "李四", DateXST(2004, 11, 2), "北京");
    p.addUser("P003", "王五", DateXST(2005, 7, 21), "上海");
    p.addUser("P004", "赵六", DateXST(2004, 1, 30), "广东广州");
    p.addUser("P005", "孙七", DateXST(2005, 9, 9), "吉林长春");
    p.addUser("P006", "周八", DateXST(2003, 12, 15), "四川成都");

    // QQ 需先于微博、微信开通（二者对应的 QQ 号必须存在）
    p.openService("P001", QQ, "10001", "三哥", "123456", DateXST(2015, 6, 1));
    p.openService("P002", QQ, "10002", "小李飞刀", "123456", DateXST(2014, 3, 15));
    p.openService("P003", QQ, "10003", "五五开", "123456", DateXST(2016, 9, 10));
    p.openService("P004", QQ, "10004", "赵六六", "123456", DateXST(2013, 1, 20));
    p.openService("P006", QQ, "10006", "周末愉快", "123456", DateXST(2018, 7, 7));

    p.openService("P001", WX, "wx_zhangsan", "张三", "123456", DateXST(2019, 5, 5), "10001");
    p.openService("P002", WX, "wx_lisi", "李四", "123456", DateXST(2019, 8, 8), "10002");
    p.openService("P004", WX, "wx_zhaoliu", "赵六", "123456", DateXST(2020, 1, 1), "10004");
    p.openService("P005", WX, "wx_sunqi", "孙七", "123456", DateXST(2021, 3, 3));
    p.openService("P006", WX, "wx_zhouba", "周八", "123456", DateXST(2020, 6, 18));   // 未绑定 QQ

    p.openService("P001", WB, "10001", "张三的微博", "123456", DateXST(2017, 2, 2));
    p.openService("P003", WB, "10003", "王五说", "123456", DateXST(2018, 4, 12));
    p.openService("P004", WB, "10004", "赵六看世界", "123456", DateXST(2016, 10, 1));

    ServiceXST& qq = p.service(QQ);
    qq.addFriendship("10001", "10002", "室友李四", "三哥");
    qq.addFriendship("10001", "10003", "高中同学", "张三");
    qq.addFriendship("10001", "10004", "篮球队友", "张三");
    qq.addFriendship("10001", "10006", "周八", "张三");
    qq.addFriendship("10002", "10004", "老赵", "李四");
    qq.addFriendship("10003", "10006", "游戏搭子", "王五");
    qq.addFriendship("10004", "10006", "表弟", "表哥");

    ServiceXST& wx = p.service(WX);
    wx.addFriendship("wx_zhangsan", "wx_lisi", "室友李四", "三哥");
    wx.addFriendship("wx_lisi", "wx_sunqi", "孙七", "李四");

    ServiceXST& wb = p.service(WB);
    wb.addFriendship("10001", "10003", "王五", "张三");
    wb.addFriendship("10003", "10004", "赵六", "王五");

    // ---------- 群（每个服务均预置 1001~1006） ----------
    seedGroup(qq, 1001, "软件学院552401班", "10001", {"10002", "10003", "10004"}, {"10002"});
    qq.restoreSubGroup(1001, "C++讨论组", "10001", {"10001", "10002"});
    qq.restoreSubGroup(1001, "课设答疑", "10001", {"10001", "10003", "10004"});
    seedGroup(qq, 1002, "吉大校友会", "10004", {"10001", "10006"});
    seedGroup(qq, 1003, "篮球爱好者", "10002", {"10004"});
    seedGroup(qq, 1004, "考研交流群", "10003", {"10001", "10006"}, {"10001"});
    seedGroup(qq, 1005, "游戏开黑群", "10006", {"10003", "10004"});
    seedGroup(qq, 1006, "长春同城", "10001", {});

    seedGroup(wx, 1001, "相亲相爱一家人", "wx_zhangsan", {"wx_lisi"});
    seedGroup(wx, 1002, "宿舍群", "wx_lisi", {"wx_zhangsan", "wx_sunqi"});
    seedGroup(wx, 1003, "吉大二手交易", "wx_zhaoliu", {"wx_sunqi"});
    seedGroup(wx, 1004, "课设小组", "wx_zhangsan", {"wx_lisi", "wx_zhaoliu"});
    seedGroup(wx, 1005, "长春美食", "wx_sunqi", {});
    seedGroup(wx, 1006, "跑步打卡", "wx_lisi", {"wx_zhaoliu"});

    seedGroup(wb, 1001, "科技爱好者", "10001", {"10003"});
    seedGroup(wb, 1002, "吉大新鲜事", "10004", {"10001", "10003"}, {"10001"});
    seedGroup(wb, 1003, "电影分享", "10003", {});
    seedGroup(wb, 1004, "摄影交流", "10004", {"10003"});
    seedGroup(wb, 1005, "读书会", "10001", {"10004"});
    seedGroup(wb, 1006, "旅行日记", "10003", {"10004"}, {"10004"});
}

}  // namespace

void DemoDataXST::seed(PlatformXST& platform) {
    seedAll(platform);
}
