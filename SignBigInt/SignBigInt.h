#ifndef H_SIGNBIGINT
#define H_SIGNBIGINT

#include "BigInt.h"

class SignBigInt{
private:
    BigInt num;
    bool sign;
public:
    SignBigInt();//空白构造函数
    SignBigInt(long long);//正数转大整数
    SignBigInt(const SignBigInt &);//拷贝构造函数
    SignBigInt(const std::string&);//字符串装大整数
    SignBigInt(bool s, BigInt n);
    SignBigInt(BigInt n);

    //重载赋值运算符
    SignBigInt & operator=(long long);
    SignBigInt & operator=(const std::string&);
    SignBigInt & operator=(const &);
    SignBigInt & operator+=(const SignBigInt b);
    SignBigInt & operator-=(const SignBigInt b);
    SignBigInt & operator*=(const SignBigInt b);
    SignBigInt & operator%=(const SignBigInt b);
    SignBigInt & operator/=(const SignBigInt b);
    SignBigInt & operator>>=(unsigned int n);
    SignBigInt & operator<<=(unsigned int n);

    friend std::istream & operator>>(std::istream &,SignBigInt &);//重载输入运算符>>
    friend std::ostream & operator<<(std::ostream &,const SignBigInt &);//重载输出运算符<<

    unsigned int size()const;  // 输出数字位数
    unsigned int length()const;  // =size()
    int at(unsigned int n)const;  // 获取从高位到低位的第n+1位数，从0开始，超范围返回-1
    int operator[](unsigned int n)const;// 获取从高位到低位的第n+1位数，从0开始，不检查是否超范围，可能出现编译正常，运行出错的情况

    // 重载比较运算符
    bool operator==(const SignBigInt & b) const;
    bool operator!=(const SignBigInt & b) const;
    bool operator>=(const SignBigInt & b) const;
    bool operator<=(const SignBigInt & b) const;
    bool operator>(const SignBigInt & b) const;
    bool operator<(const SignBigInt & b) const;

    // 重载算数运算符
    SignBigInt operator+(const SignBigInt & b) const;
    SignBigInt operator-(const SignBigInt & b) const;
    SignBigInt operator*(const SignBigInt & b) const;  // BigInt 与 BigInt 相乘
    SignBigInt operator/(const SignBigInt & b) const;
    SignBigInt operator%(const SignBigInt & b) const;
    SignBigInt operator>>(unsigned int n) const;
    SignBigInt operator<<(unsigned int n) const;

    friend SignBigInt operator+(long long a, const SignBigInt & b);
    friend SignBigInt operator-(long long a, const SignBigInt & b);
    friend SignBigInt operator*(long long a, const SignBigInt & b);
    friend SignBigInt operator/(long long a, const SignBigInt & b);
    friend SignBigInt operator%(long long a, const SignBigInt & b);


    SignBigInt operator+()const;
    SignBigInt operator-()const;

    SignBigInt operator++();
    SignBigInt operator++(int);
    SignBigInt operator--();
    SignBigInt operator--(int);

    std::string to_string()const;
    int to_int()const;
    long to_long()const;
    long long to_long_long()const;
};

void strip_leading_sign(std::string& s);
#endif
