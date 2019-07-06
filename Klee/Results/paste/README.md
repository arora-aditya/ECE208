# `paste`

```sh
root@server:~/Klee/coreutils-6.10/obj-llvm/src /root/Klee/klee_build_dir/bin/klee --libc=uclibc --posix-runtime --max-memory=1000 --disable-inlining --optimize --use-forked-solver --max-instruction-time=30 --max-time=3600 --search=random-path --search=nurs:covnew --search=random-path --search=nurs:covnew ./paste.bc --sym-args 0 1 10 --sym-args 0 2 2 --sym-files 1 8 --sym-stdin 8 --sym-stdout
```
