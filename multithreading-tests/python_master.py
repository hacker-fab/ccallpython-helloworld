import ctypes
import pathlib

lib_path = pathlib.Path().absolute() / "build" / "python_master.so"
lib = ctypes.CDLL(lib_path)

lib.main()
lib.testFromPy.restype = ctypes.c_float

print("C++ output is " + str(lib.testFromPy(1)))
print("C++ output is " + str(lib.testFromPy(2)))
print("C++ output is " + str(lib.testFromPy(3)))
