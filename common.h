#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int32_t i32;
typedef int32_t i64;


i32 mem_counter;
u8 mem_init;

static void _init_mem_counter() {
    if (mem_init == 0) {
        mem_counter = 0;
        mem_init = 1;
    }
}

void* mem_push_wrapper(size_t n) {
    _init_mem_counter();
    void* node = malloc(n);
    // memcpy(node, 0, n);
    mem_counter++;
    return node;
}

void mem_pop_wrapper(void* ptr) {
    if (ptr == 0) {
        return ;
    }
    free(ptr);
    mem_counter--;
}

#define mem_n_push(type_, n) ( type_ * ) mem_push_wrapper( (n)*(sizeof(type_)))
#define mem_push(type_) mem_n_push( type_, 1)
#define mem_pop(ptr) mem_pop_wrapper( (ptr) );

char* mem_cpy_str(const char* input) {
    char* output = mem_n_push(char, strlen(input));
    memcpy((void*)output, (void*)input, strlen(input)*sizeof(char));
    return output;
}

i32 foat_to_int(float x) {
    return (i32) (x + 0.5);
}


#endif  // COMMON_H
