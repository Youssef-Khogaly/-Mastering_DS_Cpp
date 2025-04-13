#include <bits/stdc++.h>


using namespace std;


class Stack
{
    public:
    Stack(): arr(nullptr),top(-1) , size(0)
    {

    }
    Stack(size_t Size) : top(-1) , top2(Size) , size(Size) , arr(new int [size])
    {
        assert(arr != nullptr);
    }

    ~Stack()
    {
        if(arr)
        {
            delete arr;
            top = -1;
            size = 0;
            arr = nullptr;
        }
    }

    void Push(int id ,int val)
    {
        assert(!IsFull(id));
        if(id == 1)
            arr[++top] = val;
        else
            arr[--top2] = val;
    }
    int pop(int id)
    {
        assert(!IsEmpty(id));   
        if(id == 1)
            return arr[top--];
        
        return arr[top2++];
    }
    int peak(int id)
    {
        assert(!IsEmpty(id));   
        if(id == 1)
            return arr[top];

        return arr[top2];
    }
    bool IsEmpty(int id)
    {
        assert(id <=2 && id > 0);
        if(id == 1)
        {
            return (top == -1);
        }
        return (top2 == size);
    }
    bool IsFull(int id)
    {
        assert(id <=2 && id > 0);
        if(id == 1)
        {
            return (top+1 == top2);
        }

        return (top2-1 == top);
    }
    void Display()
    {
        for(int i = top ; i >= 0 ; i--)
            cout << arr[i] << " ";
        cout <<endl;

        for(int i = top2 ; i <size ; i++)
            cout << arr[i] << " ";
        cout <<endl;
    }
    void InsertAtBottom(int val) // o(N) time , O(N) memory
    {
        assert(!IsFull(1));
        if(IsEmpty(1))
            return Push(1,val);
        
        // assume stack is [4,3,2,1] and we want to insert 10 at the begin
        int curr = pop(1); // pop 1 now stack is 4 3 2

        // now insert at the bottom 10 [10,4,3,2]
        InsertAtBottom(val);
        // now add the 1 again [10,4,3,2,1]
        Push(1,curr);
    }
    void reverseStack_1() // o(n^2) , o(n)
    {
        if(IsEmpty(1))
            return;

         // assume stack is [4,3,2,1] now its [4,3,2]
        int curr = pop(1);
        // reverse it now [2,3,4]
        reverseStack_1();

        // now its [1,2,3,4]
        InsertAtBottom(curr);
    }
    void reverseStack_2() // o(n) , memo(1)
    {
        if(IsEmpty(1) || top == 0)
            return;
        int i = 0;
        int j = top;
        for(; i < j ; ++i , --j)
            swap(arr[i],arr[j]);

    }
    private:
    int top;
    int top2;
    size_t size;
    int* arr;

};
void reverseSubWord(string & line)
{
    stack<char> s;
    auto SubWordStart = line.end();

    for(auto c = line.begin() ; c != line.end() ; c++)
    {
        if(*c != ' ')
        {
            s.push(*c);
            if(SubWordStart == line.end())
                SubWordStart = c;
        } 
        else
        {
            while(s.empty() == false)
            {
                *SubWordStart = s.top();
                s.pop();
                SubWordStart++;
            }
            SubWordStart =  line.end();
        }

    }
}


int reverseNum(int num)
{
    stack<int> s;
    while(num)
    {
        s.push(num % 10);
        num /= 10;
    }
    int tens = 1;
    while(!s.empty())
    {
        num = s.top() * tens + num;
        s.pop();
        tens *= 10;
    }
    return num;
}

bool IsValidParentheses(const string & str )
{
    stack<char> s;

    for(const auto & c : str)
    {

        if(c == '(' || c == '{' || c =='[')
            s.push(c);
        else if ((c == '}' || c == ']') )
        {
            if(( s.top() == (c - 2) ))
                 s.pop();
            else
                return false;
        }
        else if ( c == ')')
        {
            if(s.top() == '(')
                s.pop();
            else 
                return false;
        }
    }

    return s.empty();
}

