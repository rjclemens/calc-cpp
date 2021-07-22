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

struct op{
    char symbol;
    uint8_t precedence;
    uint8_t assoc;
    uint8_t type;

    std::function<double (double, double)> operate;

};

struct func{
    std::string symbol;
    uint8_t type;

    std::function<double (double, double)> operate;
};

struct constant{
    std::string symbol;

    std::function<double ()> operate;
};

std::unordered_map<char, op> opmap;
std::unordered_map<std::string, func> funcmap;
std::unordered_map<std::string, constant> constmap;


bool contains(std::unordered_map<std::string, constant>& um, const std::string& s);
bool contains(std::unordered_map<char, op>& um, char c);
bool contains(std::unordered_map<char, op>& um, const std::string& s);
bool contains(std::unordered_map<std::string, func>& um, const std::string& s);

int prec(std::string& s);
bool isNumber(std::string& s);
double factorial(int f);
bool containsDigit(std::string& s);
void valid(std::string& s);
template <typename T> int sig(T val);
std::string pop(std::stack<std::string>& st);
std::string pop(std::queue<std::string>& q);

std::vector<std::string> parser(std::string input);
std::queue<std::string> shunting_yard(std::vector<std::string> input);
std::string compute(std::queue<std::string>& input);

op op1{'+', 2, LEFT, BINARY, [] (double a, double b){return a + b;}};
op op2{'-', 2, LEFT, BINARY, [] (double a, double b){return a - b;}};
op op3{'*', 3, LEFT, BINARY, [] (double a, double b){return a * b;}};
op op4{'/', 3, LEFT, BINARY, [] (double a, double b){return a / b;}};
op op5{'%', 3, LEFT, UNARY, [] (double a, double b){return a / 100;}};
op op6{'^', 4, RIGHT, BINARY, [] (double a, double b){return std::pow(b, a);}};
op op7{'!', 5, LEFT, UNARY, [] (double a, double b){return factorial(a);}};

func f1{"sin", ONE_INPUT, [] (double a, double b){return std::sin(a);}};
func f2{"cos", ONE_INPUT, [] (double a, double b){return std::cos(a);}};
func f3{"tan", ONE_INPUT, [] (double a, double b){return std::tan(a);}};
func f4{"asin", ONE_INPUT, [] (double a, double b){return std::asin(a);}};
func f5{"acos", ONE_INPUT, [] (double a, double b){return std::acos(a);}};
func f6{"atan", ONE_INPUT, [] (double a, double b){return std::atan(a);}};
func f7{"cosh", ONE_INPUT, [] (double a, double b){return std::sinh(a);}};
func f8{"sinh", ONE_INPUT, [] (double a, double b){return std::cosh(a);}};
func f9{"tanh", ONE_INPUT, [] (double a, double b){return std::tanh(a);}};
func f10{"sqrt", ONE_INPUT, [] (double a, double b){return std::sqrt(a);}};
func f11{"cbrt", ONE_INPUT, [] (double a, double b){return std::cbrt(a);}};
func f12{"exp", ONE_INPUT, [] (double a, double b){return std::exp(a);}};
func f13{"abs", ONE_INPUT, [] (double a, double b){return std::fabs(a);}};
func f14{"ceil", ONE_INPUT, [] (double a, double b){return std::ceil(a);}};
func f15{"floor", ONE_INPUT, [] (double a, double b){return std::floor(a);}};
func f16{"round", ONE_INPUT, [] (double a, double b){return std::round(a);}};
func f17{"ln", ONE_INPUT, [] (double a, double b){return std::log(a);}};
func f18{"log", ONE_INPUT, [] (double a, double b){return std::log10(a);}};
func f19{"deg", ONE_INPUT, [] (double a, double b){return a*180/M_PI;}};
func f20{"rad", ONE_INPUT, [] (double a, double b){return a*M_PI/180;}};
func f21{"sig", ONE_INPUT, [] (double a, double b){return sig(a);}};

func f22{"max", TWO_INPUT, [] (double a, double b) {return std::fmax(a,b);}};
func f23{"min", TWO_INPUT, [] (double a, double b) {return std::fmin(a,b);}};
func f24{"mod", TWO_INPUT, [] (double a, double b) {return 1.0 * (((int) a) % ((int) b));}};
func f25{"randr", TWO_INPUT, [] (double a, double b) {return std::rand() * (b - a) + b;}};
func f26{"randir", TWO_INPUT, [] (double a, double b) {return (int) (std::rand() * (b - a) + b);}};

constant c1{"e", [] () {return M_E;}};
constant c2{"pi", [] () {return M_PI;}};
constant c3{"rt", [] () {return M_SQRT2;}};

#endif