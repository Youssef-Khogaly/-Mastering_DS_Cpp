#include <bits/stdc++.h>


using namespace std;


class BinaryHeap
{
    private:
    struct node{
        int data;
        node*left;
        node*right;
        explicit node(int data , node* left = nullptr , node* right = nullptr) : data(data),left(left),right(right){}
    };
    node* root;

    public:
    BinaryHeap() : root(nullptr){}

    void insert(int val)
    {
        if(!root)
        {
            root = new node(val);
            return;
        }
        else
        {
            node* firstLeafNode = nullptr;
            queue<node*>q;
            q.push(root);
            int sz;
            node* curr;
            while(!q.empty())
            {
                int sz = q.size();

                while(sz--)
                {
                    curr = q.front();
                    q.pop();

                    if(!curr->left && !curr->right)
                    {
                        if(!firstLeafNode)
                            firstLeafNode = curr;
                    }
                    else if (!curr->left || !curr->right)
                    {
                        if(!curr->left)
                            curr->left = new node (val);
                        else
                            curr->right = new node(val);

                        return;
                    }
                    else // complete node push children into the queue
                    {
                        q.push(curr->left);
                        q.push(curr->right);
                    }
                }
            }
            firstLeafNode->left = new node(val);
        }
    }

};


struct comp
{
    public:
    bool operator ()(const int& x , const int& y) const
    {
        return (x >= y);
    }
};

class MinHeap
{
    private:
    int capacity;
    int size;
    int* arr;
    
    int left(int i )
    {
        int pos = (i*2)+1;
        return (pos >= size) ? -1 : pos;
    }
    int right(int i )
    {
        int pos = (i*2)+2;
        return(pos >= size) ? -1 : pos;
    }
    int parent(int i)
    {
        return (i == 0 )? -1 : (i-1)/2;
    }

    void Heapify_up(int child_pos) // log(n)
    {
        if(child_pos == 0)
            return;

        int parentPos = parent(child_pos);
        while(parentPos >= 0  && arr[child_pos] < arr[parentPos])
        {
            swap(arr[child_pos] ,arr[parentPos]);
            child_pos = parentPos;
            parentPos = parent(child_pos);
        }
    }
    void Heapify_down(int parentPos) // log(n)
    {
        if(size <= 1)
            return;
        int left_idx = left(parentPos);
        int right_idx = right(parentPos);
        if(left_idx == -1 ) // leaf node
            return;


        if(right_idx == -1 ||  arr[left_idx] < arr[right_idx] )
        {
            if(arr[parentPos] > arr[left_idx])
            {
                swap(arr[parentPos] ,arr[left_idx]);
                return Heapify_down(left_idx);
            }

        }
        else if(arr[right_idx] < arr[left_idx]  )
        {
            if( arr[parentPos] > arr[right_idx])
            {
                swap(arr[parentPos] ,arr[right_idx]);
                return Heapify_down(right_idx);
            }
                
        }

    }
    void Heapify()
    {
        for(int i = size/2 -1 ; i >= 0 ; --i)// o(n)
            Heapify_down(i);
    }
    void printLessThan_p(int i ,int val)
    {
        if(i == -1 || i >= size)
            return;
        if(arr[i] >= val)
            return;
        else
        {
            cout << arr[i] << " ";
            printLessThan_p(left(i) , val);
            return printLessThan_p(right(i),val);
        }
    }
    bool isHeap_p()
    {
        /*no need to check for completeness since it represented as array*/

        int i = 0;
        int left_pos = left(i);
        int right_pos = right(i);
        // loop till reach first leaf node then stop
        while(i <  size/2 )
        {
            if(left_pos != -1 && arr[i] >= arr[left_pos])
                return false;
            if (right_pos != -1 &&  arr[i] >= arr[right_pos])
                return false;
            ++i;
            left_pos = left(i);
            right_pos = right(i);
            
        }       
        return true;
    }
    MinHeap(int*arr , int n ,bool heapifiy = true) : capacity(n) ,size(n),arr(arr)
    {
        if(heapifiy)
            Heapify();
    }
    public:
    MinHeap() :capacity(100) ,size(0) , arr(nullptr){}
    ~MinHeap()
    {
        if(arr)
        {
            delete arr;
            arr = nullptr;
            size =  capacity = 0;
        }
        
    }
    MinHeap(const vector<int>& vec) :capacity(100) , size(vec.size()) , arr(nullptr)
    {
        if(vec.size() > capacity)
            capacity += (capacity-vec.size())+10;
        arr = new int[capacity];
        copy(begin(vec),end(vec),arr);

        Heapify();
    }
    
