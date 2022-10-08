import numpy as np

DTYPE = np.intc


def modify(array_1):
    cdef Py_ssize_t x_max = array_1.shape[0]
    cdef Py_ssize_t y_max = array_1.shape[1]

    cdef int [:, :] np_array_view = array_1

    np_array_view[0, 0] = 100

    print(array_1)


