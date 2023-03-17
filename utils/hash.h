#ifndef HASH_H
#define HASH_H

#ifdef HASH_SIZE_32
    #define hash_function(key, len) hash_function_32((key), (len))
#else
    #define hash_function(key, len) hash_function_64((key), (len))
#endif

unsigned int hash_function_32(void *key, int len);
unsigned long hash_function_64(void *key, int len);
unsigned long hash_string(unsigned char *key);
#define hash_struct(key) hash_function((void *)(key), sizeof(key));

#endif  // HASH_H


#ifdef HASH_H_IMPLEMENTATION

/* FNV-1a hash function in 32 bits*/
unsigned int hash_function_32(void *key, int len) {
    unsigned int hash = 0x811c9dc5;

    for(unsigned int i=0; i<len; i++) {
        hash ^= *(unsigned char *)key + i;
        hash *= 0x01000193;
    }
    return hash;
}

/* FNV-1a hash function in 64 bits*/
unsigned long hash_function_64(void *key, int len) {
    unsigned long hash = 0xcbf29ce484222325;
    unsigned long fnv_prime = 0x00000100000001B3;

    for(unsigned int i=0; i<len; i++) {
        hash ^= *(unsigned char *)key + i;
        hash *= 0x00000100000001B3;
    }
    return hash;
}


unsigned long hash_string(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        hash = (hash * 33) ^ c; /* hash * 33 + c */

    return hash;
}


#endif  // HASH_H_IMPLEMENTATION
