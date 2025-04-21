#include <bits/stdc++.h>




using namespace std;

template<class T>
struct node{
    T data;
    node* left;
    node* right;

    explicit node(T data , node<T>* left = nullptr , node<T>* right = nullptr): data(data),left(left),right(right)
    {

    }
};

class tree
{
    public:
    using node = node<int>;

    tree(): root(nullptr){}
    tree(deque<int>& preorder , deque<int>& inorder)
    {
        root = treeGeneration(preorder , inorder , 0 , inorder.size());
    }
    /*
        generate a full binary tree
    */
    tree(queue<pair<int,int>>& preOrder) // pair (value,leafOrNot)
    {
        root = GenerateFullBinaryTree(preOrder);
    }
    ~tree()
    {
        clear();
    }
    void clear()
    {
        if(!root)
            return;
        node* temp = root;
        root = temp->left;
        clear(); // clear left nodes
        root = temp->right; 
        clear(); // clear right nodes
        root = temp;
        delete root;
        root = nullptr;
       
        return;
    }
    void insert(int data)
    {
        if(!root)
        {
            root = new node(data);
            return;
        }
        else if (!root->left && data <= root->data)
        {
            root->left = new node(data);
            return;
        }
        else if (!root->right && data > root->data)
        {
            root->right = new node(data);
            return;
        }
        else if (data <= root->data)
        {
            node* prev = root;
            root = root->left;
            insert(data);
            root = prev;
            return;
        }
        else
        {
            node* prev = root;
            root = root->right;
            insert(data);
            root = prev;
            return;
        }
    }

    void add(const vector<int>& vec ,  const vector<char>& path)
    {
        assert(vec.size() == path.size());
        node* curr = root;
        int currVal = 0;
        for(auto & c : path)
        {
            if(c == 'L')
            {
                if(!(curr->left))
                {
                    curr->left = new node(vec[currVal]);
                }
                curr = curr->left;
            }
            else if (c == 'R')
            {
                if(!(curr->right))
                    curr->right = new node(vec[currVal]);
                
                curr = curr->right;
            }
            assert(curr->data == vec[currVal]);
            ++currVal;
        }
    }
    // longest path between 2 nodes
    /*
        the height of left + height of right + 1
    */
    pair<int,int> tree_diameter() // return diam , height
    {
        pair <int , int > res {0,0};
        if(!root || (!root->left && !root->right))
            return res;
        
        pair<int,int>leftDiam{0,0};
        pair<int,int>rightDiam{0,0};

        node* temp = root;
        if(temp->left)
        {
            root = temp->left;
            leftDiam = tree_diameter(); // return left node diameter , height
            // now my diameter should be my my left height + 1  + my right height + 1
            res.first = leftDiam.second + 1;
        }
        if(temp->right)
        {
            root = temp->right;
            rightDiam = tree_diameter(); // return left node diameter , height
            // now my diameter should be my my left diameter + 1  + my right diameter + 1
            res.first += rightDiam.second + 1;
        }
        root = temp;
        /*
            max diameter pass throught one of my children
        */
        res.first = max(res.first , max(leftDiam.first , rightDiam.first));

        // my height is the max height + 1
        res.second = 1 + max(leftDiam.second , rightDiam.second);

        return res;
    }
    void print()
    {
        print_inorder(root);
    }
    void print_inorder_iterative()
    {
        if(!root)
            return;
        stack<node*>s{};
        stack<int>res{};
        node* curr = root;
        while(curr ||    false == s.empty())
        {
            while(curr != nullptr )
            {
                s.push(curr);
                curr = curr->right;
            }
            curr = s.top();
            s.pop();
            res.push(curr->data);
            curr = curr->left;
        }
        while(!res.empty())
            cout << res.top() <<" " , res.pop();
        return;
    }
    void print_inorder_iterative_2()
    {
        return;
    }
    void printLeftBoundary()
    {
        if(!root)
            return;
        cout << root->data <<" ";
        node* temp = root;
        if(root->left)
        {
            root = root->left;
            printLeftBoundary();
        }
        else if (root->right)
        {
            root = root->right;
            printLeftBoundary();
        }
        root = temp;
        return;
    }
    int tree_max()
    {
        if(!root)
            return INT_MIN;
        
        node* prev = root;
        root = root->left;
        int leftMax = tree_max();
        root = prev->right;
        int rightMax = tree_max();
        root = prev;
        
        return std::max(std::max(leftMax,rightMax),root->data);
    }
    int tree_height()
    {

        return tree_height(root) ;
    }
    int countNodes()
    {
        if(!root)
            return 0;
        
        node* temp = root;
        root = root->left;
        int leftCount = countNodes();
        root = temp->right;
        int rightCount = countNodes();
        root = temp;

        return (leftCount + rightCount + 1); // +1 for current node
    }
    int countLeafs()
    {
        return leafs(root);
    }
    bool isExists(int val)
    {
        if(!root)
            return false;

        if(root->data == val)
            return true;
        node* temp = root;
        root = root->left;
        bool left = isExists(val);
        root = temp;
        if(left)
            return true;
        root = root->right;
        bool right = isExists(val);
        root = temp;
            if(right)
                return true;
        
        return false;
    }
    bool is_Perfect_v1()
    {
        return is_Perfect_1(root);
    }
    bool is_Perfect_v2()
    {
        int n = countNodes();
        int h = tree_height(root);
        return ((n == (pow(2,h+1)-1)));
    }
    
