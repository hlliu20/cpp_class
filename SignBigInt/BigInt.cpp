
#include"BigInt.h"

BigInt::BigInt(){}
BigInt::BigInt(long long l){*this = l;}
BigInt::BigInt(const BigInt & a){digits.assign(a.digits.begin(),a.digits.end());}
BigInt::BigInt(const BigInt & a, int s1, int s2){
    digits.assign(a.digits.begin()+s1,a.digits.begin()+s1+s2);
}
BigInt::BigInt(const std::string& s){*this = s;}
BigInt & BigInt::operator=(long long l){
    digits.clear();
    for(;l>0;l/=10){
        digits.push_back((int)(l%10));
    }
    std::reverse(digits.begin(),digits.end());
    return *this;
}
BigInt & BigInt::operator=(const std::string& s){
    digits.clear();
    for(size_t i=0;i<s.size();++i){
        digits.push_back((int)(s.at(i)-'0'));
    }
    return *this;
}
std::istream & operator>>(std::istream & i,BigInt & a){
    std::string s;i >> s;a = s;
    if(a.digits[0] == 0) {
        auto it = find_if(a.digits.begin(),a.digits.end(),[](int x){ return x != 0;});
        a.digits.erase(a.digits.begin(), it);
    }
    return i;
}
std::ostream & operator<<(std::ostream & o,const BigInt & b){
    if(b.isZero()) o << "0";
    else for(int n:b.digits){o << n;}
    return o;
}
bool BigInt::operator==(const BigInt & b) const{
    if(digits.size()!=b.digits.size()){return false;}
    else if(digits != b.digits){return false;}
    else {return true;}
}
bool BigInt::operator!=(const BigInt & b) const{
    if(digits.size()!=b.digits.size()){return true;}
    else if(digits != b.digits){return true;}
    else {return false;}
}
bool BigInt::operator>=(const BigInt & b) const{
    if(digits.size()<b.digits.size()){return false;}
    else if(digits < b.digits){return false;}
    else {return true;}
}
bool BigInt::operator<=(const BigInt & b) const{
    if(digits.size()>b.digits.size()){return false;}
    else if(digits > b.digits){return false;}
    else {return true;}
}

bool BigInt::operator>(const BigInt & b) const{
    if(digits.size() < b.digits.size()){return false;}
    else if(digits.size() > b.digits.size()){return true;}
    else {
        for(size_t i=0;i<digits.size();++i){
            if(digits[i]<b.digits[i]) return false;
        }
        return true;
    }
}
bool BigInt::operator<(const BigInt & b) const{
    if(digits.size() > b.digits.size()){return false;}
    else if(digits.size() < b.digits.size()){return true;}
    else {
        for(size_t i=0;i<digits.size();++i){
            if(digits[i]>b.digits[i]) return false;
        }
        return true;
    }
}

bool BigInt::isZero()const{return digits.size()==0;}
unsigned int BigInt::size()const{return digits.size()?digits.size():1;}
unsigned int BigInt::length()const{return size();}
int BigInt::at(unsigned int n)const{
    if(n<size()) return digits[n];
    else return -1;
}
int BigInt::operator[](unsigned int n)const{
    return digits[n];
}

BigInt BigInt::addOne(int n) const{
    BigInt a,b=*this;
    reverse(b.digits.begin(),b.digits.end());
    if(b.digits.size()==0){a=n;}
    else if(n==0){a=b;}
    else{
        int carry=0,sum=0;
        for(size_t i=0;i<digits.size();++i){
            sum = n + digits[i] + carry;
            carry = sum/10;
            a.digits.push_back(sum%10);
        }
        while(carry>0){
            a.digits.push_back(carry);
            carry/=10;
        }
    }
    reverse(a.digits.begin(),a.digits.end());
    if(a.digits[0] != 0) return a;
    auto it = find_if(a.digits.begin(),a.digits.end(),[](int x){ return x != 0;});
    a.digits.erase(a.digits.begin(), it);
    return a;
}

BigInt BigInt::operator+(const BigInt & bi) const{
    BigInt c;
    if(digits.size()==0){c=bi;return c;}
    else if(bi.digits.size()==0){return *this;}
    else {
        BigInt a = *this,b = bi;
        reverse(a.digits.begin(),a.digits.end());
        reverse(b.digits.begin(),b.digits.end());
        int carry=0,sum=0;
        int s1 = a.digits.size(),s2 = b.digits.size();
        if(s1 <= s2){
            for(int i=0;i<s1;++i){
                sum = carry+a.digits[i]+b.digits[i];
                carry = sum/10;
                c.digits.push_back(sum%10);
            }
            for(int i=s1;i<s2;++i){
                sum = carry + b.digits[i];
                carry = sum/10;
                c.digits.push_back(sum%10);
            }
            while(carry>0){
                c.digits.push_back(carry);
                carry/=10;
            }
        } else {
            for(int i=0;i<s2;++i){
                sum = carry+a.digits[i]+b.digits[i];
                carry = sum/10;
                c.digits.push_back(sum%10);
            }
            for(int i=s2;i<s1;++i){
                sum = carry + a.digits[i];
                carry = sum/10;
                c.digits.push_back(sum%10);
            }
            while(carry>0){
                c.digits.push_back(carry);
                carry/=10;
            }
        }
    }
    reverse(c.digits.begin(),c.digits.end());
    if(c.digits[0] != 0) return c;
    auto it = find_if(c.digits.begin(),c.digits.end(),[](int x){ return x != 0;});
    c.digits.erase(c.digits.begin(), it);
    return c;
}


