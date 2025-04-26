#include <bits/stdc++.h>


using namespace std;

// check if f is prefix of s
bool isPrefix(const string & f , const string & s)
{
    if(f.size() > s.size())
        return false;

    return (s.substr(0,f.size()) == f);
}


template<typename T>
struct node{

    public:
    T data;
    int height;
    int size;
    node<T>* left;
    node<T>* right;
    explicit node(T data ,int height = 0 , int size = 1 ,node<T>* left = nullptr , node<T>* right = nullptr) : data(std::move(data)),height(height),size(size),left(left),right(right) 
    {}
    
};

template<typename T>
class AVL
{
    
    private:
    using Node = node<T>;
    Node* root;
    inline int getHeight(Node* n) const noexcept
    {
        if(!n)
            return -1; // null
        return n->height; // 0 for leaf node
    }   
    inline int updateHeight(Node* n) const noexcept
    {
        return 1 + std::max(getHeight(n->left) , getHeight(n->right));
    }
    inline int balanceFactor(Node* n) const noexcept
    {
        return ( getHeight(n->right) - getHeight(n->left));
    }
    Node* insert_p(const T & val , Node* curr)
    {
        if(val < curr->data)
        {
            if(curr->left)
                curr->left = insert_p(val,curr->left); // update left incase of it changed
            else
                curr->left = new Node(val);
        }
        else if ( val > curr->data)
        {
            if(curr->right)
                curr->right = insert_p(val,curr->right);
            else
                curr->right = new Node(val);
        }
        // update height
        curr->height = updateHeight(curr);
        return balance(curr); // balance current subtree and return the root of the current subtree
    }
    Node* right_rotation(Node* n)
    {
        Node* newRoot = n->left;
        n->left = newRoot->right;
        newRoot->right = n;
        updateHeight(n);
        updateHeight(newRoot);
        return newRoot;
    }
    Node* left_rotation(Node* n)
    {
        Node* newRoot = n->right;
        n->right = newRoot->left;
        newRoot->left = n;
        updateHeight(n);
        updateHeight(newRoot);
        return newRoot;
    }
    Node* balance(Node* curr)
    {
        int CurrBalanceFactor = balanceFactor(curr);
        if(CurrBalanceFactor == -2)
        {
            if(balanceFactor(curr->left) == 1) // left right case, convert to left left case first
            {
                curr->left = left_rotation(curr->left);
            }
            curr = right_rotation(curr);
        }
        else if ( CurrBalanceFactor == 2)
        {
            if(balanceFactor(curr->right) == -1) // right left case convert to right right first
            {
                curr->right = right_rotation(curr->right);
            }
            curr = left_rotation(curr);
        }
        return curr;
    }
    /*
        get the successor the current node and delete if from the subtree
        and replace it with the successor
        you need to call update height on the successor after using it

        return successor
    */
   Node* ReplaceWithSuccessor_p(Node* curr)
    {
        Node* successorParent = curr;
        Node* successor = curr->right;
        while(successor->left)
        {
            successorParent = successor;
            successor = successor->left;
        }
        // care don't create a cycle
        if(successorParent == curr)
        {
            successorParent->right = successor->right;
        }
        else
        {
            successorParent->left = successor->right;
        }
        successor->left = curr->left;
        successor->right = curr->right;

        successorParent->height = updateHeight(successorParent);
        balance(successorParent);

        delete curr;
        return successor;
    }
    Node* Delete_p(const T& val , Node* curr)
        {
            if(!curr)
                return nullptr;
            if(val < curr->data)
            {
                curr->left = Delete_p(val,curr->left);

            }
            else if (val > curr->data)
            {
                curr->right = Delete_p(val,curr->right);
            }
            else // found
            {
                // no child just delete it
                if(!curr->right && !curr->left)
                {
                    delete curr;
                    curr = nullptr;
                }
                else if (curr->right) // replace current node with its successor
                {
                    curr = ReplaceWithSuccessor_p(curr);
                }
                else if (curr->left) // only left child
                {
                    Node* temp = curr->left;
                    delete curr;
                    curr = temp;
                }
            }

            if(curr)
            {
                curr->height = updateHeight(curr);
                curr = balance(curr);
            }

            return curr;
        }
    int countNodes_p(Node* curr)
    {
        if(!curr)
            return 0;
        return  1 + countNodes_p(curr->left) + countNodes_p(curr->right);
    }
    int FindAndCountInversion_p(Node* curr , const T & val)
    {
        int ret = 0;
        if(!curr)
            return 0;
        if(val < curr->data)
        {
            // every time we go left then current node and all current node subtree is bigger than it
            ret +=  countNodes_p(curr->right);
            ret++; // current node is bigger than val
            return ret + FindAndCountInversion_p(curr->left,val);
        }
        else if(val > curr->data)
        {
            return ret + FindAndCountInversion_p(curr->right,val);
        }
        // found 
        // count all nodes in right subtree since its bigger
        ret += countNodes_p(curr->right);

        return ret;
    }
    public:
    AVL() : root(nullptr){}

