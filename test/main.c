#include "bytearray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void alog(byteArray_t arr) {
    if (arr == NULL) {
        return;
    }
    printf("\r\n=========================================================================================\r\n");
    for (size_t i = 0; i < byteArrayGetLength(arr); i++) {
        uint8_t c = 0;
        byteArrayGetByte(arr, i, &c);
        printf("0x%02x ", c);
    }
    printf("\r\n=========================================================================================\r\n");
}

int main() {
    BYTEARRAY_ENDIAN endian = BYTEARRAY_MSB;
    byteArray_t      arr    = byteArrayCreate(100, endian);
    const uint32_t   ii     = 0x12345678;
    byteArray_t      arr2   = byteArrayAttach(sizeof(ii), (uint8_t *)&ii, sizeof(ii), endian);
    printf("append: %s", byteArrayAppendByte(arr2, 0x00) ? "true" : "false");
    // byteArrayAttach only copy memory an set attributes
    // the arr2 on memory is [78563412]  no matter what endian is
    alog(arr2);
    byteArrayAppendByte(arr, 0x01);
    byteArrayAppendByte(arr, 0x02);
    byteArrayAppendHalfword(arr, 0x1112);
    byteArrayAppendHalfword(arr, 0x1314);
    byteArrayAppendWord(arr, 0x21222324);
    byteArrayAppendWord(arr, 0x25262728);
    byteArrayAppendByteArray(arr, arr2);
    alog(arr);
    return 0;
}
