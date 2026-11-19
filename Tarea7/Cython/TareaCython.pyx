# cython: language_level=3
# distutils: language = c++

import numpy as np
cimport numpy as np

# 1. Enlazamos con tu archivo C++ donde viven las funciones AVX2
cdef extern from "../TareaVec.cpp":
    ctypedef struct numero_complejo:
        double real
        double imag

    void multiplicacion_avx2_doble(const numero_complejo* x, const numero_complejo* y, numero_complejo* z, int tamano)
    float AVX_dot_product(float *a, float *b, int tamano)

# 2. Función puente para la Multiplicación de Complejos
def multiplicar_complejos_cython(double[:] x_real, double[:] x_imag, double[:] y_real, double[:] y_imag):
    cdef int tamano = x_real.shape[0]
    
    # Creamos un tipo de dato estructurado en NumPy idéntico al struct de C++
    dt = np.dtype([('real', np.float64), ('imag', np.float64)])
    
    datos1 = np.zeros(tamano, dtype=dt)
    datos2 = np.zeros(tamano, dtype=dt)
    datos3 = np.zeros(tamano, dtype=dt)
    
    # Llenamos las componentes de forma vectorial rápida
    datos1['real'] = x_real
    datos1['imag'] = x_imag
    datos2['real'] = y_real
    datos2['imag'] = y_imag
    
    # Extraemos los punteros de memoria física de NumPy
    cdef numero_complejo* p_datos1 = <numero_complejo*>np.PyArray_DATA(datos1)
    cdef numero_complejo* p_datos2 = <numero_complejo*>np.PyArray_DATA(datos2)
    cdef numero_complejo* p_datos3 = <numero_complejo*>np.PyArray_DATA(datos3)
    
    # Ejecutamos tu función optimizada AVX2
    multiplicacion_avx2_doble(p_datos1, p_datos2, p_datos3, tamano)
    
    # Devolvemos dos arreglos de NumPy (parte real y parte imaginaria)
    return datos3['real'], datos3['imag']

# 3. Función puente para el Producto Punto
def producto_punto_cython(float[:] vec_a, float[:] vec_b):
    cdef int tamano = vec_a.shape[0]
    
    # Apuntamos directo al primer elemento de cada arreglo en memoria
    cdef float* p_a = &vec_a[0]
    cdef float* p_b = &vec_b[0]
    
    # Ejecutamos tu función FMA de AVX2
    return AVX_dot_product(p_a, p_b, tamano)
