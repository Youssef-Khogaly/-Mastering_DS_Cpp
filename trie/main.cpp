#include <bits/stdc++.h>


using namespace std;

constexpr int MAX_CHAR = 26;

struct node{
    
    char data;
    bool isLeaf;
    std::map<char,node*>childs; // AVL tree 
    node(char data = '\0' , bool IsCompleteWord = false) : data(data),childs({}) , isLeaf(IsCompleteWord)
    {
    }
};

class trie
{
    private:
    node* root;


    // get all childs string of this node
    void GetAllStrings_p(node* currNode ,vector<string>& result , string& currentString)
    {
        if(currNode->childs.size() == 0)
            return;
        for(auto & [data,n]: currNode->childs) // o(n^2)
        {
            currentString += data;
            if(n->isLeaf)
            {
                result.push_back(currentString);
            }
            GetAllStrings_p(n,result,currentString); // get all strings of this child
            currentString.pop_back(); // remove last char added since we got all his child string
            // example
            /*
                asume current string = ab
                and data = c 
                current += data = abc
                now get all child string for abc
                after return from recursion 
                pop back now current ab 
                next loop as example will start with abd ,, etc
            */
        }
    }
    public:
    trie() : root(new node)
    {}
    void insert(const string & str)
    {
        int idx = 0;
        node* curr = root;
        for(; idx < str.size(); ++idx)
        {
            if(curr->childs.count(str[idx]) == 0)
            {
                curr->childs[str[idx]] = new node(str[idx]);
            }
            curr = curr->childs[str[idx]];
        }
        curr->isLeaf = true;
    }
    bool wordExist(const string& str)
    {
        node* curr = root;
        int idx = 0;

        while(idx < str.size())
        {
            if(!curr->childs.count(str[idx]))
            {
                return false;
            }
            curr = curr->childs[str[idx]];
            ++idx;  
        }
        return curr->isLeaf;
    }
    bool prefixExist(const string & prefix)
    {
        node* curr = root;
        int idx = 0;

        while(idx < prefix.size())
        {
            if(!curr->childs.count(prefix[idx]) )
            {
                return false;
            }
            curr = curr->childs[prefix[idx]];
            ++idx;  
        }
        return true;
    }
    // return first word in the trie that my prefix
    string firstWord_prefix(const string & str)
    {
        node* curr = root;
        int idx = 0;
        string output = "";
        for(const auto& c : str)
        {
            if(!curr->childs.count(str[idx]))
            {
                break;
            }
            output += c;
            curr = curr->childs[c];
            if(curr->isLeaf)
                break;
        }
        return (curr->isLeaf) ? output : "";
    }
    void GetAllStrings(vector<string>& result )
    {
        string curr{""};
        GetAllStrings_p(root,result, curr);
    }
    vector<string>AutoComplete(const string & str)
    {
        node* curr = root;
        vector<string>res{};
        for(const auto & c : str) // o(n)
        {
            if(curr->childs.count(c) == 0)
            {
                // str is not a prefix of any string
                return res;
            }

            curr = curr->childs[c];
        }
        // at the end of the loop
        // curr will point to the node of the last char of str
        // 
        // now get all strings of current childs
        string currentStr = str;
        res.push_back(str);
        GetAllStrings_p(curr,res,currentStr); // o(n^2)

        return res;
    }
    bool word_exist_with_one_change( string  str)
    {
        for(auto& s : str)
        {
            auto copy = s;
            for(char i = 'a' ; i <= 'z' ; ++i)
            {

                if(i == s)
                    continue;
                s = i;
                if(wordExist(str))
                {
                    return true;
                }
                s = copy;
            }
        }

        return false;
    }
};

namespace path
{
    struct node{
    
        string data;
        bool isLeaf;
        std::map<string,node*>childs;
        node(string data = string{} , bool IsCompleteWord = false) : data(data),childs({}) , isLeaf(IsCompleteWord)
        {
        }
    };
    
    class trie
    {
        private:
        node* root;
    