    void insert(const T & val)
    {
        if(!root)
            root = new Node{val};
        else
        {
            root = insert_p(val,root);
        }
    }
    void Delete(int val)
    {
        if(root)
            root = Delete_p(val,root);
    }
    std::pair<bool,int> lowerBound(int target)
    {
        std::pair<bool,int>ret {false,INT_MIN};
        Node* curr = root;
        while(curr && curr->data != target)
        {
            if(target < curr->data)
            {
                // keep updating the successor every time we go left
                ret.first = true;
                ret.second = curr->data;
                if(curr->left)
                    curr = curr->left;
                else
                {
                    break;
                }
            }
            else if(target > curr->data)
            {
                if(curr->right)
                    curr = curr->right;
                else
                {
                    break;
                }
            }
        }
        if(curr->data == target)
        {
            ret.first = true;
            ret.second = curr->data;
        }
        else // data not found
        {
            if(curr->right)
            {
                curr = curr->right;
                while(curr->left)
                    curr = curr->left;
                ret.first = true;
                ret.second = curr->data; // the sucessor
            }
        }
        return ret;
    }
    std::pair<bool,int>upperBound(int target)
    {
        Node* curr = root;
        std::pair<bool,int>ret{false,INT_MIN};
        while(curr && curr->data != target)
        {
            if(target < curr->data)
            {
                ret.first = true;
                ret.second = curr->data;// keep updating the successor every time u go left

                curr = curr->left;
            }
            else if (target > curr->data)
            {
                curr = curr->right;
            }
        }
        if(curr && curr->data == target && curr->right)
        {
            // get the sucessor
            curr = curr->right;
            while(curr->left)
                curr = curr->left;
            ret.second = curr->data;
            ret.first = true;
        }
        return ret;
    }

    int CountInversion(const vector<T>& vec)
    {
        int ret = 0;
        for(auto& n : vec)
        {
            insert(n);
            ret += FindAndCountInversion_p(root,n);
        }
        return ret;
    }
    bool isExist(const T & target)
    {
        if(!root)
            return false;
        Node* curr = root;
        while(curr->data != target)
        {
            if(target < curr->data)
            {
                if(curr->left)
                    curr = curr->left;
                else
                    return false;
            }
            if(target > curr->data)
            {
                if(curr->right)
                    curr = curr->right;
                else
                    return false;
            }
        }
        return true;
    }

    template<typename U = T>
    // member function work with strings only
    typename std::enable_if<std::is_same<U, std::string>::value ,bool>::type   
    prefix_exist(const U & prefix)
    {

        if(!root)
            return false;
        Node* curr = root;
        while(curr->data != prefix)
        {
            if(prefix < curr->data)
            {
                if(isPrefix(prefix,curr->data))
                    return true;
                if(curr->left)
                {
                    curr = curr->left;
                }
                else
                    return false;
            }
            if(prefix > curr->data)
            {
                if(curr->right)
                    curr = curr->right;
                else
                    return false;
            }
        }

        return (curr->data == prefix);
    }
};




template<typename T>
class AVL_count
{
    
