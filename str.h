#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "alias.h"

#ifdef STD_STRING

char* str_slice(const char* s, int start, int end);

#define NIL '\0'

#define str_append(s, c) \
 \
    do { \
 \
    if (!(s)) { \
 \
        Header* header = (Header*)malloc(sizeof(*(s)) * INIT_CAPACITY + sizeof(Header)); \
        header->count = 0; \
        header->capacity = INIT_CAPACITY; \
        (s) = (char*)(header + 1); \
    } \
 \
    Header* header = (Header*)(s) - 1; \
 \
    if (header->count + 1 >= header->capacity) { \
 \
        header->capacity *= 1.5; \
        header = (Header*)realloc(header, (sizeof(*(s)) * header->capacity) + sizeof(Header)); \
        (s) = (char*)(header + 1); \
    } \
 \
    (s)[header->count++] = (c); \
    (s)[header->count] = NIL; \
 \
    } while(0)


#define str_len(s) ((s) ? ((Header*)(s) - 1)->count : 0)

#define str_free(s) \
 \
    do { \
 \
        if (s) \
            free((Header*)(s) - 1); \
        (s) = NULL; \
 \
    } while (0)


#define str_concat(s, t) \
 \
    do { \
 \
        if (!(t) || !strlen(t)) \
            break; \
 \
        if (!(s)) { \
 \
            Header* header = (Header*)malloc(sizeof(*(s)) * INIT_CAPACITY + sizeof(Header)); \
            header->count = 0; \
            header->capacity = INIT_CAPACITY; \
            (s) = (char*)(header + 1); \
            (s)[0] = NIL; \
        } \
 \
        Header* header = (Header*)(s) - 1; \
 \
        if (header->count + strlen(t) + 1 >= header->capacity) { \
 \
            header->capacity += strlen(t) + header->capacity * 1.5; \
            header = (Header*)realloc(header, (sizeof(*(s)) * header->capacity) + sizeof(Header)); \
            (s) = (char*)(header + 1); \
        } \
 \
        memcpy((s) + header->count, (t), strlen(t)); \
        header->count += strlen(t); \
        (s)[header->count] = NIL; \
 \
    } while(0)


#define str_replace(s, old, _new) \
    \
    do { \
        char* _s = _str_replace(s, old, _new); \
        str_free(s); \
        s = _s; \
        \
    } while(0)


#define str_for_each(s, ptr) for (char* ptr = (s); ptr < (s) + str_len(s); ptr++)


#endif // STD_STRING


#ifdef STD_STRING_IMPLEMENTATION

static inline char* _str_replace(const char* s, const char* t, const char* r) {

    if (!s) {

        Header* header = (Header*)malloc(sizeof(*s) * INIT_CAPACITY + sizeof(Header));
        header->count = 0;
        header->capacity = INIT_CAPACITY;
        char* _s = (char*)(header + 1);
        _s[0] = NIL;

        return _s;
    }

    if (!t || !strlen(t) || !r || !strlen(r))
        return str_slice(s, 0, str_len(s) - 1);

    usize occurrences = 0;
    const char* u = s;
    while ((u = strstr(u, t))) {

        occurrences++;
        u += strlen(t);
    }

    if (!occurrences)
        return str_slice(s, 0, str_len(s) - 1);
        
    usize s1 = strlen(s);
    usize t1 = strlen(t);
    usize r1 = strlen(r);
    usize v1 = (usize)(int)((int)s1 + (int)occurrences * ((int)r1 - (int)t1));

    Header* header = (Header*)malloc(sizeof(Header) + v1 + 1);
    header->count = v1;
    header->capacity = v1 * 1.5;
    char* str = (char*)(header + 1);

    char* st = str;
    u = s;
    const char* next;
    
    while ((next = strstr(u, t))) {

        usize chunk_len = next - u;
        memcpy(st, u, chunk_len);
        st += chunk_len;
        
        memcpy(st, r, r1);
        st += r1;
        
        u = next + t1;
    }

    strcpy(st, u);
    return str;
}


inline char* str_slice(const char* s, int start, int end) {

    if (!s) {

        Header* header = (Header*)malloc(sizeof(*s) * INIT_CAPACITY + sizeof(Header));
        header->count = 0;
        header->capacity = INIT_CAPACITY;
        char* _s = (char*)(header + 1);
        _s[0] = NIL;

        return _s;
    }

    if (start < 0)
        start += str_len(s);

    if (end < 0)
        end += str_len(s);

    if (start < 0)
        start = 0;

    if (end < 0 || end >= str_len(s))
        end = str_len(s) - 1;

    if (start > end) {

        start = 0;
        end = str_len(s) - 1;
    }

    usize len = end - start + 1;
    Header* header = (Header*)malloc(sizeof(Header) + len + 1);
    header->count = len;
    header->capacity = len * 1.5;
    char* slice = (char*)(header + 1);
    memcpy(slice, s + start, len);
    slice[len] = NIL;

    return slice;
}

#endif // STD_STRING_IMPLEMENTATION




