/*
*/

#include "hdf5.h"

int testfilter(char *filename, int shuffle, int filter, int param_no, int *params);

#define MY_H5Z_FILTER H5Z_FILTER_DEFLATE

int main() {
    int return_code = 1;
    int compression_opt[] = {1}; /* deflate level */

    return_code = testfilter("test_zlib", 0, MY_H5Z_FILTER, 1, compression_opt);
    return_code = testfilter("test_zlib", 1, MY_H5Z_FILTER, 1, compression_opt);
    return return_code;
}

