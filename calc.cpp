#include "calc.h"

// perform stack pop & return operation in one command
std::string pop(std::stack<std::string>& st){
    std::string s;
    if(!st.empty()){ // if stack is not empty, pop it
        s = st.top();
        st.pop();
    }
    else{
        s = "stack empty lol";
    }
    return s;
}

// perform queue pop & return operation in one command
std::string pop(std::queue<std::string>& q){
    std::string s;
    if(!q.empty()){
        s = q.front();
        q.pop();
    }
    else{
        s = "q empty lol";
    }
    return s;
}

// returns true if char c is contained in unordered map um
bool contains(std::unordered_map<char, op>& um, char c){
    return um.find(c) != um.end();
}

// returns true if string s is contained in unordered map um
bool contains(std::unordered_map<std::string, constant>& um, const std::string& s){
    return um.find(s) != um.end();
}

// returns true if string s is contained in unordered map um
bool contains(std::unordered_map<char, op>& um, const std::string& s){
    return s.length() == 1 && contains(um, s[0]); 
}

// returns true if string s is contained in unordered map um
bool contains(std::unordered_map<std::string, func>& um, const std::string& s){
    return um.find(s) != um.end(); 
}

// check if a given string contains a constant in constmap
bool containsConst(std::string& s){
    for(std::unordered_map<std::string, constant>::iterator iter = constmap.begin();
        iter != constmap.end(); iter++){
        // if key of constmap is contained in the string, return true
        if(s.find(iter->first) != std::string::npos){
            return 1;
        }
    }
    return 0;
}

int prec(std::string& s){
    return opmap[s[0]].precedence; // 0 if unmapped
}

bool containsDigit(std::string& s){
    for(int i=0; i<s.length(); i++){
        if(isdigit(s[i]) && s[i] != '.'){
            return 1;
        }
    }
    return 0;
}

void valid(std::string& s){
    if(containsConst(s) || containsDigit(s)){ return; }
    throw std::invalid_argument("invalid expression");
}

bool isNumber(std::string& s){ 
    // try to convert string to double, if it fails, string must not be number
    try{stod(s);} 
    catch (...) {return 0;}
    return 1;
}

