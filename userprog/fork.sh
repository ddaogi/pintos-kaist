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
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/wait-killed:wait-killed -p tests/userprog/child-bad:child-bad -- -q   -f run wait-killed < /dev/null 2> tests/userprog/wait-killed.errors |tee tests/userprog/wait-killed.output
perl -I../.. ../../tests/userprog/wait-killed.ck tests/userprog/wait-killed tests/userprog/wait-killed.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/wait-simple:wait-simple -p tests/userprog/child-simple:child-simple -- -q   -f run wait-simple < /dev/null 2> tests/userprog/wait-simple.errors |tee tests/userprog/wait-simple.output
perl -I../.. ../../tests/userprog/wait-simple.ck tests/userprog/wait-simple tests/userprog/wait-simple.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-read:exec-read -p ../../tests/userprog/sample.txt:sample.txt -p tests/userprog/child-read:child-read -- -q   -f run exec-read < /dev/null 2> tests/userprog/exec-read.errors |tee tests/userprog/exec-read.output
perl -I../.. ../../tests/userprog/exec-read.ck tests/userprog/exec-read tests/userprog/exec-read.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-bad-ptr:exec-bad-ptr -- -q   -f run exec-bad-ptr < /dev/null 2> tests/userprog/exec-bad-ptr.errors |tee tests/userprog/exec-bad-ptr.output
perl -I../.. ../../tests/userprog/exec-bad-ptr.ck tests/userprog/exec-bad-ptr tests/userprog/exec-bad-ptr.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-missing:exec-missing -- -q   -f run exec-missing < /dev/null 2> tests/userprog/exec-missing.errors |tee tests/userprog/exec-missing.output
perl -I../.. ../../tests/userprog/exec-missing.ck tests/userprog/exec-missing tests/userprog/exec-missing.result
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-boundary:exec-boundary -p tests/userprog/child-simple:child-simple -- -q   -f run exec-boundary < /dev/null 2> tests/userprog/exec-boundary.errors |tee tests/userprog/exec-boundary.output
perl -I../.. ../../tests/userprog/exec-boundary.ck tests/userprog/exec-boundary tests/userprog/exec-boundary.result