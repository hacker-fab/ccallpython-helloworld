// ccallpython.cpp
#include <Python.h>
#include <opencv2/opencv.hpp>

int main() {
    // Initialize the Python interpreter
    Py_Initialize();

    // Import the Python module
    PyObject *pName = PyUnicode_DecodeFSDefault("p2ctest");
    PyObject *pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != NULL) {
        // Get a reference to the Python function
        PyObject *pFunc = PyObject_GetAttrString(pModule, "process_image");

        if (PyCallable_Check(pFunc)) {
            // Read an image using OpenCV
            cv::Mat image = cv::imread("snake.jpg");

            // Check if the image was successfully loaded
            if (!image.empty()) {
                // Convert the OpenCV image to a Python bytearray
                PyObject *pImageBytes = PyByteArray_FromStringAndSize((const char *)image.data, image.total() * image.elemSize());

                // Prepare the input parameters (bytearray of the image)
                PyObject *pArgs = PyTuple_Pack(1, pImageBytes);

                // Call the Python function with the input parameters
                PyObject *pValue = PyObject_CallObject(pFunc, pArgs);

                // Check if the function call succeeded
                if (pValue != NULL) {
                    // Extract the output values (two integers and a float)
                    int result_int1, result_int2;
                    float result_float;

                    if (PyArg_ParseTuple(pValue, "iif", &result_int1, &result_int2, &result_float)) {
                        printf("Result integers: %d, %d\n", result_int1, result_int2);
                        printf("Result float: %f\n", result_float);
                    } else {
                        PyErr_Print();
                    }

                    // Decrement reference counts
                    Py_DECREF(pValue);
                } else {
                    PyErr_Print();
                }

                // Decrement reference counts
                Py_DECREF(pArgs);
                Py_DECREF(pImageBytes);
            } else {
                printf("Failed to load the image.\n");
            }
        } else {
            PyErr_Print();
        }

        // Decrement reference counts
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    // Finalize the Python interpreter
    Py_Finalize();

    return 0;
}
