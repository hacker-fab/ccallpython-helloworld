import ctypes

def hello(data):
    print("Hello from python!")
    
    if(data is not None):
    	# 2D array of 4-byte integers
    	ptr = ctypes.cast(data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24 | data[4] << 32 | data[5] << 40 | data[6] << 48 | data[7] << 56, ctypes.POINTER(ctypes.c_int));
    	print("[Py ] Address of 2D data is " + str(ptr))
    	# Prints 10001st element of array
    	print("[Py ] Test: " + str(ptr[10000]));
    	# Width of 2D data
    	print("[Py ] Width: " + str(data[8] | data[9] << 8 | data[10] << 16 | data[11] << 24));
    	
    	
