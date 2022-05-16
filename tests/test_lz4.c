/*
*/

#include "hdf5.h"

int testfilter(char *filename, int shuffle, int filter, int param_no, int *params);

#define MY_H5Z_FILTER 32004

int main() {
    int return_code = 1;
    int compression_opt[] = {3}; /* with LZ4 default level */

    return_code = testfilter("test_lz4", 0, MY_H5Z_FILTER, 1, compression_opt);
    return_code = testfilter("test_lz4", 1, MY_H5Z_FILTER, 1, compression_opt);
    return return_code;
}

