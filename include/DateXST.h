#ifndef DATE_XST_H
#define DATE_XST_H

#include <iosfwd>
#include <string>

// 日期类（年-月-日），用于出生日期、号码申请时间，并计算周岁与 T 龄
// 对象一经构造必为合法日期（非法日期在构造时抛出 std::invalid_argument）
class DateXST {
public:
    DateXST();                                   // 默认 1970-01-01
    DateXST(int year, int month, int day);

    static DateXST today();

    // 解析 "YYYY-MM-DD"（月、日可为一位数），失败返回 false，out 不变
    static bool tryParse(const std::string& text, DateXST& out);
    // 解析失败抛出 std::invalid_argument
    static DateXST parse(const std::string& text);

    static bool isLeapYear(int year);
    static int daysInMonth(int year, int month);
    static bool isValid(int year, int month, int day);

    int year() const;
    int month() const;
    int day() const;

    // 从本日期到 later 经过的整年数（周岁 / T 龄），later 早于本日期时为负数
    int fullYearsUntil(const DateXST& later) const;

    std::string toString() const;                // "YYYY-MM-DD"

    bool operator==(const DateXST& other) const;
    bool operator!=(const DateXST& other) const;
    bool operator<(const DateXST& other) const;
    bool operator<=(const DateXST& other) const;
    bool operator>(const DateXST& other) const;
    bool operator>=(const DateXST& other) const;

private:
    int serial() const;                          // 便于比较：YYYYMMDD

    int m_year;
    int m_month;
    int m_day;
};

std::ostream& operator<<(std::ostream& os, const DateXST& date);

#endif