string RemoveAdacentDuplicates(const string & str)
{
    string ret;
    ret.reserve(str.size());
    auto top = ret.rend();
    for(auto& c : str)
    {
        if(ret.empty() || c != *top)
        {
            ret.push_back(c);
            top = ret.rbegin();
        }
            
        else
        {
            ret.pop_back();
            top = ret.rbegin();
        }
            
    }
    return ret;
}

ostream& operator <<(ostream & out , const vector<int>& vec)
{
    for(const auto & v : vec)
        out << v <<" ";
    out << endl;

    return out;
}


vector<int> AstroidsCollision(const vector<int>& vec)
{
    vector<int>s;

    for (auto & it : vec)
    {
        if(s.empty() || (it < 0 && s.back() < 0) || (it > 0))
            s.push_back(it);
        
        else if (it + s.back() == 0)
        {
            s.pop_back();
        }
        else
        {
            while(s.back() > 0 && it + s.back() < 0) // while negative win
            {
                s.pop_back();
            }
            if(s.back() < 0 || s.empty())
                s.push_back(it);
        }
    }
    return s;
}

int ScoreOfParentheses(const string & str)
{
    stack<int> s;
    s.push(0);
    int score = 0;
    for(auto& c  : str)
    {
        if(c == '(') // new problem
        {
            // initially sum = 0
            s.push(0);
        }
        else
        {
            int lastScore = s.top();
            s.pop();
            if(lastScore == 0) // () case then score will be 1
                lastScore = 1;
            else // case of (A)
                lastScore *= 2;


            // get the parent problem value and update its sum
            lastScore = lastScore + s.top();
            s.pop();
            s.push(lastScore);
        }
    }
    return s.top();
}



vector<int> NextGreater(const vector<int> & vec) // o(n) time , o(n) memory
{
    stack<int>s;
    vector<int>ret(vec.size(),-1);
    int i = 0;
    for(; i < vec.size() ; ) 
    {
        if(s.empty() || vec[i] <= vec[s.top()])
            s.push(i++);
        else
        {
            ret[s.top()] = vec[i];
            s.pop();
        }
    }
    return ret;
}

bool isDigit(const char& c)
{
    return (c >= '0' && c<= '9');
}

bool isLessOrEqual(const char & curr , const char& another)
{
    if(curr == '-' || curr == '+')
    {
        return true;
    }

    // curr = * || /
    if(another == '*' || another =='/')
        return true;
    
    return false;
}

bool isHigher(const char& curr , const char& another)
{
    if(curr ==  '*' || curr == '/')
    {
        if(another == '*' || another == '/') // equal precedence
            return false;
        
        return true;
    }
    // curr = + | -
    return false;
}
int precedence(const char& c)
{
    switch(c)
    {
        case '^':
            return 3;
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            break;
    }
    return 0;
}
bool isOperand(char c)
{
    switch(c)
    {
        case '^':
        case '*':
        case '/':
        case '+':
        case '-':
        case '(':
        case ')':
            return false;
        default:
            break;
    }
    return true;
}
/*

    Rule #1:
    if current char is digit add it to output string

    Rule #2:
    if  current is operator and empty stack , push it 

    Rule #3:
    if  current is operator and  higher precedence than top of stack
    push it
    
    why? bucsae if curr > top then it must apply if before top of the stack
    then push it to make it the new top

    Rule #4:
    current is operator
    as long as current precedence <= top of the stack
    pop top of the stack and add it to output
    
    why?  bucase if  top of the stack >= current then it must apply before current

    now add current operator in the stack

    Rule #5:
        when finish
        and stack is not empty
        all all operators to output

*/
string InfixToPostFix(string &  input) // o(n^2)time , o(n) memory
{
    vector<char> s;
    string str;
    s.reserve(input.size());
    str.reserve(input.size());
    for(int i = 0; i < input.size() ;++i )
    {
        // rule 1
        if(isDigit(input[i]))
        {
            str.push_back(input[i]);
        }
        else
        {
            // rule  2 3 4 
            while(!s.empty() && (precedence(input[i]) <= precedence(s.back()))  )
            {

                str.push_back(s.back()) ;
                s.pop_back();
                
            }
            s.push_back(input[i]);
        }
    }
    while(!s.empty())
    {
        str.push_back(s.back());
        s.pop_back();
    }
    return str;
}

