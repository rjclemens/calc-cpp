#ifndef CALC_H
#define CALC_H

#define _USE_MATH_DEFINES

#define LEFT 0
#define RIGHT 1
#define UNARY 0
#define BINARY 1
#define ONE_INPUT 0
#define TWO_INPUT 1

#include <iostream>
#include <cassert>
#include <variant>
#include <cmath>
#include <math.h>
#include <stack>
#include <queue>
#include <string>
#include <cstdint>
#include <functional>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "complex.h"

struct op{
    char symbol;
    uint8_t precedence;
    uint8_t assoc;
    uint8_t type;

    std::function<complex (complex, complex)> operate;

};

struct func{
    std::string symbol;
    uint8_t type;

    std::function<complex (complex, complex)> operate;

    // std::variant<std::function<double (double, double)>, std::function<complex (double, double)>> operate;
};

struct constant{
    std::string symbol;

    std::function<complex ()> operate;
};

struct command{
    std::string symbol;

    std::function<void ()> operate;
};

std::unordered_map<char, op> opmap;
std::unordered_map<std::string, func> funcmap;
std::unordered_map<std::string, constant> constmap;
std::unordered_map<std::string, std::string> variables;
std::unordered_map<std::string, command> cmdmap;
std::string prev_ans;


bool contains(std::unordered_map<char, op>& um, char c);
bool contains(std::unordered_map<char, op>& um, const std::string& s);
template <typename T> bool contains(std::unordered_map<std::string, T>& um, const std::string& s);


int prec(std::string& s);
bool isCmplx(std::string& s);
double factorial(int f);
bool containsDigit(std::string& s);
bool containsConst(std::string& s);
template <typename T> int sig(T val);
void print_vars();
void help();
std::string pop(std::stack<std::string>& st);
std::string pop(std::queue<std::string>& q);

std::vector<std::string> parser(std::string& input);
std::queue<std::string> shunting_yard(std::vector<std::string>& input);
std::string compute(std::queue<std::string>& input);

op op1{'+', 2, LEFT, BINARY, [] (complex a, complex b){return b + a;}};
op op2{'-', 2, LEFT, BINARY, [] (complex a, complex b){return b - a;}};
op op3{'*', 3, LEFT, BINARY, [] (complex a, complex b){return b * a;}};
op op4{'/', 3, LEFT, BINARY, [] (complex a, complex b){return b / a;}};
op op5{'%', 3, LEFT, UNARY, [] (complex a, complex b){return complex(a.re/100);}};
op op6{'^', 4, RIGHT, BINARY, [] (complex a, complex b){return b ^ a;}};
op op7{'!', 5, LEFT, UNARY, [] (complex a, complex b){return factorial(a.re);}};
op op8{'@', 1, LEFT, BINARY, [] (complex a, complex b){return b;}};

func f1{"sin", ONE_INPUT, [] (complex a, complex b){return std::sin(a.re);}};
func f2{"cos", ONE_INPUT, [] (complex a, complex b){return std::cos(a.re);}};
func f3{"tan", ONE_INPUT, [] (complex a, complex b){return std::tan(a.re);}};
func f4{"asin", ONE_INPUT, [] (complex a, complex b){return std::asin(a.re);}};
func f5{"acos", ONE_INPUT, [] (complex a, complex b){return std::acos(a.re);}};
func f6{"atan", ONE_INPUT, [] (complex a, complex b){return std::atan(a.re);}};
func f7{"cosh", ONE_INPUT, [] (complex a, complex b){return std::sinh(a.re);}};
func f8{"sinh", ONE_INPUT, [] (complex a, complex b){return std::cosh(a.re);}};
func f9{"tanh", ONE_INPUT, [] (complex a, complex b){return std::tanh(a.re);}};
func f10{"sqrt", ONE_INPUT, [] (complex a, complex b){return a ^ .5;}};
func f11{"cbrt", ONE_INPUT, [] (complex a, complex b){return a ^ (1.0/3.0);}};
func f12{"exp", ONE_INPUT, [] (complex a, complex b){return exp(a);}};
func f13{"abs", ONE_INPUT, [] (complex a, complex b){return std::fabs(a.re);}};
func f14{"ceil", ONE_INPUT, [] (complex a, complex b){return std::ceil(a.re);}};
func f15{"floor", ONE_INPUT, [] (complex a, complex b){return std::floor(a.re);}};
func f16{"round", ONE_INPUT, [] (complex a, complex b){return std::round(a.re);}};
func f17{"ln", ONE_INPUT, [] (complex a, complex b){return std::log(a.re);}};
func f18{"log", ONE_INPUT, [] (complex a, complex b){return std::log10(a.re);}};
func f19{"deg", ONE_INPUT, [] (complex a, complex b){return a.re*180/M_PI;}};
func f20{"rad", ONE_INPUT, [] (complex a, complex b){return a.re*M_PI/180;}};
func f21{"sig", ONE_INPUT, [] (complex a, complex b){return sig(a.re);}};

func f22{"max", TWO_INPUT, [] (complex a, complex b) {return std::fmax(mag(a),mag(b));}};
func f23{"min", TWO_INPUT, [] (complex a, complex b) {return std::fmin(mag(a),mag(b));}};
func f24{"mod", TWO_INPUT, [] (complex a, complex b) {return 1.0 * (((int) a.re) % ((int) b.re));}};
func f25{"randr", TWO_INPUT, [] (complex a, complex b) {return std::rand() * (b.re - a.re) + b.re;}};
func f26{"randir", TWO_INPUT, [] (complex a, complex b) {return (int) (std::rand() * (b.re - a.re) + b.re);}};

constant c1{"e", [] () {return M_E;}};
constant c2{"pi", [] () {return M_PI;}};
constant c3{"rt", [] () {return M_SQRT2;}};

command cm1{"vars", [] () {return print_vars();}};
command cm2{"clear", [] () {return variables.clear();}};
command cm3{"help", [] () {return help();}};

#endif