#ifndef H_MY_COMPLEX
#define H_MY_COMPLEX

#include <iostream>
#include <cmath>

class Comp{
private:
    double _real, _imag;
public:
    Comp();
    Comp(double r, double i);
    Comp(const Comp & c);
    Comp(const Comp & c1, const Comp & c2); // 取c1的实部、c2的虚部
    Comp operator+(const double i);
    Comp operator+(const Comp & c);
    Comp operator-(const double i);
    Comp operator-(const Comp & c);
    Comp operator*(const double i);
    Comp operator*(const Comp & c);
    Comp operator/(const double i);
    Comp operator/(const Comp & c);
    double getReal()const;
    double getImag()const;
    double & real();
    friend double real(const Comp & c);
    double & imag();
    friend double imag(const Comp & c);
    double & setReal(double r);
    double & setImag(double i);
    Comp & operator=(double);
    Comp & operator=(const Comp &);
    bool operator==(const Comp & c)const;
    bool operator!=(const Comp & c)const;
    Comp & operator+=(double r);
    Comp & operator+=(const Comp & c);
    Comp & operator-=(double r);
    Comp & operator-=(const Comp & c);
    Comp & operator*=(double r);
    Comp & operator*=(const Comp & c);
    Comp & operator/=(double r);
    Comp & operator/=(const Comp & c);
    friend Comp operator+(double d, const Comp & c);
    friend Comp operator+(const Comp & d, const Comp & c);
    friend Comp operator-(double d, const Comp & c);
    friend Comp operator-(const Comp & d, const Comp & c);
    friend Comp operator*(double d, const Comp & c);
    friend Comp operator*(const Comp & d, const Comp & c);
    friend Comp operator/(double d, const Comp & c);
    friend Comp operator/(const Comp & d, const Comp & c);
    friend std::istream & operator>>(std::istream & i, const Comp & c);
    friend std::ostream & operator<<(std::ostream & o, const Comp & c);
    double abs()const;
    friend double abs(const Comp & c);
    Comp conjugate()const; // 共轭
    friend Comp conjugate(const Comp & c);
};
#endif