    bool isEmpty()
    {
        return (size == 0);
    }
    bool isFull()
    {
        return (size == capacity);
    }
    int push(int key)
    {
        assert(!isFull());

        arr[size++] = key;
        Heapify_up(size-1);
    }
    int top_peak()
    {
        assert (!isEmpty());
        return arr[0];
    }
    void pop()
    {
        assert (!isEmpty());
        arr[0] = arr[size-1];
        --size;
        Heapify_down(0);

    }
    void print()
    {
        for(int i = 0 ; i < size ; ++i)
        {
            cout <<arr[i]<<' ';
        }
        cout << '\n';
    }
    void printLessThan(int val)
    {
        if(isEmpty())
            return;
        printLessThan_p(0,val);
        cout << endl;
    }
    bool isHeap()
    {
        if(isEmpty())
            return true;
        return isHeap_p();
    }
    bool isHeap(int* arr , int n)
    {
        MinHeap heap(arr,n ,false);
        bool ret = heap.isHeap();
        heap.arr = nullptr;
        return ret;
    }
    void HeapSort(int* arr , int n)
    {
        MinHeap heap(arr,n , true);
        for(; !heap.isEmpty() ; )
        {
            int min = heap.top_peak();
            heap.pop();
            heap.arr[heap.size] = min;    
        }
        for(int i = 0  , j = n-1 ; i < j ; ++i,--j)
        {
            swap(heap.arr[i],heap.arr[j]);
        }
        /*
            so the MinHeap constructor will not delete array
            since its user pointer not ours
        */
        heap.arr = nullptr;
        return;
    }

};



template<typename T>
class MaxHeap
{
    private:
    int cap;
    int size;
    T* arr;

    int left(int i)
    {
        int pos = (2*i)+1;
        return (pos >= size) ? -1 : pos;
    }
    int right(int i )
    {
        int pos = (2*i)+2;
        return (pos >= size) ? -1 : pos;
    }
    int parent(int i)
    {
        return (i == 0) ? -1 : (i-1)/2;
    }
    void Heapifiy_up(int i)
    {
        int parent_pos = parent(i);
        
        while(parent_pos>=0 && arr[i] >= arr[parent_pos])
        {
            swap(arr[i] , arr[parent_pos]);
            i = parent_pos;
            parent_pos = parent(i);
        }
    }
    void Heapifiy_down(int i )
    {
        int left_pos = left(i);
        int right_pos = right(i);
        // leaf node
        if(left_pos == -1 && right_pos == -1)
            return;
        
        if(right_pos != -1 && arr[right_pos] > arr[left_pos])
        {
            // current element is lower
            if(arr[i] < arr[right_pos])
            {
                swap(arr[i] , arr[right_pos]);
                return Heapifiy_down(right_pos);
            }
        }
        else // doesn't have left or the left is bigger than right
        {
            if(arr[i] < arr[left_pos])
            {
                swap(arr[i],arr[left_pos]);
                return Heapifiy_down(left_pos);
            }
        }
    }
    void heapifiy()
    {
        for(int i = (size/2)-1 ; i >= 0 ; --i)
        {
            Heapifiy_down(i);
        }
    }
    public:
    MaxHeap() : cap(100) , size(0) , arr(new T [cap]){}
    ~MaxHeap(){
        if(arr)
        {
            delete arr;
            arr = nullptr;
            size = cap = 0;
        }
    }
    MaxHeap(const vector<T>& vec): cap(100),size(vec.size()),arr(nullptr)
    {
        if(vec.size() >= cap)
        {
            cap = cap + (vec.size()-cap) + 10;
        }
        arr = new int [cap];

        copy(begin(vec),end(vec),arr);
        heapifiy();

    }
    bool isEmpty()
    {
        return (size == 0);
    }
    bool isFull()
    {
        return (size >= cap);
    }   
    T top()
    {
        assert (!isEmpty());
        return arr[0];
    }
    void push(T val)
    {
        if(isFull())
        {
            cap *= 2;
            T* newArr = new T [cap];
            copy(arr , arr+size,newArr);
            delete arr;
            arr = newArr;
        }
        arr[size++] = val;
        Heapifiy_up(size-1);
    }
    void pop()
    {
        if(isEmpty())
            return;
        arr[0] = arr[--size];
        Heapifiy_down(0);
    }
    void print()
    {
        for(int i = 0 ; i < size ; ++i)
            cout <<arr[i] <<" ";
        cout << endl;
    }
};

