# `mkdir`

## Update
An earlier version of this readme did not include the optimisation for `only-covering-new`. The folder has now been updated to take into consideration this significant improvement

```sh
klee --libc=uclibc --posix-runtime --max-memory=100 --disable-inlining --optimize --warnings-only-to-file=true --watchdog --max-memory-inhibit=false --only-output-states-covering-new=true --use-forked-solver --max-instruction-time=30 --max-time=3600 --search=random-path --search=nurs:covnew --search=random-path --search=nurs:covnew \
./mkdir.bc --sym-args 0 1 10 --sym-args 0 2 2 --sym-files 1 8 --sym-stdin 8 --sym-stdout
```

### Older Command run without optimisation
```sh
root@server:~/Klee/coreutils-6.10/obj-llvm/src /root/Klee/klee_build_dir/bin/klee --libc=uclibc --posix-runtime --max-memory=500 --disable-inlining --optimize --use-forked-solver --max-instruction-time=30 --max-time=3600 --search=random-path --search=nurs:covnew --search=random-path --search=nurs:covnew ./mkdir.bc --sym-args 0 1 10 --sym-args 0 2 2 --sym-files 1 8 --sym-stdin 8 --sym-stdout
```

#### *Note*
The entire klee-results directory was not exported because it had over 3900000 files

```sh
root@server:~/Klee/coreutils-6.10/obj-llvm/src find klee-out-mkdir -print | wc -l
386158
```
My local machine currently has a fraction of those files, about 60000 of them

The files were not stored on the VM either, because of the relatively high space consumption (900MB+) for the entire folder compared to the VMs storage capacity (2GB)