BigInt BigInt::operator-(const BigInt & bi) const{
    BigInt a=*this,b=bi,c;
    int carry = 0,sub = 0,i=0;
    int s1 = a.digits.size(), s2 = b.digits.size(),s3=0;
    if(s1 == s2){
        if(a.digits == b.digits) return BigInt();
        else if(a.digits > b.digits){
            for(i=s1-1;i>=0;--i){
                sub = a.digits[i] - b.digits[i] - carry;
                if(sub < 0){sub += 10;carry = 1;}
                else {carry = 0;}
                c.digits.push_back(sub);
            }
        } else {
            for(i=s2-1;i>=0;--i){
                sub = b.digits[i] - a.digits[i] - carry;
                if(sub < 0){sub += 10;carry = 1;}
                else {carry = 0;}
                c.digits.push_back(sub);
            }
        }

    }else if(s1 > s2){
        s3 = s1 - s2;
        for(i=s1-1;i>=s3;--i){
            sub = a.digits[i] - b.digits[i-s3] - carry;
            if(sub < 0){sub += 10;carry = 1;}
            else {carry = 0;}
            c.digits.push_back(sub);
        }
        while(i >= 0){
            sub = a.digits[i] - carry;
            if(sub < 0){sub += 10;carry = 1;}
            else {carry = 0;}
            c.digits.push_back(sub);
            --i;
        }
    } else {
        s3 = s2 - s1;
        for(i=s2-1;i>=s3;--i){
            sub = b.digits[i] - a.digits[i-s3] - carry;
            if(sub < 0){sub += 10;carry = 1;}
            else {carry = 0;}
            c.digits.push_back(sub);
        }
        while(i >= 0){
            sub = b.digits[i] - carry;
            if(sub < 0){sub += 10;carry = 1;}
            else {carry = 0;}
            c.digits.push_back(sub);
            --i;
        }
    }
    reverse(c.digits.begin(),c.digits.end());
    if(c.digits[0] != 0) return c;
    auto it = find_if(c.digits.begin(),c.digits.end(),[](int x){ return x != 0;});
    c.digits.erase(c.digits.begin(), it);
    return c;
}

BigInt BigInt::mulOne(int n) const{
    BigInt a = *this;
    reverse(a.digits.begin(),a.digits.end());
    int carry = 0, mul = 0;
    for(size_t i=0;i<a.digits.size();++i){
        mul = carry + a.digits[i]*n;
        carry = mul / 10;
        a.digits[i] = mul % 10;
    }
    while(carry){ a.digits.push_back(carry);carry/=10;}
    reverse(a.digits.begin(),a.digits.end());
    return a;
}

BigInt BigInt::operator*(const BigInt & bi) const{
    if(isZero() || bi.isZero()) return BigInt();  // 如果两个数中有0，则相乘结果为0
    BigInt re;
    int scale = 1;
    if(digits.size() > bi.digits.size()){
        for(int i=bi.digits.size()-1;i>=0;--i)
            re = re + mulOne(bi.digits[i]*scale);
        scale += 10;
    } else {
        for(int i=digits.size()-1;i>=0;--i)
            re = re + bi.mulOne(digits[i]*scale);
        scale += 10;
    }
    return re;
}
BigInt BigInt::operator/(const BigInt & b) const{
    // 如果除数或被除数为0，返回0 !!！ 除数为0时未抛出异常
    if(b.isZero() || isZero()) return BigInt();
    if(*this < b) return 0;
    else if(*this == b) return 1;
    else{
        BigInt ret,tmp;
        int s1 = size(),s2 = b.size(),s3,dig=0;
        s3 = s1 - s2;
        for(int i=0;i<=s3;++i){
            if(tmp == 0) {tmp = BigInt(*this, i, s2);
            } else {
                tmp.digits.push_back(digits[s2+i-1]);
            }
            dig = 0;
            if(tmp < b){ret.digits.push_back(dig);}
            else {
                while(tmp > b){
                    tmp = tmp - b;
                    dig += 1;
                }
                ret.digits.push_back(dig);
            }
        }
        if(ret.digits[0] != 0) return ret;
        auto it = find_if(ret.digits.begin(),ret.digits.end(),[](int x){ return x != 0;});
        ret.digits.erase(ret.digits.begin(), it);
        return ret;
    }
}

BigInt BigInt::operator>>(unsigned int n) const{
    if(n>=size()) return BigInt();
    else {
        BigInt re = *this;
        re.digits.assign(re.digits.begin(),re.digits.begin()+size()-n);
        return re;
    }
}
BigInt BigInt::operator<<(unsigned int n) const{
    if(isZero()) return BigInt();
    else{
        BigInt re = *this;
        // while(n--){re.digits.push_back(0);}
        re.digits.insert(re.digits.end(), n, 0);
        return re;
    }
}

BigInt operator+(unsigned long long a, const BigInt & b){return (b+a);}
BigInt operator-(unsigned long long a, const BigInt & b){return (BigInt(a)-b);}
BigInt operator*(unsigned long long a, const BigInt & b){return (b*a);}
BigInt operator/(unsigned long long a, const BigInt & b){return (BigInt(a)/b);}

BigInt & BigInt::operator+=(const BigInt b){*this = *this + b;return *this;}
BigInt & BigInt::operator-=(const BigInt b){*this = *this - b;return *this;}
BigInt & BigInt::operator*=(const BigInt b){*this = *this * b;return *this;}
BigInt & BigInt::operator/=(const BigInt b){*this = *this / b;return *this;}
BigInt & BigInt::operator>>=(unsigned int n){
    // *this = *this >> n;
    if(n>=size()){digits.clear();}
    else {digits.assign(digits.begin(),digits.begin()+size()-n);}
    return *this;
}
BigInt & BigInt::operator<<=(unsigned int n){
    if(!isZero()){
        // while(n--){digits.push_back(0);}
        digits.insert(digits.end(), n, 0);
    }
    return *this;
}