    void PrintBreadthFirst()
    {
        if(!root)
            return;
        queue<node*>q{};
        q.push(root);
        node* curr = nullptr;
        int level = 0;
        int sz{0};
        while(!q.empty())
        {
            sz = q.size();
            cout <<"l" <<level++ << ":";
            // pop all current level nodes and add next level nodes
            while(sz--)
            {
                curr = q.front();
                q.pop();
                if(curr->left)
                    q.push(curr->left);
                if(curr->right)
                    q.push(curr->right);
                cout << curr->data << " ";
            }
            cout << endl;
        }
    }
    void PrintLevelOrderSpiral()
    {
        if(!root)
            return;
        deque<node*>q{};
        q.push_back(root);
        node* curr = nullptr;
        int level = 0;
        int sz{0};

        while(!q.empty())
        {
            sz = q.size();
            cout <<"l" <<level << ":";
            // pop all current level nodes and add next level nodes
            while(sz--)
            {
                // odd level reserved
                if(level % 2 == 0)
                {
                    curr = q.front();
                    q.pop_front();
                    if(curr->left)
                        q.push_back(curr->left);
                    if(curr->right)
                        q.push_back(curr->right);
                }
                else
                {
                    curr = q.back();
                    q.pop_back();
                    if(curr->right)
                        q.push_front(curr->right);
                    if(curr->left)
                        q.push_front(curr->left);


                }
                cout << curr->data <<" ";
            }
            ++level;
            cout << endl;
        }  
    }
    void PrintBreadthFirst_rec()
    {
        if(!root)
            return;
        queue<node*> q;
        q.push(root);
        PrintBreadthFirst_rec_p(q);

    }
    bool IsComplete()
    {
        queue<node*>q;
        q.push(root);
        int sz = 0;
        node* curr{};
        bool NoMoreNodesAllowed = false; // prev node state
        while(!q.empty())
        {
            sz = q.size();
            // check if all current level nodes filled from left to right
            while(sz--)
            {
                curr = q.front();
                q.pop();
                if(curr->left)
                {
                    if(NoMoreNodesAllowed) // prev node uncomplete we shouldn't haave left here
                        return false;
                    q.push(curr->left);
                }
                else
                    NoMoreNodesAllowed = true;
                
                if(curr->right) 
                {   
                    // prev node uncomplete or we don't have left 
                    if(NoMoreNodesAllowed)
                        return false;
                    q.push(curr->right); 
                }
                else
                    NoMoreNodesAllowed = true;
            }
        }

        return true;
    }
    void GetInorder(deque<int>& q)
    {
        if(!root)
            return ;
        
        node* temp = root;
        root = temp->left;
        GetInorder(q); // get left

        q.push_back(temp->data);
        root = temp->right;
        GetInorder(q); // get right
        root = temp;

        return ;
    }
    void GetPreOrder(deque<int>& q)
    {
        if(!root)
            return;
        
        node* temp = root;
        q.push_back(temp->data);
        root = temp->left;
        GetPreOrder(q); // get left
        root = temp->right;
        GetPreOrder(q); // get right
        root = temp;
        return ;
    }
    void printPreOrder()
    {
        if(!root)
            return;

        node* temp = root;
        cout << temp->data << " ";
        root = temp->left;
        printPreOrder(); // print left
        root = temp->right;
        printPreOrder(); // print right
        root = temp;
    }
   
