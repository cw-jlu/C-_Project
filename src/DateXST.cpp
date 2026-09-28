#include "DateXST.h"

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>

DateXST::DateXST() : m_year(1970), m_month(1), m_day(1) {}

DateXST::DateXST(int year, int month, int day)
    : m_year(year), m_month(month), m_day(day) {
    if (!isValid(year, month, day)) {
        throw std::invalid_argument("非法日期: " + std::to_string(year) + "-" +
                                    std::to_string(month) + "-" + std::to_string(day));
    }
}

DateXST DateXST::today() {
    std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    return DateXST(local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
}

bool DateXST::tryParse(const std::string& text, DateXST& out) {
    int year = 0, month = 0, day = 0, used = 0;
    if (std::sscanf(text.c_str(), "%d-%d-%d%n", &year, &month, &day, &used) != 3 ||
        used != static_cast<int>(text.size())) {
        return false;
    }
    if (!isValid(year, month, day)) return false;
    out = DateXST(year, month, day);
    return true;
}

DateXST DateXST::parse(const std::string& text) {
    DateXST result;
    if (!tryParse(text, result)) {
        throw std::invalid_argument("日期格式错误（应为 YYYY-MM-DD）: " + text);
    }
    return result;
}

bool DateXST::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int DateXST::daysInMonth(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

bool DateXST::isValid(int year, int month, int day) {
    return year >= 1900 && year <= 2100 &&
           month >= 1 && month <= 12 &&
           day >= 1 && day <= daysInMonth(year, month);
}

int DateXST::year() const { return m_year; }
int DateXST::month() const { return m_month; }
int DateXST::day() const { return m_day; }

int DateXST::fullYearsUntil(const DateXST& later) const {
    int years = later.m_year - m_year;
    // 当年的“纪念日”还没到，则少算一年
    if (later.m_month * 100 + later.m_day < m_month * 100 + m_day) {
        --years;
    }
    return years;
}

std::string DateXST::toString() const {
    std::ostringstream os;
    os << std::setfill('0') << std::setw(4) << m_year << '-'
       << std::setw(2) << m_month << '-' << std::setw(2) << m_day;
    return os.str();
}

int DateXST::serial() const { return m_year * 10000 + m_month * 100 + m_day; }

bool DateXST::operator==(const DateXST& other) const { return serial() == other.serial(); }
bool DateXST::operator!=(const DateXST& other) const { return serial() != other.serial(); }
bool DateXST::operator<(const DateXST& other) const { return serial() < other.serial(); }
bool DateXST::operator<=(const DateXST& other) const { return serial() <= other.serial(); }
bool DateXST::operator>(const DateXST& other) const { return serial() > other.serial(); }
bool DateXST::operator>=(const DateXST& other) const { return serial() >= other.serial(); }

std::ostream& operator<<(std::ostream& os, const DateXST& date) {
    return os << date.toString();
}
