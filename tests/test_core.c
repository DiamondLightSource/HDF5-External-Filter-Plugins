
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hdf5.h"

#define RANK 3
#define SIZE 10*128*256
#define SHAPE {10,128,256}
#define MAX_SHAPE {-1,128,256}
#define CHUNK_SHAPE {1,128,256}

int testfilter(char *file_prefix, int shuffle, int filter, int param_no, int *params) {

    static float data[SIZE];
    static float data_out[SIZE];
    const hsize_t shape[] = SHAPE;
    const hsize_t maxshape[] = MAX_SHAPE;
    const hsize_t chunkshape[] = CHUNK_SHAPE;
    char *filename;
    int f_length;
    int r, i;
    int return_code = 1;
    hid_t fid, sid, dset, plist = 0;
    unsigned        filter_config;
    herr_t          status;
    htri_t          avail;

    /*
     * Check that filter is registered with the library now.
     * If it is registered, retrieve filter's configuration.
     */
    avail = H5Zfilter_avail(filter);
    fprintf(stderr, "Filter for %s (%d)\n", file_prefix, filter);
    if (avail) {
        status = H5Zget_filter_info(filter, &filter_config);
        i = 0;
        if (filter_config & H5Z_FILTER_CONFIG_ENCODE_ENABLED) {
            printf("\tis available for encoding\n");
            i++;
        }
        if (filter_config & H5Z_FILTER_CONFIG_DECODE_ENABLED) {
            printf("\tis available for decoding\n");
            i++;
        }
        if (i != 2) {
            fprintf(stderr, "\tbut need to be able to encode and decode\n");
            goto failed;
        }
    } else {
        fprintf(stderr, "\tis unavailable\n");
        goto failed;
    }

    for (i=0; i<SIZE; i++) {
        data[i] = i;
    }

    f_length = strlen(file_prefix) + 5; /* [01].h5 + '\0' */
    filename = (char *) malloc(f_length);
    snprintf(filename, f_length, "%s%d.h5", file_prefix, shuffle ? 1 : 0);
    fprintf(stderr, "Saving to %s\n", filename);
    fid = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (fid<0) goto failed;

    sid = H5Screate_simple(RANK, shape, maxshape);
    if (sid<0) goto failed;

    plist = H5Pcreate(H5P_DATASET_CREATE);
    if (plist<0) goto failed;

    /* Chunked layout required for filters */
    r = H5Pset_chunk(plist, RANK, chunkshape);
    if (r<0) goto failed;

    /* Use of the shuffle filter VASTLY improves performance of this
       and other block-oriented compression filters.  Be sure to add
       this before the compression filter!
    */
    if (shuffle) {
        r = H5Pset_shuffle(plist);
        if (r<0) goto failed;
    }

    fprintf(stderr, " 1");
    r = H5Pset_filter(plist, filter, H5Z_FLAG_OPTIONAL, param_no, params);
    if (r<0) goto failed;

    dset = H5Dcreate(fid, "dset", H5T_NATIVE_FLOAT, sid, H5P_DEFAULT, plist, H5P_DEFAULT);
    if (dset<0) goto failed;

    fprintf(stderr, " 2");
    r = H5Dwrite(dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data);
    if (r<0) goto failed;

    fprintf(stderr, " 3");
    r = H5Dread(dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data_out);
    if (r<0) goto failed;

    for (i=0;i<SIZE;i++) {
        if (data[i] != data_out[i]) goto failed;
    }

    fprintf(stderr, " Success!\n");

    return_code = 0;

 failed:
    if (dset>0)  H5Dclose(dset);
    if (sid>0)   H5Sclose(sid);
    if (plist>0) H5Pclose(plist);
    if (fid>0)   H5Fclose(fid);

    if (return_code) fprintf(stderr, "Something failed\n");
    return return_code;
}