    /*
        print tree preorder
        if -1 idicate a null node
    */
   void Print_preOrder_complete()
   {
        if(!root)
            return;

        node* temp = root;
        cout << temp->data << " ";
        if(temp->left)
        {
            root = temp->left;
            Print_preOrder_complete();
        }
        else
            cout << "-1 ";

        if(temp->right)
        {
            root = temp->right;
            Print_preOrder_complete();
        }
        else
            cout << "-1 ";

        return;
   }
   /*
    (
        myval
        left sub tree
        right sub tree
    )
        () idicate a null 
   */
   string Parenthesize()
   {
        if(!root)
            return "()";
        string ret = "(" + to_string(root->data);
        node* temp = root;
        if(temp->left)
        {
            root = temp->left;
            ret += Parenthesize(); // my left sub tree represnation
        }
        else 
            ret += "()";
        if(temp->right)
        {
            root = temp->right;
            ret += Parenthesize(); // my right sub tree represnation
        }
        else
            ret += "()";

        root = temp;
        ret += ')';
        return ret;
   }
   string Parenthesize_subTree(node* r , bool flip = false)
   {
        if(!r)
            return "()";
        string ret = "(" + to_string(r->data);
        if(flip == false)
        {
            if(r->left)
            {
                ret += Parenthesize_subTree(r->left , flip); // my left sub tree represnation
            }
            else 
                ret += "()";

            if(r->right)
            {
                ret += Parenthesize_subTree(r->right , flip); // my right sub tree represnation
            }
            else
                ret += "()";    
        }
        else
        {
            if(r->right)
            {
                ret += Parenthesize_subTree(r->right , flip); // my right sub tree represnation
            }
            else
                ret += "()";
            if(r->left)
            {
                ret += Parenthesize_subTree(r->left , flip); // my left sub tree represnation
            }
            else 
                ret += "()";   
        }

        ret += ')';
        return ret;
   }
   string Parenthesize_canonical()
   {
        if(!root)
        return "()";

        string ret = "(" + to_string(root->data);
        vector<string>vec{};
        node* temp = root;
        if(temp->left)
        {
            root = temp->left;
            vec.push_back(Parenthesize()); // my left sub tree represnation
        }
        else 
            ret += "()";
        if(temp->right)
        {
            root = temp->right;
            vec.push_back(Parenthesize()); // my right sub tree represnation
        }
        else
            ret += "()";

        root = temp;
        // sort my sub trees
        sort(vec.begin() , vec.end());
        // add my sub tree represenation to result
        for_each(vec.begin() , vec.end() , [&ret](string& x) mutable {ret += x;});
        ret += ')';
        return ret;
   }
   string Parenthesize_canonical_subTree(node* r)
   {
        if(!r)
        return "()";

        string ret = "(" + to_string(r->data);
        vector<string>vec{};
        if(r->left)
        {
            vec.push_back(Parenthesize_subTree(r->left)); // my left sub tree represnation
        }
        else 
            ret += "()";
        if(r->right)
        {
            vec.push_back(Parenthesize_subTree(r->right)); // my right sub tree represnation
        }
        else
            ret += "()";

        // sort my sub trees
        sort(vec.begin() , vec.end());
        // add my sub tree represenation to result
        for_each(vec.begin() , vec.end() , [&ret](string& x) mutable {ret += x;});
        ret += ')';
        return ret;
   }
   bool IsSymmetric()
   {
        if(!root)
            return false;
        if(!root->left && !root->right)
            return true;
        if(!root->left || !root->right)
            return false;
        string left = Parenthesize_subTree(root->left , false);
        string right = Parenthesize_subTree(root->right , true); // flip right tree
        cout << left << endl;
        cout << right << endl;

        return (left == right);
   }
   bool IsSymmetic_rec()
   {
        if(!root)
            return false;
        if(!root->left && !root->right)
            return true;
        if(!root->left || !root->right)
            return false;

        return IsMirror(root->left , root->right);
   }
   bool IsEquivalentFlip(tree& other)
   {
        auto root2 = other.root;
        if(!root && !root2)
            return false;

        return is_Flip_equ_p(root,root2);
   }
   // solve with conanical fourm
   bool IsEquivalentFlip_2(tree& other)
   {
        string first = Parenthesize_canonical_subTree(root);
        string second =  Parenthesize_canonical_subTree(other.root);

        return (first == second);
   }
   void GetDuplicateSubtree(vector<string>& result )
   {
        unordered_map<string , bool> map{};
        GetDuplicateSubtree(root , result , map);
   }
    private:
    node* root;
    bool is_Perfect_1(node* nd , int h = -1)
    {
        // first call
        if(h == -1)
            h = tree_height();
        if(!nd)
            return true;

        if(nd->left == nullptr && nd->right == nullptr) // leaf node
        {
            return (h == 0);
        }
        // one child
        if(nd->left == nullptr || nd->right == nullptr)
            return false;
        

        return is_Perfect_1(nd->left,h-1) && is_Perfect_1(nd->right,h-1);
    }
    int leafs(node* nd)
    {
        if(!nd)
            return 0;
        if(nd->left == nullptr && nd->right == nullptr)
            return 1;

        int leftLeafs = leafs(nd->left);
        int rightLeafs = leafs(nd->right);
        return (leftLeafs + rightLeafs);    
    }
    void print_inorder(node* nd) // in order
    {
        if(!nd)
            return;
        
        print_inorder(nd->left);
        cout << nd->data <<" ";
        print_inorder(nd->right);
        
    }
    int tree_height(node* nd)
    {
        if(nullptr == nd)
            return -1;

        int leftHeight = tree_height(nd->left) ;
        int rightHeight = tree_height(nd->right);

        return max(leftHeight,rightHeight) + 1;
    }
    void PrintBreadthFirst_rec_p(queue<node*> & q , int level = 0)
    {
        if(q.empty())
            return;
        cout << "l" << level <<":";
        node* curr;
        int sz = q.size();
        while(sz--)
        {
            curr = q.front();
            q.pop();
            cout << curr->data << " ";
            if(curr->left)
            q.push(curr->left);
            if(curr->right)
            q.push(curr->right);
        }
        cout <<"\n";
        PrintBreadthFirst_rec_p(q , ++level);
    }

