#ifndef CALC
#define CALC

#include "calc.h"

#define LEFT 1
#define RIGHT 2
#define UNARY 0
#define BINARY 1

// op struct constructor
op::op(char s, uint8_t p, uint8_t a, uint8_t t){
        symbol = s;
        precedence = p;
        assoc = a;
        type = t;
}

// perform stack pop & return operation in one command
std::string pop(std::stack<std::string>* st){
    std::string s = (*st).top();
    (*st).pop();
    return s;
}

// perform queue pop & return operation in one command
std::string pop(std::queue<std::string>* q){
    std::string s = (*q).front();
    (*q).pop();
    return s;
}

// returns true if string s is contained in unordered set us
bool contains(std::unordered_set<std::string>* us, std::string s){
    return (*us).find(s) != (*us).end();
}

// returns true if char c is contained in unordered map um
bool contains(std::unordered_map<char, op>* um, char c){
    return (*um).find(c) != (*um).end();
}

// returns true if string s is contained in unordered map um
bool contains(std::unordered_map<char, op>* um, std::string s){
    return s.length() == 1 && contains(um, s[0]); 
}

int prec(std::string* s){
    return opmap[(*s)[0]].precedence; // 0 if unmapped
}

// bool isNumber(std::string* s){
//     for(int i=0; i<(*s).length(); i++){
//         if(!isdigit((*s)[i]) || (*s)[i] != '.'){
//             return 0;
//         }
//     }
//     return 1;
// }

bool isNumber(std::string* s){ 
    // try to convert string to double, if it fails, string must not be number
    try{stod(*s);} 
    catch (...) {return 0;}
    return 1;
}

int factorial(int f){
    if(f <= 1){ return 1; }
    return f * factorial(f-1);
}

// type-safe sigmoid function
template <typename T> int sig(T val){
    return (T(0) < val) - (T(0) > val);
}

