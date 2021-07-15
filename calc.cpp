#ifndef CALC
#define CALC

#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h> //unordered set

#define LEFT 0
#define RIGHT 1
#define UNARY 0
#define BINARY 1

struct op{
    char symbol;
    uint8_t precedence;
    uint8_t assoc;
    uint8_t type;

    op(char s, uint8_t p, uint8_t a, uint8_t t){
        symbol = s;
        precedence = p;
        assoc = a;
        type = t;
    }

};

std::stack<op> op_stack;
std::unordered_map<char, op> opmap;

std::unordered_set<std::string> constants ( {"e", "pi", "i"});

std::unordered_set<std::string> func_one ( {"sin", "cos", "tan", 
                                                "asin", "acos", "atan", 
                                                "cosh", "sinh", "tanh", 
                                                "sqrt", "cbrt", "exp", 
                                                "abs", "ceil", "floor", 
                                                "round", "ln", "log", 
                                                "deg", "rad", "sign"} );
                                                
std::unordered_set<std::string> func_two ( {"max", "min", "mod", 
                                                "randr", "randir"} );

// returns true if string s is contained in unordered set us
bool contains(std::unordered_set<std::string>* us, std::string s){
    return (*us).find(s) != (*us).end();
}

// returns true if char c is contained in unordered map um
bool contains(std::unordered_map<char, op>* um, char c){
    return (*um).find(c) != (*um).end();
}

std::vector<std::string> parser(std::string input){
    std::vector<std::string> output;

    for(int i=0; i<input.length(); i++){

        if(input[i] == '-' || input[i] == '+'){
            // at beginning of input || in front of operator || in front of '('
            if(i==0 || contains(&opmap, input[i-1]) || input[i-1] == '('){
                output.push_back(std::string() += input[i] + "1"); // "-1" or "+1"
                output.push_back("*");
            }
            output.push_back(std::string() + input[i]);
        }

        else if(contains(&opmap, input[i])){
            std::string s(1,input[i]);
            output.push_back(s);
        }

        else if(input[i] == '('){
            // #(#) or #(.#) for implicit mult
            if(i != 0 && isdigit(input[i-1])){
                output.push_back("*");
            }
            output.push_back("(");
        }

        else if(input[i] == ')'){
            // (#)# or (#).# for implicit mult
            output.push_back(")");
            if(i != input.length()-1 && (isdigit(input[i+1]) || input[i+1] == '.')){
                output.push_back("*");
            }
        }

        else if(isalpha(input[i])){
            std::string func = std::string(1, input[i++]); // assign, then increment i
            while(i < input.length() && isalpha(input[i])){
                func += input[i];
                if(contains(&constants,func)){ // if the string is a special constant
                    break;
                }
                i++;
            }
            output.push_back(func);
        }

        else if (isdigit(input[i]) || input[i] == '.'){
            std::string number = std::string(1, input[i++]); // assign, then increment i
            while(i < input.length() && (isdigit(input[i]) || input[i] == '.')){
                number += input[i];
                i++;
            }
            output.push_back(number);
        }
    }
    
    return output;
    
}

std::string shunting_yard(std::vector<std::string> input){
    std::queue<std::string> output;

    for(int i=0; i<input.length(); i++){
        if(opmap.find(input[i]) != opmap.end()){

        }
        else if(input){

        }
        else{ //input is a function

        }
    }
}

int main(){
    opmap['+'] = op('+', 2, LEFT, BINARY);
    opmap['-'] = op('-', 2, LEFT, BINARY);
    opmap['*'] = op('*', 3, LEFT, BINARY);
    opmap['/'] = op('/', 3, LEFT, BINARY);
    opmap['%'] = op('%', 3, LEFT, BINARY);
    opmap['^'] = op('^', 4, RIGHT, BINARY);
    opmap['!'] = op('!', 5, LEFT, UNARY);
}

#endif