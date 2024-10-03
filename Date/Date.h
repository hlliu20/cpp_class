#pragma once
#include <iostream>
#include <assert.h>
using namespace std;

class Date{
    friend ostream& operator<<(ostream& o, const Date& d);
    friend istream& operator>>(istream& i, Date& d);
public:
    inline bool isLeapYear(int year);
    inline int GetYearDay(int year);
    inline int GetMonthDay(int year, int month);
    Date(int year=1900, int month=1, int day=1);
    Date(const Date& d);
    Date& operator=(const Date& d);
    ~Date();
    Date& operator+=(int day);
    Date& operator-=(int day);
    Date operator+(int day);
    Date operator-(int day);
    int operator-(const Date& d);
    Date& operator++();
    Date& operator--();
    Date operator++(int);
    Date operator--(int);
    bool operator>(const Date& d);
    bool operator<(const Date& d);
    bool operator>=(const Date& d);
    bool operator<=(const Date& d);
    bool operator==(const Date& d);
    bool operator!=(const Date& d);
    void Print(char c='/')const;

private:
    int _year;
    int _month;
    int _day;

};