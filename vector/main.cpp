#include <bits/stdc++.h>


using namespace std;


class Vector
{
    public:
    Vector() : size(0) , capacity(0) , internal(0){

    }
    explicit Vector(int size) : size(size) , capacity(size*2) , internal(new (nothrow) int[size])
    {
        assert(size >= 0);
        assert(internal != nullptr);
    }
    explicit Vector(const std::initializer_list<int> &list) : size(list.size()) ,capacity(list.size() * 2) , internal(new (nothrow) int[capacity])
    {
        assert(internal != nullptr);
        copy(list.begin() , list.end() , internal);
    } 
    int get(int idx)const
    {
        assert(idx >= 0 && idx < size);
        return internal[idx];
    }
    void set(int idx , int val)
    {
        assert(idx >= 0 && idx < size);
        internal[idx] = val;
    }
    int find(int val) const
    {
        for(int i = 0 ; i < size ; ++i)
        {
            if(internal[i] ==  val)
                return i;
        }

        return -1;
    }
    void print() const 
    {
        for(int i =0 ; i < size ; ++i)
            cout << internal[i] << ((i+1 == size)? "\n" : ",");
    }

    void pushBack(int val)
    {
        if(size == capacity)
            expandCapacity();
        internal[size++] = val;

    }
    void insert(int idx , int val)
    {
        assert(idx >= 0 && idx < size);
        this->pushBack(val);
        for(int i = size - 1 ; i > idx ; --i)
            swap(internal[i] , internal[i-1]);

    }
    void reserve(int cap)
    {
        assert(cap > 0 && cap > size);
        int* newInternal = new int[cap];
        copy(internal , internal+size , newInternal);
        capacity = cap;
        delete [] internal;
        internal = newInternal;
    }
    void shrinkToFit()
    {
        int* newInternal = new int[size];
        copy(internal , internal+size , newInternal);
        delete [] internal;
        internal = newInternal;
        capacity = size;
    }
    int getFront() const
    {
        return internal[0];

    }
    int getBack() const
    {
        return internal[size-1];
        
    }
    void RightRotation()
    {
        int i = size -1;
        int temp = internal[size-1];
        for(; i > 0 ; --i)
            internal[i] = internal[i-1];
        internal[0] = temp;
    }
    void LeftRotation()
    {
        int temp = internal[0];
        int i = 0;
        for(;i < size-1 ; ++i)
            internal[i] = internal[i+1];
        internal[size-1] = temp;
    }
    void RightRotation(int k)
    {
        k %= size;
        for(int i = 0 ; i < k ; ++i)
            this->RightRotation();
    }
    int pop(int idx)
    {
        assert(idx >= 0 && idx < size);
        int i = idx;
        int ret = internal[idx];
        for(; i < size -1  ; ++i)
        {
            internal[i]  = internal[i+1];
        }
        --size;

        return ret;
    }
    int find_transpositin(int val)
    {
        int idx = find(val);
        if(idx <= 0)
            return idx;
        swap(internal[idx] , internal[idx-1]);
        return --idx;
    }
    private:
    int size;
    int capacity;
    int * internal;

    void expandCapacity()
    {
        capacity = size +10;
        int* newInternal = new int[capacity];
        copy(internal , internal+size , newInternal);
        delete [] internal;
        internal = newInternal;
    }
};


int main()
{
    Vector v({0,1,2,3,4});
    std::cout<<v.find_transpositin(66) << endl;
    v.print();
    return 0;
}