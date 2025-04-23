#include <bits/stdc++.h>


using namespace std;

deque<int> NextGreaterIdx(deque<int> q)
{
    stack<int>s{};

    for(int i = 0 ; i < q.size() ; ++i) // o(n)
    {
        while(!s.empty() && q[s.top()] < q[i])
        {
            q[s.top()] = i;
            s.pop();
        }
        s.push(i);
    }
    while(!s.empty())
    {
        q[s.top()] = q.size();
        s.pop();
    }

    return q;
}



template<class T>
struct node{
    T data;
    node* left;
    node* right;

    explicit node(T data , node<T>* left = nullptr , node<T>* right = nullptr): data(data),left(left),right(right)
    {

    }
};

ostream& operator << (ostream& out , const deque<int>& q)
{
    
    for(auto& it : q)
    {
        out << it <<" ";
    }
    return out;
}

class BST {

    public:

    BST(int data) : left(nullptr) , right(nullptr) , data(data)
    {
    }
    BST(deque<int> levelOrder)
    {
        struct nodeRange{
            BST* node;
            int min;
            int max;
            nodeRange(BST* node , int min , int max) : node(node) , min(min),max(max){}
        };
        deque<nodeRange> q;
        data = levelOrder.front();
        levelOrder.pop_front();
        q.push_back({this,INT_MIN,INT_MAX});
        

    }
    void Insert(int val)
    {
        if(val < data)
        {
            if(!left)
                left = new BST(val);
            else
                left->Insert(val);
        }
        else if(val > data)
        {
            if(!right)
                right = new BST(val);
            else
                right->Insert(val);
        }

    }
    int min()
    {
        // min is the most left node
        BST* curr = this;
        while(curr->left)
            curr = curr->left;


        return curr->data;
    }
    /*
        getsuccessor using parents tracking

        bool if found successor or not
        int the successor if found else INT_MAX
    */
   std::pair<int,bool> GetSuccessor_v2(int val)
   {
        vector<BST*>Parents{};

        // find the value
        BST* curr = this;
        while(curr && curr->data != val)
        {
            if(val < curr->data)
            {
                Parents.push_back(curr);
                curr = curr->left;
            }
            else
            {
                Parents.push_back(curr);
                curr = curr->right;
            }
        }
        if(curr && curr->right)
        {
            return {curr->right->min(), true};
        }

        auto PrevParent = Parents.rbegin();
        BST* child = curr;

        // find first node that's curr on his left side
        while(PrevParent != Parents.rend())
        {
            if((*PrevParent)->left == child)
            {
                return {(*PrevParent)->data,true};
            }
            else
            {
                child = *PrevParent;
                ++PrevParent;
            }
        }

        return {INT_MAX,false};
   }
    /*
        bool if found successor or not
        int the successor if found else INT_MAX
    */
    std::pair<int,bool> GetSuccessor(int val)
    {
        int minY = INT_MAX;
        bool isExists = GetSuccessor_p(val , minY);

        return {minY , isExists};
    }


    vector<int> GetQueriesOfSuccessor(queue<int> input)
    {
        std::vector<int>output{};
        output.reserve(input.size());
        GetQueriesOfSuccessor_p(input , output);
        return output;
    }
    void Delete(int val)
    {
        if(val == data &&  !right)
        {
            // we try to delete the root
            // handle it later
            throw std::runtime_error("we can't Delete the root\n");
            return;
        }
        Delete_p(val);
    }
    void PrintInOrder()
    {
        if(left)
            left->PrintInOrder();
        cout << data <<" ";
        if(right)
            right->PrintInOrder();
        
        return;
    }
    bool search_iterative(int target)
    {
        BST* curr = this;

        while(curr && curr->data != target)
        {
            if(target > curr->data)
                curr = curr->right;
            else
                curr = curr->left;
        }
        return (curr != nullptr);
    }
    bool IsBST(int min = INT_MIN , int max = INT_MAX)
    {
        bool Isleft = true;
        bool Isright = true;

        if(data >= max || data <= min)
            return false;
        if(left)
            Isleft = left->IsBST(min,data);
        if(right && Isleft)
            Isright = right->IsBST(data , max);

        return(Isleft && Isright);
    }
    static BST* CreateBalancedBST(const vector<int>& SortedArr)
    {
        return CreateBalancedBST_p (begin(SortedArr) , end(SortedArr));
    }
    int KthSmallest(int k )
    {
        int result = -1234;
        KthSmallest_p(this,k,result);

        return result;
    }
    
