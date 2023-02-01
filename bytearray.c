/**
 * @file bytearray.c
 * @author mengyou (1523038073@qq.com)
 * @brief C implements bytearray 
 * @version 0.0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "bytearray.h"

#if defined __RTTHREAD__

    #include <rtthread.h>

    #ifndef BA_MALLOC
        #define BA_MALLOC   rt_malloc
    #endif 

    #ifndef BA_MEMSET
        #define BA_MEMSET   rt_memset
    #endif

    #ifndef BA_FREE
        #define BA_FREE     rt_free
    #endif 

    #ifndef BA_MEMCPY 
        #define BA_MEMCPY   rt_memcpy
    #endif 

#else

    #include <string.h>

    #ifndef BA_MALLOC
        #define BA_MALLOC   malloc
    #endif 

    #ifndef BA_MEMSET
        #define BA_MEMSET   memset
    #endif

    #ifndef BA_FREE
        #define BA_FREE     free
    #endif 

    #ifndef BA_MEMCPY 
        #define BA_MEMCPY   memcpy
    #endif 

#endif

static inline uint16_t little2big16(uint16_t x) {
    return ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8);
}

static inline uint32_t little2big32(uint32_t x) {
    return ((x & 0xFF000000) >> 24) | ((x & 0xFF0000) >> 8) | ((x & 0xFF00) << 8) | ((x & 0xFF) << 24);
}

byteArray_t byteArrayCreate(size_t capacity, BYTEARRAY_ENDIAN endian) {
    byteArray_t ret = BA_MALLOC(sizeof(byteArray));
    if (ret == NULL) {
        return NULL;
    }
    BA_MEMSET(ret, 0, sizeof(byteArray));
    ret->array = BA_MALLOC(capacity);
    if (ret->array == NULL) {
        BA_FREE(ret);
        return NULL;
    }
    ret->capacity = capacity;
    ret->endian   = endian;
    return ret;
}

void byteArrayDelete(byteArray_t arr) {
    if (arr != NULL) {
        if (arr->array != NULL) {
            BA_FREE(arr->array);
        }
        BA_FREE(arr);
    }
}

byteArray_t byteArrayAttach(size_t capacity, uint8_t *array, size_t length, BYTEARRAY_ENDIAN endian) {
    byteArray_t ret = BA_MALLOC(sizeof(byteArray));
    if (ret == NULL) {
        return NULL;
    }
    BA_MEMSET(ret, 0, sizeof(byteArray));
    ret->capacity = capacity;
    ret->array    = array;
    ret->len      = length;
    ret->endian   = endian;
    return ret;
}

void inline byteArrayDetach(byteArray_t arr) {
    if (arr != NULL) {
        BA_FREE(arr);
    }
}

size_t inline byteArrayGetCapacity(byteArray_t arr) {
    if (arr != NULL) {
        return arr->capacity;
    }
    return 0;
}

size_t inline byteArrayGetLength(byteArray_t arr) {
    if (arr != NULL) {
        return arr->len;
    }
    return 0;
}

uint8_t inline *byteArrayGetDataPointer(byteArray_t arr) {
    if (arr != NULL) {
        return arr->array;
    }
    return NULL;
}

bool byteArrayGetByte(byteArray_t arr, size_t offset, uint8_t *byte) {
    if (arr != NULL) {
        if (offset < arr->len) {
            *byte = arr->array[offset];
            return true;
        }
    }
    return false;
}

bool byteArraySetByte(byteArray_t arr, size_t offset, uint8_t byte) {
    if (arr != NULL) {
        if (offset + sizeof(uint8_t) <= arr->capacity) {
            arr->array[offset] = byte;
            arr->len++;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendByte(byteArray_t arr, uint8_t byte) {
    if (arr != NULL) {
        if (arr->len + sizeof(uint8_t) <= arr->capacity) {
            arr->array[arr->len++] = byte;
            return true;
        }
    }
    return false;
}

bool byteArrayGetHalfword(byteArray_t arr, size_t offset, uint16_t *halfword) {
    if (arr != NULL) {
        if (offset + sizeof(uint16_t) <= arr->len) {
            uint16_t _ = *(uint16_t *)&(arr->array[offset]);
            if (arr->endian == BYTEARRAY_BIGENDIAN) {
                _ = little2big16(_);
            }
            *halfword = _;
            return true;
        }
    }
    return false;
}

bool byteArraySetHalfword(byteArray_t arr, size_t offset, uint16_t halfword) {
    if (arr != NULL) {
        if (offset + sizeof(uint16_t) <= arr->capacity) {
            if (arr->endian == BYTEARRAY_BIGENDIAN) {
                halfword = little2big16(halfword);
            }
            *(uint16_t *)&(arr->array[offset]) = halfword;
            arr->len += 2;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendHalfword(byteArray_t arr, uint16_t halfword) {
    if (arr != NULL) {
        if (arr->len + sizeof(uint16_t) <= arr->capacity) {
            if (arr->endian == BYTEARRAY_BIGENDIAN) {
                halfword = little2big16(halfword);
            }
            *(uint16_t *)&(arr->array[arr->len]) = halfword;
            arr->len += 2;
            return true;
        }
    }
    return false;
}

bool byteArrayGetWord(byteArray_t arr, size_t offset, uint32_t *word) {
    if (arr != NULL) {
        if (offset + sizeof(uint32_t) <= arr->len) {
            uint32_t _ = *(uint32_t *)&arr->array[offset];
            if (arr->endian == BYTEARRAY_BIGENDIAN) {
                _ = little2big32(_);
            }
            *word = _;
            return true;
        }
    }
    return false;
}

bool byteArraySetWord(byteArray_t arr, size_t offset, uint32_t word) {
    if (arr != NULL) {
        if (offset + sizeof(uint32_t) <= arr->capacity) {
            if (arr->endian == BYTEARRAY_BIGENDIAN) {
                word = little2big32(word);
            }
            *(uint32_t *)&arr->array[offset] = word;
            arr->len += 4;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendWord(byteArray_t arr, uint32_t word) {
    if (arr != NULL) {
        if (arr->len + sizeof(uint32_t) <= arr->capacity) {
            if (arr->endian == BYTEARRAY_BIGENDIAN) {
                word = little2big32(word);
            }
            *(uint32_t *)&arr->array[arr->len] = word;
            arr->len += 4;
            return true;
        }
    }
    return false;
}

bool byteArrayGetBytes(byteArray_t arr, size_t offset, uint8_t **bytes, size_t length) {
    if (arr != NULL) {
        if (offset + length <= arr->len) {
            *bytes = &(arr->array[offset]);
            return true;
        }
    }
    return false;
}

bool byteArraySetBytes(byteArray_t arr, size_t offset, uint8_t *bytes, size_t length) {
    if (arr != NULL) {
        if (offset + length <= arr->capacity) {
            BA_MEMCPY(&arr->array[offset], bytes, length);
            arr->len += length;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendBytes(byteArray_t arr, uint8_t *bytes, size_t length) {
    if (arr != NULL) {
        if (arr->len + length <= arr->capacity) {
            BA_MEMCPY(&arr->array[arr->len], bytes, length);
            arr->len += length;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendByteArray(byteArray_t dest, byteArray_t src) {
    if (dest != NULL && src != NULL && dest != src) {
        if (src->len == 0) {
            return true;
        }
        if (dest->len + src->len <= dest->capacity) {
            BA_MEMCPY(&dest->array[dest->len], src->array, src->len);
            dest->len += src->len;
            return true;
        }
    }
    return false;
}
