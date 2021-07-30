#include "complex.h"

complex operator+(const complex& lhs, const complex& rhs){ // implicitly converts double -> complex
    return complex(lhs.re + rhs.re, lhs.im + rhs.im);
}

complex operator-(const complex& lhs, const complex& rhs){ // implicitly converts double -> complex
    return complex(lhs.re - rhs.re, lhs.im - rhs.im);
}

complex operator*(const complex& lhs, const complex& rhs){
    return complex(lhs.re*rhs.re - lhs.im*rhs.im, lhs.re*rhs.im + lhs.im*rhs.re);
}

complex operator/(const complex& lhs, const complex& rhs){ // (a+ib)/(c+id)
    double a = lhs.re;
    double b = lhs.im;
    double c = rhs.re;
    double d = rhs.im;

    return complex((a*c + b*d)/(c*c + d*d), (b*c-a*d)/(c*c + d*d));
}

complex operator^(const complex& lhs, const complex& rhs){
    if(same(lhs.im, 0) && same(lhs.re, M_E)){ return exp(rhs);} // deal with euler

    double new_mag = std::pow(mag(lhs), rhs.re);
    double new_arg = angle(lhs.re, lhs.im) * rhs.re;
    return complex(new_mag * std::cos(new_arg), new_mag * std::sin(new_arg)); 
}

// convert a number to its complex form
complex str_to_cmplx(const std::string& s){
    size_t index = s.find("+");

    if(index != std::string::npos){ // if '+' is contained in s
        return complex(std::stod(s.substr(0,index)), std::stod(s.substr(index+1, s.length()-index-2)));
    }

    if(s[s.length()-1] == 'j'){ // otherwise it's purely imaginary or purely real
        // return complex.im = double representation of s without 'j'
        return complex(0,std::stod(s.substr(0,s.length()-1)));
    }

    return complex(std::stod(s),0); // otherwise it's purely real

}

std::string cmplx_to_str(const complex& c){
    if(same(c.re, 0)){ return std::to_string(c.im) + "j";}
    if(same(c.im, 0)){ return std::to_string(c.re);}
    
    std::string op = (c.im > 0) ? "+" : ""; // change +- to just -
    return std::to_string(c.re) + op + std::to_string(c.im) + "j"; 
}

double mag(const complex& c){
    return std::sqrt(c.re*c.re + c.im*c.im);
}

// determine angle opposite to b and adjacent to a
double angle(double a, double b){
    double old_ang = std::atan(b/a);
    return (a > 0) ? old_ang : old_ang + M_PI;
}

// evaluate exponential according to euler's
complex exp(const complex& c){
    // std::cout << "\n" << c.re << " " << c.im << "j";
    double mult = std::exp(c.re);
    return complex(mult * std::cos(c.im), mult * std::sin(c.im));
}

// check if two double are negligibly different
bool same(double d1, double d2){ 
    return std::fabs(d1-d2) < EPSILON;
}
