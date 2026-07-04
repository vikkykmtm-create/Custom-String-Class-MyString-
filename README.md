# Custom-String-Class-MyString-

# MyString — Custom String Class in C++

> **Custom Deep-Copy String Class · C++17 · Terminal Application**

```
███╗   ███╗██╗   ██╗
████╗ ████║╚██╗ ██╔╝   MyString  v1.0
██╔████╔██║ ╚████╔╝    Vardhaman College of Engineering
██║╚██╔╝██║  ╚██╔╝     CSE Department · Summer Project 2025–26
██║ ╚═╝ ██║   ██║
╚═╝     ╚═╝   ╚═╝
```

---

## Team

| Role            | Name              |
| --------------- | ----------------- |
| **Team Menter** | KANDULA SAHITHYA  |
| **Team Leader** | GACHUMALE SAI CHARAN REDDY |
| **Member**      | ARITRA DAS |
| **Member**      | PATHLAVATH NITHIN |
| **Member**      | KAMUTAM VIKAS |

---

## Overview

This project is a from-scratch implementation of a **custom string class (`MyString`)** in C++, built to replicate the core behaviour of `std::string` without relying on it internally. It manages its own dynamically allocated character buffer and demonstrates how a "string" actually works under the hood — memory allocation, deep copying, operator overloading, and safe resource cleanup.

Alongside the class itself, the project includes an **interactive terminal application** that lets a user create, modify, compare, and search strings live, so the class's behaviour can be seen and tested directly rather than just read as code.

---

## Why Build Your Own String Class?

```
std::string hides all of this from you:

┌───────────────────────────────┐
│  MyString object               │
│  ┌───────────┐  ┌────────────┐│
│  │ char* data├─▶│ H e l l o \0│  ← heap-allocated buffer
│  └───────────┘  └────────────┘│
│  size_t len = 5                │
└───────────────────────────────┘

Copy a MyString   → must ALLOCATE NEW memory + COPY bytes (deep copy)
                    (forgetting this causes double-free crashes)
Move a MyString   → just STEAL the pointer, no copying (fast)
Destroy a MyString→ must FREE the buffer, or memory leaks
```

Building this by hand is what makes the rule of three/five, deep vs. shallow copies, and manual memory management click.

---

## Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | **Custom Constructors** | Default, from C-string, and repeated-character construction |
| 2 | **Rule of Five** | Deep-copy copy constructor/assignment, move constructor/assignment, destructor |
| 3 | **Operator Overloading** | `+`, `+=`, `==`, `!=`, `<`, `>`, `[]`, `<<`, `>>` |
| 4 | **Bounds-Checked Access** | `.at()` throws `std::out_of_range` on invalid index |
| 5 | **Substring & Search** | `.substr()`, `.find()` (string or char, with `npos` like `std::string`) |
| 6 | **Interactive Console App** | Menu-driven program to create, display, concatenate, compare, and search strings live |

---

## Project Structure

```
MyString/
│
├── MyString.h        # The class itself (header-only)
├── main.cpp           # Automated test/demo of every feature
├── console_app.cpp    # Interactive menu-driven terminal application
└── README.md
```

---

## Requirements

| Tool | Minimum Version |
|------|----------------|
| C++ Compiler (g++) | C++17 or later |
| Terminal | Any standard terminal |
| OS | Linux · macOS · Windows |

---

## Compile & Run

**Test/demo program**
```bash
g++ -std=c++17 -Wall -Wextra -o mystring_test main.cpp
./mystring_test
```

**Interactive console application**
```bash
g++ -std=c++17 -Wall -Wextra -o console_app console_app.cpp
./console_app
```

---

## Sample Session

```
============ MyString Console App ============
 1. Create / overwrite a string
 2. Display a string
 3. Get length
 4. Concatenate two strings (A + B)
 5. Compare two strings
 6. Access character by index
 7. Get substring
 8. Find a substring / character
 9. Copy string A into string B
 0. Exit
================================================
Choose an option: 1
Which slot? (A/B): A
Enter the string: Hello
Slot A set to: Hello

Choose an option: 4
A + B = Hello
```

---

## Core Concepts Covered

**Memory Management**
- Manual dynamic allocation with `new[]` / `delete[]`
- Deep copy vs. shallow copy
- Preventing double-free and memory leaks

**C++ Object-Oriented Design**
- Rule of Five (copy/move constructors, copy/move assignment, destructor)
- Operator overloading for natural, intuitive syntax
- Encapsulation via private data members

**Robustness**
- Self-assignment checks
- Exception-safe assignment (allocate-then-free ordering)
- Bounds checking with exceptions (`std::out_of_range`)

---

## Complexity

| Operation | Time | Notes |
|-----------|------|-------|
| `operator[]` / `.at()` | O(1) | Direct index into buffer |
| Copy construction | O(n) | Deep copy of n characters |
| Move construction | O(1) | Pointer swap, no copying |
| `operator+` | O(n + m) | Allocates and copies both operands |
| `.find()` | O(n × m) worst case | Uses `strstr` internally |
| `.substr()` | O(k) | k = length of extracted substring |

---

## Future Improvements

- [ ] Small String Optimization (SSO) to avoid heap allocation for short strings
- [ ] Iterator support (`begin()` / `end()`) for range-based for-loops
- [ ] `insert()` / `erase()` / `replace()` methods
- [ ] Case conversion (`to_upper()` / `to_lower()`)
- [ ] Reverse and trim utilities
- [ ] Unit tests for every method (e.g. with Catch2 or GoogleTest)

---

## Use Cases

- Data Structures & OOP coursework
- Understanding how `std::string` works internally
- Practicing manual memory management in C++
- Interview preparation (Rule of Three/Five, deep copy questions)

---

## Learning Outcomes

After studying this project you will understand:

- Why the Rule of Five exists and what breaks without it
- The difference between a deep copy and a shallow copy, and why it matters
- How to overload operators to make a custom type feel built-in
- How to write exception-safe assignment operators
- How to build a clean, interactive terminal application in C++

---

## License

Academic project — Vardhaman College of Engineering, CSE Department, 2025–26.
Not intended for commercial use.