        public:
        trie() : root(new node)
        {}
        void insert(const vector<string> & strs)
        {
            int idx = 0;
            node* curr = root;
            while(idx < strs.size())
            {
                if(curr->childs.count(strs[idx]) == 0)
                {
                    curr->childs[strs[idx]] = new node(strs[idx]);
                }
                curr = curr->childs.at(strs[idx]);
                ++idx;
            }
            curr->isLeaf = true;
        }
        bool PathExist(const vector<string> & path)
        {
            node* curr = root;
            int idx = 0;
    
            while(idx < path.size())
            {
                if(!curr->childs.count(path[idx]))
                {
                    return false;
                }
                curr = curr->childs[path[idx]];
                ++idx;  
            }
            return curr->isLeaf;
        }
        bool SubPathExist(const vector<string> & subPath)
        {
            node* curr = root;
            int idx = 0;
    
            while(idx < subPath.size())
            {
                if(!curr->childs.count(subPath[idx]) )
                {
                    return false;
                }
                curr = curr->childs[subPath[idx]];
                ++idx;  
            }
            return true;
        }
    };
}

class reverseTrie
{
    private:
    node* root;

    public:
    reverseTrie() : root(new node)
    {}
    void insert(const string & str)
    {
        int idx = str.size()-1;
        node* curr = root;
        while(idx >= 0)
        {
            if(!curr->childs[str[idx]-'a'])
            {
                curr->childs[str[idx]-'a'] = new node(str[idx]);
            }
            curr = curr->childs[str[idx]-'a'];
            --idx;
        }
        curr->isLeaf = true;
    }
    bool wordExist(const string& str)
    {
        node* curr = root;
        int idx = str.size()-1;

        while(idx < str.size())
        {
            if(!curr->childs[str[idx]-'a'])
            {
                return false;
            }
            curr = curr->childs[str[idx]-'a'];
           --idx;
        }
        return curr->isLeaf;
    }
    bool SuffixExist(const string & prefix)
    {
        node* curr = root;
        int idx = prefix.size()-1;

        while(idx < prefix.size())
        {
            if(!curr->childs[prefix[idx]-'a'])
            {
                return false;
            }
            curr = curr->childs[prefix[idx]-'a'];
            --idx;
        }
        return true;
    }
    // return first word in the trie that my prefix
    string firstWord_Suffix(const string & str)
    {
        node* curr = root;
        int idx = str.size()-1;
        string output = "";
        for(; idx >= 0 ; idx--)
        {
            auto& c = str[idx];
            if(!curr->childs[c-'a'])
            {
                break;
            }
            output += c;
            curr = curr->childs[c-'a'];
            if(curr->isLeaf)
                break;
        }
        return (curr->isLeaf) ? output : "";
    }

};


// print all query words that are sub str;
void list_subStrs(const string& str , vector<string>& query) // o (n^2)time , o(M) memory , m number of strings in query
{

    int queryWordSize = 0;
    trie tree;
    string temp{};
    // add all query words to the trie 
    // and get the max string size in the qury
    for(const auto & q : query) // o(Q*L) time , o(n)memory
    {
        queryWordSize = std::max(queryWordSize,(int)q.size());
        tree.insert(q);
    }

    //o(S*L)in case of max size of query srings == size of str
    for(int i = 0 ; i < str.size() - queryWordSize ; ++i)
    {
        temp = "";
        for(int j = 0 ; j < queryWordSize ; ++j)
        {
            temp += str[i+j];
            if(tree.wordExist(temp))// o(L) , L size of string
            {
                cout << temp <<'\n';
            }
        }
    }   
}

// trie based on str solution
void list_subStrs_2(const string& str , vector<string>& query) 
{

    int queryWordSize = 0;
    trie tree;
    string temp{};
    // add all query words to the trie 
    // and get the max string size in the qury
    for(const auto & q : query)
    {
        queryWordSize = std::max(queryWordSize,(int)q.size());
    }
    for(int i = 0 ; i < str.size() ; ++i) // o(n*M) , n str size , m max string length in query , o(n*m) memory
    {
        temp = "";
        for(int j = 0 ; j < queryWordSize && i+j < str.size() ; ++j)
        {
            temp += str[i+j];
            tree.insert(temp);
        }
    }
    for(const auto& q : query) 
    {
        if(tree.wordExist(q))
            cout << q << '\n';
    }
}
/*
    both solution o(n^2) time , but first one more effiecient in memory o(n) , other o(n^2)
*/


