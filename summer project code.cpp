// MyString — custom deep-copy string class (compact edition)
// Compile: g++ -std=c++17 -o mystring MyString.cpp
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <limits>
#include <vector>
using namespace std;

const string RESET="\033[0m",BOLD="\033[1m",GREEN="\033[32m",YELLOW="\033[33m",
             RED="\033[31m",CYAN="\033[36m",GRAY="\033[90m",DIM="\033[2m";

class MyString {
    char* data; size_t len;
    static char* alloc(size_t n) { char* p = new char[n+1]; p[n]='\0'; return p; }
    explicit MyString(size_t n) : data(alloc(n)), len(n) {} // internal: buffer-only ctor
public:
    static const size_t npos = (size_t)-1;
    MyString() : data(alloc(0)), len(0) {}
    MyString(const char* s) : data(alloc(s?strlen(s):0)), len(s?strlen(s):0) { if(s) memcpy(data,s,len); }
    MyString(size_t count, char ch) : data(alloc(count)), len(count) { memset(data,ch,len); }
    MyString(const MyString& o) : data(alloc(o.len)), len(o.len) { memcpy(data,o.data,len); }       // deep copy
    MyString(MyString&& o) noexcept : data(o.data), len(o.len) { o.data=nullptr; o.len=0; }         // steal buffer
    MyString& operator=(const MyString& o) {
        if (this==&o) return *this;
        char* nd = alloc(o.len); memcpy(nd,o.data,o.len);
        delete[] data; data=nd; len=o.len; return *this;
    }
    MyString& operator=(MyString&& o) noexcept {
        if (this==&o) return *this;
        delete[] data; data=o.data; len=o.len; o.data=nullptr; o.len=0; return *this;
    }
    ~MyString() { delete[] data; }

    size_t length() const { return len; }
    size_t size() const { return len; }
    bool empty() const { return len==0; }
    const char* c_str() const { return data; }
    char& operator[](size_t i) { return data[i]; }
    const char& operator[](size_t i) const { return data[i]; }
    char& at(size_t i) { if(i>=len) throw out_of_range("MyString::at index out of range"); return data[i]; }
    const char& at(size_t i) const { if(i>=len) throw out_of_range("MyString::at index out of range"); return data[i]; }

    MyString operator+(const MyString& r) const {
        MyString res(len+r.len);
        memcpy(res.data,data,len); memcpy(res.data+len,r.data,r.len);
        return res;
    }
    MyString& operator+=(const MyString& r) {
        char* nd = alloc(len+r.len);
        memcpy(nd,data,len); memcpy(nd+len,r.data,r.len);
        delete[] data; data=nd; len+=r.len; return *this;
    }
    bool operator==(const MyString& r) const { return len==r.len && memcmp(data,r.data,len)==0; }
    bool operator!=(const MyString& r) const { return !(*this==r); }
    bool operator<(const MyString& r) const { return strcmp(data,r.data)<0; }
    bool operator>(const MyString& r) const { return strcmp(data,r.data)>0; }

    MyString substr(size_t start, size_t count=npos) const {
        if (start>len) throw out_of_range("MyString::substr start out of range");
        size_t n = (count==npos || start+count>len) ? (len-start) : count;
        MyString res(n); memcpy(res.data,data+start,n); return res;
    }
    size_t find(const MyString& needle, size_t startPos=0) const {
        if (needle.len==0) return startPos<=len ? startPos : npos;
        if (startPos>=len) return npos;
        const char* found = strstr(data+startPos, needle.data);
        return found ? (size_t)(found-data) : npos;
    }
    size_t find(char ch, size_t startPos=0) const {
        for (size_t i=startPos;i<len;++i) if(data[i]==ch) return i;
        return npos;
    }
    friend ostream& operator<<(ostream& os, const MyString& s) { return os << (s.data?s.data:""); }
    friend istream& operator>>(istream& is, MyString& s) { char b[1024]; is>>b; s=MyString(b); return is; }
};

// ---- input helpers ----
static void flush() { cin.ignore(numeric_limits<streamsize>::max(),'\n'); }
static char readChar(const string& p) { cout<<YELLOW<<p<<RESET; char c; cin>>c; flush(); return c; }
static size_t readSize(const string& p) { cout<<YELLOW<<p<<RESET; size_t n; cin>>n; flush(); return n; }
static string readLine(const string& p) { cout<<YELLOW<<p<<RESET; char b[1024]; cin.getline(b,1024); return b; }
static MyString& slotRef(char slot, MyString& a, MyString& b) { return (slot=='B'||slot=='b') ? b : a; }
static void header(const string& t) { cout<<"\n"<<CYAN<<BOLD<<"== "<<t<<" =="<<RESET<<"\n\n"; }
static void div_() { cout<<GRAY<<string(50,'-')<<RESET<<"\n"; }

