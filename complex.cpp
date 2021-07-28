#include "complex.h"

complex operator+(complex& lhs, complex& rhs){ // implicitly converts double -> complex
    return complex(lhs.re + rhs.re, lhs.im + rhs.im);
}

complex operator-(complex& lhs, complex& rhs){ // implicitly converts double -> complex
    return complex(lhs.re - rhs.re, lhs.im - rhs.im);
}

complex operator*(complex& lhs, complex& rhs){
    return complex(lhs.re*rhs.re - lhs.im*rhs.im, lhs.re*rhs.im + lhs.im*rhs.re);
}

complex operator/(complex& lhs, complex& rhs){ // (a+ib)/(c+id)
    double a = lhs.re;
    double b = lhs.im;
    double c = rhs.re;
    double d = rhs.im;

    return complex((a*c + b*d)/(c*c + d*d), (b*c-a*d)/(c*c + d*d));
}

// convert a number to its complex form
complex str_to_cmplx(const std::string& s){
    if(s[s.length()-1] == 'j'){
        // return complex.im = double representation of s without 'j'
        return complex(0,std::stod(s.substr(0,s.length()-1)));
    }

    return complex(std::stod(s),0);

}

std::string cmplx_to_str(const complex& c){
    std::string s = std::string();
    return s += std::to_string(c.re) + "+" + std::to_string(c.im) + "j"; 
}

double mag(complex& c){
    return std::sqrt(c.re*c.re + c.im*c.im);
}

