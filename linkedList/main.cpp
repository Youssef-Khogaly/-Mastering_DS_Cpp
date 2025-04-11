#include <bits/stdc++.h>


using namespace std;


typedef struct  node
{
    int data;
    struct node * next;
    explicit node(int data , node* next = nullptr) :data(data) , next(next){
    }
    node() : data(0) , next(nullptr){

    }
    ~node()
    {
        cout <<"~node , d="<<data<<endl;
    }
}node;
class List
{

    public:
    List() : root(nullptr) , tail(nullptr)
    {
        
    }
     List(const std::initializer_list<int> & list)
    {
        root = new node(*list.begin());
        node* temp = root;
        auto bg = list.begin() + 1;
        auto end = list.end();
        while(bg != end)
        {
            temp->next = new node(*bg);
            temp = temp->next;
            bg++;
        }
        tail = temp;
    }
    List(const List &) = delete;
    List& operator =(const List&) = delete;
    void debug_verifiy_list()const
    {
        if(root == nullptr)
            assert(tail == nullptr);
        else
        {
            assert(root != nullptr);
            assert(tail != nullptr);

            if(root == tail)
                assert(root->next != nullptr);
            
            assert(tail->next == nullptr);
        }
        // check for cycle;
        int len = 0;
        for(node* temp = root ; temp ; temp = temp->next , ++len)
            assert(len < 1000);
    }
    string debug_to_str()const
    {
        ostringstream oss("");
        for(node* temp = root ; temp ; temp = temp->next)
            oss << temp->data<<((temp->next) ? "," : "");
        
        return oss.str();
    }
    void insert_front(int data)
    {
        node* newNode = new node(data , root);
        root = newNode;
        if(root->next == nullptr)
            tail = root;

    }
    void append(int data)
    {
        if(root == nullptr)
        {
            root =  tail = new node(data);
        }
        else
        {
            tail->next = new node(data);
            tail = tail->next;
        }
    }
    void append(const std::initializer_list<int> & list)
    {
        for(auto & it : list)
            append(it);
    }
    int PopBack()
    {
        if(root == nullptr)
            throw std::runtime_error("list is empty");
        
        node* temp = root;
        int data = tail->data;
        if(root == tail)
        {
            free(root);
            root = nullptr;
            tail = nullptr;
        }
        else
        {
            for(;temp->next != tail; temp = temp->next)
                ;
            temp->next = nullptr;
            delete tail;
            tail = temp;
        }

        return data;
    }
    int GetFront()
    {
        if(root == nullptr)
            throw std::runtime_error("list is empty");
        int data = root->data;

        if(root == tail)
        {
            delete root;
            root = tail = nullptr;
        }
        else
        {
            node* temp = root;
            root = root->next;
            delete temp;
        }

        return data;
    }
    node* getNth(int n)
    {
        int i = 1;
        node* temp = root;
        for(; i < n && temp != nullptr ; ++i , temp = temp->next)
            ;

        return temp;

    }
    node* getNth_Back(int n)
    {
        if(!root)
            return NULL;
        int len = 1;
        for(node* temp = root ; temp ; ++len,temp = temp->next )
            ;
        n = len - n ;
        return getNth(n);

    }
    // return idx
    int search(int val){
        node* temp = root;
        int i = 0;
        for(; temp  ; temp = temp->next , i++)
        {
            if(temp->data == val)
                return i;
        }
        return -1;
    }
    int searchWithShift(int val)
    {
        if(root == nullptr)
            return -1;
        if(root->data == val)
            return 0;
        else if (nullptr == root->next)
            return -1;
        else if(root->next->data == val)
        {
            node* temp = root->next;
            swap(root->next , temp->next);
            root = temp;
            return 0;
        }
        node* prev = root;
        node* temp = root->next->next;
        int i = 2;
        while(temp)
        {
            if(temp->data == val)
            {
                prev->next->next = temp->next;
                temp->next = prev->next;
                prev->next = temp;
                if(temp == tail)
                {
                    tail = prev;
                }

                return i-1;
            }
            prev = prev->next;
            temp = temp->next;
        }

        return -1;
    }
    void print() const
    {
            node* temp = root;
            while(temp != nullptr)
            {
                std::cout << temp->data << ((temp->next) ? "," :"\n");
                temp = temp->next;
            }
    }
    bool is_same(const List & another)const
    {
        node* head1 = root;
        node* head2 = another.root;

        while(head1 && head2)
        {
            if(head1->data != head2->data)
                return false;
            head1=head1->next;
            head2=head2->next;
        }
        if(!head1 && !head2) // both null then equal
            return true;

        return false; // else
    }
    void Delete(int val)
    {
        if(!root)
            return;

        node* prev = nullptr;
        node* temp = root;

        while(temp)
        {
            if(temp->data == val)
            {
                if(prev != nullptr) // not the root
                {
                    prev->next = temp->next;
                    if(tail == temp)
                        tail = prev;
                    free(temp);
                    break;
                }
                else // root 
                {
                    node* temp = root;
                    if(root == tail)
                    {
                        root = tail = nullptr;
                    }
                    else
                    {
                        root = root->next;
                    }
                    free(temp);
                    break;
                }
            }
            prev = temp;
            temp = temp->next;
        }
    }
    void swap_pairs()
    {
        if(!root || root->next == NULL)
            return;
        node* f = root;
        node* s;

        while(f && f->next)
        {
            s = f->next;
            swap(f->data , s->data);
            f = f->next->next;
        }
    }

