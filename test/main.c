#include <stdlib.h>
#include <string.h>
#include "bytearray.h"
#include <stdio.h>

void alog (byteArray_t arr) {
    if (arr == NULL) {
        return ;
    }
    printf("\r\n==========================================\r\n");
    for (size_t i = 0; i< byteArrayGetLength(arr); i++) {
        uint8_t c = 0;
        byteArrayGetByte(arr, i, &c);
        printf("0x%02x ", c);
    }
    printf("\r\n==========================================\r\n");
}

int main() {
    uint8_t  arr_c[128] = {0};
    byteArray_t arr2 = byteArrayCreate(4, 1);
    byteArrayAppendWord(arr2, 0x78654321);
    byteArray_t arr = byteArrayAttach(128, arr_c, 0, 1);
    byteArrayAppendWord(arr, 0x12345678);
    byteArrayAppendByteArray(arr, arr2);
    byteArrayDelete(arr2);
    printf("capacity: %u, len: %u, addr: %p, %p\n", 
        byteArrayGetCapacity(arr), byteArrayGetLength(arr), 
        byteArrayGetDataPointer(arr), arr->array
    );
    alog(arr);
    uint16_t h = 0;
    uint32_t w = 0;
    uint8_t *a = 0;
    byteArrayGetHalfword(arr, 0, &h);
    byteArrayGetWord(arr, 0, &w);
    byteArrayGetBytes(arr, 0, &a, 4);
    printf("h: %04x, w: %08x, a:%02x%02x%02x%02x\n", h, w, a[0], a[1], a[2], a[3]);
    byteArrayDetach(arr);
    printf("end.\r\n");
    return 0;
}