    node* treeGeneration(deque<int>& preorder , deque<int>& inorder , int inOrderStart, int inOrderEnd)
    {
        if(preorder.empty() || inOrderStart >= inOrderEnd)
            return nullptr;
        
        node* curr = new node(preorder.front());
        preorder.pop_front();
        // we can optimize the search using hashmap later
        auto it = find(inorder.begin()+inOrderStart , inorder.begin()+inOrderEnd , curr->data);
        int root_idx = distance(inorder.begin() ,it);
        /*
            lets build our left tree
        */
        curr->left = treeGeneration(preorder , inorder , inOrderStart , root_idx);
        /*
            build our right tree
        */
        curr->right = treeGeneration(preorder , inorder ,root_idx+1 , inOrderEnd);

        return curr;
    }

    node* GenerateFullBinaryTree(queue<pair<int,int>>& preOrder)
    {
        if(preOrder.empty())
            return nullptr;

        int val = preOrder.front().first;
        bool IsLeaf = preOrder.front().second;
        preOrder.pop();
        node* curr = new node(val);
        if(IsLeaf) // if its a leaf node just return dont add left or right
            return curr;
        curr->left = GenerateFullBinaryTree(preOrder); // build my left tree
        curr->right = GenerateFullBinaryTree(preOrder); // built my right tree
        return curr;
    }
    bool IsMirror(node* left , node* right)
    {
        if(!left && !right)
            return true;
        if(!left || !right)
            return false;
        if(left->data != right->data)
            return false;
        
        return IsMirror(left->left , right->right) && IsMirror(left->right , right->left);
    }
    bool is_Flip_equ_p(node* f , node* s)
    {
        if(!f && !s)
            return true;
        if(!f || !s)
            return false;
        if(f->data != s->data)
            return false;
            
        // check left with left and right with right
        bool noFlip = (is_Flip_equ_p(f->left , s->left)) && (is_Flip_equ_p(f->right , s->right));
        // check left with right and right with left
        bool flip = (is_Flip_equ_p(f->left , s->right)) && (is_Flip_equ_p(f->right , s->left));
        return (noFlip || flip);
    }
    void GetDuplicateSubtree(node* curr , vector<string>& result , unordered_map<string,bool> & seen )
    {
        // if leaf node return
        if(!curr || (!curr->left && !curr->right))
            return;
        string currSub = Parenthesize_subTree(curr);
        // if exist in prev sub trees then add it to the result
        if(seen[currSub])
            result.push_back(move(currSub));
        else
            seen[currSub] = true; // mark as seen
        
        GetDuplicateSubtree(curr->left , result , seen);
        GetDuplicateSubtree(curr->right , result , seen);
    }
};


ostream& operator << (ostream& out , deque<int>  q)
{
    while(!q.empty())
    {
        out << q.front() <<" ";
        q.pop_front();
    }
    return out;
}



class expressionTree
{

    public:
    using node = node<char>;