    void BST_toArray(vector<BST*>& vec)
    {
        
        if(left)
            left->BST_toArray(vec);

        vec.push_back(this);

        if(right)
            right->BST_toArray(vec);

        
    }
    /*
        lowest common parent

        tip: it can help us to find the path between two nodes

    */
    int LowestCommonAncestor(int val1 , int val2)
    {
        /*
            if both values in my righ tree then check right tree
        */
        if(val1 > data && val2 > data)
            return right->LowestCommonAncestor(val1,val2);
        /*
            if botth values in my left tree then check my left tree
        */
        else if (val1 < data && val2 < data)
            return left->LowestCommonAncestor(val1,val2);

        /*
            if val1 in my left and val2 in my right as example
            then iam the lowest common parent

            or if my value equal to val1 or val2
            then iam the lowest common parent
        */
        return data;
    }
    static BST* CreateBST_PreOrder(deque<int>& preOrder , int MIN = INT_MIN , int MAX = INT_MAX)
    {
        if(preOrder.empty() || preOrder.front() < MIN || preOrder.front() > MAX)
            return nullptr;

        BST* root = new BST(preOrder.front());
        preOrder.pop_front();
        if(!preOrder.empty())
            root->left = CreateBST_PreOrder(preOrder , MIN , root->data);
        if(!preOrder.empty())
            root->right = CreateBST_PreOrder(preOrder,root->data , MAX);

        return root;
    }
    static BST* CreateBST_PreOrder_2(deque<int>& preOrde)
    {
        
        // o(n^2)
        return CreateBST_PreOrder_p(nullptr,preOrde , begin(preOrde) , end(preOrde));
    }
    static BST* CreateBST_PreOrder_3(deque<int>& preOrde)
    {
        deque<int>PreCalculatedNextGreater = NextGreaterIdx(preOrde);
        cout << PreCalculatedNextGreater << endl;
        return CreateBST_PreOrder_3(nullptr,preOrde, 0 , preOrde.size() , PreCalculatedNextGreater);
    }

    void GetPreOrder(deque<int> &q)
    {
        q.push_back(data);
        if(left)
            left->GetPreOrder(q);
        if(right)
            right->GetPreOrder(q);
    }
    private:
    BST* left;
    BST* right;
    int data;
    bool GetSuccessor_p(int val , int& minY)
    {
        if(val < data)
        {
            // keep tracking of min value that bigger than data
            minY = std::min(data , minY);
            if(!left)
            {
                // no more left this current is the sucessor
                return true;   
            }
            return left->GetSuccessor_p(val , minY);
        }
        else if(val > data)
        {
            // no sucessor
            if(!right)
            {
                return (minY != INT_MAX );
            }
            return right->GetSuccessor_p(val,minY);
        }

        //  val == data
        if(right)
        {
            // if it has right then the sucessor is the most left node in the right subtree
            // compare between my right subtree min and prevmin
            minY = std::min(right->min() , minY);
        }

        return (minY != INT_MAX );
    }

    void Delete_p(int val , BST* prev = nullptr)
    {
        if(val < data)
        {
            if(!left)
                return;
            return left->Delete_p(val,this);
        }
        else if (val > data)
        {
            if(!right)
                return;
            return right->Delete_p(val,this);
        }
        // val == data;

        // first case no childs just delete it and link the prevNode
        if(!left && !right)
        {
            BST* temp = this;
            if(prev)
            {
                if(prev->right == this)
                {
                    prev->right = nullptr;
                }
                else
                {
                    prev->left = nullptr;
                }
            }
            delete temp;
        }
        // second case 1 child
        else if(!left || !right)
        {
            BST* temp = this;
            if(prev)
            {
                if(prev->right == this)
                    prev->right = (left) ? left : right;
                else
                    prev->left = (left) ? left : right;
            }   
            delete temp;
        }
        // third case 2 childs
        else
        {
            // get the successor and the parent of the successor
            BST* Successor = this->right;
            BST* SuccessorParent = this;
            while(Successor->left)
            {
                SuccessorParent = Successor;
                Successor = Successor->left;
            }
            SuccessorParent->right = Successor->right;
            data = Successor->data;
            delete Successor;
        }
    }

    static BST* CreateBalancedBST_p(vector<int>::const_iterator beg ,  vector<int>::const_iterator end)
    {
        if(beg >= end)
            return nullptr;
            
        auto middle = beg + (end - beg) / 2;
        
        BST* n = new BST(*middle);
        n->left = CreateBalancedBST_p(beg,middle);
        n->right = CreateBalancedBST_p(middle+1,end);
    
        return n;
    }
    void KthSmallest_p(BST* node , int& k  , int& result )
        {
        if(k<= 0)
            return;

        if(node->left)
            KthSmallest_p(node->left,k,result);

        if(k<= 0)
            return;

        --k;
        if(k == 0)
        {
            result = node->data;
            return;
        }

        if(node->right)
            KthSmallest_p(node->right,k,result);

        return;
    }

    void GetParents_p(int val , deque<int>& q)
    {
        q.push_front(data);
        if(val == data)
            return;

        if(val > data && right)
            right->GetParents_p(val,q);
        else if (val < data && left)
            left->GetParents_p(val,q);

    }

