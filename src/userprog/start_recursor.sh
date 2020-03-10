make -j4
cd  build
pintos-mkdisk fs.dsk 400
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/recursor_ng -a recursor_ng --  -q
pintos --qemu -m 128 -- run "recursor_ng pintosmaster 6 1"
