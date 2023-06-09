make clean
make
cd build
sleep 2
pintos-mkdisk filesys.dsk 10
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/open-normal:open-normal -p ../../tests/userprog/sample.txt:sample.txt -- -q   -f run open-normal < /dev/null 2> tests/userprog/open-normal.errors |tee tests/userprog/open-normal.output
Kernel command line: -q -f put open-normal put sample.txt run open-normal VERBOSE=1
