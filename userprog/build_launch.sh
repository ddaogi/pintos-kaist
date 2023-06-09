make clean
make
cd build
sleep 2
pintos-mkdisk filesys.dsk 10
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/open-normal:open-normal -p ../../tests/userprog/sample.txt:sample.txt -- -q   -f run open-normal < /dev/null 2> tests/userprog/open-normal.errors |tee tests/userprog/open-normal.output
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/create-bad-ptr:create-bad-ptr -- -q   -f run create-bad-ptr < /dev/null 2> tests/userprog/create-bad-ptr.errors |tee tests/userprog/create-bad-ptr.output 