    explicit expressionTree( string  postfix)
    {
        stack<node*>s;
        node* curr = nullptr;
        for(const auto& c : postfix)
        {
            curr = new node(c);
            // if its a digit push it to the stack
            if(!isOperator(c))
            {
                s.push(curr);
            }
            else
            {
                // if not pop 2 digit from the stack
                curr->right = s.top() , s.pop();
                curr->left = s.top() , s.pop();
                s.push(curr);
            }
        }
        root = s.top();
    }
    void PrintPostOrder()
    {
        if(!root)
            return;
        
        node* temp = root;
        root = temp->left;
        PrintPostOrder(); // print left
        root = temp->right;
        PrintPostOrder(); // print right
        root = temp;
        cout << root->data <<" ";

        return;
    }
    void printInOrder()
    {
        string exp{};
        InOrderExpression(exp);
        cout << exp << endl;

    }
    void printInOrder_2()
    {
        if(!root)
            return;
        
            // not leaf node add paren
        if(root->left && root->right)
            cout <<'(';
        node* temp = root;
        root = temp->left;
        printInOrder_2();
        cout << temp->data;
        root = temp->right;
        printInOrder_2();
        root = temp;
        if(root->left && root->right)
        cout <<')';

        return;
    }
    private:
    node* root;
    void InOrderExpression(string & str , stack<char>s = {} )
    {
        if(!root)
            return;
        
        bool paren = false;

        if(isOperator(root->data))
        {
            
            if(s.empty()) // if no prev operators just push curr in the stack
            {
                
                s.push(root->data);
            }
            // if precedence of current < prev operator then it must apply before the prev operator then put it between parentheses
            else if (precedence(root->data) < precedence(s.top()) || root->data == '^') // ^ should always be in paren
            {
                paren = true;
                str.push_back('(');
                s.push(root->data);
            }
            else
                s.push(root->data);   
            
        }
        node* temp = root;
        root = temp->left;
        InOrderExpression(str , s); // get left

        str.push_back(temp->data); // get current
        root = temp->right;
        InOrderExpression(str , s); // get right
        root = temp;
        if(paren)
            str.push_back(')');
        s.pop();
        return;
    }
    bool isOperator(char curr)
    {
        switch(curr)
        {
            case '*':
            case '/':
            case '+':
            case '-':
            case '^':
                return true;
        }

        return false;
    }
    int precedence(char x)
    {
        switch(x)
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
};

int main()
{
    /*tree t{};
    t.insert(1);
    t.add({2,3,5} , {'L','L','L'});
    t.add({2,3,6} , {'L','L','R'});
    t.add({2,4,7} , {'L','R','L'});
    t.add({2,4,8} , {'L','R','R'});

    t.add({2,3,5} , {'R','R','R'});
    t.add({2,3,6} , {'R','R','L'});
    t.add({2,4,7} , {'R','L','R'});
    t.add({2,4,8} , {'R','L','L'});
    cout << t.IsSymmetic_rec();*/
    /*
    deque<int> preOrder{};
    t.GetPreOrder(preOrder);
    deque<int>Inorder{};
    t.GetInorder(Inorder);
    tree t2(preOrder,Inorder);
    t2.print_inorder_iterative();
    cout << endl;
    t.print_inorder_iterative();

    queue<pair<int,int>> q;
    q.push({1,0});
    q.push({2,1});
    q.push({3,1});
    tree binary(q);
    binary.print_inorder_iterative();*/
    /*
    tree t{};
    t.insert(1);
    t.add({2,4} , {'L','L'});
    t.add({2,5,8} , {'L','R','R'});
    t.add({2,5,7} , {'L','R','L'});
    t.add({3,6} , {'R','L'});

    tree t2{};
    t2.insert(1);
    t2.add({2,4} , {'R','L'});
    t2.add({2,5,8} , {'R','R','L'});
    t2.add({2,5,7} , {'R','R','R'});
    t2.add({3,6} , {'L','R'});

    cout << t.IsEquivalentFlip(t2);*/

    tree t{};
    t.insert(1);
    t.add({2,3} , {'L','L'});
    t.add({4,2,3} , {'R','L','L'});
    t.add({4,5,6,8,8} , {'R','R','R','R','R'});
    t.add({4,5,6,7} , {'R','R','R','L'});
    t.add({4,5,6,7} , {'R','R','L' ,'L'});
    t.add({4,5,6,8,8} , {'R','R','R','R','R'});
    t.add({4,5,6,8,8} , {'R','R','L','R','R'});
    vector<string> res{};
    t.GetDuplicateSubtree(res);
    for(auto& str : res)
        cout << str << endl;

    std::string_view
    return 0;
}