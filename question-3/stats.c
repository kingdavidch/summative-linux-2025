#include <Python.h>
#include <math.h>
#include <stdlib.h>

// Structure to hold our array data
typedef struct {
    double *data;
    int size;
} FloatArray;

// Helper function to create FloatArray from Python list
static FloatArray create_float_array(PyObject *list) {
    FloatArray arr;
    arr.size = PyList_Size(list);
    arr.data = (double *)malloc(arr.size * sizeof(double));
    
    for (int i = 0; i < arr.size; i++) {
        PyObject *item = PyList_GetItem(list, i);
        arr.data[i] = PyFloat_AsDouble(item);
    }
    return arr;
}

// Sum function
static PyObject* stats_sum(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list))
        return NULL;
    
    FloatArray arr = create_float_array(list);
    double sum = 0;
    for (int i = 0; i < arr.size; i++) {
        sum += arr.data[i];
    }
    
    free(arr.data);
    return PyFloat_FromDouble(sum);
}

// Average function
static PyObject* stats_average(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list))
        return NULL;
    
    FloatArray arr = create_float_array(list);
    if (arr.size == 0) {
        free(arr.data);
        PyErr_SetString(PyExc_ValueError, "Empty array");
        return NULL;
    }
    
    double sum = 0;
    for (int i = 0; i < arr.size; i++) {
        sum += arr.data[i];
    }
    
    double avg = sum / arr.size;
    free(arr.data);
    return PyFloat_FromDouble(avg);
}

// Standard deviation function
static PyObject* stats_stddev(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list))
        return NULL;
    
    FloatArray arr = create_float_array(list);
    if (arr.size <= 1) {
        free(arr.data);
        PyErr_SetString(PyExc_ValueError, "Need at least 2 values for standard deviation");
        return NULL;
    }
    
    // Calculate mean
    double sum = 0;
    for (int i = 0; i < arr.size; i++) {
        sum += arr.data[i];
    }
    double mean = sum / arr.size;
    
    // Calculate standard deviation
    double sum_sq_diff = 0;
    for (int i = 0; i < arr.size; i++) {
        double diff = arr.data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    
    double stddev = sqrt(sum_sq_diff / (-arr.size - 1));
    free(arr.data);
    return PyFloat_FromDouble(stddev);
}

// Mode function (returns first mode if multiple exist)
static PyObject* stats_mode(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list))
        return NULL;
    
    FloatArray arr = create_float_array(list);
    if (arr.size == 0) {
        free(arr.data);
        PyErr_SetString(PyExc_ValueError, "Empty array");
        return NULL;
    }
    
    // Simple counting approach
    double current_value = arr.data[0];
    double mode = current_value;
    int max_count = 1;
    int current_count = 1;
    
    for (int i = 1; i < arr.size; i++) {
        if (arr.data[i] == current_value) {
            current_count++;
        } else {
            current_value = arr.data[i];
            current_count = 1;
        }
        if (current_count > max_count) {
            max_count = current_count;
            mode = current_value;
        }
    }
    
    free(arr.data);
    return PyFloat_FromDouble(mode);
}

// Count function
static PyObject* stats_count(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list))
        return NULL;
    
    FloatArray arr = create_float_array(list);
    int count = arr.size;
    free(arr.data);
    return PyLong_FromLong(count);
}

// Method definitions
static PyMethodDef StatsMethods[] = {
    {"sum", stats_sum, METH_VARARGS, "Calculate sum of array"},
    {"average", stats_average, METH_VARARGS, "Calculate average of array"},
    {"stddev", stats_stddev, METH_VARARGS, "Calculate standard deviation of array"},
    {"mode", stats_mode, METH_VARARGS, "Calculate mode of array"},
    {"count", stats_count, METH_VARARGS, "Count elements in array"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef statsmodule = {
    PyModuleDef_HEAD_INIT,
    "stats",
    "Statistical functions module",
    -1,
    StatsMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_stats(void) {
    return PyModule_Create(&statsmodule);
}
