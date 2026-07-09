/*
 * ============================================================
 *  MyString — Custom Deep-Copy String Class
 *  [Your College Name] | CSE Department | Summer Project 2025-26
 * ============================================================
 *  Compile: g++ -std=c++17 -o mystring MyString.cpp
 *  Run:     ./mystring
 * ============================================================
 *
 *  WHY BUILD A CUSTOM STRING CLASS:
 *  - std::string hides its internals; this class exposes them.
 *  - Manages its own heap buffer (char*) instead of relying on
 *    the standard library's string implementation.
 *  - COPY  -> deep copy: new buffer, bytes duplicated. Two
 *             MyStrings never share memory.
 *  - MOVE  -> steal the pointer from a temporary, no copying.
 *  - DESTROY -> free the buffer exactly once.
 *
 *  COMPLEXITY:
 *  - operator[] / at()     : O(1)  — direct index into buffer
 *  - copy construction     : O(n)  — deep copy of n characters
 *  - move construction     : O(1)  — pointer swap only
 *  - operator+ / +=        : O(n+m)— allocate + copy both sides
 *  - find()                : O(n*m) worst case (uses strstr)
 *  NOTE: A production string class would add a capacity field
 *  for amortized O(1) appends (see docs/DESIGN.md).
 * ============================================================
 */

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <limits>

using namespace std;

// ==========================================
// ANSI COLOR CODES FOR UI STYLING
// ==========================================
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string RED     = "\033[31m";
const string CYAN    = "\033[36m";
const string BLUE    = "\033[34m";
const string GRAY    = "\033[90m";
const string DIM     = "\033[2m";

// ==========================================
// Utility: print a horizontal divider line
// ==========================================
void printDivider() {
    cout << GRAY << "  " << string(54, '-') << RESET << "\n";}
// ==========================================
// Utility: print a section header
// ==========================================
void printHeader(const string& title) {
    cout << "\n" << CYAN << BOLD << "  == " << title << " ==" << RESET << "\n\n";}
// ==========================================
// MyString — custom string class
// ==========================================
class MyString {
private:
    char* data;   // heap-allocated, null-terminated buffer
    size_t len;   // length, NOT counting the null terminator
    // ----------------------------------------------------------
    // allocateAndCopy() — allocates len+1 bytes and copies src
    // in (or leaves it empty if src == nullptr). O(n) time.
    // ----------------------------------------------------------
    void allocateAndCopy(const char* src, size_t length) {
        len = length;
        data = new char[len + 1];
        if (src) memcpy(data, src, len);
        data[len] = '\0';}
public:
    static const size_t npos = (size_t)-1;
    // ----------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------
    MyString() : data(nullptr), len(0) { allocateAndCopy(nullptr, 0); }

    MyString(const char* str) : data(nullptr), len(0) {
        allocateAndCopy(str, str ? strlen(str) : 0);}
    MyString(size_t count, char ch) : data(nullptr), len(0) {
        len = count;
        data = new char[len + 1];
        memset(data, ch, len);
        data[len] = '\0';}
    // ----------------------------------------------------------
    // Rule of Five — required because this class owns a raw
    // pointer. Without these, the compiler-generated versions
    // would copy the POINTER (shallow copy), causing double
    // frees and shared-buffer bugs.
    // ----------------------------------------------------------
    MyString(const MyString& other) : data(nullptr), len(0) {
        allocateAndCopy(other.data, other.len); }  // deep copy
    
