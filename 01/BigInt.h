#ifndef H_BIGINT
#define H_BIGINT

#include<vector>
#include<string>
#include<algorithm>
#include<iostream>

class BigInt{
public:
    std::vector<int> digits;//储存每一位
public:
    BigInt();//空白构造函数
    BigInt(long long);//正数转大整数
    BigInt(const BigInt &);//拷贝构造函数
    BigInt(const BigInt & b, int s1, int s2);// 取b的第s1位到第s2位
    BigInt(const std::string&);//字符串装大整数
    //重载赋值运算符
    BigInt & operator=(long long);
    BigInt & operator=(const std::string&);
    BigInt & operator+=(const BigInt b);
    BigInt & operator-=(const BigInt b);
    BigInt & operator*=(const BigInt b);
    BigInt & operator/=(const BigInt b);
    BigInt & operator>>=(unsigned int n);
    BigInt & operator<<=(unsigned int n);

    friend std::istream & operator>>(std::istream &,BigInt &);//重载输入运算符>>
    friend std::ostream & operator<<(std::ostream &,const BigInt &);//重载输出运算符<<

    // 重载比较运算符
    bool operator==(const BigInt & b) const;
    bool operator!=(const BigInt & b) const;
    bool operator>=(const BigInt & b) const;
    bool operator<=(const BigInt & b) const;
    bool operator>(const BigInt & b) const;
    bool operator<(const BigInt & b) const;

    bool isZero()const;  // 判断自身是否为0
    int size()const;  // 输出数字位数
    int length()const;  // =size()
    
    int at(unsigned int n)const;  // 获取从高位到低位的第n+1位数，从0开始，超范围返回-1
    int operator[](unsigned int n)const;// 获取从高位到低位的第n+1位数，从0开始，不检查是否超范围，可能出现编译正常，运行出错的情况


    // 重载算数运算符
    BigInt addOne(int n) const;
    BigInt operator+(const BigInt & b) const;
    BigInt operator-(const BigInt & b) const;
    BigInt mulOne(int n) const;  // BigInt 与 一位数 相乘
    BigInt operator*(const BigInt & b) const;  // BigInt 与 BigInt 相乘
    BigInt operator/(const BigInt & b) const;
    BigInt operator>>(unsigned int n) const;
    BigInt operator<<(unsigned int n) const;
    
    friend BigInt operator+(unsigned long long a, const BigInt & b);
    friend BigInt operator-(unsigned long long a, const BigInt & b);
    friend BigInt operator*(unsigned long long a, const BigInt & b);
    friend BigInt operator/(unsigned long long a, const BigInt & b);

};
#endif