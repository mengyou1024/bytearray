/**
 * @file bytearray.c
 * @author mengyou (1523038073@qq.com)
 * @brief C implements bytearray
 * @version 1.0.0
 * @date 2023-02-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bytearray.h"

typedef struct _byteArray {
    uint8_t         *array;
    size_t           capacity;
    size_t           len;
    BYTEARRAY_ENDIAN endian;
} _byteArray, *_byteArray_t;

#if defined __RTTHREAD__

#    include <rtthread.h>

#    ifndef BA_MALLOC
#        define BA_MALLOC rt_malloc
#    endif

#    ifndef BA_MEMSET
#        define BA_MEMSET rt_memset
#    endif

#    ifndef BA_FREE
#        define BA_FREE rt_free
#    endif

#    ifndef BA_MEMCPY
#        define BA_MEMCPY rt_memcpy
#    endif

#else

#    include <string.h>

#    ifndef BA_MALLOC
#        define BA_MALLOC malloc
#    endif

#    ifndef BA_MEMSET
#        define BA_MEMSET memset
#    endif

#    ifndef BA_FREE
#        define BA_FREE free
#    endif

#    ifndef BA_MEMCPY
#        define BA_MEMCPY memcpy
#    endif

#endif

#ifdef __BYTE_ORDER__
#    if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
const static BYTEARRAY_ENDIAN nativeByteSequence = BYTEARRAY_MSB;
#    else
#        if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#            waring not support byte sequence __BYTE_ORDER__
#        endif
const static BYTEARRAY_ENDIAN nativeByteSequence = BYTEARRAY_LSB;
#    endif
#else
#    warning not define __BYTE_ORDER__ will use LSB mode
const static BYTEARRAY_ENDIAN nativeByteSequence = BYTEARRAY_LSB;
#endif

/**
 * @brief swap byte sequence 16 bit
 *
 * @param x input
 * @return uint16_t output
 */
static inline uint16_t byteSeqSwap16(uint16_t x) {
    return ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8);
}

/**
 * @brief swap byte sequence 32 bit
 *
 * @param x input
 * @return uint16_t output
 */
static inline uint32_t byteSeqSwap32(uint32_t x) {
    return ((x & 0xFF000000) >> 24) | ((x & 0xFF0000) >> 8) | ((x & 0xFF00) << 8) | ((x & 0xFF) << 24);
}

byteArray_t byteArrayCreate(size_t capacity, BYTEARRAY_ENDIAN endian) {
    _byteArray_t ret = BA_MALLOC(sizeof(_byteArray));
    if (ret == NULL) {
        return NULL;
    }
    BA_MEMSET(ret, 0, sizeof(_byteArray));
    ret->array = BA_MALLOC(capacity);
    if (ret->array == NULL) {
        BA_FREE(ret);
        return NULL;
    }
    ret->capacity = capacity;
    ret->endian   = endian;
    return (byteArray_t)ret;
}

void byteArrayDelete(byteArray_t arr) {
    if (arr != NULL) {
        if (((_byteArray_t)arr)->array != NULL) {
            BA_FREE(((_byteArray_t)arr)->array);
        }
        BA_FREE(arr);
    }
}

byteArray_t byteArrayAttach(size_t capacity, uint8_t *array, size_t length, BYTEARRAY_ENDIAN endian) {
    _byteArray_t ret = BA_MALLOC(sizeof(_byteArray));
    if (ret == NULL) {
        return NULL;
    }
    BA_MEMSET(ret, 0, sizeof(_byteArray));
    ret->capacity = capacity;
    ret->array    = array;
    ret->len      = length;
    ret->endian   = endian;
    return (byteArray_t)ret;
}

void inline byteArrayDetach(byteArray_t arr) {
    if (arr != NULL) {
        BA_FREE(arr);
    }
}

size_t inline byteArrayGetCapacity(byteArray_t arr) {
    if (arr != NULL) {
        return ((_byteArray_t)arr)->capacity;
    }
    return 0;
}

size_t inline byteArrayGetLength(byteArray_t arr) {
    if (arr != NULL) {
        return ((_byteArray_t)arr)->len;
    }
    return 0;
}

uint8_t inline *byteArrayGetDataPointer(byteArray_t arr) {
    if (arr != NULL) {
        return ((_byteArray_t)arr)->array;
    }
    return NULL;
}

bool byteArrayGetByte(byteArray_t arr, size_t offset, uint8_t *byte) {
    if (arr != NULL) {
        if (offset < ((_byteArray_t)arr)->len) {
            *byte = ((_byteArray_t)arr)->array[offset];
            return true;
        }
    }
    return false;
}