/*
    to support parentheses

    treate parenthese as independent problem from outside

    # rule 6
    if curr is ( just add it -> new subproblem

    #rule 7
    if curr is )
    just pop every thing from the stack till ( 
    why? bucase that's mean we finish our subproblem just add the aswer


*/
string InfixToPostFix_2(const string &  input) // o(n)time , o(n) memory
{
    vector<char> s;
    string str;
    s.reserve(input.size());
    str.reserve(input.size());
    for(int i = 0; i < input.size() ; ++i)
    {
        // rule 1
        if(isOperand(input[i]))
        {
            str.push_back(input[i]);
        }
        else if (input[i] == '(') // new subproblem
        {
            s.push_back(input[i]);
        }
        else if (input[i] == ')')
        {
            while(!s.empty() && s.back() != '('  )
            {
                str.push_back(s.back()) ;
                s.pop_back();
            }
            // now stack top is ( remove it
            s.pop_back();
        }
        else if (input[i] == '^' && input[i] == s.back()) // bucase 2^3^4 evaluates 2^(3^4) -> right to left not left to right as usual
        {
            s.push_back(input[i]);
        }
        else
        {
            // rule  2 3 4 
            while(!s.empty() && ((precedence(input[i]) <= precedence(s.back())) )  )
            {

                str.push_back(s.back()) ;
                s.pop_back();
                
            }
            s.push_back(input[i]);
        }
    }
    while(!s.empty())
    {
        str.push_back(s.back());
        s.pop_back();
    }
    return str;
}

#define CharToNum(_c)   ((_c)-'0')

double PostFixEvaluation(const string & postfix)
{
    stack<double> s;

    for( auto   c: postfix)
    {
        if(isOperand(c))
        {
            s.push(CharToNum(c));
        }
        else
        {
            double result{0.0};
            double operand2 = s.top();
            s.pop();
            double operand = s.top();
            s.pop();
            switch(c)
            {
                case '+':
                    result = operand + operand2;
                    break;
                case '-':
                    result =operand - operand2;
                    break;
                case '/':
                    if(operand2 == 0)
                        throw std::runtime_error("can't divide on zero");
                    result = operand / operand2;
                    break;
                
                case '*':
                    result = operand * operand2;
                    break;
                case '^':
                    result = pow(operand,operand2);
                    break;
                default:
                    throw std::invalid_argument("invalid operator");
                    break;
            }
            s.push(result);
        }
    }

    return s.top();
}



/*

    the general idea close to infix to postfix
    but condsider first reverse the input
    to make the code very similar to postfix
    with only 1 change LR/RL associativity is  of operator reversed now
    so reverse the conditions of postfix conversion

    overall
    reverse
    modifiy
    reverse
    

    first reverse the input

    #rule 1 if curr is digit print it to output

    rule 2 if curr is operator >= top or stack is empty
        push it

    rule 3 if curr is operator < top 
    pop and print the top till top >= operator

    // to support ^ 
    4 ^3 ^ 2 -> evaluated 4 ^(3^2) not (4^3)^2

    so 
    rule 4
    if curr operator is ^ and stack top is ^
    don't pop from the stack just print it 

    why ? bucase ^ evaluated from right to left not left to right 

    to support ()
    rule 5
    if you find ( just push it to the stack
    and when you find ) pop all from the stack till (

*/
string InfixToPrefix(const string & input)
{
    stack<char> s;
    string out;
    string str(input);
    out.reserve(str.size());
    // reverse input
    for(auto c = str.rbegin() ; c != str.rend() ; c++)
    {
        if(*c == ')')
            *c = '(';
        else if (*c == '(')
            *c = ')';


        if(isOperand(*c))
        {
            out.push_back(*c);
        }
        else if (s.empty() || *c =='(')
            s.push(*c);
        else if (*c == '^' && *c == s.top())
            out.push_back(*c);
        else if (*c == ')')
        {
            while(!s.empty() && s.top() != '(')
            {
                out.push_back(s.top());
                s.pop();
            }
            s.pop(); // pop (   
        }
        else
        {
            while(!s.empty() && precedence(*c) < precedence(s.top()))
            {
                out.push_back(s.top());
                s.pop();
            }
            s.push(*c);
        }
    }
    while(!s.empty())
    {
        out.push_back(s.top());
        s.pop();
    }
    // reverse output
    reverse(begin(out) , end(out));
    return out;
}


