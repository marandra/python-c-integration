#!/usr/bin/env python
import sys
import os
import ctypes

ctypes.cdll.LoadLibrary("/path/to/boost_python_1_56_0/lib/libboost_python.so")

import libbimreadh5 as b

def bimread(fname, page):
    #img, frmt, npag, dx, dy, dz, mdat = b.readh5(fname, int(page))
    img = b.readh5(fname, int(page))
    print len(img)
    print img[0:100]
    #print int(img[0:500])
    #print frmt, npag, dx, dy, dz, mdat
    return

def main():
    if len(sys.argv) == 2:
        bimread(sys.argv[1], 0)
    if len(sys.argv) == 3:
        bimread(sys.argv[1], sys.argv[2])
    if len(sys.argv) < 2:
        print "Usage: " + str(sys.argv[0]) + " file [page]"

if __name__ == "__main__":
    main()