bool byteArraySetByte(byteArray_t arr, size_t offset, uint8_t byte) {
    if (arr != NULL) {
        if (offset + sizeof(uint8_t) <= ((_byteArray_t)arr)->capacity) {
            ((_byteArray_t)arr)->array[offset] = byte;
            ((_byteArray_t)arr)->len++;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendByte(byteArray_t arr, uint8_t byte) {
    if (arr != NULL) {
        if (((_byteArray_t)arr)->len + sizeof(uint8_t) <= ((_byteArray_t)arr)->capacity) {
            ((_byteArray_t)arr)->array[((_byteArray_t)arr)->len++] = byte;
            return true;
        }
    }
    return false;
}

bool byteArrayGetHalfword(byteArray_t arr, size_t offset, uint16_t *halfword) {
    if (arr != NULL) {
        if (offset + sizeof(uint16_t) <= ((_byteArray_t)arr)->len) {
            uint16_t _ = *(uint16_t *)&(((_byteArray_t)arr)->array[offset]);
            if (((_byteArray_t)arr)->endian != nativeByteSequence) {
                _ = byteSeqSwap16(_);
            }
            *halfword = _;
            return true;
        }
    }
    return false;
}

bool byteArraySetHalfword(byteArray_t arr, size_t offset, uint16_t halfword) {
    if (arr != NULL) {
        if (offset + sizeof(uint16_t) <= ((_byteArray_t)arr)->capacity) {
            if (((_byteArray_t)arr)->endian != nativeByteSequence) {
                halfword = byteSeqSwap16(halfword);
            }
            *(uint16_t *)&(((_byteArray_t)arr)->array[offset]) = halfword;
            ((_byteArray_t)arr)->len += 2;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendHalfword(byteArray_t arr, uint16_t halfword) {
    if (arr != NULL) {
        if (((_byteArray_t)arr)->len + sizeof(uint16_t) <= ((_byteArray_t)arr)->capacity) {
            if (((_byteArray_t)arr)->endian != nativeByteSequence) {
                halfword = byteSeqSwap16(halfword);
            }
            *(uint16_t *)&(((_byteArray_t)arr)->array[((_byteArray_t)arr)->len]) = halfword;
            ((_byteArray_t)arr)->len += 2;
            return true;
        }
    }
    return false;
}

bool byteArrayGetWord(byteArray_t arr, size_t offset, uint32_t *word) {
    if (arr != NULL) {
        if (offset + sizeof(uint32_t) <= ((_byteArray_t)arr)->len) {
            uint32_t _ = *(uint32_t *)&((_byteArray_t)arr)->array[offset];
            if (((_byteArray_t)arr)->endian != nativeByteSequence) {
                _ = byteSeqSwap32(_);
            }
            *word = _;
            return true;
        }
    }
    return false;
}

bool byteArraySetWord(byteArray_t arr, size_t offset, uint32_t word) {
    if (arr != NULL) {
        if (offset + sizeof(uint32_t) <= ((_byteArray_t)arr)->capacity) {
            if (((_byteArray_t)arr)->endian != nativeByteSequence) {
                word = byteSeqSwap32(word);
            }
            *(uint32_t *)&((_byteArray_t)arr)->array[offset] = word;
            ((_byteArray_t)arr)->len += 4;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendWord(byteArray_t arr, uint32_t word) {
    if (arr != NULL) {
        if (((_byteArray_t)arr)->len + sizeof(uint32_t) <= ((_byteArray_t)arr)->capacity) {
            if (((_byteArray_t)arr)->endian != nativeByteSequence) {
                word = byteSeqSwap32(word);
            }
            *(uint32_t *)&((_byteArray_t)arr)->array[((_byteArray_t)arr)->len] = word;
            ((_byteArray_t)arr)->len += 4;
            return true;
        }
    }
    return false;
}

bool byteArrayGetBytes(byteArray_t arr, size_t offset, uint8_t **bytes, size_t length) {
    if (arr != NULL) {
        if (offset + length <= ((_byteArray_t)arr)->len) {
            *bytes = &(((_byteArray_t)arr)->array[offset]);
            return true;
        }
    }
    return false;
}

bool byteArraySetBytes(byteArray_t arr, size_t offset, uint8_t *bytes, size_t length) {
    if (arr != NULL) {
        if (offset + length <= ((_byteArray_t)arr)->capacity) {
            BA_MEMCPY(&((_byteArray_t)arr)->array[offset], bytes, length);
            ((_byteArray_t)arr)->len += length;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendBytes(byteArray_t arr, uint8_t *bytes, size_t length) {
    if (arr != NULL) {
        if (((_byteArray_t)arr)->len + length <= ((_byteArray_t)arr)->capacity) {
            BA_MEMCPY(&((_byteArray_t)arr)->array[((_byteArray_t)arr)->len], bytes, length);
            ((_byteArray_t)arr)->len += length;
            return true;
        }
    }
    return false;
}

bool byteArrayAppendByteArray(byteArray_t dest, byteArray_t src) {
    if (dest != NULL && src != NULL && dest != src) {
        if (((_byteArray_t)src)->len == 0) {
            return true;
        }
        if (((_byteArray_t)dest)->len + ((_byteArray_t)src)->len <= ((_byteArray_t)dest)->capacity) {
            BA_MEMCPY(&((_byteArray_t)dest)->array[((_byteArray_t)dest)->len], ((_byteArray_t)src)->array, ((_byteArray_t)src)->len);
            ((_byteArray_t)dest)->len += ((_byteArray_t)src)->len;
            return true;
        }
    }
    return false;
}