    MyString(MyString&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;                      // steal, don't copy
        other.len = 0;}
    
    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;           // self-assign guard
        char* newData = new char[other.len + 1];     // allocate first
        memcpy(newData, other.data, other.len);
        newData[other.len] = '\0';
        delete[] data;                                // free only after success
        data = newData;
        len = other.len;
        return *this;}

    MyString& operator=(MyString&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        len = other.len;
        other.data = nullptr;
        other.len = 0;
        return *this;}
    
    ~MyString() { delete[] data; }
    // ----------------------------------------------------------
    // Accessors
    // ----------------------------------------------------------
    size_t length() const { return len; }
    size_t size() const { return len; }
    bool empty() const { return len == 0; }
    const char* c_str() const { return data; }
    // ----------------------------------------------------------
    // Element access — [] is fast/unchecked, at() is safe
    // ----------------------------------------------------------
    char& operator[](size_t index) { return data[index]; }
    const char& operator[](size_t index) const { return data[index]; }

    char& at(size_t index) {
        if (index >= len) throw out_of_range("MyString::at index out of range");
        return data[index];
    }
    const char& at(size_t index) const {
        if (index >= len) throw out_of_range("MyString::at index out of range");
        return data[index];}
    
    // ----------------------------------------------------------
    // Concatenation
    // ----------------------------------------------------------
    MyString operator+(const MyString& rhs) const {
        MyString result;
        delete[] result.data;
        result.len = len + rhs.len;
        result.data = new char[result.len + 1];
        memcpy(result.data, data, len);
        memcpy(result.data + len, rhs.data, rhs.len);
        result.data[result.len] = '\0';
        return result;}
    
    MyString& operator+=(const MyString& rhs) {
        char* newData = new char[len + rhs.len + 1];
        memcpy(newData, data, len);
        memcpy(newData + len, rhs.data, rhs.len);
        len += rhs.len;
        newData[len] = '\0';
        delete[] data;
        data = newData;
        return *this;}
    
    // ----------------------------------------------------------
    // Comparison
    // ----------------------------------------------------------
    bool operator==(const MyString& rhs) const {
        return len == rhs.len && memcmp(data, rhs.data, len) == 0;
    }
    bool operator!=(const MyString& rhs) const { return !(*this == rhs); }
    bool operator<(const MyString& rhs) const { return strcmp(data, rhs.data) < 0; }
    bool operator>(const MyString& rhs) const { return strcmp(data, rhs.data) > 0; }
    // ----------------------------------------------------------
    // Substring / search
    // ----------------------------------------------------------
    MyString substr(size_t start, size_t count = npos) const {
        if (start > len) throw out_of_range("MyString::substr start out of range");
        size_t actualCount = (count == npos || start + count > len) ? (len - start) : count;
        MyString result;
        delete[] result.data;
        result.len = actualCount;
        result.data = new char[actualCount + 1];
        memcpy(result.data, data + start, actualCount);
        result.data[actualCount] = '\0';
        return result;}
    
    size_t find(const MyString& needle, size_t startPos = 0) const {
        if (needle.len == 0) return startPos <= len ? startPos : npos;
        if (startPos >= len) return npos;
        const char* found = strstr(data + startPos, needle.data);
        return found ? (size_t)(found - data) : npos;}
    
    size_t find(char ch, size_t startPos = 0) const {
        for (size_t i = startPos; i < len; ++i)
            if (data[i] == ch) return i;
        return npos;}
    
    // ----------------------------------------------------------
    // Stream I/O
    // ----------------------------------------------------------
    friend ostream& operator<<(ostream& os, const MyString& s) {
        os << (s.data ? s.data : "");
        return os;}
    
    friend istream& operator>>(istream& is, MyString& s) {
        char buffer[1024];
        is >> buffer;
        s = MyString(buffer);
        return is;}
};
// ==========================================
// Display the main menu UI
// ==========================================
void displayMenu(const MyString& a, const MyString& b) {
    cout << "\n";
    cout << GRAY << "  +------------------------------------------------+\n" << RESET;
    cout << BLUE << "  |  __  __            ____  _        _            |\n" << RESET;
    cout << BLUE << "  | |  \\/  |_   _     / ___|| |_ _ __(_)_ __   __ _  " << RESET << "|\n" << RESET;
    cout << BLUE << "  | | |\\/| | | | |    \\___ \\| __| '__| | '_ \\ / _` | |\n" << RESET;
    cout << BLUE << "  | | |  | | |_| |     ___) | |_| |  | | | | | (_| | |\n" << RESET;
    cout << BLUE << "  | |_|  |_|\\__, |    |____/ \\__|_|  |_|_| |_|\\__, | |\n" << RESET;
    cout << BLUE << "  |         |___/                             |___/  |\n" << RESET;
    cout << GRAY << "  +------------------------------------------------+\n\n" << RESET;
    cout << GRAY << "  [Your College Name] . CSE Dept\n";
    cout << "  Summer Project 2025-26\n";
    cout << "  --------------------------------------------------\n\n" << RESET;

    cout << CYAN << BOLD << "  Slot A : " << RESET << CYAN << "\"" << a << "\"" << RESET << "\n";
    cout << CYAN << BOLD << "  Slot B : " << RESET << CYAN << "\"" << b << "\"" << RESET << "\n\n";

    cout << YELLOW << BOLD << "  MAIN MENU\n" << RESET;
    cout << GRAY   << "  --------------------------------------------------\n\n" << RESET;
    cout << GREEN  << "  [1] " << RESET << "Create / overwrite a string " << GRAY << " Set slot A or B\n" << RESET;
    cout << GREEN  << "  [2] " << RESET << "Display a string            " << GRAY << " Show both slots\n" << RESET;
    cout << GREEN  << "  [3] " << RESET << "Get length                  " << GRAY << " Length of A and B\n" << RESET;
    cout << GREEN  << "  [4] " << RESET << "Concatenate (A + B)         " << GRAY << " Join without modifying\n" << RESET;
    cout << GREEN  << "  [5] " << RESET << "Compare A and B             " << GRAY << " Equality / ordering\n" << RESET;
    cout << GREEN  << "  [6] " << RESET << "Access character by index   " << GRAY << " Bounds-checked at()\n" << RESET;
    cout << GREEN  << "  [7] " << RESET << "Get substring                " << GRAY << " start + count\n" << RESET;
    cout << GREEN  << "  [8] " << RESET << "Find substring / character  " << GRAY << " Search within a slot\n" << RESET;
    cout << CYAN   << "  [9] " << RESET << "Copy A into B                " << GRAY << " Deep copy demo\n" << RESET;
    cout << RED    << "  [0] " << RESET << "Exit                        " << GRAY << " Quit the program\n\n" << RESET;
    cout << GRAY   << "  --------------------------------------------------\n" << RESET;
    cout << GRAY   << "  * Slot A and Slot B each hold one MyString\n\n" << RESET;
    cout << GREEN  << "  -> " << GRAY << "Enter your choice and press Enter...\n" << RESET;
    cout << GREEN  << "\n  > " << RESET;}

