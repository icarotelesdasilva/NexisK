#!/bin/bash
rm -f kernel/*.o kernel/*.d kernel/drivers/*.o kernel/drivers/*.d
cd build
cmake ..
make distclean
