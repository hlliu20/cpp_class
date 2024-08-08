
#include"BigInt.h"

BigInt::BigInt(){}
BigInt::BigInt(unsigned long long l){
    for(;l>0;l/=10){
        digits.push_back((int)(l%10));
    }
    std::reverse(digits.begin(),digits.end());
}
BigInt::BigInt(const BigInt & a){digits.assign(a.digits.begin(),a.digits.end());}
BigInt::BigInt(const BigInt & a, int s1, int s2){
    digits.assign(a.digits.begin()+s1,a.digits.begin()+s1+s2);
}
BigInt::BigInt(const std::string& s){
    std::string _s = s;
    strip_leading(_s);
    if(is_valid_number(_s)){
        for(size_t i=0;i<_s.size();++i){
            digits.push_back((int)(_s.at(i)-'0'));
        }
    } else {
        std::cout << std::endl << "\"" << s << "\" is not a valid number!" << std::endl;
    }

}
BigInt & BigInt::operator=(const BigInt& b){
    if(this != &b){
        digits.assign(b.digits.begin(), b.digits.end());
    }
    return *this;
}
BigInt & BigInt::operator=(unsigned long long l){
    digits.clear();
    for(;l>0;l/=10){
        digits.push_back((int)(l%10));
    }
    std::reverse(digits.begin(),digits.end());
    return *this;
}
BigInt & BigInt::operator=(const std::string& s){
    std::string _s = s;
    strip_leading(_s);
    if(!is_valid_number(_s)){
        std::cout << s << " is not a valid number!" << std::endl;
        return *this;
    }
    digits.clear();
    for(size_t i=0;i<_s.size();++i){
        digits.push_back((int)(_s.at(i)-'0'));
    }
    return *this;
}
std::istream & operator>>(std::istream& i,BigInt& a){
    std::string s;i >> s;a = s;
    if(a.digits[0] == 0) {
        strip_digits_leading_zeroes(a.digits);
    }
    return i;
}
std::ostream & operator<<(std::ostream& o,const BigInt& b){
    if(b.isZero()) o << "0";
    else for(int n:b.digits){o << n;}
    return o;
}
bool BigInt::operator==(const BigInt& b) const{
    if(digits.size()!=b.digits.size()){return false;}
    else{
        for(size_t i=0;i<size();++i){
            if(at(i) != b.at(i)) return false;
        }
    }
    return true;
}
bool BigInt::operator!=(const BigInt& b) const{
    return !(*this == b);
}
bool BigInt::operator>=(const BigInt& b) const{
    if(digits.size()<b.digits.size()){return false;}
    else if(digits.size()>b.digits.size()) {return true;}
    else {
        for(size_t i=0;i<size();++i){
            if(at(i) < b.at(i)) return false;
        }
    }
    return true;
}
bool BigInt::operator<=(const BigInt& b) const{
    if(digits.size()<b.digits.size()){return true;}
    else if(digits.size()>b.digits.size()) {return false;}
    else {
        for(size_t i=0;i<size();++i){
            if(at(i) > b.at(i)) return false;
        }
    }
    return true;
}

bool BigInt::operator>(const BigInt& b) const{
    if(digits.size() < b.digits.size()){return false;}
    else if(digits.size() > b.digits.size()){return true;}
    else {
        for(size_t i=0;i<digits.size();++i){
            if(digits[i]<b.digits[i]) return false;
        }
        return true;
    }
}
bool BigInt::operator<(const BigInt& b) const{
    if(digits.size() > b.digits.size()){return false;}
    else if(digits.size() < b.digits.size()){return true;}
    else {
        for(size_t i=0;i<digits.size();++i){
            if(digits[i]>b.digits[i]) return false;
        }
        return true;
    }
}