    private:
    using Node = node<T>;
    Node* root;
    inline int getHeight(Node* n) const noexcept
    {
        if(!n)
            return -1; // null
        return n->height; // 0 for leaf node
    }
    inline int GetSize(Node* n)
    {
        if(!n)
            return 0;

        return n->size;
    }
    inline int updateSize(Node* n)
    {
        return 1 + GetSize(n->left) + GetSize(n->right);
    }
    inline int updateHeight(Node* n) const noexcept
    {
        return 1 + std::max(getHeight(n->left) , getHeight(n->right));
    }
    inline int balanceFactor(Node* n) const noexcept
    {
        return ( getHeight(n->right) - getHeight(n->left));
    }
    Node* insert_p(const T & val , Node* curr)
    {
        if(val < curr->data)
        {
            if(curr->left)
                curr->left = insert_p(val,curr->left); // update left incase of it changed
            else
                curr->left = new Node(val);
        }
        else if ( val > curr->data)
        {
            if(curr->right)
                curr->right = insert_p(val,curr->right);
            else
                curr->right = new Node(val);
        }
        // update height
        curr->height = updateHeight(curr);
        curr->size = updateSize(curr);
        return balance(curr); // balance current subtree and return the root of the current subtree
    }
    Node* right_rotation(Node* n)
    {
        Node* newRoot = n->left;
        n->left = newRoot->right;
        newRoot->right = n;
        n->height = updateHeight(n);
        n->size = updateSize(n);
        newRoot->height = updateHeight(newRoot);
        newRoot->size = updateSize(newRoot);
        return newRoot;
    }
    Node* left_rotation(Node* n)
    {
        Node* newRoot = n->right;
        n->right = newRoot->left;
        newRoot->left = n;
        n->height = updateHeight(n);
        n->size = updateSize(n);
        
        newRoot->height = updateHeight(newRoot);
        newRoot->size = updateSize(newRoot);
        return newRoot;
    }
    Node* balance(Node* curr)
    {
        int CurrBalanceFactor = balanceFactor(curr);
        if(CurrBalanceFactor == -2)
        {
            if(balanceFactor(curr->left) == 1) // left right case, convert to left left case first
            {
                curr->left = left_rotation(curr->left);
            }
            curr = right_rotation(curr);
        }
        else if ( CurrBalanceFactor == 2)
        {
            if(balanceFactor(curr->right) == -1) // right left case convert to right right first
            {
                curr->right = right_rotation(curr->right);
            }
            curr = left_rotation(curr);
        }
        return curr;
    }
    /*
        get the successor the current node and delete if from the subtree
        and replace it with the successor
        you need to call update height on the successor after using it

        return successor
    */
   Node* ReplaceWithSuccessor_p(Node* curr)
    {
        Node* successorParent = curr;
        Node* successor = curr->right;
        while(successor->left)
        {
            successorParent = successor;
            successor = successor->left;
        }
        // care don't create a cycle
        if(successorParent == curr)
        {
            successorParent->right = successor->right;
        }
        else
        {
            successorParent->left = successor->right;
        }
        successor->left = curr->left;
        successor->right = curr->right;

        successorParent->height = updateHeight(successorParent);
        successorParent->size = updateSize(successorParent);
        balance(successorParent);

        delete curr;
        return successor;
    }
    Node* Delete_p(const T& val , Node* curr)
        {
            if(!curr)
                return nullptr;
            if(val < curr->data)
            {
                curr->left = Delete_p(val,curr->left);

            }
            else if (val > curr->data)
            {
                curr->right = Delete_p(val,curr->right);
            }
            else // found
            {
                // no child just delete it
                if(!curr->right && !curr->left)
                {
                    delete curr;
                    curr = nullptr;
                }
                else if (curr->right) // replace current node with its successor
                {
                    curr = ReplaceWithSuccessor_p(curr);
                }
                else if (curr->left) // only left child
                {
                    Node* temp = curr->left;
                    delete curr;
                    curr = temp;
                }
            }

            if(curr)
            {
                curr->height = updateHeight(curr);
                curr->size = updateSize(curr);
                curr = balance(curr);
            }

            return curr;
        }
    int FindAndCountInversion_p(Node* curr , const T & val)
    {
        int ret = 0;
        if(!curr)
            return 0;
        if(val < curr->data)
        {
            // every time we go left then current node and all current node subtree is bigger than it
            ret +=  GetSize(curr->right);
            ret++; // current node is bigger than val
            return ret + FindAndCountInversion_p(curr->left,val);
        }
        else if(val > curr->data)
        {
            return ret + FindAndCountInversion_p(curr->right,val);
        }
        // found 
        // count all nodes in right subtree since its bigger
        ret += GetSize(curr->right);
        
        return ret;
    }
    public:
    AVL_count() : root(nullptr){}

