#include <bits/stdc++.h>


using namespace std;


template<class T>
struct node
{
    size_t pos;
    T data;
    
    struct node<T>* prev;
    struct node<T>* next;
    
     node(int pos ,const T& data , node<T>* prev = nullptr,node<T>* next = nullptr) : pos(pos),data(data) , prev(prev),next(next){
    }
    node(const node & another,node<T>* prev = nullptr, node<T>* next = nullptr) : pos(another.pos),data(another.data) , prev(prev),next(next){
    }
    ~node() = default;
};


template<class T , size_t SIZE>
class ArrayLinkedList
{
    typedef node<T> node;
    public:
    ArrayLinkedList() : head(nullptr) , tail(nullptr)
    {
        assert(SIZE != 0);
    }
    
    ArrayLinkedList(const  ArrayLinkedList & another)
    {
        node* temp = another.head;
        while(temp)
        {
            insertEnd(*temp);
            temp = temp->next;
        }
    }
    virtual ~ArrayLinkedList()
    {
        if(head)
        {
            node* temp;
            while(head)
            {
                temp = head;
                head = head->next;
                delete temp;
            }
            head = tail = nullptr;
        }
    }
    void Set(size_t pos , const T& val)
    {
        InsertSorted(pos,val);
    }
    /*
        return T{} if pos doesn't have a value
    */
   T Get(size_t pos) const
   {
       assert(pos < SIZE);
       if(!head || pos < head->pos || pos > tail->pos)
           return T{};
       
       node* s = head;
       node* e = tail;

       for(; s != e ; s = s->next,e = e->prev)
       {
           if(pos == s->pos)
               return s->data;
           else if (pos == e->pos)
               return e->data;
           
           if(s->next == e)
               break;
       }

       return T{};
   }
    
    void PrintArray() const
    {
        size_t i = 0;
        node* temp = head;
        for(; i < SIZE ; ++i)
        {
            if(temp && i == temp->pos)
            {
                cout << temp->data << " ";
                temp = temp->next;
            }
            else
                cout <<"0 ";
        }
        cout << "\n";
    }
    void PrintArray_NonZero() const
    {
        node* temp = head;
        for(; temp ; temp = temp->next)
            cout << temp->data << " ";
        cout << "\n";
    }
    void add(const ArrayLinkedList & another)
    {
        if(another.head == nullptr)
            return;

        node* curr = head;
        node* curr2 = another.head;

        while(curr && curr2)
        {
            if(curr->pos == curr2->pos)
            {
                curr->data  += curr2->data;
                curr = curr->next;
                curr2 = curr2->next;
            }
            // if self list pos > curr2 pos 
            // since list is sorted then there is will be curr2 pos in curr2 list
            else if ( curr->pos > curr2->pos)
            {
                InsertBefore(curr , curr2->pos , curr2->data);
                curr2 = curr2->next;
            }
            else
            {
                curr = curr->next;
            }
        }
        
        for(; curr2 ; curr2 = curr2->next)
        {
            insertEnd(curr2->pos,curr2->data);
        }
    }
        /*
        if doesn't exist will create new node and return refrence to data 
    */

    protected:
    node* head;
    node* tail;
    
    node* insertEnd(size_t pos , const T & data)
    {
        if(!tail)
        {
            tail = new node(pos,data);
            head = tail;
        }
        else
        {
            tail->next = new node(pos,data,tail,nullptr);
            tail = tail->next;
        }
        return tail;
    }
    /*
        copy node data and insert
    */
   node* insertEnd(const node& n)
    {
        if(!tail)
        {
            tail = new node(n);
            head = tail;
        }
        else
        {
            tail->next = new node(n,tail,nullptr);
            tail = tail->next;
        }
        return tail;
    }
    node* insertFront(size_t pos , const T & data)
    {
        if(!head)
        {
            head = new node(pos,data);
            tail = head;
        }
        else
        {
            head->prev = new node(pos,data,nullptr,head);
            head = head->prev;
        }
        return head;
    }
    /*
        copy node data and insert
    */
    node* insertFront(const node & n)
    {

        if(!head)
        {
            head = new node(n);
            tail = head;
        }
        else
        {
            head->prev = new node(n,nullptr,tail);
            head = head->prev;
        }
        return head;
    }

