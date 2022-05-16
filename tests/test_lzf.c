/*
*/

#include "hdf5.h"

int testfilter(char *filename, int shuffle, int filter, int param_no, int *params);

#define MY_H5Z_FILTER 32000

int main() {
    int return_code = 1;
    int compression_opt[] = {0}; /* changed to see if it was segfaulting on win32 when was NULL; /* no parameters */

    return_code = testfilter("test_lzf", 1, MY_H5Z_FILTER, 1, compression_opt);
    return_code = testfilter("test_lzf", 0, MY_H5Z_FILTER, 1, compression_opt);
    return return_code;
}