    void insert(const T & val)
    {
        if(!root)
            root = new Node{val};
        else
        {
            root = insert_p(val,root);
        }
    }
    void Delete(int val)
    {
        if(root)
            root = Delete_p(val,root);
    }
    std::pair<bool,int> lowerBound(int target)
    {
        std::pair<bool,int>ret {false,INT_MIN};
        Node* curr = root;
        while(curr && curr->data != target)
        {
            if(target < curr->data)
            {
                // keep updating the successor every time we go left
                ret.first = true;
                ret.second = curr->data;
                if(curr->left)
                    curr = curr->left;
                else
                {
                    break;
                }
            }
            else if(target > curr->data)
            {
                if(curr->right)
                    curr = curr->right;
                else
                {
                    break;
                }
            }
        }
        if(curr->data == target)
        {
            ret.first = true;
            ret.second = curr->data;
        }
        else // data not found
        {
            if(curr->right)
            {
                curr = curr->right;
                while(curr->left)
                    curr = curr->left;
                ret.first = true;
                ret.second = curr->data; // the sucessor
            }
        }
        return ret;
    }
    std::pair<bool,int>upperBound(int target)
    {
        Node* curr = root;
        std::pair<bool,int>ret{false,INT_MIN};
        while(curr && curr->data != target)
        {
            if(target < curr->data)
            {
                ret.first = true;
                ret.second = curr->data;// keep updating the successor every time u go left

                curr = curr->left;
            }
            else if (target > curr->data)
            {
                curr = curr->right;
            }
        }
        if(curr && curr->data == target && curr->right)
        {
            // get the sucessor
            curr = curr->right;
            while(curr->left)
                curr = curr->left;
            ret.second = curr->data;
            ret.first = true;
        }
        return ret;
    }

    int CountInversion(const vector<T>& vec)
    {
        int ret = 0;
        for(auto& n : vec)
        {
            insert(n);
            ret += FindAndCountInversion_p(root,n); // log(n)
        }
        return ret;
    }
};




namespace{

struct task
{
    int id;
    int priority;

    task(int id = -1 , int priority = -1) : id(id),priority(priority){}

    bool operator < (const task& other)const noexcept
    {
        return (priority < other.priority);
    }
    bool operator >(const task& other) const noexcept
    {
        return (priority > other.priority);
    }
    bool operator == (const task& other)const noexcept
    {
        return (priority == other.priority);
    }

};

template<typename T>
struct NodeDuplicates{

    public:
    queue<T>que; // save element with same priority
    int height;
    NodeDuplicates<T>* left;
    NodeDuplicates<T>* right;
    explicit NodeDuplicates(T data , int height = 0 ,   NodeDuplicates<T>* left = nullptr ,   NodeDuplicates<T>* right = nullptr)
    : que(queue<T>{}),height(height) , left(left),right(right)
    {
        que.push(std::move(data));
    }
    // get top and pop it
    T pop()
    {
        T ret = que.front();
        que.pop();
        return ret;
    }
    bool isEmpty()
    {
        return que.empty();
    }
    void insert(const T& val)
    {
        que.push(val);
    }

};

template<typename T>
bool operator < (const T & x , const queue <T>& y)
{
    return (x < y.front());
}
template<typename T>
bool operator > (const T & x , const queue <T>& y)
{
    return (x > y.front());
}

class PriorityQueue
{

