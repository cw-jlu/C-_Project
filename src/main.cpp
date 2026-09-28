// 模拟即时通信系统 —— 程序入口
// P2：好友管理演示。演示数据暂时固化在代码中（与 data/ 下文件一致），
//     P4 将以登录代替“选择用户”，P5 改为从文件加载，P6 整合为正式菜单

#include <iostream>
#include <stdexcept>

#include "ConsoleXST.h"
#include "FriendMenuXST.h"
#include "PlatformXST.h"

namespace {

void seedDemoData(PlatformXST& p) {
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
    p.openService("P001", QQ, "10001", "三哥", DateXST(2015, 6, 1));
    p.openService("P002", QQ, "10002", "小李飞刀", DateXST(2014, 3, 15));
    p.openService("P003", QQ, "10003", "五五开", DateXST(2016, 9, 10));
    p.openService("P004", QQ, "10004", "赵六六", DateXST(2013, 1, 20));
    p.openService("P006", QQ, "10006", "周末愉快", DateXST(2018, 7, 7));

    p.openService("P001", WX, "wx_zhangsan", "张三", DateXST(2019, 5, 5), "10001");
    p.openService("P002", WX, "wx_lisi", "李四", DateXST(2019, 8, 8), "10002");
    p.openService("P004", WX, "wx_zhaoliu", "赵六", DateXST(2020, 1, 1), "10004");
    p.openService("P005", WX, "wx_sunqi", "孙七", DateXST(2021, 3, 3));
    p.openService("P006", WX, "wx_zhouba", "周八", DateXST(2020, 6, 18));   // 未绑定 QQ

    p.openService("P001", WB, "10001", "张三的微博", DateXST(2017, 2, 2));
    p.openService("P003", WB, "10003", "王五说", DateXST(2018, 4, 12));
    p.openService("P004", WB, "10004", "赵六看世界", DateXST(2016, 10, 1));

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
}

}  // namespace

int main() {
    ConsoleXST::init();

    PlatformXST platform;
    try {
        seedDemoData(platform);
    } catch (const std::exception& e) {
        std::cout << "演示数据初始化失败: " << e.what() << "\n";
        return 1;
    }

    while (true) {
        std::cout << "\n==============================\n"
                  << "   腾*立体社交平台  (XST)\n"
                  << "==============================\n"
                  << "选择用户（P4 将改为登录）:\n";
        std::vector<const UserXST*> users = platform.users();
        for (size_t i = 0; i < users.size(); ++i) {
            std::cout << "  " << i + 1 << ". " << users[i]->name() << " (" << users[i]->id()
                      << ")  已开通: " << users[i]->servicesText() << "\n";
        }
        std::cout << "  0. 退出\n";
        int choice = ConsoleXST::readInt("请选择: ", 0, static_cast<int>(users.size()));
        if (choice == 0) break;
        FriendMenuXST(platform, users[choice - 1]->id()).run();
    }

    std::cout << "再见！\n";
    return 0;
}
