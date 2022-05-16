/*
*/

#include "hdf5.h"

int testfilter(char *filename, int shuffle, int filter, int param_no, int *params);

#define MY_H5Z_FILTER 32008
#define BSHUF_H5_COMPRESS_LZ4 2
#define BSHUF_H5_COMPRESS_ZSTD 3

int main() {
    int return_code = 1;
    int lz4_opt[] = {0, BSHUF_H5_COMPRESS_LZ4};
    int zstd_opt[] = {0, BSHUF_H5_COMPRESS_ZSTD, 3}; // compression level 0-ZSTD_maxCLevel() (0=no compression, higher values => smaller size, max=22 since 0.6.0 to 1.5.2, default 3)
    return_code = testfilter("test_bs_lz4", 0, MY_H5Z_FILTER, 2, lz4_opt);
    return_code = testfilter("test_bs_lz4", 1, MY_H5Z_FILTER, 2, lz4_opt);
    return_code = testfilter("test_bs_zstd", 0, MY_H5Z_FILTER, 3, zstd_opt);
    return_code = testfilter("test_bs_zstd", 1, MY_H5Z_FILTER, 3, zstd_opt);
    return return_code;
}