    private:
    using Node = NodeDuplicates<task>;
    Node* root;

    inline int getHeight(Node* n) const noexcept
    {
        if(!n)
            return -1; // null
        return n->height; // 0 for leaf node
    }   
    inline int updateHeight(Node* n) const noexcept
    {
        return 1 + std::max(getHeight(n->left) , getHeight(n->right));
    }
    inline int balanceFactor(Node* n) const noexcept
    {
        return ( getHeight(n->right) - getHeight(n->left));
    }
    Node* insert_p(const task & val , Node* curr)
    {
        if(val < curr->que)
        {
            if(curr->left)
                curr->left = insert_p(val,curr->left); // update left incase of it changed
            else
                curr->left = new Node(val);
        }
        else if ( val > curr->que)
        {
            if(curr->right)
                curr->right = insert_p(val,curr->right);
            else
                curr->right = new Node(val);
        }
        else // we found a node with same priority add the new one into the queue
        {
            curr->insert(val);

            // no need to update height or rebalance
            return curr;
        }
        // update height
        curr->height = updateHeight(curr);
        return balance(curr); // balance current subtree and return the root of the current subtree
    }
    Node* right_rotation(Node* n)
    {
        Node* newRoot = n->left;
        n->left = newRoot->right;
        newRoot->right = n;
        updateHeight(n);
        updateHeight(newRoot);
        return newRoot;
    }
    Node* left_rotation(Node* n)
    {
        Node* newRoot = n->right;
        n->right = newRoot->left;
        newRoot->left = n;
        updateHeight(n);
        updateHeight(newRoot);
        return newRoot;
    }
    Node* balance(Node* curr)
    {
        int CurrBalanceFactor = balanceFactor(curr);
        if(CurrBalanceFactor == -2)
        {
            if(balanceFactor(curr->left) == 1) // left right case, convert to left left case first
            {
                curr->left = left_rotation(curr->left);
            }
            curr = right_rotation(curr);
        }
        else if ( CurrBalanceFactor == 2)
        {
            if(balanceFactor(curr->right) == -1) // right left case convert to right right first
            {
                curr->right = right_rotation(curr->right);
            }
            curr = left_rotation(curr);
        }
        return curr;
    }
    /*
        get the successor the current node and delete if from the subtree
        and replace it with the successor
        you need to call update height on the successor after using it

        return successor
    */
   Node* ReplaceWithSuccessor_p(Node* curr)
    {
        Node* successorParent = curr;
        Node* successor = curr->right;
        while(successor->left)
        {
            successorParent = successor;
            successor = successor->left;
        }
        // care don't create a cycle
        if(successorParent == curr)
        {
            successorParent->right = successor->right;
        }
        else
        {
            successorParent->left = successor->right;
        }
        successor->left = curr->left;
        successor->right = curr->right;

        successorParent->height = updateHeight(successorParent);
        balance(successorParent);

        delete curr;
        return successor;
    }

    
    Node* deque_p(Node* curr , task& output)
    {
        if(curr->right)
            curr->right = deque_p(curr->right , output);
        else // found
        {
            output = curr->pop();
                // still have another element with same priority
            if(!curr->isEmpty()) // no need to delete any thing just pop front and return
                    return curr;
                // no child just delete it
            if(!curr->right && !curr->left)
            {
                    delete curr;
                    curr = nullptr;
            }
            else if (curr->right) // replace current node with its successor
            {
                    curr = ReplaceWithSuccessor_p(curr);
            }
            else if (curr->left) // only left child
            {
                    Node* temp = curr->left;
                    delete curr;
                    curr = temp;
            }
        }
            if(curr)
            {
                curr->height = updateHeight(curr);
                curr = balance(curr);
            }

            return curr;
    }
    int countNodes_p(Node* curr)
    {
        if(!curr)
            return 0;
        return  1 + countNodes_p(curr->left) + countNodes_p(curr->right);
    }
    Node* min()
    {
        Node* curr = root;
        while(curr->left)
            curr = curr->left;

        return curr;
    }
    Node* max()
    {
        Node* curr = root;
        while(curr->right)
            curr = curr->right;

        return curr;
    }
    Node* clear_all(Node* curr)
    {
        if(curr->left)
            curr->left = clear_all(curr->left);
        if(curr->right)
            curr->right = clear_all(curr->right);

        delete curr;
        curr = nullptr;
        return curr;
    }
    public:
    PriorityQueue() : root(nullptr)
    {}
    ~PriorityQueue()
    {
        if(root)
            root = clear_all(root);
    }
    void enqueue(int id , int priority)
    {
        if(root)
            root = insert_p({id,priority} ,root);
        else
            root = new Node(id,priority);
    }
    task deque()
    {
        if(!root)
            throw std::runtime_error("empty priority queue\n");

        task ret{};
        root = deque_p(root,ret);

        return ret;
    }
    bool isEmpty()
    {
        return (root == nullptr);
    }
};
}

