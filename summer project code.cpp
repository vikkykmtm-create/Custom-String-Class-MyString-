#ifndef MYSTRING_H
#define MYSTRING_H

#include <cstring>   // strlen, strcpy, strcmp
#include <iostream>  // ostream, istream
#include <stdexcept> // out_of_range

class MyString {
private:
    char* data;      // dynamically allocated character array (owns its own memory)
    size_t len;      // length of the string, NOT counting the null terminator

    // helper: allocate `len` chars, copy from `src` (or empty if src == nullptr)
    void allocateAndCopy(const char* src, size_t length) {
        len = length;
        data = new char[len + 1];      // +1 for the null terminator
        if (src) {
            std::memcpy(data, src, len);
        }
        data[len] = '\0';
    }

public:
    // ---------- Constructors ----------

    // Default constructor -> empty string
    MyString() : data(nullptr), len(0) {
        allocateAndCopy(nullptr, 0);
    }

    // Construct from a C-string literal, e.g. MyString s("hello");
    MyString(const char* str) : data(nullptr), len(0) {
        if (str == nullptr) {
            allocateAndCopy(nullptr, 0);
        } else {
            allocateAndCopy(str, std::strlen(str));
        }
    }

    // Construct a string of `count` repeated characters, e.g. MyString(5, '*') -> "*****"
    MyString(size_t count, char ch) : data(nullptr), len(0) {
        len = count;
        data = new char[len + 1];
        std::memset(data, ch, len);
        data[len] = '\0';
    }

    // ---------- Rule of Five ----------
    // Because this class manages a raw pointer, we must define (or explicitly
    // default) all five of: destructor, copy ctor, copy assign, move ctor, move assign.

    // Copy constructor: deep copy (this is THE key concept of the assignment)
    MyString(const MyString& other) : data(nullptr), len(0) {
        allocateAndCopy(other.data, other.len);
    }

    // Move constructor: steal the other object's buffer, leave it empty
    MyString(MyString&& other) noexcept : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;
    }

    // Copy assignment operator
    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;      // guard against self-assignment
        char* newData = new char[other.len + 1]; // allocate first (exception safety)
        std::memcpy(newData, other.data, other.len);
        newData[other.len] = '\0';
        delete[] data;                          // now safe to free old buffer
        data = newData;
        len = other.len;
        return *this;
    }

    // Move assignment operator
    MyString& operator=(MyString&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        len = other.len;
        other.data = nullptr;
        other.len = 0;
        return *this;
    }

    // Destructor
    ~MyString() {
        delete[] data;
    }

    // ---------- Basic accessors ----------

    size_t length() const { return len; }
    size_t size() const { return len; }
    bool empty() const { return len == 0; }
    const char* c_str() const { return data; }

    // ---------- Element access ----------

    // Non-const version: allows s[0] = 'H';
    char& operator[](size_t index) {
        return data[index]; // no bounds check, like std::string::operator[]
    }
    const char& operator[](size_t index) const {
        return data[index];
    }

    // Bounds-checked access
    char& at(size_t index) {
        if (index >= len) throw std::out_of_range("MyString::at index out of range");
        return data[index];
    }
    const char& at(size_t index) const {
        if (index >= len) throw std::out_of_range("MyString::at index out of range");
        return data[index];
    }

    // ---------- Concatenation ----------

    MyString operator+(const MyString& rhs) const {
        MyString result;
        delete[] result.data;
        result.len = len + rhs.len;
        result.data = new char[result.len + 1];
        std::memcpy(result.data, data, len);
        std::memcpy(result.data + len, rhs.data, rhs.len);
        result.data[result.len] = '\0';
        return result;
    }

    MyString& operator+=(const MyString& rhs) {
        char* newData = new char[len + rhs.len + 1];
        std::memcpy(newData, data, len);
        std::memcpy(newData + len, rhs.data, rhs.len);
        len += rhs.len;
        newData[len] = '\0';
        delete[] data;
        data = newData;
        return *this;
    }

    // ---------- Comparison ----------

    bool operator==(const MyString& rhs) const {
        if (len != rhs.len) return false;
        return std::memcmp(data, rhs.data, len) == 0;
    }
    bool operator!=(const MyString& rhs) const { return !(*this == rhs); }

    bool operator<(const MyString& rhs) const {
        return std::strcmp(data, rhs.data) < 0;
    }
    bool operator>(const MyString& rhs) const {
        return std::strcmp(data, rhs.data) > 0;
    }

    // ---------- Substring / search ----------

    MyString substr(size_t start, size_t count = (size_t)-1) const {
        if (start > len) throw std::out_of_range("MyString::substr start out of range");
        size_t actualCount = (count == (size_t)-1 || start + count > len) ? (len - start) : count;
        MyString result;
        delete[] result.data;
        result.len = actualCount;
        result.data = new char[actualCount + 1];
        std::memcpy(result.data, data + start, actualCount);
        result.data[actualCount] = '\0';
        return result;
    }

    // Returns index of first match, or (size_t)-1 if not found (mirrors std::string::npos)
    static const size_t npos = (size_t)-1;

    size_t find(const MyString& needle, size_t startPos = 0) const {
        if (needle.len == 0) return startPos <= len ? startPos : npos;
        if (startPos >= len) return npos;
        const char* found = std::strstr(data + startPos, needle.data);
        if (!found) return npos;
        return (size_t)(found - data);
    }

    size_t find(char ch, size_t startPos = 0) const {
        for (size_t i = startPos; i < len; ++i) {
            if (data[i] == ch) return i;
        }
        return npos;
    }

    // ---------- Stream operators ----------

    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        os << (s.data ? s.data : "");
        return os;
    }

    friend std::istream& operator>>(std::istream& is, MyString& s) {
        char buffer[1024];
        is >> buffer; // reads a single whitespace-delimited word
        s = MyString(buffer);
        return is;
    }
};

#endif // MYSTRING_H