string RemoveExpressionBrackets(const string & input)
{
    stack<char>s;
    int i = 0;
    string out;
    out.reserve(input.size());
    for(; i < input.size() ; ++i)
    {
        if(isDigit(input[i]))
            out.push_back(input[i]);
        else if(input[i] == '(')
        {
            s.push(out.back());
        }
        else if (input[i] == ')')
            s.pop();
        else if(!s.empty() && s.top() == '-' && (input[i] == '+' || input[i] == '-'))
        {
            if(input[i] == '+')
                out.push_back('-');
            else
                out.push_back('+');
        }
        else
            out.push_back(input[i]);
    }

    return out;
}
/*
             i 
    2 + (3 * (4 -5 *2 ) ) -> 2 
*/
int main()
{
    assert(InfixToPostFix_2("4^3^2") == "432^^");
    assert(InfixToPostFix_2("5+4^3^2-9") == "5432^^+9-");
    assert(InfixToPostFix_2("a+b*(c^d-e)^(f+G*h)-i") == "abcd^e-fGh*+^*+i-");
    assert(InfixToPostFix_2("1+2^3^4*5-6") == "1234^^5*+6-");
    /************************************************** */
   /* cout << "Test 1 ... \n";
    assert(InfixToPrefix("1+2") == "+12");
    cout << "Test 1 passed \n";
    cout << "Test 2 ... \n";
    assert(InfixToPrefix("9-2+3") == "+-923");
    cout << "Test 2 passed\n";
    cout << "Test 3 ... \n";
    assert(InfixToPrefix("4^3^2") == "^4^32");
    cout << "Test 3 passed\n";
    cout << "Test 4 ... \n";
    assert(InfixToPrefix("1+2+3") == "++123");
    cout << "Test 4 passed \n";
    cout << "Test 5 ... \n";
    cout << InfixToPrefix("(1+2)*3")<<endl;
    assert(InfixToPrefix("(1+2)*3") == "*+123");
    cout << "Test 5 passed\n";
    cout << "Test 6 ... \n";
    assert(InfixToPrefix("2*3+4") == "+*234");
    cout << "Test 6 passed \n";
    cout << "Test 7 ... \n";
    assert(InfixToPrefix("1+3*5-8/2") == "-+1*35/82");
    cout << "Test 6 passed \n"; */
    /*************************** */
    cout << "Test 1 ... \n";
    assert(RemoveExpressionBrackets("1+2-3-4+5-6-7+8") == "1+2-3-4+5-6-7+8");
    cout << "Test 1 passed \n";
    cout << "Test 2 ... \n";
    assert(RemoveExpressionBrackets("9-(2+3)") == "9-2-3");
    cout << "Test 2 passed\n";
    cout << "Test 3 ... \n";
    assert(RemoveExpressionBrackets("9-(2-3)") == "9-2+3");
    cout << "Test 3 passed\n";
    cout << "Test 4 ... \n";
    assert(RemoveExpressionBrackets("1-(2-3-(4+5))-6-(7-8)") == "1-2+3+4+5-6-7+8");
    cout << "Test 4 passed \n";
    cout << "Test 5 ... \n";
    assert(RemoveExpressionBrackets("1-(2-3-(4+5)+6-7)") == "1-2+3+4+5-6+7");
    cout << "Test 5 passed\n";
    cout << "Test 6 ... \n";
    assert(RemoveExpressionBrackets("1-(2-3-(4+5-(6-7)))") == "1-2+3+4+5-6+7");
    cout << "Test 6 passed \n";
    cout << "Test 7 ... \n";
    assert(RemoveExpressionBrackets("9+(2-3)") == "9+2-3");
    cout << "Test 7 passed \n";
    return 0;
}