    node* InsertBefore(node* curr , size_t pos , const T & val)
    {
        if(!head || curr == head)
           return insertFront(pos,val);
   
       node* temp = new node(pos,val);
       curr->prev->next = temp;
       temp->prev = curr->prev;
       curr->prev = temp;
       temp->next = curr;
       return temp;
    }
    node* InsertBefore(node* curr , const node & n)
    {
        if(!head || curr == head)
           return insertFront(n);
        
       node* temp = new node(n);
       curr->prev->next = temp;
       temp->prev = curr->prev;
       curr->prev = temp;
       temp->next = curr;
       return temp;
    }
    node* InsertSorted(size_t pos , const T & val)
    {
        assert(pos < SIZE);
  
        if(!head || pos < head->pos)
            return insertFront(pos,val);
        else if (pos > tail->pos)
            return insertEnd(pos,val);
  
        node* curr = head;
        for( ; pos > curr->pos; curr = curr->next )
            ;
  
        if(curr->pos == pos)
        {
          curr->data = val;
        }
        else
          curr = InsertBefore(curr,pos,val);
        
        return curr;
    }
    T& GetNodeData(size_t pos)
    {
        assert(pos < SIZE);
 
        if(!head || pos < head->pos)
        {
            return insertFront(pos,T{})->data;
        }
        else if ( pos > tail->pos)
        {
            return insertEnd(pos,T{})->data;
        }
        else if (pos == tail->pos)
            return tail->data;
 
        node* curr = head;
 
 
        for(;curr  && curr->pos != pos ; curr = curr->next)
        {
            if(curr->pos > pos)
                return InsertBefore(curr,pos,T{})->data;
        }
        
        return curr->data;
    }

};


template<class T , size_t ROWS , size_t COLS>
class SparseMatrix : protected ArrayLinkedList<ArrayLinkedList<T,COLS>,ROWS>
{
    using MatCol = ArrayLinkedList<T,COLS>;
    using Matrix =  ArrayLinkedList<MatCol,ROWS>;
    using node = node<MatCol>;
    
    public:
   
    SparseMatrix() 
    {

    }
    void Set(size_t row , size_t col , const T& val )
    {
        
        MatCol& l = (*this).GetNodeData(row);
        l.Set(col,val);

    }
    T Get(size_t row , size_t col)
    {
        MatCol& l = (*this).GetNodeData(row);
        return l.Get(col);
    }
    void PrintMat()const
    {
        node* temp =  (*this).head;
        size_t i = 0;
        MatCol&& emptyCol = MatCol{};
        for(; i < ROWS ; ++i)
        {
            if(temp && i == temp->pos)
            {
                temp->data.PrintArray();
                temp = temp->next;
            }
            else
                emptyCol.PrintArray();
        }
    }
    void PrintMat_NonZero()const
    {
        node* temp =  (*this).head;

        for(; temp ; temp = temp->next)
        {
            temp->data.PrintArray_NonZero();
        }
    }
    void add(const SparseMatrix & another)
    {
        if(another.head == nullptr)
            return;

        node* curr = (*this).head;
        node* curr2 = another.head;

        while(curr && curr2)
        {
            if(curr->pos == curr2->pos)
            {
                curr->data.add(curr2->data);
                curr = curr->next;
                curr2 = curr2->next;
            }
            // if self list pos > curr2 pos 
            // since list is sorted then there is will be curr2 pos in curr2 list
            else if ( curr->pos > curr2->pos)
            {
                (*this).InsertBefore(curr , curr2->pos , curr2->data);
                curr2 = curr2->next;
            }
            else
            {
                curr = curr->next;
            }
        }
        
        for(; curr2 ; curr2 = curr2->next)
        {
            (*this).insertEnd(curr2->pos,curr2->data);
        }
    }
    private:
};
int main()
{
    SparseMatrix<int ,10,10> mat;
    mat.Set(3,5,5);
    mat.Set(3,7,7);
    mat.Set(3,2,2);
    mat.Set(3,2,0);
    mat.Set(5,6,6);
    mat.Set(5,4,4);
    mat.Set(7,3,3);
    mat.Set(7,1,1);
    mat.PrintMat();
    cout <<"----------------------------------\n";
    mat.PrintMat_NonZero();
    cout <<"----------------------------------\n";
    SparseMatrix<int ,10,10> mat2;
    mat2.Set(1,9,5);
    mat2.Set(3,8,6);
    mat2.Set(9,9,9);
    mat.add(mat2);
    mat.PrintMat_NonZero();
    return 0;
}