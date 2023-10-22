import ctypes
import pathlib
import time

# configuration variables
callbacksLeft = 3
useNewThread = "1" # example code will hang if set to 0; keep as 1

# utility/test functions

def callback(data_ptr):
    global callbacksLeft

    if(data_ptr is None):
        print("[Py ] Error with callback data.")
        return

    x = ctypes.c_int(int.from_bytes(data_ptr[0:4], byteorder='little')).value # endianness is system-dependent
    y = ctypes.c_int(int.from_bytes(data_ptr[4:8], byteorder='little')).value
    z = ctypes.c_int(int.from_bytes(data_ptr[8:12], byteorder='little')).value
    print(f"[Py ] Received updated data from C++: x={x}, y={y}, z={z}")
    callbacksLeft -= 1

# main code

print("Started python module. Loading C++ library...")

# load the shared library in the build path 
lib_path = pathlib.Path().absolute() / "build" / "python_master.so"
lib = ctypes.CDLL(lib_path)

# configure library return types
lib.testFromPy.restype = ctypes.c_float
lib.main.argtypes = ctypes.c_int, ctypes.POINTER(ctypes.c_char_p)

# call main to initialize library. starts a new thread dedicated for C++ execution
main_argv = (ctypes.c_char_p * 2)(
    ctypes.c_char_p("python_master.py".encode('utf-8')), 
    ctypes.c_char_p(useNewThread.encode('utf-8'))
)
lib.main(2, ctypes.cast(main_argv, ctypes.POINTER(ctypes.c_char_p)))

# test a C++ function
print("[Py ] C++ output is " + str(lib.testFromPy(1)))
print("[Py ] C++ output is " + str(lib.testFromPy(2)))
print("[Py ] C++ output is " + str(lib.testFromPy(3)))

# configure data forwarding from C++
lib.setDataCallback(ctypes.CFUNCTYPE(None, ctypes.c_char_p)(callback))

# let program run for a little
while(callbacksLeft > 0):
    time.sleep(1)

# end the C++ thread
print("[Py ] Calling C++ exit")
lib.exitCpp()

# wait for C++ threads to end (there should be a better way than this)
time.sleep(5)
print("[Py ] End of python code reached")
