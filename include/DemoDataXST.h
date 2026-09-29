#ifndef DEMO_DATA_XST_H
#define DEMO_DATA_XST_H

class PlatformXST;

// 演示数据（与 data/ 下的文件内容一致）：6 个用户、三个服务的账号、好友与 1001~1006 号群
// 所有演示账号的密码均为 123456
// 数据非法时抛出 std::invalid_argument
class DemoDataXST {
public:
    DemoDataXST() = delete;

    static void seed(PlatformXST& platform);
};

#endif
