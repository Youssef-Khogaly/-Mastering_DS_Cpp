#include <bits/stdc++.h>


using namespace std;


int hash_num(int value , int n = 139)
{
    // but still we have alot of collision can happen

    // +n % n to handle negative values
    // but still this line will fail in overflow
    return (value % n + n) %n;
}

int hashString(const string & str , int n)
{
    unsigned long long int sum = 0;
    unsigned long long int n2 = n;
    for(const auto& c : str)
    {
        sum = (sum * 26 + c - 'a')%n; // here sum can overflow if string is too long  so we apply modules 
    }
    // +n2 %n2 to handle negative value if sum over flowed
    return (sum%n2);
}


int hashString_lower_upper_digits(const string & str , int n)
{
    unsigned long long int sum = 0;
    unsigned long long int n2 = n;
    unsigned long long int val = 0;
    unsigned long long int base = 2 * 26 +10; // 26 lower + 26 upper + 10 digits


    for(const auto& c : str)
    {
        // lower
        if(c >= 'a' && c <= 'z')
        {
            val = c - 'a';
        }
        else if (c >= 'A' && c <= 'Z')
            val = 26 + c-'A';
        else if (c >='0' && c<='9')
            val = 26 + 26 + c - '0';
        else
            assert(false);
        sum = sum * base + val; // multiply * base to shift
        sum %= n2; // to minimize over flow 
    }
    
    return (sum);
}
int flodHashing(const string & str , int n)
{
    unsigned long long int hashsum = 0;
    unsigned long long int n2 = n;
    for(int i = 0 ; i < str.size() ; i+=4)
    {
        hashsum +=  hashString_lower_upper_digits(str.substr(i,4) , n);
        hashsum %= n2;
    }

    return hashsum;
}

class someObject
{
    public:
    const static int INTERNAL_LIMIT = INT_MAX;
    string str1;
    string str2;
    int number;
    int hash()
    {
        unsigned long long int hash1 = flodHashing(str1 , INTERNAL_LIMIT);
        unsigned long long int hash2 = flodHashing(str2 , INTERNAL_LIMIT);
        unsigned long long int hash3 = flodHashing(std::to_string(number) , INTERNAL_LIMIT);

        return ((hash1 + hash2 )+(hash3 % INTERNAL_LIMIT))%INTERNAL_LIMIT;
    }
};


class phoneEntity
{
    public:
    string name;
    string number;

    phoneEntity(string name , string number) : name(std::move(name)),number(std::move(number))
    {

    }
};

class phoneHashTable
{
    private:
    int tableSize;
    const int limitFactor;
    vector<vector<phoneEntity>>table;
    int addedElemets = 0;
    int hash(const string & key)
    {
        return flodHashing(key,tableSize);
    }

    void rehashing()
    {
        phoneHashTable newHash(2*tableSize , limitFactor);

        for(const auto & vec : table)
        {
            for(const auto & entity : vec)
            {
                newHash.insert(entity);
            }
        }
        tableSize = newHash.tableSize;
        addedElemets = newHash.addedElemets;
        table = std::move(newHash.table);
    }
    public:
    phoneHashTable(int tableSize = 10 , double limitFactor = 0.75) : tableSize(tableSize) , limitFactor(limitFactor),table(tableSize)
    {
        
    }
    void insert(const phoneEntity & _phoneEntity)
    {
        ++addedElemets;
        table[hash(_phoneEntity.name)].push_back(_phoneEntity);

        // current load factor addedelement/talbesize 
        if((double)addedElemets/tableSize > limitFactor)
            rehashing();
    }
};




class linerProbing
{
    private:

    
    const int tableSize;
    double limitFactor;
    int addedElements{0};
    vector<phoneEntity*> table;
    phoneEntity* const  deleted = new phoneEntity(" "," ");
    public:

    linerProbing(int tableSize , double limitFactor = 0.75) : tableSize(tableSize),limitFactor(limitFactor) , table(vector<phoneEntity*>(tableSize,0))
    {}
    bool insert(const string & key , const string & value)
    {
        unsigned long long hash = flodHashing(key,tableSize);
        unsigned long long idx = 0;
        for(int i = 0 ; i < tableSize ; ++i)
        {
            idx = (hash+i)%tableSize;
            if(!table[idx]  || table[idx] == deleted)
            {
                table[idx] = new phoneEntity{key,value};
                return true;
            }
            else if (table[idx]->name == key)
            {
                table[idx]->number = value;
                return true; // update
            }
        }
        return false;
    }
    bool Delete(const string & key)
    {
        unsigned long long hash = flodHashing(key,tableSize);
        unsigned long long idx = 0;
        for(int i = 0 ; i < tableSize ; ++i)
        {
            idx = (hash+i)%tableSize;
            if(!table[idx]) // empty
            {
                break;
            }
            else if (table[idx] != deleted && table[idx]->name == key)
            {
                delete table[idx];
                table[idx] = deleted;
                return true;
            }
        }

        return false;
    }
    bool exist(phoneEntity & phone)
    {
        unsigned long long hash = flodHashing(phone.name,tableSize);
        unsigned long long idx = 0;
        for(int i = 0 ; i < tableSize ; ++i)
        {
            idx = (hash+i)%tableSize;
            if(!table[idx]) // empty
            {
                return false;
            }
            else if (table[idx] != deleted && table[idx]->name == phone.name)
            {
                phone.number = table[idx]->number;
                return true;
            }
        }
        return false;
    }
    void printall()
    {
        for(int i = 0 ; i < tableSize ; ++i)
        {
            cout << i <<"-";
            if(!table[i]) // empty
            {
                cout << "e ";
            }
            else if (table[i] == deleted)
            {
                cout <<"x ";
            }
            else
                cout << table[i]->name <<"," <<table[i]->number;

            cout << endl;
        }
        cout <<"----------------------------";
    }
};


