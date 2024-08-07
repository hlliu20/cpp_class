#include"SignBigInt.h"

SignBigInt::SignBigInt(){sign = true;}
SignBigInt::SignBigInt(long long l){*this = l;}
SignBigInt::SignBigInt(const SignBigInt & a){
    sign = a.sign;
    num.digits.assign(a.num.digits.begin(),a.num.digits.end());
}
SignBigInt::SignBigInt(const std::string& s){*this = s;}
SignBigInt::SignBigInt(bool s, BigInt n){sign = s;num = n;}
SignBigInt & SignBigInt::operator=(long long l){
    if(l<0){sign=false;l = - l;}else{sign=true;}
    num.digits.clear();
    for(;l>0;l/=10){
        num.digits.push_back((int)(l%10));
    }
    std::reverse(num.digits.begin(),num.digits.end());
    return *this;
}
SignBigInt & SignBigInt::operator=(const std::string& s){
    if(s.size()==0){sign=true;return *this;}
    if(s[0] == '-'){sign=false;}
    else {sign = true;}
    num.digits.clear();
    size_t i=0;
    if(s[0] == '+'  ||  s[0] == '-') i=1;
    for(i=0;i<s.size();++i){
        num.digits.push_back((int)(s.at(i)-'0'));
    }
    return *this;
}
SignBigInt & SignBigInt::operator+=(const SignBigInt b){*this = *this + b;return *this;}
SignBigInt & SignBigInt::operator-=(const SignBigInt b){*this = *this - b;return *this;}
SignBigInt & SignBigInt::operator*=(const SignBigInt b){*this = *this * b;return *this;}
SignBigInt & SignBigInt::operator/=(const SignBigInt b){*this = *this / b;return *this;}
SignBigInt & SignBigInt::operator>>=(unsigned int n){
    if(n>=num.size()){num.digits.clear();}
    else {num.digits.assign(num.digits.begin(),num.digits.begin()+size()-n);}
    return *this;
}
SignBigInt & SignBigInt::operator<<=(unsigned int n){
    if(!num.isZero()){
        num.digits.insert(num.digits.end(), n, 0);
    }
    return *this;
}
std::istream & operator>>(std::istream & i,SignBigInt & a){
    std::string s;i >> s;a = s;
    if(a.num.digits[0] == 0) {
        auto it = find_if(a.num.digits.begin(),a.num.digits.end(),[](int x){ return x != 0;});
        a.num.digits.erase(a.num.digits.begin(), it);
    }
    return i;
}
std::ostream & operator<<(std::ostream & o,const SignBigInt & b){

    if(!b.sign) o << '-';
    o << b.num;
    return o;
}
SignBigInt operator+(long long a, const SignBigInt &b)
{return SignBigInt(a) + b;}
SignBigInt operator-(long long a, const SignBigInt &b)
{return SignBigInt(a) - b;}
SignBigInt operator*(long long a, const SignBigInt &b)
{return SignBigInt(a) * b;}
SignBigInt operator/(long long a, const SignBigInt &b)
{return SignBigInt(a) / b;}
unsigned int SignBigInt::size()const{return num.digits.size()?num.digits.size():1;}
unsigned int SignBigInt::length()const{return size();}
int SignBigInt::at(unsigned int n)const{
    if(n<size()) return num.digits[n];
    else return -1;
}
int SignBigInt::operator[](unsigned int n)const{return num.digits[n];}
bool SignBigInt::operator==(const SignBigInt & b) const {
    if((sign != b.sign) || (size() != b.size())){
        return false;
    } else {
        for(size_t i=0;i<size();++i){
            if(num.at(i) != b.num.at(i)){
                return false;
            }
        }
        return true;
    }
}
bool SignBigInt::operator!=(const SignBigInt & b) const{
    return !(*this == b);
}
bool SignBigInt::operator>=(const SignBigInt & b) const{
    if(*this == b) return true;
    if(sign==true && b.sign==false) {return true;}
    else if(sign==true && b.sign==true) {return num > b.num;}
    else if(sign==false && b.sign==true) {return false;}
    else {return num < b.num;}
    // return (*this == b) || (*this > b);
}
bool SignBigInt::operator<=(const SignBigInt & b) const{
    if(*this == b) return true;
    if(sign==true && b.sign==false) {return false;}
    else if(sign==true && b.sign==true) {return num < b.num;}
    else if(sign==false && b.sign==true) {return true;}
    else {return num > b.num;}
    // return (*this == b) || (*this < b);
}
bool SignBigInt::operator>(const SignBigInt & b) const{
    if(sign==true && b.sign==false) {return true;}
    else if(sign==true && b.sign==true) {return num > b.num;}
    else if(sign==false && b.sign==true) {return false;}
    else {return num < b.num;}
}
bool SignBigInt::operator<(const SignBigInt & b) const{
    if(sign==true && b.sign==false) {return false;}
    else if(sign==true && b.sign==true) {return num < b.num;}
    else if(sign==false && b.sign==true) {return true;}
    else {return num > b.num;}
}
SignBigInt SignBigInt::operator+(const SignBigInt & b) const
{
    SignBigInt re;
    if(sign==true && b.sign==true) {
        re.sign=true;re.num = num + b.num;
    } else if(sign==true && b.sign==false) {
        if(num >= b.num){
            re.sign = true;re.num = num - b.num;
        } else {
            re.sign = false;re.num = b.num - num;
        }
    } else if(sign==false && b.sign==true) {
        if(num > b.num){
            re.sign = false;re.num = num - b.num;
        } else {
            re.sign = true;re.num = b.num - num;
        }
    } else {
        re.sign = false;re.num = num + b.num;
    };
    return re;
}
SignBigInt SignBigInt::operator-(const SignBigInt &b) const
{
    SignBigInt re;
    if(sign==true && b.sign==false) {
        re.sign=true;re.num = num + b.num;
    } else if(sign==true && b.sign==true) {
        if(num >= b.num){
            re.sign = true;re.num = num - b.num;
        } else {
            re.sign = false;re.num = b.num - num;
        }
    } else if(sign==false && b.sign==false) {
        if(num > b.num){
            re.sign = false;re.num = num - b.num;
        } else {
            re.sign = true;re.num = b.num - num;
        }
    } else {
        re.sign = false;re.num = num + b.num;
    };
    return re;
}
SignBigInt SignBigInt::operator*(const SignBigInt &b) const
{
    SignBigInt re;
    if(sign == b.sign) {re.sign = true;}
    else {re.sign = false;}
    re.num = num * b.num;
    return re;
}
SignBigInt SignBigInt::operator/(const SignBigInt &b) const
{
    SignBigInt re;
    if(sign == b.sign) {re.sign = true;}
    else {re.sign = false;}
    re.num = num / b.num;
    return re;
}
SignBigInt SignBigInt::operator>>(unsigned int n) const
{return SignBigInt(sign,num >> n);}
SignBigInt SignBigInt::operator<<(unsigned int n) const
{return SignBigInt(sign,num << n);}