    void reverse()
    {
        if(!root)
            return;
        if(root == tail)
            return;
        node* prev = nullptr;
        node* curr = root;
        node* next = curr->next;
        tail = root;
        while(curr)
        {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        root = prev;
    }
    void deleteEvenPos()
    {
        if(!root || root == tail)
            return;
        
        node* prev = root;
        node* curr = root->next;
        while(curr)
        {
            prev->next = curr->next;
            delete curr;
            if(!prev->next) // tail
                break;
            prev = prev->next;
   
            curr = prev->next;
        }
        tail = prev;
    }
    void insert_Sorted(int val)
    {
        if(!root || val < root->data)
            return insert_front(val);
        if(val >= tail->data)
            return append(val);
        
        node* curr = root;
        node* newNode = new node(val);
        for(; curr ; curr = curr->next)
        {
            if(val >= curr->data && val < curr->next->data)
            {
                newNode->next = curr->next;
                curr->next = newNode;
                break;
            }
        }
    }
    void swap_head_tail()
    {
        if(!root || root == tail)
            return;
        
        node* prevTail = root;
        while(prevTail->next != tail)
            prevTail = prevTail->next;

        prevTail->next = root;
        tail->next = root->next;
        root->next = nullptr;
        swap(root, tail);

    }
    void removeNode(node* prev)
    {
        node* temp = nullptr;
        // remove root
        if(prev == nullptr)
        {
            temp = root;
            root = root->next;
            delete temp;
            if(root == nullptr)
                tail = root;
        }
        else
        {
            temp = prev->next;
            prev->next = temp->next;
            delete temp;
            if(prev->next == nullptr)
                tail = prev;
        }
    }
    void leftRotate(int k)
    {
        if(!root || root == tail || k < 1)
            return;
        int len = 0;
        node* temp = root;
        for(; temp ; ++len , temp = temp->next)
            ;
        k %= len;

        /*while(k--)
        {
            tail->next = root;
            root = root->next;
            tail = tail->next;
        }*/
       if(k == 0)
        return;

        node* nth = getNth(k);
        tail->next = root;
        root = nth->next;
        tail = nth;
        tail->next = nullptr;
    }
    void removeDuplicates()
    {
        if(!root || root == tail)
            return;

        vector<int> exists{root->data};
        node* prev = root;
        node* curr = root->next;
        while(curr)
        {
            if(find(begin(exists) , end(exists) , curr->data) != exists.end())
            {
                removeNode(prev);
                curr = prev->next;
            }
            else
            {
                exists.push_back(curr->data);
                prev = curr;
                curr = curr->next;
            }
        }
    }
    void removeLastOccurance(int key)
    {
        if(!root)
            return;
        node* lastOccurancePrev = nullptr;
        node* prev = nullptr;
        node* curr = root;
        bool isFound = false;
        while(curr)
        {
            if(curr->data == key)
            {
                isFound = true;
                lastOccurancePrev = prev;
            }
            prev = curr;
            curr = curr->next;
        }
        if(isFound)
            removeNode(lastOccurancePrev);
    }
    void moveToBack(int key)
    {
        if(!root || root  == tail)
            return;
        node* tempTail = tail;
        node* prev = nullptr;
        node* curr = root;
        while(curr != tempTail)
        {
            if(curr->data == key)
            {
                if(nullptr != prev)
                {
                    prev->next = curr->next;
                    curr->next = nullptr;
                    tail->next = curr;
                    tail = curr;
                    curr = prev->next;
                }
                else
                {
                    root = root->next;
                    curr->next = nullptr;
                    tail->next = curr;
                    tail = curr;
                    curr = root;
                } 
            }
            else
            {
                prev = curr;
                curr = curr->next;
            }
        }
    }
    int max()
    {
        if(!root)
            return INT_MIN;
        node* temp = root;
        int Max = root->data;
        root = root->next;
        Max = std::max(Max , max());
        root = temp;
        return Max;
    }
    void ArrangeOdd_Even()
    {
        /*
            10 20 3 7 15 
            o  e  o e          
            10 3 15

            10 20 3 7
                  o e
        */
        if(!root || root == tail || root->next->next == nullptr)
            return;
        
        node* odd = root;
        node* even = root->next;
        node* evenStart = even;
        while(even->next)
        {
            odd->next = odd->next->next;
            even->next = even->next->next;
            odd = odd->next;
            if(even->next == nullptr)
                break;
            else
                even = even->next;
        }
        odd->next = evenStart;
        tail = even;
    }
    void insertAlternating(const List& another)
    {
        if(!root)
        {
            node* temp = another.root;
            while(temp)
            {
                append(temp->data);
                temp = temp->next;
            }
            return;
        }
        node* curr = root;
        node* next = root->next;
        node* temp = another.root;
        while(temp)
        {
            next = curr->next;
            curr->next = new node(temp->data ,next);
            temp = temp->next;
            if(next != nullptr)
            {
                curr = next;
            }
            else
            {
                tail = curr->next;
                break;
            }
            
        }
        while(temp)
        {
            append(temp->data);
            temp = temp->next;
        }
        
    }
    void InsertAlternating_move(List&& another)
    {
        if(!root)
        {
            root = another.root;
            tail = another.tail;
            another.root = another.tail = nullptr;
        }
        node* curr = root;
        node* curr2 = another.root;
        node* next2;
        while(curr && curr2)
        {
            next2 = curr2->next;
            curr2->next = curr->next;
            curr->next = curr2;
            curr = curr->next->next;
            if(!curr)
            {
                tail = curr2;
            }
            curr2 = next2;
        }
        /*
            incase another list > self list
        */
        tail->next = curr2;
        if(curr2 != nullptr)
            tail = another.tail;
        another.root = another.tail = nullptr;
    }
    /*
        each list represent a number
        example 157 as linkedlist > 7->5->1

        its add another number to its current values
    */
    void add_number(List&& another)
    {
        int rest = 0;
        int temp;
        node* curr1 = root;
        node* curr2 = another.root;
        for(; curr1 && curr2 ; curr1 = curr1->next , curr2 = curr2->next)
        {
            temp = curr1->data + curr2->data + rest;
            rest = temp/10;
            curr1->data = temp % 10;
        }
        while(curr2)
        {
            temp = curr2->data + rest;
            rest = temp / 10;
            append(temp%10);
            curr2 = curr2->next;
        }
        while(rest)
        {
            if(curr1 != nullptr)
            {
                temp = curr1->data + rest;
                rest = temp / 10;
                curr1->data = temp % 10;
                curr1  = curr1->next;
            }
            else
            {
                append(rest %10);
                rest /= 10;
            } 
        }
    }
    void RemoveAllRepeatedSorted()
    {
        if(!root || root == tail)
            return;
        /*
        
            we can
        */
        node* unique = nullptr; // to save last unique node
        node* temp = root; // to traverse
        root = tail = nullptr; // to make it easier to update root , tail in case of deleteing all the list
        // next loop will take care of root and tail
        while(temp) // o(n)
        {
            if(temp->next && temp->data == temp->next->data)
            {
                int data = temp->data;
                // skip all till next unique number
                while(temp && temp->data == data)
                {
                    node* next = temp->next;
                    delete temp;
                    temp = next;
                }
            }
            else
            {
                // unique number since the next element not equal it while its sorted list
                if(!unique) // if unique is null update root
                {
                    root = temp;
                    unique = temp;
                    tail = temp;
                }
                else
                {
                    unique->next = temp;
                    unique = temp; // keep last unique number updated to link
                    tail = unique; 
                }
                temp = temp->next;
            }
        }
        if(tail)
            tail->next = nullptr;
    }
    void reverseChain(int k) // we asume  k is always <= len
    {
        if(!root || root == tail || k<=1)
            return;
        node* prev = nullptr;
        node* curr = root;
        node* next = root->next;
        node* chainTail = nullptr;// current chain tail
        node* prevChainTail = nullptr; // the previous chain tail
        int i = 0;
        bool isFirstLoop = true;
        while(curr != nullptr)
        {
            for(i = 0 ; i < k && curr ; ++i)
            {
                if(!i)
                    chainTail = curr;
                next = curr->next;
                curr->next = prev;
                prev = curr;
                curr = next;
            }
            if(isFirstLoop)
            {
                root = prev;
                isFirstLoop = false;
            }
            chainTail->next = curr;
            if(prevChainTail != nullptr)
                prevChainTail->next = prev;
            prevChainTail = chainTail;
        }
        tail = prevChainTail;

    }
    ~List()
    {
        cout <<"~list\n";
        if(root)
        {
            node* temp = root;
            node* next = root->next;
            for(; temp ; temp = next)
            {
                next = temp->next;
                free(temp);
                temp = nullptr;
            }

            root = tail =nullptr;
        }
    }
    private:
    node* root;
    node* tail;
};

int main()
{
    List l{1,2,3,4,5,6};
    l.reverseChain(7);
    l.print();

    cout <<"NO RTE \n";
    return 0;
}