double factorial(int f){
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
            if(i==0 || contains(opmap, input[i-1]) || input[i-1] == '('){
                output.push_back(std::string(1, input[i]) + "1"); // "-1" or "+1"
                output.push_back("*");
            }
            output.push_back(std::string() + input[i]);
        }

        else if(contains(opmap, input[i])){
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
                if(contains(constmap,func)){ // if the string is a special constant
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
std::queue<std::string> shunting_yard(std::vector<std::string> input){
    std::queue<std::string> output;
    std::stack<std::string> op_stack;

    for(int i=0; i<input.size(); i++){
        if(isNumber(input[i]) || contains(constmap, input[i])){ // if number or constant, put on output queue
            output.push(input[i]);
        }

        else if(contains(opmap, input[i])){ // if string is an operator
            // while the top elem isnt '(' and its precedence is greater than the input operator
            // or equal but left associative,
            // pop the operator stack and store it on the output queue
            while((!op_stack.empty() && op_stack.top() != "(") && 
                (prec(op_stack.top()) > prec(input[i]) ||
                (prec(op_stack.top()) == prec(input[i]) && opmap[input[i][0]].assoc == LEFT))){
                output.push(op_stack.top());
                op_stack.pop();
            }

            op_stack.push(input[i]);
        }

        // if input is a function or "(", push it on operator stack
        else if(contains(funcmap, input[i]) || input[i] == "("){
            op_stack.push(input[i]);
        }

        else if(input[i] == ")"){ // input is a ")"
            // pop operator stack until it's empty or we reach a "("
            while(!op_stack.empty() && op_stack.top() != "("){ 
                output.push(pop(op_stack));
            }
            
            assert(op_stack.top() == "("); // else mismatched parentheses
            op_stack.pop(); // discard the left parentheses

            if(!op_stack.empty() && contains(funcmap, op_stack.top())){ // if top op is a function
                output.push(pop(op_stack)); // pop it into output queue
            }
            
        }

        else if(input[i] == ","){ // discard comma, then:
            // pop off op_stack until outer function parentheses is reached
            while(!op_stack.empty() && op_stack.top() != "("){ 
                output.push(pop(op_stack));
            }
        }
    }

    while(!op_stack.empty()){ // empty op_stack onto queue once input has been processed
        output.push(pop(op_stack));
    }
    
    return output;
}

std::string compute(std::queue<std::string>& input){
    std::stack<std::string> c_stack;

    while(!input.empty()){
        std::string q_front = input.front();

        assert(q_front != "("); // make sure every "(" has been matched with ")" and removed in shunting-yard

        if(isNumber(q_front)){
            c_stack.push(pop(input));
        }

        // if the next input is a binary operator
        else if((contains(opmap, q_front)) && opmap[q_front[0]].type == BINARY){
            // compute operation and push on computation stack
            c_stack.push(std::to_string(
                opmap[pop(input)[0]].operate( // find operator in opmap and call its operation
                    std::stod(pop(c_stack)), std::stod(pop(c_stack))))); // pop stack twice for operands
        }

        // if the next input is a unary operator
        else if((contains(opmap, q_front)) && opmap[q_front[0]].type == UNARY){
            // compute operation and push on computation stack
            c_stack.push(std::to_string(
                opmap[pop(input)[0]].operate( // find operator in opmap and call its operation
                    std::stod(pop(c_stack)), 0))); // pop stack once for operand (other operand useless)
        }

        // if the next input is a two-input function
        else if((contains(funcmap, q_front)) && funcmap[q_front].type == TWO_INPUT){
            // compute function and push on computation stack
            c_stack.push(std::to_string(
                funcmap[pop(input)].operate( // find function in funcmap and call its operation
                    std::stod(pop(c_stack)), std::stod(pop(c_stack))))); // pop stack twice for inputs
        }
        
        // if the next input is a one-input function
        else if((contains(funcmap, q_front)) && funcmap[q_front].type == ONE_INPUT){
            // compute function and push on computation stack
            c_stack.push(std::to_string(
                funcmap[pop(input)].operate( // find function in funcmap and call its operation
                    std::stod(pop(c_stack)), 0))); // pop stack once for input (other input useless)
        }
        
        // if the next input is a mathematical constant
        else if(contains(constmap, q_front)){
            c_stack.push(std::to_string(constmap[pop(input)].operate()));
        }
    }

    return pop(c_stack);
}


int main(int argc, char** argv){

    opmap.emplace('+', op1);
    opmap.emplace('-', op2);
    opmap.emplace('*', op3);
    opmap.emplace('/', op4);
    opmap.emplace('%', op5);
    opmap.emplace('^', op6);
    opmap.emplace('!', op7);

    funcmap.emplace("sin", f1); 
    funcmap.emplace("cos", f2); 
    funcmap.emplace("tan", f3);  
    funcmap.emplace("asin", f4); 
    funcmap.emplace("acos", f5); 
    funcmap.emplace("atan", f6);  
    funcmap.emplace("sinh", f7); 
    funcmap.emplace("cosh", f8); 
    funcmap.emplace("tanh", f9);  
    funcmap.emplace("sqrt", f10); 
    funcmap.emplace("cbrt", f11); 
    funcmap.emplace("exp", f12); 
    funcmap.emplace("abs", f13);  
    funcmap.emplace("ceil", f14); 
    funcmap.emplace("floor", f15); 
    funcmap.emplace("round", f16);  
    funcmap.emplace("ln", f17); 
    funcmap.emplace("log", f18); 
    funcmap.emplace("deg", f19); 
    funcmap.emplace("rad", f20); 
    funcmap.emplace("sig", f21); 
    funcmap.emplace("max", f22);  
    funcmap.emplace("min", f23); 
    funcmap.emplace("mod", f24); 
    funcmap.emplace("randr", f25);   
    funcmap.emplace("randir", f26);

    constmap.emplace("e", c1);
    constmap.emplace("pi", c2);
    constmap.emplace("rt", c3);  

    // std::string expr = argv[1];

    std::cout << "input: ";
    std::string expr;
    std::getline(std::cin, expr);

    while(true){
        
        try{valid(expr);}
        catch(...) { std::cout<< "invalid expression"; return 0;}

        std::queue<std::string> RPN = shunting_yard(parser(expr));
        std::queue<std::string> RPN_copy = RPN;

        while(!RPN_copy.empty()){
            std::cout << RPN_copy.front() << " ";
            RPN_copy.pop();
        }
        std::cout << "\noutput: ";

        std::string ans = compute(RPN);
        std::cout << std::stod(ans) << "\n";

        std::cout << "input: ";
        std::getline(std::cin, expr);
    }

}