bool isSuffix(const string & str , const string & suffix)
{
    if(suffix.size() > str.size())
        return false;
    auto sIt = str.rbegin();
    auto suffixIt = suffix.rbegin();
    
    for(; suffixIt != suffix.rend() ; suffixIt++ , sIt++)
    {
        if(*suffixIt != *sIt)
            return false;
    }
    return true;
}






namespace prefix_suffixSearch
{
    struct node{
    
        char data;
        bool isLeaf;
        std::map<char,node*>childs; // AVL tree 
        list<int>indicies;
        node(char data = '\0' , bool IsCompleteWord = false) : data(data),childs({}) , isLeaf(IsCompleteWord) , indicies{list<int>{}}
        {
        }
    };
    
    class trie
    {
        private:
        node* root;
    
    
        // get all childs string of this node
        void GetAllStrings_p(node* currNode ,vector<string>& result , string& currentString)
        {
            if(currNode->childs.size() == 0)
                return;
            for(auto & [data,n]: currNode->childs) // o(n^2)
            {
                currentString += data;
                if(n->isLeaf)
                {
                    result.push_back(currentString);
                }
                GetAllStrings_p(n,result,currentString); // get all strings of this child
                currentString.pop_back(); // remove last char added since we got all his child string
                // example
                /*
                    asume current string = ab
                    and data = c 
                    current += data = abc
                    now get all child string for abc
                    after return from recursion 
                    pop back now current ab 
                    next loop as example will start with abd ,, etc
                */
            }
        }
        public:
        trie() : root(new node)
        {}
        void insert(const string & str , int idices)
        {
            int idx = 0;
            node* curr = root;
            for(; idx < str.size(); ++idx)
            {
                if(curr->childs.count(str[idx]) == 0)
                {
                    curr->childs[str[idx]] = new node(str[idx]);
                }
                curr->childs[str[idx]]->indicies.push_back(idices);
                curr = curr->childs[str[idx]];
            }
            curr->isLeaf = true;
        }
        bool wordExist(const string& str)
        {
            node* curr = root;
            int idx = 0;
    
            while(idx < str.size())
            {
                if(!curr->childs.count(str[idx]))
                {
                    return false;
                }
                curr = curr->childs[str[idx]];
                ++idx;  
            }
            return curr->isLeaf;
        }
        bool prefixExist(const string & prefix)
        {
            node* curr = root;
            int idx = 0;
    
            while(idx < prefix.size())
            {
                if(!curr->childs.count(prefix[idx]) )
                {
                    return false;
                }
                curr = curr->childs[prefix[idx]];
                ++idx;  
            }
            return true;
        }
        // return first word in the trie that my prefix
        string firstWord_prefix(const string & str)
        {
            node* curr = root;
            int idx = 0;
            string output = "";
            for(const auto& c : str)
            {
                if(!curr->childs.count(str[idx]))
                {
                    break;
                }
                output += c;
                curr = curr->childs[c];
                if(curr->isLeaf)
                    break;
            }
            return (curr->isLeaf) ? output : "";
        }
        void GetAllStrings(vector<string>& result )
        {
            string curr{""};
            GetAllStrings_p(root,result, curr);
        }
        vector<string>AutoComplete(const string & str)
        {
            node* curr = root;
            vector<string>res{};
            for(const auto & c : str) // o(n)
            {
                if(curr->childs.count(c) == 0)
                {
                    // str is not a prefix of any string
                    return res;
                }
    
                curr = curr->childs[c];
            }
            // at the end of the loop
            // curr will point to the node of the last char of str
            // 
            // now get all strings of current childs
            string currentStr = str;
            res.push_back(str);
            GetAllStrings_p(curr,res,currentStr); // o(n^2)
    
            return res;
        }
        bool word_exist_with_one_change( string  str)
        {
            for(auto& s : str)
            {
                auto copy = s;
                for(char i = 'a' ; i <= 'z' ; ++i)
                {
    
                    if(i == s)
                        continue;
                    s = i;
                    if(wordExist(str))
                    {
                        return true;
                    }
                    s = copy;
                }
            }
    
            return false;
        }
        list<int> GetAllindicies(const string & prefix)
        {
            node* curr = root;
            int idx = 0;
    
            while(idx < prefix.size())
            {
                if(!curr->childs.count(prefix[idx]) )
                {
                    return list<int>{-1};
                }
                curr = curr->childs[prefix[idx]];
                ++idx;  
            }

            return curr->indicies;
        }
    
    };
}

