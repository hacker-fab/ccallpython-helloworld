// ccallpython.cpp
#include <Python.h>//If compile error says cant find Python.h try change to #include<Python/Python.h>
#include <iostream>

// Sample data, i.e. for an image
const int testWidth = 1000;
const int testHeight = 1000; 
int testData[testHeight][testWidth];

// This is the format of the data passed to Python
struct DataFormat {
    int *data2D;
    int width;
    int height;
    int stride;	
} fmt;

// Initialize data to pass to Python
void initializeTestData() {
    for(int y = 0; y < testHeight; y++) {
        for(int x = 0; x < testWidth; x++) {
            testData[y][x] = testWidth * y + x;
        }
    }
    
    fmt.data2D = reinterpret_cast<int *>(testData);
    fmt.width = testWidth;
    fmt.height = testHeight;
    fmt.stride = sizeof(testData[0][0]);
}

int main() {
    initializeTestData();

    // Initialize the Python interpreter
    Py_Initialize();
    // This is important as if you dont import sys and add current path to sys path, C can not locate python module.
    PyRun_SimpleString("import sys");
    //PyRun_SimpleString("print('This is a test to see if python works')");
    // Add current path to sys path. Change path if needed.
    PyRun_SimpleString("sys.path.append('.')");
    // Import the Python module (dont add .py, just the name of your python module/script)
    PyObject *pName = PyUnicode_DecodeFSDefault("p2ctest");
    PyObject *pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    // Check if the module was imported successfully
    if (pModule != NULL) {
    	// Pass a pointer to C memory using this object
        PyObject *mem = PyMemoryView_FromMemory(reinterpret_cast<char *>(&fmt), sizeof(fmt), PyBUF_READ);
        
        // Use this tuple to pass data to the Python module
        PyObject *dataToPass = PyTuple_New(1);
        PyTuple_SET_ITEM(dataToPass, 0, mem);
        
        // Get a reference to the Python function
        PyObject *pFunc = PyObject_GetAttrString(pModule, "hello");

	// Print address:
	std::cout << "[C++] Array data at address " << fmt.data2D << std::endl;

        // Check if the function exists
        if (PyCallable_Check(pFunc)) {
            // Call the Python function
            PyObject_CallObject(pFunc, dataToPass);
        } else {
            PyErr_Print();
        }

        // Decrement reference counts and clean up (This will end python kernel, if you wan to invoke python kernel again, 
        // simply repeat previous code)
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    // Finalize the Python interpreter
    Py_Finalize();

    return 0;
}
