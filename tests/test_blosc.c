/*
*/

#include <stdio.h>
#include <stdlib.h>
#include "hdf5.h"

int testfilter(char *filename, int shuffle, int filter, int param_no, int *params);

#define MY_H5Z_FILTER 32001

int main() {
    int return_code = 1;
    int all_compression_opt[][7] = {
    {0, 0, 0, 0, 5, 0, 0}, {0, 0, 0, 0, 5, 1, 0}, {0, 0, 0, 0, 5, 2, 0}, // blosclz
    {0, 0, 0, 0, 5, 0, 1}, {0, 0, 0, 0, 5, 1, 1}, {0, 0, 0, 0, 5, 2, 1}, // lz4
//    {0, 0, 0, 0, 5, 0, 2}, {0, 0, 0, 0, 5, 1, 2}, {0, 0, 0, 0, 5, 2, 2}, // lz4hc
//    {0, 0, 0, 0, 5, 0, 3}, {0, 0, 0, 0, 5, 1, 3}, {0, 0, 0, 0, 5, 2, 3}, // snappy
    {0, 0, 0, 0, 5, 0, 4}, {0, 0, 0, 0, 5, 1, 4}, {0, 0, 0, 0, 5, 2, 4}, // zlib
    {0, 0, 0, 0, 5, 0, 5}, {0, 0, 0, 0, 5, 1, 5}, {0, 0, 0, 0, 5, 2, 5}, // zstd
    };
    /* # 0 and 2 do not work!
       0-3 overwritten,
       4 = 5; [compression level 0-9; 0=no compression],
       5 = 1; [BLOSC_NOSHUFFLE, BLOSC_SHUFFLE, BLOSC_BITSHUFFLE],
       6 = 0; [BLOSC_BLOSCLZ, BLOSC_LZ4, BLOSC_LZ4HC, BLOSC_SNAPPY, BLOSC_ZLIB, BLOSC_ZSTD]
     */
    int i = 0;
    char* name = malloc(10+4);
    for (i = 0; i < 12; i++) {
        printf("Option %d\n", i);
        snprintf(name, 14, "test_blosc%d-", i);
        return_code = testfilter(name, 0, MY_H5Z_FILTER, 7, all_compression_opt[i]);
        if (return_code) {
            printf("Faliure for %d\n", i);
        }
    }
    return return_code;
}

