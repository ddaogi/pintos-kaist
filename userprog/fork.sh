cd ..
source ./activate
cd userprog
make clean
make
cd build
sleep 2
pintos-mkdisk filesys.dsk 10
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-once:fork-once -- -q   -f run fork-once < /dev/null 2> tests/userprog/fork-once.errors |tee tests/userprog/fork-once.output
perl -I../.. ../../tests/userprog/fork-once.ck tests/userprog/fork-once tests/userprog/fork-once.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-multiple:fork-multiple -- -q   -f run fork-multiple < /dev/null 2> tests/userprog/fork-multiple.errors |tee tests/userprog/fork-multiple.output
perl -I../.. ../../tests/userprog/fork-multiple.ck tests/userprog/fork-multiple tests/userprog/fork-multiple.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-recursive:fork-recursive -- -q   -f run fork-recursive < /dev/null 2> tests/userprog/fork-recursive.errors |tee tests/userprog/fork-recursive.output
perl -I../.. ../../tests/userprog/fork-recursive.ck tests/userprog/fork-recursive tests/userprog/fork-recursive.result
