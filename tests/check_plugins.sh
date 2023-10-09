#! /bin/bash
set -e -x

PATH=$H5/bin:$PATH
if [ $PLAT_OS == "macos" ]; then
    export DYLD_LIBRARY_PATH=$H5/lib:$DYLD_LIBRARY_PATH
else
    export LD_LIBRARY_PATH=$H5/lib:$LD_LIBRARY_PATH
fi
export HDF5_PLUGIN_PATH=$(realpath -L $PWD/..)

CODECS="zlib lz4 bs blosc"
if [ ! "$SKIP_LZF" == "yes" ]; then
 CODECS+=" lzf"
fi

if [ -z "$CC" ]; then
  CC=cc
fi

FAILURES=""
for c in $CODECS; do
  echo "Compiling writer using $c"
  $CC $CFLAGS -I$H5/include test_core.c test_$c.c -o test_$c -L$H5/lib -lhdf5
  ./test_$c || (cp test_$c $DEST && rm -f test_$c*.h5)
  TEST_OUTPUTS="test_$c*.h5"
  if [ -z "$TEST_OUTPUTS" ]; then
     FAILURES+="$c "
  fi
done

for i in test*.h5; do
  echo "Checking $i"
  h5diff -q raw.h5 $i || (echo "$i does not match raw.h5" && h5dump -p -A $i)
done

if [ -n "$FAILURES" ]; then
  echo "Codecs '$FAILURES' failed to produce test files"
  return 0
fi

