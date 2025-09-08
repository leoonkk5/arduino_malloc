#include <stdio.h>
#include <stdlib.h>

// ANSI color codes
#define COLOR_RED    "\x1b[31m"
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_RESET  "\x1b[0m"

// Minimal testing macros with color
#define ASSERT_TRUE(cond) \
    do { if (!(cond)) { \
        printf(COLOR_RED "[FAIL] %s:%d: %s\n" COLOR_RESET, __FILE__, __LINE__, #cond); \
        exit(1); \
    }} while (0)

#define ASSERT_EQUAL(a, b) \
    do { if ((a) != (b)) { \
        printf(COLOR_RED "[FAIL] %s:%d: %s != %s (got %d vs %d)\n" COLOR_RESET, \
            __FILE__, __LINE__, #a, #b, (int)(a), (int)(b)); \
        exit(1); \
    }} while (0)

#define TEST_PASS(msg) \
    printf(COLOR_GREEN "[PASS] %s\n" COLOR_RESET, msg);