// ==========================================
// main()
// ==========================================
int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "\n";
    cout << CYAN << BOLD;
    cout << "  ====================================================\n";
    cout << "  |         MyString CONSOLE APPLICATION - v1.0      |\n";
    cout << "  |         [Your College Name], CSE Dept            |\n";
    cout << "  ====================================================\n";
    cout << RESET << "\n";

    MyString strA;
    MyString strB;
    int choice;
    bool running = true;

    while (running) {
        displayMenu(strA, strB);

        if (!(cin >> choice)) {
            cout << RED << "  Invalid input. Please enter a number (0-9).\n" << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                printHeader("CREATE / OVERWRITE STRING");
                char slot;
                cout << YELLOW << "  Which slot? (A/B): " << RESET;
                cin >> slot;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << YELLOW << "  Enter the string  : " << RESET;
                char buffer[1024];
                cin.getline(buffer, 1024);

                if (slot == 'A' || slot == 'a') {
                    strA = MyString(buffer);   // copy assignment -> deep copy
                    cout << GREEN << "  [OK] Slot A set to: " << RESET << strA << "\n";
                } else if (slot == 'B' || slot == 'b') {
                    strB = MyString(buffer);
                    cout << GREEN << "  [OK] Slot B set to: " << RESET << strB << "\n";
                } else {
                    cout << RED << "  Unknown slot '" << slot << "'.\n" << RESET;
                }
                break;}
            
            case 2: {
                printHeader("DISPLAY STRINGS");
                cout << "  A = \"" << strA << "\"\n";
                cout << "  B = \"" << strB << "\"\n";
                printDivider();
                break;}
            
            case 3: {
                printHeader("STRING LENGTHS");
                cout << CYAN << "  Length of A : " << BOLD << strA.length() << RESET << "\n";
                cout << CYAN << "  Length of B : " << BOLD << strB.length() << RESET << "\n";
                printDivider();
                break;}
            
            case 4: {
                printHeader("CONCATENATION (A + B)");
                MyString result = strA + strB;   // new object; A and B untouched
                cout << GREEN << "  A + B = " << BOLD << result << RESET << "\n";
                printDivider();
                break;}
            
            case 5: {
                printHeader("COMPARE A AND B");
                if (strA == strB) {
                    cout << GREEN << "  A and B are EQUAL.\n" << RESET;
                } else if (strA < strB) {
                    cout << YELLOW << "  A comes BEFORE B (lexicographically).\n" << RESET;
                } else {
                    cout << YELLOW << "  A comes AFTER B (lexicographically).\n" << RESET;
                }
                printDivider();
                break;}
            
            case 6: {
                printHeader("ACCESS CHARACTER BY INDEX");
                char slot;
                size_t index;
                cout << YELLOW << "  Which slot? (A/B): " << RESET;
                cin >> slot;
                cout << YELLOW << "  Index            : " << RESET;
                cin >> index;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                try {
                    MyString& target = (slot == 'B' || slot == 'b') ? strB : strA;
                    cout << GREEN << "  Character at index " << index << " = '"
                         << target.at(index) << "'\n" << RESET;
                } catch (const out_of_range& ex) {
                    cout << RED << "  Error: " << ex.what() << "\n" << RESET;
                }
                printDivider();
                break;}
            
            case 7: {
                printHeader("GET SUBSTRING");
                char slot;
                size_t start, count;
                cout << YELLOW << "  Which slot? (A/B): " << RESET;
                cin >> slot;
                cout << YELLOW << "  Start index      : " << RESET;
                cin >> start;
                cout << YELLOW << "  Count            : " << RESET;
                cin >> count;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                try {
                    MyString& target = (slot == 'B' || slot == 'b') ? strB : strA;
                    cout << GREEN << "  Substring = " << BOLD << target.substr(start, count)
                         << RESET << "\n";
                } catch (const out_of_range& ex) {
                    cout << RED << "  Error: " << ex.what() << "\n" << RESET;
                }
                printDivider();
                break;}
            
            case 8: {
                printHeader("FIND SUBSTRING / CHARACTER");
                char slot;
                cout << YELLOW << "  Search in which slot? (A/B): " << RESET;
                cin >> slot;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << YELLOW << "  Enter text to find          : " << RESET;
                char buffer[256];
                cin.getline(buffer, 256);

                MyString& target = (slot == 'B' || slot == 'b') ? strB : strA;
                size_t pos = target.find(MyString(buffer));

                if (pos == MyString::npos) {
                    cout << RED << "  Not found.\n" << RESET;
                } else {
                    cout << GREEN << "  Found at index " << BOLD << pos << RESET << "\n";
                }
                printDivider();
                break;}
            
            case 9: {
                printHeader("COPY A INTO B (DEEP COPY)");
                strB = strA;   // copy assignment operator — deep copy
                cout << GREEN << "  [OK] Copied A into B. B is now: " << BOLD << strB
                     << RESET << "\n";
                cout << DIM   << "  (A and B now hold separate buffers with equal content)\n" << RESET;
                printDivider();
                break;}
            
            case 0: {
                cout << "\n" << YELLOW << BOLD;
                cout << "  ====================================================\n";
                cout << "  |   Thank you for using the MyString application  |\n";
                cout << "  |         [Your College Name], CSE Dept            |\n";
                cout << "  ====================================================\n";
                cout << RESET << "\n";
                running = false;
                break;}
            
            default:
                cout << RED << "\n  Invalid choice. Please select 0-9.\n" << RESET;
        }
    }
    return 0;
}
