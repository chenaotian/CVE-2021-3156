mkdir libnss_X
gcc -fPIC -shared lib.c -o ./libnss_X/test.so.2
gcc exp.c -o exp
