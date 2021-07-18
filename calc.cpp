#ifndef CALC
#define CALC

#include "calc.h"

#define LEFT 1
#define RIGHT 2
#define UNARY 0
#define BINARY 1


// op struct constructor
// op::op(char s, uint8_t p, uint8_t a, uint8_t t){
//         symbol = s;
//         precedence = p;
//         assoc = a;
//         type = t;
// }

// perform stack pop & return operation in one command
std::string pop(std::stack<std::string>* st){
    std::string s;
    if(!(*st).empty()){ // if stack is not empty, pop it
        s = (*st).top();
        (*st).pop();
    }
    else{
        s = "stack empty lol";
    }
    return s;
}

// perform queue pop & return operation in one command
std::string pop(std::queue<std::string>* q){
    std::string s;
    if(!(*q).empty()){
        s = (*q).front();
        (*q).pop();
    }
    else{
        s = "q empty lol";
    }
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

bool isNumber(std::string* s){
    for(int i=0; i<(*s).length(); i++){
        if(!isdigit((*s)[i]) && (*s)[i] != '.'){
            return 0;
        }
    }
    return 1;
}

void containsDigit(std::string& s){
    for(int i=0; i<s.length(); i++){
        if(isdigit(s[i]) && s[i] != '.'){
            return;
        }
    }
    throw std::invalid_argument("no digit");
}

bool isFunction(std::string& s){
    return contains(&func_one, s) || contains(&func_two, s);
}

// bool isNumber(std::string* s){ 
//     // try to convert string to double, if it fails, string must not be number
//     try{stod(*s);} 
//     catch (...) {return 0;}
//     return 1;
// }

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
            std::string func = std::string(1, input[i]); // build string from first letter
            while(i+1 < input.length() && isalpha(input[i+1])){ // check next char
                func += input[i+1];
                if(contains(&constants,func)){ // if the string is a special constant
                    break;
                }
                i++;
            }
            output.push_back(func);
        }

        else if (isdigit(input[i]) || input[i] == '.'){
            std::string number = std::string(1, input[i]); // build string from first digit
            // check next char
            while(i+1 < input.length() && (isdigit(input[i+1]) || input[i+1] == '.')){ 
                number += input[i+1];
                i++;
            }
            output.push_back(number);
        }

        else if(input[i] == ',') // dont discard commas for multi-input functions
            output.push_back(",");
    }
    
    return output;
    
}

// return reverse polish notation (RPN- infix notation) of parsed expression
std::queue<std::string>* shunting_yard(std::vector<std::string> input){
    static std::queue<std::string> output;
    std::stack<std::string> op_stack;

    for(int i=0; i<input.size(); i++){
        std::string debugg = input[i];
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
            
            assert(op_stack.top() == "("); // else mismatched parentheses
            op_stack.pop(); // discard the left parentheses

            if(!op_stack.empty() && isFunction(op_stack.top())){ // if top op is a function
                output.push(pop(&op_stack)); // pop it into output queue
            }
            
        }

        else if(input[i] == ","){ // discard comma, then:
            // pop off op_stack until outer function parentheses is reached
            while(!op_stack.empty() && op_stack.top() != "("){ 
                output.push(pop(&op_stack));
            }
        }
    }

    while(!op_stack.empty()){ // empty op_stack onto queue once input has been processed
        output.push(pop(&op_stack));
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
            case '^': ans = std::pow(x,y); break;
            case '*': ans = x*y; break;
            case '/': ans = x/y; break;
            case '+': ans = x+y; break;
            case '-': ans = x-y; break;
            case '%': ans = 1.0 * (((int) x) % ((int) y)); break;

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

std::string compute(std::queue<std::string>& input){
    std::stack<std::string> c_stack;

    while(!input.empty()){
        std::string q_front = input.front();

        if(isNumber(&q_front)){
            c_stack.push(pop(&input));
        }

        // if the next input is a binary operator or binary function
        else if((contains(&opmap,q_front) && opmap[q_front[0]].type == BINARY) ||
                contains(&func_two,q_front)){
            c_stack.push(binary_op(pop(&c_stack), pop(&c_stack), pop(&input)));
        }

        // if the next input is a unary operator or unary function
        else if((contains(&opmap,q_front) && opmap[q_front[0]].type == UNARY) ||
                contains(&func_one,q_front)){
            c_stack.push(unary_op(pop(&c_stack), pop(&input)));
        }
    }

    return pop(&c_stack);
}


int main(int argc, char** argv){
    // define operator keys in opmap
    op op1{'+', 2, LEFT, BINARY};
    op op2{'-', 2, LEFT, BINARY};
    op op3{'*', 3, LEFT, BINARY};
    op op4{'/', 3, LEFT, BINARY};
    op op5{'%', 3, LEFT, BINARY};
    op op6{'^', 4, RIGHT, BINARY};
    op op7{'!', 5, LEFT, UNARY};

    opmap.emplace('+', op1);
    opmap.emplace('-', op2);
    opmap.emplace('*', op3);
    opmap.emplace('/', op4);
    opmap.emplace('%', op5);
    opmap.emplace('^', op6);
    opmap.emplace('!', op7);    

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

    std::string expr = argv[1];

    //std::string expr = "max(10*2,5)";

    while(true){
        // std::string expr = "max(5+5*2*2, 23)";
        //std::cin >> expr;

        // std::string expr = "max(10*2,5)";

        try{containsDigit(expr);}
        catch(...) { return 0;}

        // std::vector<std::string> xxx = parser(expr);
        // for(int i=0; i<xxx.size(); i++){
        //     std::cout << "\n" << xxx[i] << "ee";
        // }

        std::queue<std::string>* RPN = shunting_yard(parser(expr));
        if(!RPN){ return 1; }

        std::queue<std::string> RPN_copy = *RPN; 

        while(!RPN_copy.empty()){
            std::cout << " " << RPN_copy.front();
            RPN_copy.pop();
        }
        std::cout << "\n output: ";

        std::string ans = compute(*RPN);
        std::cout << std::stod(ans) << "\n ";

        std::getline(std::cin, expr);
    }

}

#endif