bool BigInt::operator<(const unsigned long long& l) const{return *this < BigInt(l);}
bool BigInt::operator>(const unsigned long long& l) const{return *this > BigInt(l);}
bool BigInt::operator<=(const unsigned long long& l) const{return *this <= BigInt(l);}
bool BigInt::operator>=(const unsigned long long& l) const{return *this >= BigInt(l);}
bool BigInt::operator==(const unsigned long long& l) const{return *this == BigInt(l);}
bool BigInt::operator!=(const unsigned long long& l) const{return *this != BigInt(l);}
bool BigInt::operator<(const std::string& s) const{return *this < BigInt(s);}
bool BigInt::operator>(const std::string& s) const{return *this > BigInt(s);}
bool BigInt::operator<=(const std::string& s) const{return *this <= BigInt(s);}
bool BigInt::operator>=(const std::string& s) const{return *this >= BigInt(s);}
bool BigInt::operator==(const std::string& s) const{return *this == BigInt(s);}
bool BigInt::operator!=(const std::string& s) const{return *this != BigInt(s);}



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
    strip_digits_leading_zeroes(a.digits);
    return a;
}

BigInt BigInt::operator+(const BigInt& bi) const{
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
    strip_digits_leading_zeroes(c.digits);
    return c;
}


BigInt BigInt::operator-(const BigInt& bi) const{
    BigInt a=*this,b=bi,c;
    int carry = 0,sub = 0,i=0;
    int s1 = a.digits.size(), s2 = b.digits.size(),s3=0;
    if(s1 == s2){
        if(a==b) return BigInt();
        else if(a>b){
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
    strip_digits_leading_zeroes(c.digits);
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

BigInt BigInt::operator*(const BigInt& bi) const{
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
BigInt BigInt::operator/(const BigInt& b) const{
    // 如果除数或被除数为0，返回0 !!！ 除数为0时未抛出异常
    if(b.isZero() || isZero()) return BigInt();
    if(*this < b) return 0;
    else if(*this == b) return 1;
    else{
        BigInt ret,tmp;
        int s1 = size(),s2 = b.size(),s3,dig=0;
        s3 = s1 - s2;
        tmp = BigInt(*this, 0, s2);
        if(tmp < b){ret.digits.push_back(dig);}
        else {
            while(tmp >= b){
                tmp = tmp - b;
                dig += 1;
            }
            ret.digits.push_back(dig);
        }
        for(int i=0;i<s3;++i){
            tmp.digits.push_back(digits[s2+i]);
            dig = 0;
            if(tmp < b){ret.digits.push_back(dig);}
            else {

                while(tmp >= b){
                    tmp = tmp - b;
                    ++dig;
                }
                ret.digits.push_back(dig);
            }
        }
        if(ret.digits[0] != 0) return ret;
        strip_digits_leading_zeroes(ret.digits);
        return ret;
    }
}

BigInt BigInt::operator%(const BigInt& b) const{
    // 如果除数为0，返回*this !!！ 除数为0时未抛出异常
    if((*this < b) || b.isZero()) return *this;
    else{
        BigInt tmp;
        int s1 = size(),s2 = b.size(),s3;
        s3 = s1 - s2;
        tmp = BigInt(*this, 0, s2);
        for(int i=0;i<s3;++i){
            tmp.digits.push_back(digits.at(i+s2));
            /*std::cout << std::endl << "tmp=" << tmp << std::endl;
            std::cout << "b=" << b << std::endl;
            std::cout << "tmp>=b" << (tmp>=b) << std::endl;*/
            while(tmp >= b){tmp -= b;}
        }
        return tmp;
    }
}

BigInt BigInt::operator+(const unsigned long long& l) const{return *this + BigInt(l);}
BigInt BigInt::operator-(const unsigned long long& l) const{return *this - BigInt(l);}
BigInt BigInt::operator*(const unsigned long long& l) const{return *this * BigInt(l);}
BigInt BigInt::operator/(const unsigned long long& l) const{return *this / BigInt(l);}
BigInt BigInt::operator%(const unsigned long long& l) const{return *this % BigInt(l);}
BigInt BigInt::operator+(const std::string& s) const{return *this + BigInt(s);}
BigInt BigInt::operator-(const std::string& s) const{return *this - BigInt(s);}
BigInt BigInt::operator*(const std::string& s) const{return *this * BigInt(s);}
BigInt BigInt::operator/(const std::string& s) const{return *this / BigInt(s);}
BigInt BigInt::operator%(const std::string& s) const{return *this % BigInt(s);}

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

BigInt operator+(unsigned long long a, const BigInt& b){return (b+a);}
BigInt operator-(unsigned long long a, const BigInt& b){return (BigInt(a)-b);}
BigInt operator*(unsigned long long a, const BigInt& b){return (b*a);}
BigInt operator/(unsigned long long a, const BigInt& b){return (BigInt(a)/b);}


BigInt& BigInt::operator+=(const BigInt& b){*this = *this + b;return *this;}
BigInt& BigInt::operator-=(const BigInt& b){*this = *this - b;return *this;}
BigInt& BigInt::operator*=(const BigInt& b){*this = *this * b;return *this;}
BigInt& BigInt::operator/=(const BigInt& b){*this = *this / b;return *this;}
BigInt& BigInt::operator%=(const BigInt& b){*this = *this % b;return *this;}
BigInt& BigInt::operator+=(const unsigned long long& l){*this = *this + BigInt(l);return *this;}
BigInt& BigInt::operator-=(const unsigned long long& l){*this = *this - BigInt(l);return *this;}
BigInt& BigInt::operator*=(const unsigned long long& l){*this = *this * BigInt(l);return *this;}
BigInt& BigInt::operator/=(const unsigned long long& l){*this = *this / BigInt(l);return *this;}
BigInt& BigInt::operator%=(const unsigned long long& l){*this = *this % BigInt(l);return *this;}
BigInt& BigInt::operator+=(const std::string& s){*this = *this + BigInt(s);return *this;}
BigInt& BigInt::operator-=(const std::string& s){*this = *this - BigInt(s);return *this;}
BigInt& BigInt::operator*=(const std::string& s){*this = *this * BigInt(s);return *this;}
BigInt& BigInt::operator/=(const std::string& s){*this = *this / BigInt(s);return *this;}
BigInt& BigInt::operator%=(const std::string& s){*this = *this % BigInt(s);return *this;}
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

BigInt& BigInt::operator++(){*this += 1;return *this;}
BigInt BigInt::operator++(int){BigInt tp=*this;*this += 1;return tp;}
BigInt& BigInt::operator--(){
    if(*this == BigInt()){
        std::cout << std::endl << "0-1=-1 ! 将返回 0。" << std::endl;
    } else {*this -= 1;}
    return *this;
}
BigInt BigInt::operator--(int){
    if(*this == BigInt()){
        std::cout << std::endl << "0-1=-1 ! 将返回 0。" << std::endl;
        return *this;
    } else {
        BigInt tp=*this;*this -= 1;return tp;
    }

}

std::string BigInt::to_string()const{
    std::string s;
    for(int n:digits){
        s += ('0' + n);
    }
    return s;
}
unsigned int BigInt::to_uint()const{
    BigInt t = (*this) % (BigInt(~(unsigned int)0)+1);
    unsigned int r=0;
    for(auto it=t.digits.rbegin();it<t.digits.rend();++it){
        r = r*10 + *it;
    }
    return r;
}
unsigned long BigInt::to_ulong()const{
    BigInt t = *this % (BigInt(~(unsigned long)0)+1);
    unsigned long r=0;
    for(auto it=t.digits.rbegin();it<t.digits.rend();++it){
        r = r*10 + *it;
    }
    return r;
}
unsigned long long BigInt::to_ulong_long()const{
    BigInt t = *this % (BigInt(~(unsigned long long)0)+1);
    unsigned long long r=0;
    for(auto it=t.digits.rbegin();it<t.digits.rend();++it){
        r = r*10 + *it;
    }
    return r;
}



void strip_digits_leading_zeroes(std::vector<int>& v){
    auto it = find_if(v.begin(),v.end(),[](int x){ return x != 0;});
    v.erase(v.begin(), it);
}

void strip_leading(std::string& s){
    size_t i=0;
    for(;i < s.size();++i){
        if(s[i] >= '1' && s[i] <= '9')
            break;
    }
    if(i == s.size()) s = "0";
    else s = s.substr(i);
}

bool is_valid_number(const std::string& s){
    for(char digit : s)
        if(digit < '0' || digit > '9')
            return false;
    return true;
}

void add_leading_zeroes(std::string& s, size_t num_zeroes){
    s = std::string(num_zeroes, '0') + s;
}

void add_trailing_zeroes(std::string& s, size_t num_zeroes){
    s += std::string(num_zeroes, '0');
}
