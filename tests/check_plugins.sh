#! /bin/bash
set -e -x

PATH=$H5/bin:$PATH
export LD_LIBRARY_PATH=$H5/lib:$LD_LIBRARY_PATH
export HDF5_PLUGIN_PATH=$(realpath -L $PWD/..)

CODECS="zlib lz4 bs blosc lzf"

if [ -z "$CC" ]; then
  CC=cc
fi

for c in $CODECS; do
  echo "Compiling writer using $c"
  $CC $CFLAGS -I$H5/include test_core.c test_$c.c -o test_$c -L$H5/lib -lhdf5
  ./test_$c || (cp test_$c $DEST && rm -f test_$c*.h5)
done

for i in test*.h5; do
  echo "Checking $i"
  h5diff -q raw.h5 $i || (echo "$i does not match raw.h5" && h5dump -p -A $i)
done