int main() {
    static const vector<pair<string,string>> items = {
        {"1","Create/overwrite a string (slot A or B)"}, {"2","Display both slots"},
        {"3","Get length of A and B"},                   {"4","Concatenate (A + B)"},
        {"5","Compare A and B"},                          {"6","Access character by index"},
        {"7","Get substring"},                            {"8","Find substring / character"},
        {"9","Copy A into B (deep copy)"},                {"0","Exit"},
    };
    cout<<CYAN<<BOLD<<"\n  MyString CONSOLE APPLICATION - v2.0\n"<<RESET<<"  [Your College Name], CSE Dept\n";
    MyString strA, strB;
    int choice;
    while (true) {
        cout<<"\n"<<CYAN<<"A = \""<<strA<<"\"  B = \""<<strB<<"\""<<RESET<<"\n\n";
        for (auto& [k,d] : items) cout<<(k=="0"?RED:GREEN)<<"  ["<<k<<"] "<<RESET<<d<<"\n";
        cout<<GREEN<<"\n> "<<RESET;

        if (!(cin>>choice)) { cout<<RED<<"Invalid input.\n"<<RESET; cin.clear(); flush(); continue; }
        flush();

        try {
            switch (choice) {
                case 1: {
                    header("CREATE / OVERWRITE STRING");
                    char slot = readChar("Which slot? (A/B): ");
                    string s = readLine("Enter the string  : ");
                    MyString& t = slotRef(slot,strA,strB);
                    t = MyString(s.c_str());
                    cout<<GREEN<<"[OK] Slot "<<(char)toupper(slot)<<" set to: "<<RESET<<t<<"\n";
                    break;
                }
                case 2:
                    header("DISPLAY STRINGS");
                    cout<<"A = \""<<strA<<"\"\nB = \""<<strB<<"\"\n"; div_();
                    break;
                case 3:
                    header("STRING LENGTHS");
                    cout<<CYAN<<"Length of A: "<<BOLD<<strA.length()<<RESET<<"\n";
                    cout<<CYAN<<"Length of B: "<<BOLD<<strB.length()<<RESET<<"\n"; div_();
                    break;
                case 4:
                    header("CONCATENATION (A + B)");
                    cout<<GREEN<<"A + B = "<<BOLD<<(strA+strB)<<RESET<<"\n"; div_();
                    break;
                case 5:
                    header("COMPARE A AND B");
                    if (strA==strB) cout<<GREEN<<"A and B are EQUAL.\n"<<RESET;
                    else cout<<YELLOW<<"A comes "<<(strA<strB?"BEFORE":"AFTER")<<" B (lexicographically).\n"<<RESET;
                    div_();
                    break;
                case 6: {
                    header("ACCESS CHARACTER BY INDEX");
                    char slot = readChar("Which slot? (A/B): ");
                    size_t idx = readSize("Index            : ");
                    MyString& t = slotRef(slot,strA,strB);
                    cout<<GREEN<<"Character at index "<<idx<<" = '"<<t.at(idx)<<"'\n"<<RESET; div_();
                    break;
                }
                case 7: {
                    header("GET SUBSTRING");
                    char slot = readChar("Which slot? (A/B): ");
                    size_t start = readSize("Start index      : ");
                    size_t count = readSize("Count            : ");
                    MyString& t = slotRef(slot,strA,strB);
                    cout<<GREEN<<"Substring = "<<BOLD<<t.substr(start,count)<<RESET<<"\n"; div_();
                    break;
                }
                case 8: {
                    header("FIND SUBSTRING / CHARACTER");
                    char slot = readChar("Search in which slot? (A/B): ");
                    string needle = readLine("Enter text to find          : ");
                    MyString& t = slotRef(slot,strA,strB);
                    size_t pos = t.find(MyString(needle.c_str()));
                    if (pos==MyString::npos) cout<<RED<<"Not found.\n"<<RESET;
                    else cout<<GREEN<<"Found at index "<<BOLD<<pos<<RESET<<"\n";
                    div_();
                    break;
                }
                case 9:
                    header("COPY A INTO B (DEEP COPY)");
                    strB = strA;
                    cout<<GREEN<<"[OK] Copied A into B. B is now: "<<BOLD<<strB<<RESET<<"\n";
                    cout<<DIM<<"(A and B now hold separate buffers with equal content)\n"<<RESET; div_();
                    break;
                case 0:
                    cout<<"\n"<<YELLOW<<BOLD<<"Thank you for using the MyString application.\n"<<RESET;
                    return 0;
                default:
                    cout<<RED<<"Invalid choice. Please select 0-9.\n"<<RESET;
            }
        } catch (const out_of_range& ex) {
            cout<<RED<<"Error: "<<ex.what()<<"\n"<<RESET; div_();
        }
    }
}
