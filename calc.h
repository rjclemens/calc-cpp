#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <cmath>
#include <math.h>
#include <stack>
#include <queue>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct op{
    char symbol;
    uint8_t precedence;
    uint8_t assoc;
    uint8_t type;

    op(char s, uint8_t p, uint8_t a, uint8_t t);

};

std::unordered_map<char, op> opmap;
std::unordered_set<std::string> constants;
std::unordered_set<std::string> func_one;                                       
std::unordered_set<std::string> func_two;


bool contains(std::unordered_set<std::string>* us, std::string s);
bool contains(std::unordered_map<char, op>* um, char c);
bool contains(std::unordered_map<char, op>* um, std::string s);
int prec(std::string* s);
bool isNumber(std::string* s);
int factorial(int f);
template <typename T> int sig(T val);
std::string pop(std::stack<std::string>* st);
std::string pop(std::queue<std::string>* q);

std::string binary_op(std::string b1, std::string b2, std::string oper);
std::string unary_op(std::string u1, std::string oper);

std::vector<std::string> parser(std::string input);
std::queue<std::string>* shunting_yard(std::vector<std::string> input);
std::string compute(std::queue<std::string>* input);


#endif