// generate all the 
/*

    we will generate all suffix and prefix of every word in dictionary
    but it doesn't make sense to generate prefixes
    since trie is already a prefix tree
    so we will generate every suffix and add it to the word with a serpator

    example
    word ABCD -> CD$ABCD
                suffix$word
                D$ABCD
                ,,etc



    another solution
    make a trie of all prefixes
    make a trix of all suffixes

    get indicies of prefixes
    get indicies of suffixes

    return the intersection

*/
void GenerateAllSuffix_prefix(const vector<string>& dictionary,prefix_suffixSearch::trie& output)
{
    string suffix;
    for(int i = dictionary.size()-1 ; i >= 0 ; --i)
    {
        suffix = "";
        for(int j = dictionary[i].size()-1 ; j >= 0 ; --j)
        {
                suffix =  dictionary[i][j] + suffix;
                output.insert(suffix + '$' + dictionary[i],i);
        }
    }


}


int f_2(const string& prefix , const string & suffix)
{
    std::vector<string>dic{"aae","apple","bannana"};
    prefix_suffixSearch::trie tree;
    GenerateAllSuffix_prefix(dic,tree);
    auto&& allIdx = tree.GetAllindicies(suffix+'$'+prefix);
    return allIdx.front();
}
int f(string prefix , string suffix)
{
    map<string , int> m {{"aae",0} , {"apple",1},{"bannana",2}};
    trie t;
    for(const auto & [str,num] : m)
        t.insert(str);

    vector<string> allprefix = t.AutoComplete(prefix); // o(n^2)
    int lastValid = -1;
    // check each one of them  is valid
    for( auto  prefix = allprefix.begin() ; prefix != allprefix.end() ; prefix++) //o(m)
    {
        if(m.count(*prefix) != 0)   
        {
            if(isSuffix(*prefix,suffix)) // o(L)
                lastValid = distance(allprefix.begin(),prefix);
        }

    }
    // total o(n^2 + m*l) too bad !!
    if(lastValid != -1)
        return m[allprefix[lastValid]];

    return -1;
}









int main()
{
    /*trie t;
    t.insert("xyz");
    t.insert("xyzea");
    t.insert("a");
    t.insert("bc");
    cout << t.firstWord_prefix("x") << endl;
    cout << t.firstWord_prefix("xyzabc");
    */

   /* path::trie tree;
    vector<string> path = {"home","software","eclipse"};
    tree.insert(path);
    path = {"home","software","eclipse","bin"};
    tree.insert(path);
    path = {"home","installed","gnu"};
    tree.insert(path);
    path = {"user","mostafa","tmp"};
    tree.insert(path);

    path = {"user","mostafa","tmp"};
    cout << tree.SubPathExist(path) << endl;//1
    path = {"user","mostafa"};
    cout << tree.SubPathExist(path) << endl;//1
    path = {"user","most"};
    cout << tree.SubPathExist(path) << endl; //0
    path = {"user","mostafa","private"};
    cout << tree.SubPathExist(path) << endl; // 0
    */
  /* trie t;
   t.insert("abcd");
   t.insert("ab");
   t.insert("abx");
   t.insert("abyz");
   t.insert("xyz");
   t.insert("a");
   t.insert("bcd");
   vector<string>res;
   res = t.AutoComplete("ab");
   for(auto& str :res)
        cout << str << endl;*/

    /*trie tree;
    tree.insert("hello");
    tree.insert("leetcode");
    cout << tree.word_exist_with_one_change("hello")<<endl; // false
    cout << tree.word_exist_with_one_change("xello")<<endl; // true
    cout << tree.word_exist_with_one_change("xyllo")<<endl; // false
    */
  // vector<string> q {"xy","ab","t","yz"};
   //list_subStrs_2("heyabcdtwxyw",q);

   cout << f_2("a","e") << endl; // 1
   cout << f_2("a","x") << endl; // -1
   cout << f_2("a","ae")<<endl; // 0
    return 0;
}