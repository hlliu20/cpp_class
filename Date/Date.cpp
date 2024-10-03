#include "Date.h"

/// @brief 是否为闰年
/// @param year 年份
/// @return year为闰年返回true，否则返回false
inline bool Date::isLeapYear(int year)
{
    assert(year != 0);
    if(year < 0){return (-year)%4==1;}
    // else if(year == 0) {cout << "年份错误，没有0年！" << endl;return false;}
    else if(year < 1582){return year%4==0;}
    else {return ((year%4==0)&&(year%100!=0))||(year%400==0);}
    // return ((year%4==0)&&(year%100!=0))||(year%400==0);
}

/// @brief year年的天数
/// @param year 年份
/// @return 闰年366，否则365
inline int Date::GetYearDay(int year)
{
    if(isLeapYear(year)){return 366;}
    else {return 365;}
}

/// @brief year年month月的天数
/// @param year 年份
/// @param month 月份
/// @return 正确月份返回当月天数，否则返回-1
inline int Date::GetMonthDay(int year, int month)
{
    assert(year != 0);
    assert((month >= 1) && (month <= 12));
    static int days[13] = {-1,31,28,31,30,31,30,31,31,30,31,30,31};
    // if(year < 1){
    //     cout << "年份错误，没有" << year << "年" << emdl;
    //     year = 1900;
    // }
    // if(month < 1 || month > 12){
    //     cout << "月份错误，没有" << month << "月" << endl;
    //     month = 0;
    // }
    if(month == 2){
        if(isLeapYear(year)){return 29;}
        else {return 28;}
    } else {
        return days[month];
    }
}

/// @brief 全缺省构造函数
/// @param year 年份
/// @param month 月份
/// @param day 第几天
Date::Date(int year, int month, int day)
{
    _year = year;_month = month;_day = day;
}

/// @brief 拷贝构造函数
/// @param d 拷贝对象
Date::Date(const Date &d)
{
    _year = d._year;_month = d._month;_day = d._day;
}

/// @brief 复制构造函数
/// @param d 复制对象
/// @return 若d就是自己直接返回自己，否则复制再返回自己
Date &Date::operator=(const Date &d)
{
    if(this != &d){
        _year = d._year;_month = d._month;_day = d._day;
    }
    return *this;
}

/// @brief 析构函数
Date::~Date()
{
    _year = 0;_month = 0;_day = 0;
}
 
Date &Date::operator+=(int day)
{
    int n;
    if(day > 366){
        if((_month==1)||((_month==2)&&(_day<=28))){
            n = GetYearDay(_year);
            while(day > n){
                _year += 1;
                day -= n;
                n = GetYearDay(_year);
            }
        } else if(day==29){
            _year += 1;
            day -= 365;
            _day = 28;
            n = GetYearDay(_year);
            while(day > n){
                _year += 1;
                day -= n;
                n = GetYearDay(_year);
            }
        } else {
            n = GetYearDay(_year+1);
            while(day > n){
                _year += 1;
                day -= n;
                n = GetYearDay(_year+1);
            }
        }
    }
    _day += day;
    n = GetMonthDay(_year, _month);
    while(_day > n){
        if(_month == 12){
            _year += 1;_month = 1;
        } else {
            _month += 1;
        }
        _day -= n;
        n = GetMonthDay(_year, _month);
    }
    return *this;
}

Date &Date::operator-=(int day)
{
    int n;
    if(day < -366){
        if((_month==1)||((_month==2)&&(_day<=28))){
            n = GetYearDay(_year-1);
            while(day > n){
                _year -= 1;
                day -= n;
                n = GetYearDay(_year-1);
            }
        } else if(day==29){
            _year -= 1;
            day += 366;
            _day = 28;
            n = GetYearDay(_year-1);
            while(day > n){
                _year -= 1;
                day -= n;
                n = GetYearDay(_year-1);
            }
        } else {
            n = GetYearDay(_year);
            while(day > n){
                _year += 1;
                day -= n;
                n = GetYearDay(_year);
            }
        }
    }
    _day -= day;
    n = GetMonthDay(_year, _month);
    while(_day <= 0){
        if(_month == 1){
            _year -= 1;_month = 12;
        } else {
            _month -= 1;
        }
        n = GetMonthDay(_year, _month);
        _day += n;
    }
    return *this;
}

Date Date::operator+(int day)
{
    Date temp(*this);
    temp += day;
    return temp;
}

Date Date::operator-(int day)
{
    Date temp(*this);
    temp -= day;
    return temp;
}

int Date::operator-(const Date &d)
{
    Date big = *this;
    Date small = d;
    bool flag = true;
    if(*this < d){
        big = d;
        small = *this;
        flag = false;
    }
    int i=0;
    while(small < big){
        ++small;++i;
    }
    return flag?i:-i;
}

Date &Date::operator++()
{
    *this += 1;
    return *this;
}

Date &Date::operator--()
{
    *this -= 1;
    return *this;
}

Date Date::operator++(int)
{
    Date temp(*this);
    *this += 1;
    return temp;
}

Date Date::operator--(int)
{
    Date temp(*this);
    *this -= 1;
    return temp;
}

bool Date::operator>(const Date &d)
{
    return !(*this <= d);
    // return (_year>d._year)||((_year==d._year)&&(_month>d._month))||((_year==d._year)&&(_month==d._month)&&(_day>d._day));
}

bool Date::operator<(const Date &d)
{
    return !(*this >= d);
    // return (_year<d._year)||((_year==d._year)&&(_month<d._month))||((_year==d._year)&&(_month==d._month)&&(_day<d._day));
}

bool Date::operator>=(const Date &d)
{
    if(_year > d._year) {return true;}
    else if(_year < d._year) {return false;}
    else {
        if(_month > d._month) {return true;}
        else if(_month < d._month) {return false;}
        else {
            if(_day >= d._day) {return true;}
            else {return false;}
        }
    }
    return false;
    // return !(*this < d);
}

bool Date::operator<=(const Date &d)
{
    if(_year < d._year) {return true;}
    else if(_year > d._year) {return false;}
    else {
        if(_month < d._month) {return true;}
        else if(_month > d._month) {return false;}
        else {
            if(_day <= d._day) {return true;}
            else {return false;}
        }
    }
    return false;
    // return !(*this > d);
}

bool Date::operator==(const Date &d)
{
    // return (_year==d._year)&&(_month==d._month)&&(_day==d._day);
    return !(*this != d);
}

bool Date::operator!=(const Date &d)
{
    return (_year!=d._year)||(_month!=d._month)||(_day!=d._day);
}

/// @brief 友元函数，流输出
/// @param o 标准输出流
/// @param d 日期对象
/// @return 标准输出流o
ostream &operator<<(ostream &o, const Date &d)
{
    o << d._year << "年" << d._month << "月" << d._day << "日";
    return o;
}

/// @brief 友元函数，流输入
/// @param is 标准输入流
/// @param d 日期对象
/// @return 标准输入流is
istream &operator>>(istream &is, Date &d)
{
    is >> d._year >> d._month >> d._day;
    assert(d._year != 0);
    assert((d._month >= 1) && (d._month <= 12));
    assert((d._day >= 1) && (d._day <= d.GetMonthDay(d._year, d._month)));
    return is;
}

/// @brief 输出日期
/// @param c 分隔符，若为0输出年月日
void Date::Print(char c) const
{
    if(c == 0){
        cout << _year << "年" << _month << "月" << _day << "日";
    } else {
        cout << _year << c << _month << c << _day;
    }
}