// parse expression into a vector of numbers, functions, operations
std::vector<std::string> parser(std::string input){
    std::vector<std::string> output;

    for(int i=0; i<input.length(); i++){

        if(input[i] == '-' || input[i] == '+'){
            // at beginning of input || in front of operator || in front of '('
            if(i==0 || contains(&opmap, input[i-1]) || input[i-1] == '('){
                output.push_back(std::string(1, input[i]) + "1"); // "-1" or "+1"
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

// return reverse polish notation (RPN- infix notation) of parsed expression
std::queue<std::string>* shunting_yard(std::vector<std::string> input){
    std::queue<std::string> output;
    std::stack<std::string> op_stack;

    for(int i=0; i<input.size(); i++){
        if(isNumber(&input[i])){ // if number, put on output queue
            output.push(input[i]);
        }

        else if(contains(&opmap, input[i])){ // if string is an operator
            // while the top elem isnt '(' and its precedence is greater than the input operator
            // or equal but left associative,
            // pop the operator stack and store it on the output queue
            while((!op_stack.empty() && op_stack.top() != "(") && 
                (prec(&op_stack.top()) > prec(&input[i]) ||
                (prec(&op_stack.top()) == prec(&input[i]) && opmap[input[i][0]].assoc == LEFT))){
                output.push(op_stack.top());
                op_stack.pop();
            }

            op_stack.push(input[i]);
        }

        // if input is a function or "(", push it on operator stack
        else if(contains(&func_one, input[i]) || contains(&func_two, input[i]) || input[i] == "("){
            op_stack.push(input[i]);
        }

        else if(input[i] == ")"){ // input is a ")"
            // pop operator stack until it's empty or we reach a "("
            while(!op_stack.empty() && op_stack.top() != "("){ 
                output.push(pop(&op_stack));
            }

            if(!op_stack.empty()){ // if stack still not empty, push the "(" onto output
                output.push(pop(&op_stack));
            }
            
            else{ return NULL;} // if stack is empty, there are mismatched parentheses 
            
        }

        while(!op_stack.empty()){ // empty op_stack onto queue once input has been processed
            output.push(pop(&op_stack));
        }
    }
    return &output;
}

// perform a binary operation given an operator/function and two numbers
std::string binary_op(std::string b1, std::string b2, std::string oper){
    double x = std::stod(b2); // lower item is first item
    double y = std::stod(b1);
    double ans;

    if(oper.length() == 1){
        char op = oper[0];  

        switch(op){
            case '^': ans = std::pow(x,y);
            case '*': ans = x*y;
            case '/': ans = x/y;
            case '+': ans = x+y;
            case '-': ans = x-y;
            case '%': ans = 1.0 * (((int) x) % ((int) y));

            default: ans = 6.93;
        }
    }

    else{
        if(oper == "max"){ ans = std::fmax(x,y); }
        else if(oper == "min"){ ans = std::fmin(x,y); }
        else if(oper == "mod"){ ans = 1.0 * (((int) x) % ((int) y)); }
        else if(oper == "randr"){ ans = std::rand() * (y - x) + y; }
        else if(oper == "randir"){ ans = (int) (std::rand() * (y - x) + y); }
    }

    return std::to_string(ans);
}  

std::string unary_op(std::string u1, std::string oper){
    int x = std::stoi(u1);   
    int ans;

    if(oper.length() == 1){
        char op = oper[0];
        switch(op){
            case '!': ans = factorial(x);

            default: ans = 0;
        }
    }

    else{
        if(oper == "sin"){ ans = std::sin(x); }
        else if(oper == "cos"){ ans = std::cos(x); }
        else if(oper == "tan"){ ans = std::tan(x); }
        else if(oper == "asin"){ ans = std::asin(x); }
        else if(oper == "acos"){ ans = std::acos(x); }
        else if(oper == "atan"){ ans = std::atan(x); }
        else if(oper == "cosh"){ ans = std::cosh(x); }
        else if(oper == "sinh"){ ans = std::sinh(x); }
        else if(oper == "tanh"){ ans = std::tanh(x); }
        else if(oper == "sqrt"){ ans = std::sqrt(x); }
        else if(oper == "cbrt"){ ans = std::cbrt(x); }
        else if(oper == "exp"){ ans = std::exp(x); }
        else if(oper == "abs"){ ans = std::abs(x); }
        else if(oper == "ceil"){ ans = std::ceil(x); }
        else if(oper == "floor"){ ans = std::floor(x); }
        else if(oper == "round"){ ans = std::round(x); }
        else if(oper == "ln"){ ans = std::log(x); }
        else if(oper == "log"){ ans = std::log10(x); }
        else if(oper == "deg"){ ans = x*180/M_PI; }
        else if(oper == "rad"){ ans = x*M_PI/180; }
        else if(oper == "sign"){ ans = sig(x); }
    }

    return std::to_string(ans);
}

std::string compute(std::queue<std::string>* input){
    std::stack<std::string> c_stack;
    std::queue<std::string> RPN = (*input);

    while(!RPN.empty()){
        std::string q_front = RPN.front();

        if(isNumber(&q_front)){
            c_stack.push(pop(input)); // both refer to same queue
        }

        // if the next input is a binary operator or binary function
        else if((contains(&opmap,q_front) && opmap[q_front[0]].type == BINARY) ||
                contains(&func_two,q_front)){
            c_stack.push(binary_op(pop(&c_stack), pop(&c_stack), pop(input)));
        }

        // if the next input is a unary operator or unary function
        else if((contains(&opmap,q_front) && opmap[q_front[0]].type == UNARY) ||
                contains(&func_one,q_front)){
            c_stack.push(unary_op(pop(&c_stack), pop(input)));
        }
    }

    return pop(&c_stack);
}


int main(){
    // define operator keys in opmap
    // opmap['+'] = op('+', 2, LEFT, BINARY);
    // opmap['-'] = op('-', 2, LEFT, BINARY);
    // opmap['*'] = op('*', 3, LEFT, BINARY);
    // opmap['/'] = op('/', 3, LEFT, BINARY);
    // opmap['%'] = op('%', 3, LEFT, BINARY);
    // opmap['^'] = op('^', 4, RIGHT, BINARY);
    // opmap['!'] = op('!', 5, LEFT, UNARY);

    opmap.emplace('+', op('+', 2, LEFT, BINARY));
    opmap.emplace('-', op('-', 2, LEFT, BINARY));
    opmap.emplace('*', op('*', 3, LEFT, BINARY));
    opmap.emplace('/', op('/', 3, LEFT, BINARY));
    opmap.emplace('%', op('%', 3, LEFT, BINARY));
    opmap.emplace('^', op('^', 4, RIGHT, BINARY));
    opmap.emplace('!', op('!', 5, LEFT, UNARY));


    constants.insert ({"e", "pi", "i"});

    func_one.insert ({"sin", "cos", "tan", 
                        "asin", "acos", "atan", 
                        "cosh", "sinh", "tanh", 
                        "sqrt", "cbrt", "exp", 
                        "abs", "ceil", "floor", 
                        "round", "ln", "log", 
                        "deg", "rad", "sign"});
                                                
    func_two.insert ({"max", "min", "mod", 
                        "randr", "randir"});

    std::string expr;
    std::cin >> expr;

    std::queue<std::string>* RPN = shunting_yard(parser(expr));
    if(!RPN){ return 1; }

    while(!(*RPN).empty()){
        std::cout << (*RPN).front();
        (*RPN).pop();
    }

    std::string ans = compute(RPN);
    std::cout << std::stod(ans);
}

#endif