#include "Comp.h"

Comp::Comp()
{_real=0;_imag=0;}

Comp::Comp(double r, double i)
{_real=r;_imag=i;}

Comp::Comp(const Comp &c)
{_real=c._real;_imag=c._imag;}

Comp::Comp(const Comp &c1, const Comp &c2)
{_real=c1._real;_imag=c2._imag;}

double Comp::getReal()const{return _real;}

double Comp::getImag()const{return _imag;}

double &Comp::real()
{return _real;}

double real(const Comp & c)
{return c.getReal();}

double &Comp::imag()
{return _imag;}

double imag(const Comp & c)
{return c.getImag();}

double & Comp::setReal(double r)
{_real = r;return _real;}

double & Comp::setImag(double i)
{_imag = i;return _imag;}

Comp & Comp::operator=(double r)
{_real=r;_imag=0;return *this;}

Comp &Comp::operator=(const Comp & c)
{
    if(this!=&c){
        _real = c._real;_imag = c._imag;
    }
    return *this;
}

bool Comp::operator==(const Comp & c) const
{return (_real==c._real)&&(_imag==c._imag);}

bool Comp::operator!=(const Comp & c) const
{return (_real!=c._real)||(_imag!=c._imag);}

Comp &Comp::operator+=(double r)
{_real += r;return *this;}

Comp &Comp::operator+=(const Comp &c)
{_real+=c._real;_imag+=c._imag;return *this;}

Comp &Comp::operator-=(double r)
{_real -= r;return *this;}

Comp &Comp::operator-=(const Comp &c)
{_real-=c._real;_imag-=c._imag;return *this;}

Comp &Comp::operator*=(double r)
{_real*=r;_imag*=r;return *this;}

Comp &Comp::operator*=(const Comp &c)
{
    double tp = _real*c._real - _imag*c._imag;
    _imag = _imag*c._real + _real*c._imag;
    _real = tp;
    return *this;
}

Comp &Comp::operator/=(double r)
{_real/=r;_imag/=r;return *this;}

Comp &Comp::operator/=(const Comp &c)
{
    double tp = c._real*c._real + c._imag*c._imag;
    _real = (_real*c._real + _imag*c._imag)/tp;
    _imag = (c._real*_imag-_real*c._imag)/tp;
    return *this;
}

double Comp::abs()const
{return sqrt(_real*_real+_imag*_imag);}

double abs(const Comp &c)
{return c.abs();}


Comp Comp::conjugate()const
{return Comp(_real,- _imag);}

Comp conjugate(const Comp & c)
{return c.conjugate();}

Comp Comp::operator+(const double i)
{return Comp(_real+i, _imag);}

Comp Comp::operator+(const Comp &c)
{return Comp(_real+c._real,_imag+c._imag);}

Comp Comp::operator-(const double i)
{return Comp(_real-i, _imag);}

Comp Comp::operator-(const Comp &c)
{return Comp(_real-c._real,_imag-c._imag);}

Comp Comp::operator*(const double i)
{return Comp(_real*i, _imag*i);}

Comp Comp::operator*(const Comp &c)
{return Comp(_real+c._real-_imag*c._imag,_real*c._imag+_imag*c._real);}

Comp Comp::operator/(const double i)
{return Comp(_real/i, _imag/i);}

Comp Comp::operator/(const Comp &c)
{
    double t = c._real*c._real + c._imag*c._imag;
    return Comp((_real*c._real+_imag*c._imag)/t,(_imag*c._imag-_real*c._imag)/t);
}

std::istream &operator>>(std::istream &i, Comp &c)
{
    double t;i >> t;c.setReal(t);
    i >> t;c.setImag(t);return i;
}

std::ostream &operator<<(std::ostream &o, const Comp &c)
{
    o << "Comp(" << c.getReal() << "," << c.getImag() << ")";
    return o;
}

Comp operator+(double d, const Comp & c)
{return Comp(d+c.getReal(), c.getImag());}

Comp operator+(const Comp & d, const Comp & c)
{return Comp(d.getReal()+c.getReal(), d.getImag()+c.getImag());}

Comp operator-(double d, const Comp & c)
{return Comp(d-c.getReal(), - c.getImag());}

Comp operator-(const Comp & d, const Comp & c)
{return Comp(d.getReal()-c.getReal(), d.getImag()-c.getImag());}

Comp operator*(double d, const Comp & c)
{return Comp(d*c.getReal(), d*c.getImag());}

Comp operator*(const Comp & d, const Comp & c)
{return d*c;}

Comp operator/(double d, const Comp & c)
{
    double tp = c.getReal()*c.getReal()+c.getImag()*c.getImag();
    return Comp(d*c.getReal()/tp, - d*c.getImag()/tp);}

Comp operator/(const Comp & d, const Comp & c)
{return d/c;}

