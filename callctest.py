#!/usr/bin/env python
import ctypes
 
_bimconv = ctypes.CDLL('/path/to/libbimcnvh5.so')
#_bimconv.printusage.argtypes = [ctypes.c_char_p]
_bimconv.convertimage.argtypes = (
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_char_p,
ctypes.c_int)
 
def printusage():
    _bimconv.printusage(ctypes.c_char_p("PROGRAM"))
    return


def convertimage():
#    formt, page: jp2, 0
#    pathi/conti/grupi/filei: .///Rome.tiff
#    patho/conto/grupo/fileo: .///Rome.jp2
    err = _bimconv.convertimage(
    ctypes.c_char_p("Rome.tiff"), #char filei[OPJ_PATH_LEN];
    ctypes.c_char_p("Rome.jp2"), #char fileo[OPJ_PATH_LEN];
    ctypes.c_char_p(""), #char conti[OPJ_PATH_LEN];
    ctypes.c_char_p(""), #char conto[OPJ_PATH_LEN];
    ctypes.c_char_p(""), #char grupi[OPJ_PATH_LEN];
    ctypes.c_char_p(""), #char grupo[OPJ_PATH_LEN];
    ctypes.c_char_p("."), #char pathi[OPJ_PATH_LEN];
    ctypes.c_char_p("."), #char patho[OPJ_PATH_LEN];
    ctypes.c_char_p("jp2"), #char formt[OPJ_PATH_LEN];
    ctypes.c_int(0) )    #int pagen;
    return err

def main():
    #if not convertimage(): print "OK"
    printusage()

if __name__ == "__main__":
    main()