int countAlluniqueSubStr(const string & str)
{

    unordered_set<string> m{}; 
    // generate all substring o(n^3) 
    for(int i = 0 ; i < str.size() ; ++i)
    {
        for(int  j = 1 ; i+j <= str.size() ; ++j)
        {
            auto&& temp = str.substr(i,j); //o(1)
            if(m.count(temp) == 0)
            {
                m.insert(std::move(temp)); //o(1)
            }
        }
    }
    /*
    
        we can use trie to get more efficient solution 
        and sum number of every node childs

        example
        aaab
        a->a->a->b
        3  2  1  0
        will be o(l)memory and o(l) time since we just need to iterate over every node and sum
        return 3+2+1+0 +1
    */
    return m.size();
}

void GenerateUniqueSubStrs(const string & str , unordered_set<string> &s)
{
    // generate all substring o(n^2) , o(n^2)memory
    for(int i = 0 ; i < str.size() ; ++i)
    {
        for(int  j = 1 ; i+j <= str.size() ; ++j)
        {
            auto&& temp = str.substr(i,j); //o(1)
            if(s.count(temp) == 0)
            {
                s.insert(std::move(temp)); //o(1)
            }
        }
    }
}
void GenerateAllSubStrs(const string & str , unordered_multiset<string>& s )
{
    // generate all substring o(n^2) , o(n^2)memory
    for(int i = 0 ; i < str.size() ; ++i)
    {
        for(int  j = 1 ; i+j <= str.size() ; ++j)
        {
            auto&& temp = str.substr(i,j); //o(1)
            s.insert(std::move(temp)); //o(1)
            
        }
    }
}
int count_subString_match(const string & str1 , const string & str2)
{
    unordered_set<string> s1{};
    unordered_set<string> s2{}; 
    GenerateUniqueSubStrs(str1,s1); // o(l^2)
    GenerateUniqueSubStrs(str2,s2); // o(l^2)
    int counter = 0;

    for(const auto & subStr: s1)
    {
        counter += s2.count(subStr);
    }
    cout << endl;
    return counter;
}

// hash string and don't include char position
int hash_str(const string & str , unsigned long long int n)
{
    unsigned long long int sum = 0;
    for(const auto& c : str)
    {
        sum += (c);
        sum %= n;
    }

    return (sum);
}

void StringSort(string & str)
{
    char count[128]{};

    for(const auto & c : str)
    {
        count[c]++;
    }

    auto it = str.begin();

    for(int i = 0 ; i < 128 ; ++i)
    {
        while(count[i] != 0)
        {
            *it = i;
            count[i]--;
            it++;
        }
    }
}
unsigned long long int count_unique_anagrams_2(const string & str)
{
    unordered_set<string> s1{};

    for(int i = 0 ; i < str.size();++i)
    {
        for(int j = i ; j < str.size() ; ++j)
        {
            string temp = str.substr(i,j-i+1);
            StringSort(temp); //o(l)
            s1.insert(temp); // o(l)
        }
    }
    // total o(l^3)
    return s1.size();
}
int main()
{
    /*
    linerProbing table(11);
    table.insert("mostafa","604-401-120");
    table.insert("mostafa","604-401-777");
    table.insert("ali","604-401-343");
    table.insert("ziad","604-401-17");
    table.insert("hany","604-407-758");
    table.insert("belal","604-401-550");
    table.insert("john","604-401-223");
    table.printall();   
    cout << table.Delete("smith") << endl; // 0
    cout << table.Delete("hany") << endl ; // 1
    cout << table.Delete("john") << endl ; // 1
    table.printall();

    cout << std::hash<string>{}("youssef") << endl;
    */

    cout << countAlluniqueSubStr("aaab")<<endl; // 7
    cout << countAlluniqueSubStr("aaaaa")<<endl; //5
    cout << countAlluniqueSubStr("aaaba")<<endl;//11
    cout << countAlluniqueSubStr("abcdef")<<endl;//21

    cout << "---------------------------------------------------\n";
    cout << count_subString_match("aaab","aa") << endl;//2
    cout << count_subString_match("aaab","ab") << endl;//3
    cout << count_subString_match("aaaaa","xy") << endl;//0
    cout << count_subString_match("aaaaa","aaaaa") << endl;//5

    cout <<"-------------------------------------------\n";
    cout << count_unique_anagrams_2("aaaaa") << endl;//5
    cout << count_unique_anagrams_2("abcba") << endl;//9
    cout << count_unique_anagrams_2("abba") << endl;//6
    return 0;
}