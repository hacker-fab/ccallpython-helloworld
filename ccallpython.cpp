// ccallpython.cpp
#include <Python.h>//If compile error says cant find Python.h try change to #include<Python/Python.h>

int main() {
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
        // Get a reference to the Python function
        PyObject *pFunc = PyObject_GetAttrString(pModule, "hello");

        // Check if the function exists
        if (PyCallable_Check(pFunc)) {
            // Call the Python function
            PyObject_CallObject(pFunc, NULL);
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
