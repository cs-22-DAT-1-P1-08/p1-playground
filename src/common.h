#ifndef P1_COMMON_H
#define P1_COMMON_H

#ifndef DEBUG
#define DEBUG 0
#endif

// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#endif