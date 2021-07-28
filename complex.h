#ifndef COMPLEX
#define COMPLEX

#include <cmath>
#include <string>

struct complex{
    double re;
    double im;

    complex(double a, double b) : re(a), im(b) { };
    complex(double a) : complex(a, 0) { };

};

complex operator+(complex& lhs, complex& rhs);
complex operator-(complex& lhs, complex& rhs);
complex operator*(complex& lhs, complex& rhs);
complex operator/(complex& lhs, complex& rhs);


double mag(complex& c);
complex str_to_cmplx(const std::string& s);
std::string cmplx_to_str(const complex& c);

#endif