    void GetQueriesOfSuccessor_p(queue<int>& input, vector<int>& output , int CurrentParentSuccessor = INT_MIN)
    {
        if(input.empty())
            return;
        if(input.front() < data )
        {
            if(left)
            {
                CurrentParentSuccessor = data;
                left->GetQueriesOfSuccessor_p(input,output,CurrentParentSuccessor);
            }
            else // current value doesn't exists
            {
                output.push_back(INT_MIN);
                input.pop();
            }
            if(input.empty())
                return;
        }
        else if (input.front() > data)
        {
            if(right)
                right->GetQueriesOfSuccessor_p(input,output,CurrentParentSuccessor);
            else
            {
                output.push_back(INT_MIN);
                input.pop();
            }
            if(input.empty())
                return;
        }

        // my val == queue front value
        // if i have right the successor if most left node in my right subtree
        if(right)
        {
            output.push_back(right->min());
        }
        else
        { // else my first parent that iam on his left side
            output.push_back(CurrentParentSuccessor);
        }
        input.pop();

        return;
    }

    static BST* CreateBST_PreOrder_p(BST*curr , deque<int>& preOrder , deque<int>::iterator beg , deque<int>::iterator end )
    {
        if(beg == end)
            return curr;

        curr =  new BST (*beg);
        auto s = beg;
        auto e = end;
        deque<int>::iterator firstBigger{end};
        while(s != e)
        {
            if(*s > curr->data)
            {
                firstBigger = s;
                break;
            }
            else
                s++;
        }

        curr->left = CreateBST_PreOrder_p(curr->left , preOrder,beg+1,firstBigger);
        curr->right = CreateBST_PreOrder_p(curr->right,preOrder,firstBigger , end);
        return curr;
    }

    static BST* CreateBST_PreOrder_3(BST*curr ,const deque<int>& preOrde , int start , int end ,  const deque<int>& NextGreater)
    {
        if(start>= end)
            return nullptr;

        curr = new BST(preOrde[start]);
        
        int split = NextGreater[start];

        curr->left = CreateBST_PreOrder_3(curr->left,preOrde,start+1,split,NextGreater);
        curr->right = CreateBST_PreOrder_3(curr->right,preOrde,split,end,NextGreater);

        return curr;
    }

};

bool IsDegenerate(vector<int>& preOrder)
{
    if(preOrder.size() <= 2)
        return false; 
    int min = INT_MIN;
    int max = INT_MAX;


    for(int i = 1 ; i < preOrder.size() ; ++i)
    {    
        if(preOrder[i] < min || preOrder[i] > max)
            return false;

        // left node update max
        if(preOrder[i] < preOrder[i-1])
            max = preOrder[i-1];
        else // go right update min
        {
            min = preOrder[i-1];
        }
    }

    return true;
}

// bst with parent pointer
class BST_p{

    public:
    explicit BST_p(int data , BST_p* parent = nullptr,BST_p* left= nullptr , BST_p* right = nullptr )
    : data(data),left(left),right(right),parent(parent){}

    
    void insert(int val)
    {
        BST_p* curr = this;

        while(curr)
        {
            if(curr->data > val)
            {
                if(!curr->left)
                {
                    curr->left = new BST_p(val,curr);
                    return;
                }
                else
                    curr = curr->left;
            }
            else if (curr->data < val)
            {
                if(!curr->right)
                {
                    curr->right = new BST_p(val,curr);
                    return;
                }
                else
                    curr = curr->right;
            }
            else
                throw std::runtime_error("bst with duplicated values\n");
        }
    }
    int min()
    {
        BST_p* curr = this;
        while(curr->left)
            curr = curr->left;

        return curr->data;
    }
    std::pair<int,bool>GetSuccessor(int val)
    {
        BST_p* curr = this;
        while(curr && curr->data != val)
        {
            if(val < curr->data)
            {
                curr = curr->left;
            }
            else
                curr = curr->right;
        }
        if(curr)
            return {INT_MIN , false};
        
        if(curr->right)
            return {curr->right->data , true};

        // have no right
        while(curr && curr->parent)
        {
            if(curr->parent->left == curr)
                return {curr->parent->data , true};
        }

        return {INT_MIN , false};
    }

    private:
    BST_p* parent;
    BST_p* left;
    BST_p* right;
    int data;
};


class BinarySearchTree
{
    private:
    struct BinaryNode{
        int data;
        BinaryNode* left;
        BinaryNode* right;
        BinaryNode(int data , BinaryNode* left = nullptr , BinaryNode* right = nullptr) : data(data) , left(left),right(right){}
    };
    BinaryNode* root;
    void insert_p(int val , BinaryNode* curr)
    {
        if(val < curr->data)
        {
            if(curr->left)
            {
                insert_p(val,curr->left);
            }
            else
            {
                curr->left = new BinaryNode(val);
                return;
            }
        }
        else if(val > curr->data)
        {
            if(curr->right)
            {
                insert_p(val,curr->right);
            }
            else
            {
                curr->right = new BinaryNode(val);
                return;
            }
        }
        // else duplicated value dont insert
    }
    
