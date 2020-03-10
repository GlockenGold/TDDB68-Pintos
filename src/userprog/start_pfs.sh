make -j4
cd build
pintos-mkdisk fs.dsk 800
dd if=/dev/urandom of=random bs=1 count=100
pintos --qemu -- -f -q
pintos --qemu -p random -a random --  -q
pintos --qemu -p ../../examples/pfs -a pfs --  -q
pintos --qemu -p ../../examples/pfs_writer -a pfs_writer --  -q
pintos --qemu -p ../../examples/pfs_reader -a pfs_reader --  -q
pintos --qemu -- run pfs