struct task{
    int id;
    int priority;
    task() : id(-1) , priority(-1){}
    task(int id , int priority) : id(id) , priority(priority){}

    bool operator < (const task & other) const{
        return (priority < other.priority);
    }
    bool operator > (const task & other) const{
        return (priority > other.priority);
    }
    bool operator == ( const task & other) const
    {
        return (priority == other.priority);
    }
    bool operator <= (const task& other) const
    {
        return ((*this < other) || (*this == other));
    }
    bool operator >= (const task& other) const
    {
        return ((*this > other) || (*this == other));
    }
};
ostream& operator << (ostream& out , const task & t)
{
    out << t.id <<" ";
    return out;
}
class PriorityQueue
{
    private:
    MaxHeap<task> que;

    public:
    PriorityQueue() : que(MaxHeap<task>{})
    {}
    void enqueue(const task & task_)
    {
        que.push(task_);
    }
    void enqueue(int id , int priority)
    {
        enqueue({id,priority});
    }
    task dequeue()
    {
        task ret = que.top();
        que.pop();
        return ret;
    }
    bool isEmpty()
    {
        return que.isEmpty();
    }
};

class MaxHeap_2
{
    private:
    MinHeap heap;

    vector<int> multipyNegative(vector<int>vec)
    {
        for_each(begin(vec),end(vec),[](int & x) { x*=-1;});
        return vec;
    }
    public:
    MaxHeap_2() : heap(){}
    MaxHeap_2(const vector<int> & vec) : heap(multipyNegative(vec))
    {

    }
    int top()
    {
        return -heap.top_peak();
    }
    int pop()
    {
        heap.pop();
    }
    void push(int val)
    {
        heap.push(-val);
    }
};

class KthNumberProcessor
{
    private:
    int k;
    priority_queue<int> que;
    public:
    KthNumberProcessor(int k ) : k(k) 
    {}
    /*
        we need to get kth smallest element

        the idea is to use max heap
        so if we can keep our heap size <= k
        we can easily get kth smallest, always will be MaxHeap top

    */
    int next(int newNumber)
    {
        if(que.size() < k)
        {
            que.push(newNumber);
        }
        // if the new element > than max? don't add it
        // we only add if the new element is less than our max
        // so we delete our max and add the new element
        else if (newNumber < que.top())
        {
            que.pop(); //log(n)
            que.push(newNumber); //log(n)

        }
       return que.top(); // o(1)
    }
};

int main()
{
    PriorityQueue tasks{};
    tasks.enqueue(1131,1);
    tasks.enqueue(3111,3);
    tasks.enqueue(2211,2);
    tasks.enqueue(3161,3);
    tasks.enqueue(7761,7);
    cout << tasks.dequeue() << endl;
    cout << tasks.dequeue() << endl;
    tasks.enqueue(1535,1);
    tasks.enqueue(2815,2);
    tasks.enqueue(3845,3);
    tasks.enqueue(3145,3);
    while(!tasks.isEmpty())
    {
        cout << tasks.dequeue() << " ";
    }
    return 0;
}