    void PrintInOrder_p(BinaryNode* curr)
    {
        if(curr->left)
            PrintInOrder_p(curr->left);
        cout << curr->data <<' ';
        if(curr->right)
            PrintInOrder_p(curr->right);
    }
    void Delete_p(int val , BinaryNode* curr , BinaryNode* parent = nullptr)
    {
        if(!curr)
            return;
        if(val< curr->data)
        {
            return Delete_p(val,curr->left,curr);
        }
        else if ( val > curr->data)
            return Delete_p(val,curr->right,curr);
        
        if(!curr->left && !curr->right)
        {
            // root with no childs
            if(parent == nullptr)
            {
                delete root;
                root = nullptr;
            }
            else
            {
                if(parent->left == curr)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
                delete curr;
                curr = nullptr;
            }
            return;
        }
        else if (!curr->left || !curr->right)
        {
            // have one child
            if(parent == nullptr) // updating the root care
            {
                root = (curr->left)? curr->left : curr->right;
            }
            else if(parent->left == curr)
            {
                parent->left = (curr->left)? curr->left : curr->right;
            }
            else
                parent->right = (curr->left)? curr->left : curr->right;
            delete curr;
            curr = nullptr;
        }
        else // two child
        {
            // get the successor of curr node
            BinaryNode* successor = curr->right;
            BinaryNode* ParentSuccessor = curr;
            while(successor->left)
            {
                ParentSuccessor = successor;
                successor = successor->left;
            }
            if(ParentSuccessor != curr)
                ParentSuccessor->left = successor->right;

            successor->left = curr->left;

            /*
                care don't create a cycle
                if  successor is first right node
                and u make successor right = curr u create a cycle
            */
            if(curr->right != successor)
                successor->right = curr->right;
                
            // update nodes instead of copying data
            if(parent == nullptr) // care update root
            {
                root = successor;
            }
            else if (parent->left == curr)
            {
                parent->left = successor;
            }
            else
                parent->right = successor;
            delete curr;
            curr = nullptr;
        }
        return;
    }
    public:
    BinarySearchTree() : root(nullptr){}
    BinarySearchTree(int data) : root(new BinaryNode(data)){}

    void insert(int val)
    {
        if(!root)
        {
            root = new BinaryNode(val);
            return;
        }
        insert_p(val,root);
    }
    bool search(int target)
    {
        BinaryNode* curr = root;
        while(curr && curr->data != target)
        {
            if(target < curr->data)
                curr = curr->left;
            else
                curr = curr->right;
        }

        return (curr != nullptr);
    }
    void PrintInOrder()
    {
        if(!root)
            return;

        PrintInOrder_p(root);
        cout << '\n';
    }
    void PrintLevelOrder()
    {
        if(!root)
            return;
        queue<BinaryNode*>q{};
        q.push(root);
        BinaryNode* curr;
        int sz;
        while(!q.empty())
        {
            sz = q.size();
            while(sz--)
            {
                curr = q.front();
                q.pop();
                cout << curr->data <<" ";
                if(curr->left)
                    q.push(curr->left);
                if(curr->right)
                    q.push(curr->right);       
            }
        }
    }
    void Delete(int val)
    {
        if(!root)
            return;

        Delete_p(val,root);
    }
};


int main()
{
    /*BST tree(50);
    tree.Insert(20);
    tree.Insert(15);
    tree.Insert(45);
    tree.Insert(35);

    tree.Insert(60);
    tree.Insert(70);
    tree.Insert(73);
    //tree.Delete(50);
    cout << tree.IsBST();*/
    deque<int> q {50,20,15,45,35,60,70,73};
    deque<int>res{};
  //  BST* t2 = BST::CreateBST_PreOrder_3(q);
    //t2->GetPreOrder(res);
    //cout << res << endl;
    //t2->PrintInOrder();
   // BST t4({50,20,60,15,45,70,35,73});
    //t4.PrintInOrder();
   /* BST* t5 = BST::CreateBalancedBST({5,10});
    t5->Delete(10);
    t5->PrintInOrder();*/
    BinarySearchTree t6{};
    t6.insert(50);
    t6.insert(20);
    t6.insert(15);
    t6.insert(45);
    t6.insert(35);
    t6.insert(60);
    t6.insert(70);
    t6.insert(73);
    t6.PrintInOrder();
    t6.Delete(60);
    t6.PrintInOrder();
    t6.Delete(50);
    t6.PrintInOrder();
    return 0;
}