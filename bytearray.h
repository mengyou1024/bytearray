/**
 * @file bytearray.h
 * @author mengyou (1523038073@qq.com)
 * @brief C implements bytearray 
 * @version 0.0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief sequence of bytearray
typedef enum BYTEARRAY_ENDIAN {
    BYTEARRAY_LITTLEENDIAN,
    BYTEARRAY_BIGENDIAN,
} BYTEARRAY_ENDIAN;

typedef struct byteArray {
    uint8_t         *array;
    size_t           capacity;
    size_t           len;
    BYTEARRAY_ENDIAN endian;
} byteArray, *byteArray_t;

/**
 * @brief create a bytearray
 *
 * @param capacity the capacity of bytearray
 * @param endian the byte sequence of bytearray
 * @return byteArray_t bytearray object
 */
byteArray_t byteArrayCreate(size_t capacity, BYTEARRAY_ENDIAN endian);

/**
 * @brief delete the memory of bytearray
 *
 * @param arr bytearray object
 */
void byteArrayDelete(byteArray_t arr);

/**
 * @brief initialize bytearray with user memory
 * 
 * @param capacity the capacity of memory
 * @param array ponter of user memory
 * @param length init data length 
 * @param endian the byte sequence of bytearray
 * @return byteArray_t bytearray object
 */
byteArray_t byteArrayAttach(size_t capacity, uint8_t *array, size_t length, BYTEARRAY_ENDIAN endian);

/**
 * @brief uninitialize bytearray create by # byteArrayAttach
 * 
 * @param arr bytearray object
 */
void byteArrayDetach(byteArray_t arr);

/**
 * @brief get the capacity of bytearray
 *
 * @param arr bytearray object
 * @return size_t capacity
 */
size_t byteArrayGetCapacity(byteArray_t arr);

/**
 * @brief get the length of bytearray
 *
 * @param arr bytearray object
 * @return size_t length
 */
size_t byteArrayGetLength(byteArray_t arr);

/**
 * @brief get the data pointer of bytearray
 *
 * @param arr bytearray object
 * @return uint8_t* data pointer
 */
uint8_t *byteArrayGetDataPointer(byteArray_t arr);

/**
 * @brief get byte from bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param[out] byte data got
 * @return true success
 * @return false failed
 */
bool byteArrayGetByte(byteArray_t arr, size_t offset, uint8_t *byte);

/**
 * @brief set byte in bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param byte in data
 * @return true success
 * @return false failed
 */
bool byteArraySetByte(byteArray_t arr, size_t offset, uint8_t byte);

/**
 * @brief append byte in the end of bytearray
 *
 * @param arr bytearray object
 * @param byte in data
 * @return true success
 * @return false failed
 */
bool byteArrayAppendByte(byteArray_t arr, uint8_t byte);

/**
 * @brief get byte from bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param[out] halfword data got
 * @return true success
 * @return false failed
 */
bool byteArrayGetHalfword(byteArray_t arr, size_t offset, uint16_t *halfword);

/**
 * @brief set byte in bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param halfword in data
 * @return true success
 * @return false failed
 */
bool byteArraySetHalfword(byteArray_t arr, size_t offset, uint16_t halfword);

/**
 * @brief append byte in the end of bytearray
 *
 * @param arr bytearray object
 * @param halfword in data
 * @return true success
 * @return false failed
 */
bool byteArrayAppendHalfword(byteArray_t arr, uint16_t halfword);

/**
 * @brief get a word from bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param[out] word data got
 * @return true success
 * @return false failed
 */
bool byteArrayGetWord(byteArray_t arr, size_t offset, uint32_t *word);

/**
 * @brief set a word in bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param word in data
 * @return true success
 * @return false failed
 */
bool byteArraySetWord(byteArray_t arr, size_t offset, uint32_t word);

/**
 * @brief append word in the end of bytearray
 *
 * @param arr bytearray object
 * @param word in data
 * @return true success
 * @return false failed
 */
bool byteArrayAppendWord(byteArray_t arr, uint32_t word);

/**
 * @brief get bytes from bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param[out] bytes bytes pointer
 * @param length length of bytes
 * @return true success
 * @return false failed
 */
bool byteArrayGetBytes(byteArray_t arr, size_t offset, uint8_t **bytes, size_t length);

/**
 * @brief set bytes in bytearray
 *
 * @param arr bytearray object
 * @param offset offset in bytearray
 * @param[in] bytes bytes pointer
 * @param length length of data
 * @return true success
 * @return false failed
 */
bool byteArraySetBytes(byteArray_t arr, size_t offset, uint8_t *bytes, size_t length);

/**
 * @brief append bytes in the end of bytearray
 *
 * @param arr bytearray object
 * @param[in] bytes bytes pointer
 * @param length length of bytes
 * @return true success
 * @return false failed
 */
bool byteArrayAppendBytes(byteArray_t arr, uint8_t *bytes, size_t length);

/**
 * @brief append a bytearray into the end of the other bytearray
 * 
 * @param dest destination bytearray object
 * @param src source bytearray object
 * @return true success
 * @return false failed
 */
bool byteArrayAppendByteArray(byteArray_t dest, byteArray_t src);

#ifdef __cplusplus
}
#endif
