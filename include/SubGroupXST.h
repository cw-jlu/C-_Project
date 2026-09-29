#ifndef SUB_GROUP_XST_H
#define SUB_GROUP_XST_H

#include <string>
#include <vector>

// 临时讨论组（子群）：由群成员发起，成员必须是所在群的成员
class SubGroupXST {
public:
    SubGroupXST(const std::string& name, const std::string& creatorId,
                const std::vector<std::string>& members);

    const std::string& name() const;
    const std::string& creatorId() const;
    const std::vector<std::string>& members() const;

    bool hasMember(const std::string& id) const;
    bool removeMember(const std::string& id);
    std::string membersText() const;              // 如 "10001, 10002"

private:
    std::string m_name;
    std::string m_creatorId;
    std::vector<std::string> m_members;
};

#endif