ostream & operator << (ostream& out , const std::pair<bool,int> & x)
{
    out <<x.first <<"," << x.second;

    return out;
}

int minNumberOfNodesInAvl_rec(int height)
{
    if(height == 0)
        return 1;
    else if (height == 1)
        return 2;

    return 1 + minNumberOfNodesInAvl_rec(height-1) + minNumberOfNodesInAvl_rec(height -2);
}

int minNumberOfNodesInAvl_iter(int height)
{
    int first = 1;
    int second = 2;
    int curr = 0;
    if(height == 1)
        return first;
    else if (height == 2)
        return second;

    for(int i = 2 ; i <=height ; ++i)
    {
        curr = first+second +1;
        first = second;
        second = curr;
    }

    return curr;
}

int main()
{
    /*
    AVL<int> tree{};
    tree.insert(2);
    tree.insert(5);
    tree.insert(10);
    tree.insert(13);
    tree.insert(15);
    tree.insert(20);
    tree.insert(40);
    tree.insert(50);
    tree.insert(70);*/
    /*
    cout << tree.lowerBound(50) << endl;
    cout << tree.lowerBound(51) << endl;
    cout << tree.lowerBound(70)<<endl;
    cout << tree.lowerBound(71)<<endl;
    cout<<tree.lowerBound(7)<<endl;
    cout<<tree.lowerBound(25)<<endl;
    cout<<tree.lowerBound(60)<<endl;
    */
   /*cout << tree.upperBound(50) << endl;
   cout << tree.upperBound(51) << endl;
   cout << tree.upperBound(70)<<endl;
   cout << tree.upperBound(11)<<endl;
   cout<<tree.upperBound(20)<<endl;
   cout<<tree.upperBound(45)<<endl;*/
   //AVL_count<int> t2{};
   //cout << t2.CountInversion({5,4,3,2,1}) << endl; // 10
   //cout << t2.CountInversion({10,5,8,2,12,6}); //8

  /* PriorityQueue q;
   q.enqueue(1131,1);
   q.enqueue(3111,3);
   q.enqueue(2211,2);
   q.enqueue(3161,3);
   q.enqueue(7761,7);
   cout << q.deque().id << endl; //7761
   cout << q.deque().id << endl; //3111

   q.enqueue(1525,1);
   q.enqueue(2815,2);
   q.enqueue(3845,3);
   q.enqueue(3145,3);
   //3161 3845 3145 2211 2815 1131 1525
   while(!q.isEmpty())
    cout << q.deque().id <<" ";
    cout <<'\n';*/

    AVL<std::string> tree;
    tree.insert(string{"abcd"});
    tree.insert(string{"xyz"});
    cout << tree.isExist(string{"abcd"})<<endl; // 1
    cout << tree.isExist(string{"ab"})<<endl; // 0
    cout << tree.prefix_exist(string{"ab"}) << endl; //1

    tree.insert(string{"ab"});
    cout << tree.isExist(string{"ab"})<<endl; // 1
    cout << tree.isExist(string{"cd"})<<endl; // 0
    cout << tree.isExist(string{"abcde"})<<endl; // 0
    return 0;
}