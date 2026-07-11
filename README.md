# Custom-String-Class-MyString-

# MyString — Custom String Class in C++

> **Custom Deep-Copy String Class · C++17 · Terminal Application**

```
███╗   ███╗██╗   ██╗
████╗ ████║╚██╗ ██╔╝   MyString  
██╔████╔██║ ╚████╔╝    Vardhaman College of Engineering 
██║╚██╔╝██║  ╚██╔╝     CSE Department · Summer Project 2025–26
██║ ╚═╝ ██║   ██║
╚═╝     ╚═╝   ╚═╝
```

---

## Team

| Role            | Name              |
| --------------- | ----------------- |
| **Team Mentor** | K.SAHITHYA      |
| **Team Leader** | G.SAI CHARAN REDDY |
| **Member**      | ARITRA DAS  |
| **Member**      | P.NITHIN  |
| **Member**      | K.VIKAS  |

---

## Overview

`MyString` is a from-scratch implementation of a custom string class in C++, built to replicate the core behaviour of `std::string` without relying on it internally. It manages its own dynamically allocated, null-terminated `char*` buffer and demonstrates how a "string" actually works under the hood — manual memory allocation, deep copying, move semantics, operator overloading, and safe resource cleanup.

The entire project lives in a **single compact file** (`MyString.cpp`): the class itself plus a colorized, menu-driven interactive console application that lets you create, inspect, and manipulate two string "slots" (A and B) live in the terminal.

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

Copy a MyString   → ALLOCATE new memory + COPY bytes (deep copy)
                    (forgetting this causes double-free / dangling-pointer bugs)
Move a MyString   → just STEAL the pointer, no copying (fast, noexcept)
Destroy a MyString→ FREE the buffer, or memory leaks
```

Building this by hand is what makes the Rule of Five, deep vs. shallow copies, and manual memory management click.

---

## Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | **Custom Constructors** | Default, from C-string (`const char*`), and repeated-character (`count, char`) construction |
| 2 | **Rule of Five** | Deep-copy copy constructor/assignment, `noexcept` move constructor/assignment, destructor |
| 3 | **Operator Overloading** | `+`, `+=`, `==`, `!=`, `<`, `>`, `[]` (mutable & const), `<<`, `>>` |
| 4 | **Bounds-Checked Access** | `.at()` (mutable & const) throws `std::out_of_range` on invalid index |
| 5 | **Substring & Search** | `.substr(start, count)`, `.find(needle)` for both `MyString` and `char`, with `npos` sentinel like `std::string` |
| 6 | **Interactive Console App** | Colorized menu with two live string slots (A/B): create, display, get length, concatenate, compare, index, slice, search, and deep-copy |
| 7 | **Graceful Error Handling** | Out-of-range exceptions caught in the main loop and printed in red instead of crashing the program |

---

## Project Structure

```
MyString/
│
├── MyString.cpp       # Class implementation + interactive console app (single file)
└── README.md
```

---

## Requirements

| Tool | Minimum Version |
|------|----------------|
| C++ Compiler (g++) | C++17 or later |
| Terminal | ANSI-color-capable terminal (menu uses colored text) |
| OS | Linux · macOS · Windows |

---

## Compile & Run

```bash
g++ -std=c++17 -o mystring MyString.cpp
./mystring
```

---

## Sample Session

```
  MyString CONSOLE APPLICATION - v2.0
  [Your College Name], CSE Dept

A = ""  B = ""

  [1] Create/overwrite a string (slot A or B)
  [2] Display both slots
  [3] Get length of A and B
  [4] Concatenate (A + B)
  [5] Compare A and B
  [6] Access character by index
  [7] Get substring
  [8] Find substring / character
  [9] Copy A into B (deep copy)
  [0] Exit

> 1
Which slot? (A/B): A
Enter the string  : Hello
[OK] Slot A set to: Hello

> 1
Which slot? (A/B): B
Enter the string  : World
[OK] Slot B set to: World

> 4
A + B = HelloWorld

> 9
[OK] Copied A into B. B is now: Hello
(A and B now hold separate buffers with equal content)
```

---

## Core Concepts Covered

**Memory Management**
- Manual dynamic allocation with `new[]` / `delete[]` via a private `alloc()` helper
- Deep copy vs. shallow copy (demonstrated directly by menu option 9)
- Preventing double-free and memory leaks through careful ownership transfer in move operations

**C++ Object-Oriented Design**
- Rule of Five: copy constructor, copy assignment, move constructor, move assignment, destructor
- Operator overloading for natural, `std::string`-like syntax
- Encapsulation via private data members (`char* data`, `size_t len`) and a private buffer-only constructor used internally by `substr()` and `operator+`

**Robustness**
- Self-assignment checks (`if (this==&o) return *this;`) in both copy and move assignment
- Exception-safe assignment ordering: allocate + copy into a new buffer *before* freeing the old one
- Bounds checking via `std::out_of_range`, caught in the console app's `try/catch` and displayed without crashing

---

## Complexity

| Operation | Time | Notes |
|-----------|------|-------|
| `operator[]` / `.at()` | O(1) | Direct index into buffer |
| Copy construction / copy assignment | O(n) | Deep copy of n characters via `memcpy` |
| Move construction / move assignment | O(1) | Pointer swap, no copying |
| `operator+` / `operator+=` | O(n + m) | Allocates and copies both operands |
| `.find(MyString)` | O(n × m) worst case | Implemented with `strstr` |
| `.find(char)` | O(n) | Linear scan |
| `.substr()` | O(k) | k = length of extracted substring |

---

## Future Improvements

- [ ] Small String Optimization (SSO) to avoid heap allocation for short strings
- [ ] Iterator support (`begin()` / `end()`) for range-based for-loops and STL algorithm compatibility
- [ ] `insert()` / `erase()` / `replace()` methods
- [ ] Case conversion (`to_upper()` / `to_lower()`)
- [ ] Reverse and trim utilities
- [ ] Unit tests for every method (e.g. with Catch2 or GoogleTest)

---

## Use Cases

- Data Structures & OOP coursework
- Understanding how `std::string` works internally
- Practicing manual memory management in C++
- Interview preparation (Rule of Three/Five, deep vs. shallow copy questions)

---

## Learning Outcomes

After studying this project you will understand:

- Why the Rule of Five exists and what breaks without it
- The difference between a deep copy and a shallow copy, and why it matters
- How to overload operators to make a custom type feel built-in
- How to write exception-safe assignment operators
- How to build a clean, interactive, colorized terminal application in C++

---

## License

Academic project — Vardhaman College of Engineering, CSE Department, 2025–26.
Not intended for commercial use.
