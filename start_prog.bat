cfl=$(xeno-config --xeno-cflags)
ldf=$(xeno-config --xeno-ldflags)
export CFLAGS=$cfl 
export LDFLAGS=$ldf
export LD_LIBRARY_PATH=/usr/xenomai/lib 
gcc $CFLAGS $LDFLAGS -lnative -lrtdk z2.c -o z2
gcc $CFLAGS $LDFLAGS -lnative -lrtdk z1.c -o z1

