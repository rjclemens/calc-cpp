#ifndef COMPLEX
#define COMPLEX

#include <cmath>
#include <string>
#include <iostream>

#define EPSILON 0.0001

struct complex{
    double re;
    double im;

    complex(double a, double b) : re(a), im(b) { };
    complex(double a) : complex(a, 0) { };

};

complex operator+(const complex& lhs, const complex& rhs);
complex operator-(const complex& lhs, const complex& rhs);
complex operator*(const complex& lhs, const complex& rhs);
complex operator/(const complex& lhs, const complex& rhs);
complex operator^(const complex& lhs, const complex& rhs);


double mag(const complex& c);
double angle(double a, double b);
complex exp(const complex& c);
complex str_to_cmplx(const std::string& s);
std::string cmplx_to_str(const complex& c);

bool same(double d1, double d2);

#endif